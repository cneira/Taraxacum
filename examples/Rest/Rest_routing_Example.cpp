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

namespace uRest {
void biz(const Rest::Request &request, Http::ResponseWriter response) {
  response.send(Http::Code::Ok, "Here is routing");
}
}

int main() {
  using namespace Taraxacum;
  //   If you need routing on your microservice, just use the
  //   Routing_Microservice template
  //   it takes as parameter a non static free function pointer with the
  //   signature
  //   void (const Rest::Request &request, Http::ResponseWriter response)
  //
  std::shared_ptr<Uservice_Interface> usvc_rest_with_routing =
      AddProviders_shared<Routing_Microservice<uRest::biz>>();

  std::cout << "Start answering requets on port 9032, using 2 threads and on "
               "the route \"/stars/response\" using http GET"
            << std::endl;

  usvc_rest_with_routing->Answer(9032, 2, "/stars/response", HTTP_METHOD::GET);

  return 0;
}
