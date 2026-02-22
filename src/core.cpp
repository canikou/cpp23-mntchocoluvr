// SPDX-License-Identifier: MIT

#include "app/core.hpp"

namespace app {
std::string greeting() { return "Hello from app core"; }

std::string greeting_for(std::string_view name) {
    if (name.empty()) {
        return greeting();
    }
    return "Hello, " + std::string(name);
}
} // namespace app
