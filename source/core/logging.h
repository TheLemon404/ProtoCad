//
// Created by Osprey on 5/15/2025.
//

#pragma once

#ifndef LOGGING_H
#define LOGGING_H
#include <string>

#endif //LOGGING_H

namespace ProtoCADCore {
    class Logging {
    public:
        static void Log(std::string message);
        static void Warn(std::string message);
        static void Error(std::string message);
    };
}