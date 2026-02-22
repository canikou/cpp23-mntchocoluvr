# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.21)

# ---- Edit this ONE variable per project ----
set(APP_NAME "MyApp")
# --------------------------------------------
project(${APP_NAME} VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ProjectOptions)
include(ClangTools)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_WARNINGS "Enable strict compiler warnings" ON)
option(ENABLE_SANITIZERS "Enable sanitizers for Debug builds (GCC/Clang)" ON)
option(ENABLE_IPO "Enable interprocedural optimization in Release builds" ON)
option(ENABLE_CLANG_TOOLS "Enable clang-format and clang-tidy helper targets" ON)
option(ENABLE_FETCH_TEST_DEPS "Allow fetching test dependencies with FetchContent" ON)

# Qt is optional. When ON, we try to locate Qt; if not found, we continue without it.
# You can explicitly disable Qt in CI with: -DENABLE_QT=OFF
option(ENABLE_QT "Enable Qt6 Widgets integration (optional)" ON)

set(CORE_TARGET "${APP_NAME}Core")
set(TEST_TARGET "${APP_NAME}_tests")
set(PACKAGE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${APP_NAME}")

file(GLOB_RECURSE PROJECT_FORMAT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hxx
)

file(GLOB_RECURSE PROJECT_LINT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(${CORE_TARGET}
    src/core.cpp
)
add_library(${APP_NAME}::Core ALIAS ${CORE_TARGET})
set_target_properties(${CORE_TARGET} PROPERTIES EXPORT_NAME Core)

target_include_directories(${CORE_TARGET}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(${CORE_TARGET} PUBLIC cxx_std_23)
project_apply_default_build_options(${CORE_TARGET})

add_executable(${APP_NAME}
    src/main.cpp
)
target_link_libraries(${APP_NAME} PRIVATE ${CORE_TARGET})
target_compile_features(${APP_NAME} PRIVATE cxx_std_23)
project_apply_default_build_options(${APP_NAME})

# Stable dev binary name: Debug/Dev builds -> app(.exe), Release -> APP_NAME
set_target_properties(${APP_NAME} PROPERTIES
    OUTPUT_NAME "$<IF:$<CONFIG:Release>,${APP_NAME},app>"
)

# ---- Optional app icon resources (enabled only when files exist) ----
set(APP_RC_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
set(APP_ICO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.ico")
set(APP_QRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.qrc")
set(APP_PNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.png")

if (WIN32 AND EXISTS "${APP_RC_PATH}" AND EXISTS "${APP_ICO_PATH}")
    target_sources(${APP_NAME} PRIVATE "${APP_RC_PATH}")
endif()
# -------------------------------------------------------------------

# ---- Qt6 (optional but pre-wired) ----
if (ENABLE_QT)
    find_package(Qt6 QUIET COMPONENTS Widgets)
else()
    set(Qt6_FOUND OFF)
endif()

if (Qt6_FOUND)
    target_link_libraries(${APP_NAME} PRIVATE Qt6::Widgets)
    target_compile_definitions(${APP_NAME} PRIVATE QT_NO_KEYWORDS)

    # Only add qrc when it exists (and only useful if the app actually uses it)
    if (EXISTS "${APP_QRC_PATH}" AND EXISTS "${APP_PNG_PATH}")
        set_target_properties(${APP_NAME} PROPERTIES AUTORCC ON)
        target_sources(${APP_NAME} PRIVATE "${APP_QRC_PATH}")
    endif()
endif()
# --------------------------------------

if (BUILD_TESTING)
    find_package(doctest CONFIG QUIET)
    if (NOT TARGET doctest::doctest)
        if (ENABLE_FETCH_TEST_DEPS)
            include(FetchContent)
            set(DOCTEST_WITH_TESTS OFF CACHE BOOL "" FORCE)
            set(DOCTEST_NO_INSTALL ON CACHE BOOL "" FORCE)
            FetchContent_Declare(doctest
                URL https://github.com/doctest/doctest/archive/refs/tags/v2.4.12.tar.gz
                DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            )
            FetchContent_MakeAvailable(doctest)
        else()
            message(FATAL_ERROR "doctest was not found. Install doctest or set ENABLE_FETCH_TEST_DEPS=ON.")
        endif()
    endif()

    add_executable(${TEST_TARGET}
        tests/test_main.cpp
        tests/core_tests.cpp
        tests/string_tests.cpp
    )
    target_link_libraries(${TEST_TARGET} PRIVATE ${CORE_TARGET} doctest::doctest)
    target_compile_features(${TEST_TARGET} PRIVATE cxx_std_23)
    project_apply_default_build_options(${TEST_TARGET})

    add_test(NAME ${APP_NAME}.core COMMAND ${TEST_TARGET})
endif()

project_add_clang_tool_targets(
    FORMAT_SOURCES ${PROJECT_FORMAT_SOURCES}
    LINT_SOURCES ${PROJECT_LINT_SOURCES}
)

install(TARGETS ${CORE_TARGET} ${APP_NAME}
    EXPORT ${APP_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${APP_NAME}Targets
    FILE ${APP_NAME}Targets.cmake
    NAMESPACE ${APP_NAME}::
    DESTINATION ${PACKAGE_INSTALL_DIR}
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${APP_NAME}Config.cmake
    INSTALL_DESTINATION ${PACKAGE_INSTALL_DIR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${APP_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${APP_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${APP_NAME}ConfigVersion.cmake
    DESTINATION ${PACKAGE_INSTALL_DIR}
)
