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

#include <iostream>
#include "../../Microservices/Uservice_Interface.h"
#include "../../transport/Endpoint.h"
#include <functional>

class myservice : public Uservice, public Http::Handler {
public:
    int err_cnt, ok_cnt;
    std::string name, version;
    std::function<std::string(std::string)> sol;

    myservice() = default;

    myservice(const std::string _name, const std::string _ver, std::function<std::string(std::string)> f)
            : name(_name), version(_ver), sol(f) {};


HTTP_PROTOTYPE(myservice)

    void onRequest(const Http::Request &Request, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, sol("aa"));
    }

    void Answer(int port) {
        try {
            std::cout << "Http Endpoint starting" << std::endl;
            std::string saddress = "*:" + std::to_string(port);
            std::cout << "Address " << saddress << std::endl;
            Http::listenAndServe<myservice>(saddress);
        } catch (std::runtime_error &e) {
            std::cout << "Address Already in use port :" << port << std::endl;
        }
    }

};

int main() {
    auto fp = [](auto a) -> std::string { return "hohoho"; };
    Uservice_Interface *usvc = new Publisher(new Logging(new Endpoint(new myservice("micro", "1.2", fp))));
    usvc->Log();
    usvc->Circuit_Break();
    usvc->Publish();
    usvc->Answer(9001);
    return 0;
}