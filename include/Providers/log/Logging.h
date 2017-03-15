//
// Created by cnb on 3/10/17.
//

#ifndef USERVICES_LOGGING_H
#define USERVICES_LOGGING_H

#include <Common/Uservice_Interface.h>


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
