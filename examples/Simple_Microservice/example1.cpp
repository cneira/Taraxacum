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

#include "../../Common/Microservice.h"

#include <iostream>

namespace microservices {
struct Service {
    const std::string operator()(std::string request) { return "Hello Hello "; };
};
}

int main() {

    Uservice_Interface *usvc =
            AddProviders<Publisher, Logging, CircuitBreaker,
                    Microservice<microservices::Service>>();

    // Call the providers that decorate this microservice

    usvc->Log();
    usvc->Circuit_Break();
    usvc->Publish();
    usvc->Measure();

    // Start listening for requests

    usvc->Answer(9004);

    return 0;
}
