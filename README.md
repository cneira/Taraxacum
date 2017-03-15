# Saproling Cluster

[![Build Status](https://travis-ci.org/cneira/Uservices.svg?branch=master)](https://travis-ci.org/cneira/Uservices)

Is a framework for creating microservices written in pure C++-11.
It aims to be a fast, full featured and with an easy to use API implementation for microservices.
It's sort of inspired on [Go-Kit](https://gokit.io/) for microservices. It uses the excellent frameworks pistache for REST and HTTP and rapidjson. 

Note that this project it is under active developing, doesn't have a stable interface and was not tested enough. So if you are looking for something stable and ready to be used in production then it's better to come back later or help me growing this project to the quality level you need.

# Dependencies

* Pistache http://pistache.io/
* RapidJson http://rapidjson.org/

# Installation
```bash
git clone  https://github.com/cneira/Saproling-Cluster.git
cd Uservices && mkdir build && cd build && cmake .. / && make && sudo make install
```

# Usage

Uservices follow the [Decorator Patern](https://en.wikipedia.org/wiki/Decorator_pattern).
Decorators in Uservices are called Providers

## Instanciating a microservice
 To instanciate a microservice we use the template AddProviders_shared template.
 
 ```cpp
   std::shared_ptr<Uservice_Interface> microservice =  AddProviders_shared<Microservice<microservices::Service>>();
   microservice->Answer(8080,2);
 ```
## Add Providers
```cpp
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

## Simple microservice
This microservice just returns the string "Hello Hello" and the  http request received 
```cpp
#include <Uservices/Common/Microservice.h>

namespace microservices {
struct Service {
  const std::string operator()(std::string http_request) {
    return "Hello Hello " + http_request;
  };
};

int main() {
   std::shared_ptr<Uservice_Interface> usvc =
        AddProviders_shared<CircuitBreaker, Logging,
                            Microservice<microservices::Service>>();
    // Start answering requests on port 9030 using 2 threads
    usvc->Answer(9030, 2);
}
```

## Rest Microservice
If you need to consume and output json,rapidjson is used, there is Rest microservice template.

```cpp 
// The same but using the RestService template
namespace microservices {
struct Restsvc {
  void operator()(rapidjson::Document &Doc) {
    rapidjson::Value &s = Doc["stars"];
    s.SetInt(s.GetInt() + 1);
  }
};

}
int main() {
  // Now Using a template.
  //  The RestService template takes care for you json parsing validation.
  std::shared_ptr<Uservice_Interface> usvc_rest =
      AddProviders_shared<CircuitBreaker, Logging,
                          Microservice<RestService<microservices::Restsvc>>>();
return 0;                          
}
```
## Microservice Routing
There is a template where you need to have routes on your microservices
```cpp


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

## Current Goals

  * Implement the Circuit Breaker, Logging, Observability and Load Balancer providers.
  * Code Refactoring
  * Stabilize API
 