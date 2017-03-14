/*
 *  Simple example using the framework.
 *  Copyright (C) Carlos Neira <cneirabustos@gmail.com>
 *
 * This program is free software ; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation ; either version 2 of the License, or
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

#include "../../Common/Microservice.h"

#include <iostream>

// a Service is just defined as a functor holding the logic you will add to the
// Microservice, at the moment we only take and return strings from the http
// request.
// RestService consumes json using rapidjson lib using the example at
// https://github.com/miloyip/rapidjson
// You could test the RestService
// curl -i -H "Accept: application/json" -X POST -d
// '{"project":"uservices","stars": 10}' http://localhost:9029

namespace microservices {
struct Service {
  const std::string operator()(std::string http_request) {
    return "Hello Hello " + http_request;
  };
};

struct RestService {
  const std::string operator()(std::string http_request) {

    rapidjson::Document d;

    if (d.Parse(http_request.data()).HasParseError()) {
      std::cout << "Error parsing json" << std::endl;
      // throw std::invalid_argument("json parse error");
      return "{\"err:\" \"error parsing json\" }";
    }

    // 2. Modify it by DOM.
    rapidjson::Value &s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return buffer.GetString();
  };
};
// The same but using the RestService template
struct Restsvc {
  void operator()(rapidjson::Document &Doc) {
    rapidjson::Value &s = Doc["stars"];
    s.SetInt(s.GetInt() + 1);
  }
};
struct svcroute {
  void operator()(const Rest::Request &request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "Here is routing");
  }
};
}

namespace uRest {
void biz(const Rest::Request &request, Http::ResponseWriter response) {
  response.send(Http::Code::Ok, "Here is routing");
}
}

int main() {

  // Call the providers that decorate this microservice, you could add more
  // taking a look at the Providers folder

//  std::shared_ptr<Uservice_Interface> usvc =
//      AddProviders_shared<CircuitBreaker, Logging, Publisher,
//                          Microservice<microservices::RestService>>();
//
//  usvc->Circuit_Break();
//  usvc->Measure();
//  // Start answering requests on port 9030 using 2 threads
//  usvc->Answer(9030, 2);
//
//  // Now Using a template.
//  //  The RestService template takes care for you json parsing validation.
//  std::shared_ptr<Uservice_Interface> usvc_rest =
//      AddProviders_shared<CircuitBreaker, Logging, Publisher,
//                          Microservice<RestService<microservices::Restsvc>>>();
//
//  usvc_rest->Circuit_Break();
//  usvc_rest->Measure();
//  // Start answering requests on port 9030 using 2 threads
//  usvc_rest->Answer(9031, 2);

  // If you need routing on your microservice, just use the
  // Routing_Microservice template
  // it takes as parameter a non static free function pointer with the signature
  // void (const Rest::Request &request, Http::ResponseWriter response)

  std::shared_ptr<Uservice_Interface> usvc_rest_with_routing =
      AddProviders_shared<CircuitBreaker, Logging, Publisher,
                          Routing_Microservice<uRest::biz>>();
  // Start answering requets on port 9031, using 2 threads and on the route
  //  /starts/response, and HTTP method : GET, POST, PUT, DELETE
  usvc_rest_with_routing->Answer(9032, 2, "/stars/response",HTTP_METHOD::GET);

  return 0;
}
