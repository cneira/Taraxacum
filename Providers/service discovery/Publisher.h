//
// Created by cnb on 3/10/17.
//

#ifndef USERVICES_PUBLISHER_H
#define USERVICES_PUBLISHER_H

#include "../../Common/Uservice_Interface.h"

class Publisher : public Provider {

public:
    Publisher(std::shared_ptr<Uservice_Interface> usvc) : Provider(usvc) {};

    void Publish() {
        Provider::Publish();
        std::cout << "Publisher adding " << std::endl;
    }
};

#endif //USERVICES_PUBLISHER_H
