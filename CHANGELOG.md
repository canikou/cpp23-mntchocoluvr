# Changelog

All notable changes to this project will be documented in this file.

The format is based on Keep a Changelog, and this project adheres to Semantic Versioning where practical.

## [Unreleased]

### Added
- Initial template baseline with CMake presets, VS Code workflow, testing, and GitHub CI.
- CMake modules for reusable warnings/sanitizers/IPO options.
- CI-safe presets (`ci-debug`, `ci-release`, `ci-lint`) and optional vcpkg presets.
- Doctest-based multi-file test suite.
- Clang formatting and linting configs with `format`, `format-check`, and `lint` targets.
- Package export/install support for downstream `find_package`.
- Bootstrap script for project name/namespace initialization.

### Changed
- GitHub Actions now covers Windows MSYS2 plus Linux GCC/Clang matrix and lint checks.
- Release/debug output naming now uses config-aware generator expressions.
