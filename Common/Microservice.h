/*
 * Microservice Class
 *
 *
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
#ifndef USERVICES_MICROSERVICE_H
#define USERVICES_MICROSERVICE_H

#include "Uservice_Interface.h"

template<typename F>
class Microservice : public Uservice, public Http::Handler {
private:
    std::string microservice_response;

    void onRequest(const Http::Request &Request, Http::ResponseWriter response) {
        try {
            microservice_response = F()(Request.body());
        } catch (std::bad_function_call &e) {
            std::cout << "Service does not provided Function to serve" << std::endl;
            return;
        }
        response.send(Http::Code::Ok, microservice_response);
    }

public:
    Microservice() = default;

HTTP_PROTOTYPE(Microservice)

    void Answer(int port) {
        try {
            std::cout << "Http Endpoint starting" << std::endl;
            std::string saddress = "*:" + std::to_string(port);
            std::cout << "Address " << saddress << std::endl;
            Http::listenAndServe<Microservice>(saddress);
        } catch (std::runtime_error &e) {
            std::cout << "Address Already in use port :" << port << std::endl;
        }
    }
};

#endif // USERVICES_MICROSERVICE_H
