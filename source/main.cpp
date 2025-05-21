//
// Created by Osprey on 5/15/2025.
//

#include <algorithm>
#include <iostream>
#include <ostream>

#include "application.h"

int main() {
    Application app = Application(OPENGL);
    app.Initialize();
    app.Run();
}
