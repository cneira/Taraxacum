/*
 * Interfaces for Endpoints layer for Uservices
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

#ifndef USERVICES_ENDPOINT_H
#define USERVICES_ENDPOINT_H
#include "../Microservices/Uservice_Interface.h"
#include "pistache/endpoint.h"
#include <functional>

using namespace Net;
using namespace std::placeholders;
class Endpoint : public Http::Handler, public Decorator {
private:
    std::string answer;
public:
  HTTP_PROTOTYPE(Endpoint)
  Endpoint() = default;
  Endpoint(Uservice_Interface *usvc) : Decorator(usvc){};

  void onRequest(const Http::Request &request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok,"Hello Hello");

  }

  void Answer(int port) {
    Decorator::Answer(port);
    try {
      std::cout << "Http Endpoint starting" << std::endl;
      std::string saddress = "*:" + std::to_string(port);
      std::cout << "Address " << saddress << std::endl;
      Http::listenAndServe<Endpoint>(saddress);
    } catch (std::runtime_error &e) {
      std::cout << "Address Already in use port :" << port << std::endl;
    }
  }

};
#endif // USERVICES_ENDPOINT_H
