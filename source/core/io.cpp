//
// Created by Osprey on 5/16/2025.
//

#include "io.h"

#include <iostream>

#include "logging.h"

namespace ProtoCADCore {
     std::vector<char> IO::ReadFileSPV(const std::string& filename) {
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

    const char* IO::ReadFileGLSL(const std::string& filename){
        std::ifstream::pos_type size;
         char * memblock;
         std::string text;

         // file read based on example in cplusplus.com tutorial
         std::ifstream file (filename, std::ios::in| std::ios::binary| std::ios::ate);
         if (file.is_open())
         {
             size = file.tellg();
             memblock = new char [1 + size];
             file.seekg (0, std::ios::beg);
             file.read (memblock, size);
             file.close();
             memblock[size] = '\0';
             std::cout << "file " << filename << " loaded" << std::endl;
             text.assign(memblock);
         }
         else
         {
             std::cout << "Unable to open file " << filename << std::endl;
             exit(1);
         }
         return memblock;
     }
}
