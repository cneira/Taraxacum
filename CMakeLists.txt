cmake_minimum_required(VERSION 3.6)
set(CMAKE_FIND_LIBRARY_PREFIXES /usr/lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
set(Boost_INCLUDE_DIR /usr/include)
set(Boost_LIBRARY_DIR /usr/lib)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
find_package(Boost REQUIRED COMPONENTS )
find_package(RapidJSON)

if (RapidJSON_FOUND)
    include_directories(${RapidJSON_INCLUDE_DIRS})
endif ()

if (LOG4CPP_FOUND)
    include_directories(${LOG4CPP_INCLUDE_DIRS})
endif ()

project(example1)

set(CMAKE_CXX_STANDARD 14)
include_directories("/usr/local/include" "/usr/include" )
set(SOURCE_FILES examples/Simple_Microservice/Rest_example.cpp Common/Uservice_Interface.h Common/Microservice.h Providers/metrics/Metrics.h Providers/log/Logging.h Providers/circuitbreaker/CircuitBreaker.h)
set(SOURCE_FILES_REST examples/Rest/Rest_routing_Example.cpp Common/Uservice_Interface.h Common/Microservice.h Providers/metrics/Metrics.h Providers/log/Logging.h Providers/circuitbreaker/CircuitBreaker.h)


add_executable(Rest_example ${SOURCE_FILES})
target_link_libraries(Rest_example net_static pthread)
add_executable(Rest_routing_Example ${SOURCE_FILES_REST})
target_link_libraries(Rest_routing_Example net_static pthread)
