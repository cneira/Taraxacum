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

#include <Microservice.h>
#include <iostream>
#include <InfluxDB.h>
#include "InfluxDB.h"

namespace uRest {
    InfluxDB inflx;
    std::string data;

    void biz(const Rest::Request &request, Http::ResponseWriter response) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        response.send(Http::Code::Ok, "Here is routing");
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double response_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
        data = "response_time,host=server02 value=" + std::to_string(response_time);
        inflx.Write(data);
        inflx.Query("select * ");
    };

    struct aux {
        const void operator()(const Rest::Request &request, Http::ResponseWriter response) {
            response.send(Http::Code::Ok, "Here is routing functor");
        }
    };
}

int main() {
    using namespace Taraxacum;
    //   If you need routing on your microservice, just use the
    //   Routing_Microservice template
    //   it takes as parameter a non static free function pointer with the
    //   signature
    //   void (const Rest::Request &request, Http::ResponseWriter response)
    //

    auto usvc = Routing_Microservice<uRest::biz>();

    std::cout << "Start answering requets on port 9032, using 2 threads and on "
            "the route \"/stars/response\" using http GET"
              << std::endl;

    usvc.Answer(9032, 2, "/stars/response", HTTP_METHOD::GET);

    return 0;
}
