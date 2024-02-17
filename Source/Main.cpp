#include <iostream>
#include <sstream>
#include "Application.h"

using namespace std;

int main(int argc, char ** argv) {
    try {
        Application app;
        app.mainLoop();
    } 
    catch(const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        cout << error_msg << endl;
        return -1;
    }

    return 0;
}
