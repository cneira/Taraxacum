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
#include <ServiceDiscovery.h>
#include <csignal>
#include <cstdlib>
namespace uRest {

    std::string data;
    Consuld consul;

    void biz(const Rest::Request &request, Http::ResponseWriter response) {

        response.send(Http::Code::Ok,
		      "{ \"status\": \"ok\", \"arf\": \"8000233230445005032332@\",\"microservice_name\": \"app2\" }");



    };


    void signalHandler(int signum) {
        std::cout << "Interrupt signal (" << signum << ") received.\n";
        std::string v = consul.Upstream();
        std::cout << "Reloading Consul data: New upstream " << v << std::endl;

    }
};


int main() {
    using namespace Taraxacum;
    auto usvc = Routing_Microservice<uRest::biz>();

    std::string port = "8040";
    if(const char* env_p = std::getenv("PORT"))
    {
        std::cout << "Your PORT is: " << env_p << '\n';
        port = env_p;
    }


    signal(SIGHUP, uRest::signalHandler);

    usvc.Answer(atoi(port.data()), 2, "/app2", HTTP_METHOD::GET);

    return 0;
}

