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

int main() {

    //   If you need routing on your microservice, just use the
    //   Routing_Microservice template
    //   it takes as parameter a non static free function pointer with the
    //   signature
    //   void (const Rest::Request &request, Http::ResponseWriter response)
    //   RestShellscript will execute a command defined in a config.json file in
    //   the path of the microservice
    //    config.json should have at least the cmd key : { "cmd": "your shell
    //    command" }
    using namespace Taraxacum;

    auto usvc = Routing_Microservice<Routing::ShellCmd>();
    std::cout
            << "Start answering requets on port 9032, using 2 threads and on "
                    "the route \"/services/script1\" using http GET, this will "
                    "execute the command specified in config.json and return the result"
            << std::endl;

    usvc.Answer(9032, 2, "/services/script1",
                HTTP_METHOD::GET);

    return 0;
}
