// SPDX-License-Identifier: MIT

#include "app/core.hpp"
#include "doctest/doctest.h"

TEST_CASE("greeting returns baseline message") { CHECK_EQ(app::greeting(), "Hello from app core"); }

TEST_CASE("greeting_for uses friendly prefix") {
    CHECK_EQ(app::greeting_for("Codex"), "Hello, Codex");
}
