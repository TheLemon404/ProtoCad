//
// Created by Osprey on 5/15/2025.
//

#include <algorithm>
#include <iostream>
#include <ostream>

#include "application.h"

int main() {

    std::cout << "Hello World!" << std::endl;

    Application app = Application();
    app.Initialize();
    app.Run();
}
