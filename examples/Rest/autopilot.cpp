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
#include <InfluxDB.h>
#include <ppconsul/agent.h>
#include <ServiceDiscovery.h>
#include <csignal>
#include <cstdlib>
namespace uRest {

    std::string data;
    Consuld consul;
    InfluxDB Influx;
    void biz(const Rest::Request &request, Http::ResponseWriter response) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        response.send(Http::Code::Ok, "Here is routing");
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double response_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
        data = "response_time,host=server02 value=" + std::to_string(response_time);
        std::cout << "Sending Data to Influx " << data <<std::endl;
        Influx.Write(data);

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
    uRest::Influx.Query("q=CREATE DATABASE sol02");
    std::string port = "9032";
    if(const char* env_p = std::getenv("PORT"))
    {
        std::cout << "Your PORT is: " << env_p << '\n';
        port = env_p;
    }


    signal(SIGHUP, uRest::signalHandler);

    usvc.Answer(atoi(port.data()), 2, "/solutions/app1", HTTP_METHOD::GET);

    return 0;
}

