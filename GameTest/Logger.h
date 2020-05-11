#pragma once

#include "TypeAliases.h"

class Logger {
public:
    static void error(const std::string& category, const std::string& message) {
        std::cerr << "Error: " << category << ":" << message << "\n";
    }
};
