//
// Created by cnb on 3/10/17.
//

#ifndef USERVICES_LOGGING_H
#define USERVICES_LOGGING_H

#include "../../Common/Uservice_Interface.h"
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

// DEBUG < INFO < NOTICE < WARN < ERROR < CRIT < ALERT < FATAL = EMERG

#define LOG_EMERG(__logstream)  __logstream << log4cpp::Priority::EMERG << CODE_LOCATION
#define LOG_ALERT(__logstream)  __logstream << log4cpp::Priority::ALERT << CODE_LOCATION
#define LOG_CRIT(__logstream)  __logstream << log4cpp::Priority::CRIT << CODE_LOCATION
#define LOG_ERROR(__logstream)  __logstream << log4cpp::Priority::ERROR << CODE_LOCATION
#define LOG_WARN(__logstream)  __logstream << log4cpp::Priority::WARN << CODE_LOCATION
#define LOG_NOTICE(__logstream)  __logstream << log4cpp::Priority::NOTICE << CODE_LOCATION
#define LOG_INFO(__logstream)  __logstream << log4cpp::Priority::INFO << CODE_LOCATION
#define LOG_DEBUG(__logstream)  __logstream << log4cpp::Priority::DEBUG << CODE_LOCATION


class Logging : public Provider {
public:
    Logging(std::shared_ptr <Uservice_Interface> usvc) : Provider(usvc) {};

    void Log() {
        Provider::Log();
        std::cout << "Logging here " << std::endl;
    }
};


class Log4cppProvider : public  Provider {
public:
    Log4cppProvider(std::shared_ptr <Uservice_Interface> usvc) : Provider(usvc) {};

    void Log() {
        Provider::Log();
        std::cout << "Logging here " << std::endl;
    }
};


#endif //USERVICES_LOGGING_H
