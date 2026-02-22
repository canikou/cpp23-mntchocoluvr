// SPDX-License-Identifier: MIT

#include "app/core.hpp"
#include "doctest/doctest.h"

TEST_CASE("greeting_for falls back to default greeting when input is empty") {
    CHECK_EQ(app::greeting_for(""), app::greeting());
}

TEST_CASE("greeting_for preserves whitespace in provided name") {
    CHECK_EQ(app::greeting_for("  Team  "), "Hello,   Team  ");
}
