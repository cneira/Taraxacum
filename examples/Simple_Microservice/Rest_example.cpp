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

// a Service is just defined as a functor holding the logic you will add to the
// Microservice, at the moment we only take and return strings from the http
// request.
// RestService consumes json using rapidjson lib using the example at
// https://github.com/miloyip/rapidjson
// You could test the RestService
// curl -i -H "Accept: application/json" -X POST -d
// '{"project":"uservices","stars": 10}' http://localhost:9029

namespace microservices {

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
  }
};
}

int main() {

  //   Call the providers that decorate this microservice, you could add more
  //   taking a look at the Providers folder

  std::shared_ptr<Uservice_Interface> usvc =
      AddProviders_shared<CircuitBreaker, Logging,
                          Microservice<microservices::RestService>>();
  std::cout << "You could test the RestService using " << std::endl
            << "curl -i -H \"Accept: application/json\" -X POST -d\"  "
               "\"'{\"project\":\"uservices\",\"stars\": 10}' "
               "http://localhost:9029 "
            << std::endl;

  // Start answering requests on port 9030 using 2 threads
  usvc->Answer(9030, 2);

  return 0;
}
