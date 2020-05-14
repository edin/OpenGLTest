#pragma once

#include "TypeAliases.h"

namespace Nuts {
    class Logger {
    public:
        static void Error(const std::string& category, const std::string& message) {
            std::cerr << "Error: " << category << ":" << message << "\n";
        }
    };
}