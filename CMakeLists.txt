cmake_minimum_required(VERSION 3.6)
set(CMAKE_FIND_LIBRARY_PREFIXES /usr/lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
set(Boost_INCLUDE_DIR /usr/include)
set(Boost_LIBRARY_DIR /usr/lib)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS )
find_package(RapidJSON)

if (RapidJSON_FOUND)
    include_directories(${RapidJSON_INCLUDE_DIRS})
endif ()

project(Uservices)


set(CMAKE_CXX_STANDARD 14)
include_directories("/usr/local/include" "/usr/include" )
set(SOURCE_FILES examples/Simple_Microservice/example1.cpp Common/Uservice_Interface.h Providers/transport/Endpoint.h Common/Microservice.h "Providers/service discovery/Publisher.h" Providers/metrics/Metrics.h Providers/log/Logging.h Providers/circuitbreaker/CircuitBreaker.h)
add_executable(Uservices ${SOURCE_FILES})
target_link_libraries(Uservices net_static pthread)
