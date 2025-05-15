//
// Created by Osprey on 5/15/2025.
//

#include "logging.h"

#include <iostream>
#include <ostream>

namespace ProtoCADCore {
    void Logging::Log(std::string message) {
        std::cout << "Log: " + message << std::endl;
    }

    void Logging::Warn(std::string message) {
        std::cout << "Warn: " + message << std::endl;
    }


    void Logging::Error(std::string message) {
        throw std::runtime_error("Error: " + message);
    }
}
