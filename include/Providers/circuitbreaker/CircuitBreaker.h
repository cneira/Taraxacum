//
// Created by cnb on 3/10/17.
//

#ifndef USERVICES_CIRCUITBREAKER_H
#define USERVICES_CIRCUITBREAKER_H


#include "Common/Uservice_Interface.h"

class CircuitBreaker : public Provider {
public:
    CircuitBreaker(std::shared_ptr<Uservice_Interface> usvc) : Provider(usvc) {};
    int threshold_trx;

    void Circuit_Break() {
        Provider::Circuit_Break();
        std::cout << "CircuitBreaker here" << std::endl;
    }
};

#endif //USERVICES_CIRCUITBREAKER_H
