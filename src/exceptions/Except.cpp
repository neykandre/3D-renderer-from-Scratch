#include "Except.h"

#include <exception>
#include <iostream>

void renderer::react() {
    try {
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unexpected exception" << std::endl;
    }
}
