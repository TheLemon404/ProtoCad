//
// Created by Osprey on 5/16/2025.
//

#pragma once

#ifndef IO_H
#define IO_H

#include <fstream>
#include <vector>

#endif //IO_H

namespace ProtoCADCore {
    class IO {
    public:
        static std::vector<char> ReadFileSPV(const std::string& filename);
        static const char* ReadFileGLSL(const std::string& filename);
    };
}

