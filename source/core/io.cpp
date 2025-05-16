//
// Created by Osprey on 5/16/2025.
//

#include "io.h"

#include "logging.h"

namespace ProtoCADCore {
     std::vector<char> IO::ReadFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            Logging::Error("failed to open file!");
        }

         size_t fileSize = (size_t) file.tellg();
         std::vector<char> buffer(fileSize);
         file.seekg(0);
         file.read(buffer.data(), fileSize);
         file.close();

         return buffer;
    }
}
