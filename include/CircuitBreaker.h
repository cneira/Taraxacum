/*
 * Circuit Breaker Provider
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

#ifndef USERVICES_CIRCUITBREAKER_H
#define USERVICES_CIRCUITBREAKER_H


#include "Uservice_Interface.h"

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
