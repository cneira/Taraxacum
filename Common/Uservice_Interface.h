/*
 * Interfaces for microservices.
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
#ifndef USERVICES_USERVICE_INTERFACE_H
#define USERVICES_USERVICE_INTERFACE_H

#include "pistache/endpoint.h"
#include <functional>
#include <iostream>
#include <string>

using namespace Net;

class Uservice_Interface {

public:
  virtual void Publish() = 0;

    virtual void Answer(int port) = 0;
  virtual void Measure() = 0;
  virtual void Log() = 0;
  virtual void Circuit_Break() = 0;
};

class Uservice : public Uservice_Interface {

public:
  int err_cnt, ok_cnt;
  std::string name, version;
  Uservice() = default;
  Uservice(const int _port, const std::string _name, std::string _ver)
      : name(_name), version(_ver){};

  void Publish() {
    std::cout << "Publish Uservice " << this->name << std::endl;
  }
  void Answer(int port) {
    std::cout << "Uservice Answering on Port " << port << std::endl;
  }

  void Measure() { std::cout << " Uservice Measure" << std::endl; }

  void Log() { std::cout << " Uservice Log " << std::endl; }

  void Circuit_Break() {
    std::cout << " Uservice Circuite Breaker " << std::endl;
  }
};

class Provider : public Uservice_Interface {

protected:
  Uservice_Interface *usvc;

public:
    Provider(Uservice_Interface *_usvc) : usvc(_usvc) {};

    Provider() = default;

  void Publish() { usvc->Publish(); }

    void Answer(int port) { usvc->Answer(port); }

  void Measure() { usvc->Measure(); }

  void Log() { usvc->Log(); }

  void Circuit_Break() { usvc->Circuit_Break(); }
};



#endif // USERVICES_USERVICE_INTERFACE_H
