//
// Created by cnb on 3/10/17.
//

#ifndef USERVICES_METRICS_H
#define USERVICES_METRICS_H

#include <Common/Uservice_Interface.h>

class Metrics : public Provider {
public:
    Metrics(std::shared_ptr<Uservice_Interface> usvc) : Provider(usvc) {};
    void Measure() {
        Provider::Measure();
        std::cout << "Metrics Here" << std::endl;
    }
};

#endif //USERVICES_METRICS_H
