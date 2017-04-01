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
#include "Metrics.h"
#include "Uservice_Interface.h"
#include "pstream.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <cstdio>
#include <fstream>
#include <sstream>

namespace Taraxacum {
    using namespace Net::Rest;

    template<typename F>
    class Microservice : public Http::Handler, public Uservice {
    private:
        std::string res, name, data;
        double response_time;

        void onRequest(const Http::Request &Request, Http::ResponseWriter response) {
            res = F()(Request.body());
            response.send(Http::Code::Ok, res);
            name = F().name;
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

    template<void (*F)(const Rest::Request &, Http::ResponseWriter)>
    class Routing_Microservice {
    private:
        std::string microservice_response;

    public:
        Routing_Microservice() = default;

        void Answer(int port, int threads, const char *route,
                    HTTP_METHOD http_method) {
            try {

                Router router;
                std::shared_ptr<Net::Http::Endpoint> httpEndpoint;
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
 AddProviders<Publisher,Logging,CircuitBreaker,Microservice<your business logic>>();
*/

    template<typename T>
    std::shared_ptr<T> AddProviders_shared() {
        return std::make_shared<T>();
    }

    template<typename T, typename Arg1, typename... Args>
    std::shared_ptr<T> AddProviders_shared() {
        return std::make_shared<T>(T(AddProviders_shared<Arg1, Args...>()));
    }

    namespace Non_Routing {

/*
   The caller just needs to define a functor that takes a reference to a rapidjson::Document,
   functor will be in charge of updating or generating a new json document.
*/

        template<typename F>
        struct RestService {
            const std::string operator()(std::string http_request) {

                rapidjson::Document d;
                if (d.Parse(http_request.data()).HasParseError()) {
                    std::stringstream ss;
                    ss << "{\"errcode:\"" << (unsigned) d.GetErrorOffset()
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
    }
    namespace Routing {

/*
This micro service will parse a config.json file and execute the cmd
especified in cmd key.
The json response will be : { "cmd_output": <command output> , "err": <errmsg if any>  }
*/
        void ShellCmd(const Rest::Request &request, Http::ResponseWriter response) {

            FILE *fp = fopen("./config.json", "rb");
            if (fp == nullptr) {
                std::cout << "Exception opening/reading configuration file" << std::endl;
                std::cout << "Is config.json created ?" << std::endl;
                std::cout << "config.json should have a key named cmd :" << std::endl;
                std::cout << "{:cmd \"<your shell commands>\" ,... \"}\"" << std::endl;
                throw std::runtime_error(std::strerror(errno));
            }

            char readBuffer[65536];
            rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
            rapidjson::Document d;
            std::stringstream ss;
            if (d.ParseStream(is).HasParseError()) {

                ss << "{\"errcode:\"" << (unsigned) d.GetErrorOffset()
                   << ", \"errmsg\":" << rapidjson::GetParseError_En(d.GetParseError())
                   << "}";
                response.send(Http::Code::Ok, ss.str());
                fclose(fp);
            }
            if (d.HasMember("cmd")) {
                rapidjson::Value &s = d["cmd"];

                redi::ipstream proc(s.GetString(),
                                    redi::pstreams::pstdout | redi::pstreams::pstderr);
                std::string line;
                std::stringstream ssout, sserr, jresp;

                while (std::getline(proc.out(), line))
                    ssout << line;

                while (std::getline(proc.err(), line))
                    sserr << line;

                jresp << "{ \"cmd\":"
                      << "\" " << ssout.str() << "\""
                      << ", \"err\":"
                      << "\"" << sserr.str() << "\""
                              "}";
                fclose(fp);
                response.send(Http::Code::Ok, jresp.str());
            } else {
                fclose(fp);
                ss << "{\"errcode:\"" << "001"
                   << ", \"errmsg\":" << "config.json does not have cmd key"
                   << "}";
                response.send(Http::Code::Ok, ss.str());
            }

        }
    }

    namespace Tools {
        void hexdump(void *ptr, int buflen) {
            unsigned char *buf = (unsigned char *) ptr;
            int i, j;
            for (i = 0; i < buflen; i += 16) {
                printf("%06x: ", i);
                for (j = 0; j < 16; j++)
                    if (i + j < buflen)
                        printf("%02x ", buf[i + j]);
                    else
                        printf("   ");
                printf(" ");
                for (j = 0; j < 16; j++)
                    if (i + j < buflen)
                        printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
                printf("\n");
            }
        }
    }
}

#endif // USERVICES_MICROSERVICE_H
