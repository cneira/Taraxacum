![](Taraxacum.jpg)

# Taraxacum
     
  [![Build Status](https://travis-ci.org/cneira/Taraxacum.svg?branch=master)](https://travis-ci.org/cneira/Taraxacum)   
     
Is a framework for creating microservices written in pure C++-11.
It aims to be a fast, full featured and with an easy to use API implementation for microservices.
It's sort of inspired on [Go-Kit](https://gokit.io/) for microservices. It uses the excellent frameworks pistache for REST and HTTP and rapidjson. 


Note that this project it is under active developing, doesn't have a stable interface and was not tested enough. So if you are looking for something stable and ready to be used in production then it's better to come back later or help me growing this project to the quality level you need.

# Dependencies

* Pistache  http://pistache.io/

* RapidJson http://rapidjson.org/


# Installation
```bash
git clone  https://github.com/cneira/Taraxacum.git
cd  Taraxacum && mkdir build && cd build && cmake ../ && make && sudo make install
```

# Usage

Taraxacum follows the [Decorator Patern](https://en.wikipedia.org/wiki/Decorator_pattern).
In Taraxacum Decorators are just called Providers to reflect for example that we could add a graphite provider to include metrics.

## Instanciating a microservice
 To instanciate a microservice we use the template AddProviders_shared template.
 
 ```cpp
   std::shared_ptr<Uservice_Interface> microservice =  AddProviders_shared<Microservice<microservices::Service>>();
   microservice->Answer(8080,2);
 ```
## Add Providers
```cpp
using namespace Taraxacum;
 // Here we are adding a CircuitBreaker and a Logging Providers
    std::shared_ptr<Uservice_Interface> usvc =
         AddProviders_shared<CircuitBreaker, Logging,
                             Microservice<microservices::Service>>();
     // Start answering requests on port 9030 using 2 threads
     usvc->Answer(9030, 2);
     // called some providers
     usvc->Circuit_Break();
     usvc->Log();
```

# Examples

## Simple micro service
This microservice just returns the string "Hello Hello" and the  http request received 
```cpp
#include "Taraxacum/Microservice.h"
using namespace Taraxacum;
struct Service {
  const std::string operator()(std::string http_request) {
    return "Hello Hello " + http_request;
  };


int main() {
   std::shared_ptr<Uservice_Interface> usvc =
        AddProviders_shared<CircuitBreaker, Logging,
                            Microservice<Service>>();
    // Start answering requests on port 9030 using 2 threads
    usvc->Answer(9030, 2);
}
```

## Rest Micro service
If you need to consume and output json,rapidjson is used, there is Rest microservice template.

```cpp 
using namespace Taraxacum;
// The same but using the RestService template

struct Restsvc {
  void operator()(rapidjson::Document &Doc) {
    rapidjson::Value &s = Doc["stars"];
    s.SetInt(s.GetInt() + 1);
  }
};


int main() {
  // Now Using a template.
  //  The RestService template takes care for you json parsing validation.
  std::shared_ptr<Uservice_Interface> usvc_rest =
      AddProviders_shared<CircuitBreaker, Logging,
                          Microservice<RestService<Restsvc>>>();
return 0;                          
}
```
## Micro service Routing
There is a template where you need to have routes on your microservices
```cpp

using namespace Taraxacum;
namespace uRest {
    void biz(const Rest::Request &request, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, "Here is routing");
    }
}
int main() {
std::shared_ptr<Uservice_Interface> usvc_rest_with_routing =
      AddProviders_shared<CircuitBreaker, Logging,
                          Routing_Microservice<uRest::biz>>();

  // Start answering requets on port 9031, using 2 threads and on the route
  //  "/stars/response", and HTTP method : GET, POST, PUT, DELETE

  usvc_rest_with_routing->Answer(9032, 2, "/stars/response", HTTP_METHOD::GET);
  return 0;
}
```
## Shell command micro service
 This will create a micro service that expects a config.json with the following format { "cmd": \<your shell command> }
 and will respond a json with the format {"output_cmd": "\<shell command output>" ,"err": "\<error message if any>" }
```cpp
 using namespace Taraxacum;
  std::shared_ptr<Uservice_Interface> usvc_rest_with_routing =
      AddProviders_shared<Routing_Microservice<uServices::ShellCmd>>();

  std::cout
      << "Start answering requets on port 9032, using 2 threads and on "
         "the route \"/services/script1\" using http GET, this will "
         "execute the command specified in config.json and return the result"
      << std::endl;

  usvc_rest_with_routing->Answer(9032, 2, "/services/script1",
                                 HTTP_METHOD::GET);

```

## Performance tests

For the Shellcmd micro services the results are the following

using apache benchmark tool with the following parameters:
```bash

$  ab -n 2000  -k -v 4 -c 200   http://127.0.0.1:9032/services/script1
{ "cmd":" Linux 6fd4b134-4c25-cbab-9982-c292a79c2f04 4.3.0 BrandZ virtual linux x86_64 x86_64 x86_64 GNU/Linux", "err":""}
LOG: Response code = 200


Completed 2000 requests
Finished 2000 requests


Server Software:
Server Hostname:        127.0.0.1
Server Port:            9032

Document Path:          /services/script1
Document Length:        122 bytes

Concurrency Level:      200
Time taken for tests:   9.527 seconds
Complete requests:      2000
Failed requests:        0
Keep-Alive requests:    2000
Total transferred:      372000 bytes
HTML transferred:       244000 bytes
Requests per second:    209.92 [#/sec] (mean)
Time per request:       952.726 [ms] (mean)
Time per request:       4.764 [ms] (mean, across all concurrent requests)
Transfer rate:          38.13 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.9      0       6
Processing:     8  892 224.5    907    1198
Waiting:        8  892 224.5    907    1198
Total:          8  892 223.9    907    1198

Percentage of the requests served within a certain time (ms)
  50%    907
  66%   1004
  75%   1045
  80%   1065
  90%   1148
  95%   1170
  98%   1191
  99%   1195
 100%   1198 (longest request)

```



## TODO

  - [ ]  Check current design and refactor 
  - [X]  Define which providers are not needed 
  - [ ]  Remove/Add Providers 
  - [X]  Stabilize API
  - [ ]  Stress test microservices
  - [ ]  Implement API gateway pattern
  - [ ]  Add Observability to micro services
  - [ ]  Create more examples
 
