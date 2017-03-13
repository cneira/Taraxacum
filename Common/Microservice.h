/*
 * Microservice Template Class
 *
 *
 *  Copyright (C) Carlos Neira <cneirabustos@gmail.com>
 *
 * This program is free software ; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation ; either version 3 of the License, or
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
#include "../Providers/circuitbreaker/CircuitBreaker.h"
#include "../Providers/log/Logging.h"
#include "../Providers/service discovery/Publisher.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/NDC.hh"
#include "log4cpp/PropertyConfigurator.hh"

#define CODE_LOCATION __FILE__


#define LOG_EMERG(__logstream)  __logstream << log4cpp::Priority::EMERG << CODE_LOCATION
#define LOG_ALERT(__logstream)  __logstream << log4cpp::Priority::ALERT << CODE_LOCATION
#define LOG_CRIT(__logstream)  __logstream << log4cpp::Priority::CRIT << CODE_LOCATION
#define LOG_ERROR(__logstream)  __logstream << log4cpp::Priority::ERROR << CODE_LOCATION
#define LOG_WARN(__logstream)  __logstream << log4cpp::Priority::WARN << CODE_LOCATION
#define LOG_NOTICE(__logstream)  __logstream << log4cpp::Priority::NOTICE << CODE_LOCATION
#define LOG_INFO(__logstream)  __logstream << log4cpp::Priority::INFO << CODE_LOCATION
#define LOG_DEBUG(__logstream)  __logstream << log4cpp::Priority::DEBUG << CODE_LOCATION

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

// This allows us to prettify the decorator pattern when adding more providers
// to a microservice.
// the last class instantiated should always be the Microservice.
// Example:
// AddProviders<Publisher,Logging,CircuitBreaker,Microservice<yourfunctorthatyouwillservice>>();

template<typename T>
T *AddProviders() { return new T;  }

template<typename T, typename Arg1, typename... Args>
T *AddProviders() {
    return new T(AddProviders<Arg1, Args...>());
}
//
template<typename T>
std::shared_ptr<T> AddProviders_shared() { return  std::shared_ptr<T> (new T);  }

template<typename T, typename Arg1, typename... Args>
std::shared_ptr<T> AddProviders_shared() {
    return std::shared_ptr<T>( new T(AddProviders_shared<Arg1, Args...>()));
}


#endif // USERVICES_MICROSERVICE_H
