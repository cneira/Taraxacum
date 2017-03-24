/*
 * Microservice Template Class
 *
 *
 *  Copyright (C) Carlos Neira <cneirabustos@gmail.com>
 *
 * This program is free software ; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation ; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY ; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the program ; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef USERVICES_MICROSERVICE_H
#define USERVICES_MICROSERVICE_H

#include "CircuitBreaker.h"
#include "Logging.h"
#include "Uservice_Interface.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include "pstream.h"

using namespace Net::Rest;
template <typename F>
class Microservice : public Uservice, public Http::Handler {
private:
  std::string microservice_response;

  void onRequest(const Http::Request &Request, Http::ResponseWriter response) {
    microservice_response = F()(Request.body());
    response.send(Http::Code::Ok, microservice_response);
  }

public:
  Microservice() = default;

  HTTP_PROTOTYPE(Microservice)

  void Answer(int port, int threads) {
    try {
      std::cout << "Http Endpoint starting" << std::endl;
      std::string saddress = "*:" + std::to_string(port);
      std::cout << "Address " << saddress << std::endl;
      auto opts = Http::Endpoint::options().threads(threads);
      Http::listenAndServe<Microservice>(saddress, opts);
    } catch (std::runtime_error &e) {
      std::cout << "Address Already in use port :" << port << std::endl;
      std::cout << "Stack:" << e.what() << std::endl;
    }
  }
};

template <void (*F)(const Rest::Request &, Http::ResponseWriter)>
class Routing_Microservice : public Uservice {
private:
  std::string microservice_response;

public:
  Routing_Microservice() = default;

  void Answer(int port, int threads, const char *route,
              HTTP_METHOD http_method) {
    try {

      Router router;
      std::shared_ptr<Net::Http::Endpoint> httpEndpoint;
      std::cout << "Http Endpoint starting" << std::endl;
      Net::Port _port(port);
      Net::Address addr(Net::Ipv4::any(), port);
      httpEndpoint = std::make_shared<Net::Http::Endpoint>(addr);

      auto opts = Net::Http::Endpoint::options().threads(threads).flags(
          Net::Tcp::Options::InstallSignalHandler);

      httpEndpoint->init(opts);

      switch (http_method) {

      case HTTP_METHOD::GET:
        Routes::Get(router, route, Routes::bind(F));
        break;
      case HTTP_METHOD::POST:
        Routes::Post(router, route, Routes::bind(F));
        break;
      case HTTP_METHOD::PUT:
        Routes::Put(router, route, Routes::bind(F));
        break;
      case HTTP_METHOD::DELETE:
        Routes::Delete(router, route, Routes::bind(F));
        break;
      }
      httpEndpoint->setHandler(router.handler());
      std::cout << "Start serving" << std::endl;
      httpEndpoint->serve();

    } catch (std::runtime_error &e) {
      std::cout << "Address Already in use port :" << port << std::endl;
      std::cout << "Stack:" << e.what() << std::endl;
    }
  }
};

/*
 This allows us to prettify the decorator pattern when adding more providers
 to a microservice.
 the last class instantiated should always be the Microservice.
 Example:
 AddProviders<Publisher,Logging,CircuitBreaker,Microservice<yourfunctorthatyouwillservice>>();
*/

//
template <typename T> std::shared_ptr<T> AddProviders_shared() {
  return std::make_shared<T>();
}

template <typename T, typename Arg1, typename... Args>
std::shared_ptr<T> AddProviders_shared() {
  return std::make_shared<T>(T(AddProviders_shared<Arg1, Args...>()));
}
/*
   Template for microservices, in this case this service will handle json
   requests.
   The caller just needs to define a functor that takes a ref to a json document
   as parameter.
*/

template <typename F> struct RestService {
  const std::string operator()(std::string http_request) {

    rapidjson::Document d;
    if (d.Parse(http_request.data()).HasParseError()) {
      std::stringstream ss;
      ss << "{\"errcode:\"" << (unsigned)d.GetErrorOffset()
         << ", \"errmsg\":" << rapidjson::GetParseError_En(d.GetParseError())
         << "}";
      return ss.str();
    }

    F()(d);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    return buffer.GetString();
  };
};


#endif // USERVICES_MICROSERVICE_H
