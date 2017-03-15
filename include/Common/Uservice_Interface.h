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
#include "pistache/router.h"
#include <Common/Uservice_version.h>
#include <cstdint>
#include <iostream>
#include <string>
using namespace Net;

enum class HTTP_METHOD : std::int8_t { GET = 1, POST = 2, PUT = 3, DELETE = 4 };

class Uservice_Interface {

public:
  virtual void Answer(int port, int threads) = 0;
  virtual void Answer(int port, int threads, const char *route,
                      HTTP_METHOD http_method) = 0;
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

  void Answer(int port, int threads) {
    std::cout << "Uservice Answering on Port " << port << std::endl;
  }
  void Answer(int port, int threads, const char *route,
              HTTP_METHOD http_method) {
    std::cout << "Uservice Answering on Port " << port << "route is" << route
              << std::endl;
  }

  void Measure() { std::cout << " Uservice Measure" << std::endl; }

  void Log() { std::cout << " Uservice Log " << std::endl; }

  void Circuit_Break() {
    std::cout << " Uservice Circuite Breaker " << std::endl;
  }
};

class Provider : public Uservice_Interface {

protected:
  std::shared_ptr<Uservice_Interface> usvc;

public:
  Provider(std::shared_ptr<Uservice_Interface> _usvc) : usvc(_usvc){};

  Provider() = default;



  void Answer(int port, int threads) { usvc->Answer(port, threads); }
  void Answer(int port, int threads, const char *route,
              HTTP_METHOD http_method) {
    usvc->Answer(port, threads, route, http_method);
  }

  void Measure() { usvc->Measure(); }

  void Log() { usvc->Log(); }

  void Circuit_Break() { usvc->Circuit_Break(); }
};

#endif // USERVICES_USERVICE_INTERFACE_H
