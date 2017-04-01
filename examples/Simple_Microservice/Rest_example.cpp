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

// a Service is just defined as a functor holding the logic you will add to the
// Microservice, at the moment we only take and return strings from the http
// request.
// RestService consumes json using rapidjson lib using the example at
// https://github.com/miloyip/rapidjson
// You could test the RestService
// curl -i -H "Accept: application/json" -X POST -d
// '{"project":"uservices","stars": 10}' http://localhost:9029

struct speakjson {
    std::string name = "microservice name";
    std::string version = "1.0";
    double response_time;
  const std::string operator()(std::string http_request) {
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    rapidjson::Document d;
    if (d.Parse(http_request.data()).HasParseError()) {

      std::stringstream ss;
      ss << "{\"errcode:\"" << (unsigned)d.GetErrorOffset()
         << ", \"errmsg\":" << rapidjson::GetParseError_En(d.GetParseError())
         << "}";
      return ss.str();
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

      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      response_time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
    return buffer.GetString();
  }
};

struct aux {
    void operator()(rapidjson::Document &d) {}
};


int main() {
  using namespace Taraxacum;
  //   Call the providers that decorate this microservice, you could add more
  //   taking a look at the Providers folder

  std::shared_ptr<Uservice_Interface> usvc =
      AddProviders_shared<Microservice<speakjson>>();
  std::cout << "You could test the RestService using " << std::endl
            << "curl -i -H \"Accept: application/json\" -X POST -d\"  "
               "\"'{\"project\":\"uservices\",\"stars\": 10}' "
               "http://localhost:9029 "
            << std::endl;
    // Start answering requests on port 9030 using 2 threads;
  usvc->Answer(9029, 2);


  return 0;
}
