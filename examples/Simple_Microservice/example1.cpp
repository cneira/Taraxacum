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

#include "../../Common/Uservice_Interface.h"
#include "../../Common/Microservice.h"

#include <iostream>

struct Service {
    const std::string operator()(std::string icr) {
        return "got this request " + icr;
    };
};


int main() {
    std::cout << "Service name " << std::endl;
    Uservice_Interface *usvc =
            new Publisher(new Logging(new Microservice<Service>));
    usvc->Log();
    usvc->Circuit_Break();
    usvc->Publish();
    usvc->Answer(9004);
    return 0;
}
