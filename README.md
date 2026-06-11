# cpp-cmake2 template

# App Details / Info

## Summary of App

This repository is a production-style C++23 app template designed for fast iteration and long-term maintainability.
It includes a runnable executable, a reusable core library, a test suite, CI pipelines, and packaging support.

## Major Features

- C++23 baseline across build configuration and editor settings.
- CMake presets for local development, CI, linting, and optional vcpkg workflow.
- Built-in tests using `doctest`.
- Built-in static analysis and formatting entry points (`lint`, `format`, `format-check`).
- GitHub Actions coverage for Windows (MSYS2 UCRT64) and Linux (GCC/Clang).
- Install/export support for downstream `find_package(...)` usage.

## Technical Information

- Language: C++23
- Build system: CMake + Ninja
- Test framework: `doctest`
- Primary targets:
  - Library: `${APP_NAME}Core`
  - Executable: `${APP_NAME}`
  - Tests: `${APP_NAME}_tests`
- Debug/dev output name: `app(.exe)`
- Release output name: `${APP_NAME}(.exe)`

# Usage Guide

## Download Guide

- For end users, download binaries from your repository's GitHub Releases page.
- If no release binary is available, run from source using the steps in [Getting Started](#getting-started).

## App Usage

1. Launch the built executable.
2. The default template app prints a greeting from the core library.
3. Optional Qt/icon resources are auto-enabled when matching files/dependencies exist.

# Getting Started

## Development Prerequisites

- Windows local workflow assumes `gcc`, `cmake`, `ninja`, and `gdb` are available on `PATH`.
- MSYS2 UCRT64 is recommended, but the repo does not require a hardcoded install directory. If CMake cannot find dependencies, set `CMAKE_PREFIX_PATH` or create a local `CMakeUserPresets.json`.
- Useful machine-level tools for future maintenance: `rg`, `fd`, `jq`, `fzf`, `gh`, and Chocolatey/winget for installing missing CLI tools.
- VS Code extensions:
  - `ms-vscode.cpptools`
  - `ms-vscode.cmake-tools`

Notes on workflow assumptions:

- Local presets (`debug`, `release`) are intentionally path-agnostic and use tools from `PATH`.
- CI presets (`ci-debug`, `ci-release`, `ci-lint`) are path-agnostic for GitHub runners.

## Setup Steps

1. Clone the repository.
2. Configure debug:
   - `cmake --preset debug`
3. Build debug:
   - `cmake --build --preset debug --parallel`
4. Run tests:
   - `ctest --preset debug`
5. Run the executable:
   - `build/debug/app.exe`

# Workflow Process

## Conventions

- Prefer presets over ad-hoc CMake commands.
- Keep debug output stable at `build/debug/app.exe`.
- Keep release output named by `APP_NAME`.
- Run tests before opening a pull request.
- When touching style/lint-sensitive code, run:
  - `cmake --build --preset debug --target format-check`
  - `cmake --build --preset debug --target lint`

## LLM / Coding Agent Summary

- Use `cmake --preset <preset>` + `cmake --build --preset <preset>` + `ctest --preset <preset>`.
- Default local development path:
  - Configure/build/test with `debug`.
- CI parity path:
  - Configure/build/test with `ci-debug`.
  - Configure/build with `ci-release`.
- Keep optional Qt/icon behavior intact unless explicitly changing requirements.
- Include SPDX MIT headers where appropriate for new source files.

# Other Relevant Info

## CMake Presets

- Local: `debug`, `release`
- CI: `ci-debug`, `ci-release`, `ci-lint`
- Optional vcpkg: `vcpkg-debug`, `vcpkg-release` (requires `VCPKG_ROOT`)

## GitHub Actions

- Triggered on `push` and `pull_request`.
- Jobs:
  - Windows MSYS2 UCRT64: debug build/test + release build
  - Linux matrix: GCC and Clang debug build/test + release build
  - Lint and format checks

## Packaging / Install / Export

1. `cmake --preset release`
2. `cmake --build --preset release --parallel`
3. `cmake --install build/release --prefix dist/install`
4. Downstream usage:
   - `find_package(${APP_NAME} CONFIG REQUIRED)`

## Optional Integrations

- `vcpkg.json` manifest integration via vcpkg presets.
- Bootstrap script for quickly renaming a new project:
  - `powershell -ExecutionPolicy Bypass -File scripts/bootstrap.ps1 -AppName SheetMaster -Namespace sheetmaster -DryRun`
  - `powershell -ExecutionPolicy Bypass -File scripts/bootstrap.ps1 -AppName SheetMaster -Namespace sheetmaster`

## Project Metadata

- Contributing guide: `CONTRIBUTING.md`
- Security policy: `SECURITY.md`
- License: MIT (`LICENSE`)
