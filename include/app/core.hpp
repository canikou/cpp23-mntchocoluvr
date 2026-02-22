// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <string_view>

namespace app {
std::string greeting();
std::string greeting_for(std::string_view name);
} // namespace app
