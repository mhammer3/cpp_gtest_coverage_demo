cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES C CXX)

#
# --- Compiler Settings ---
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# --- Coverage optional ---
#
option(ENABLE_COVERAGE "Enable test coverage instrumentation" OFF)

if(ENABLE_COVERAGE)
    message(STATUS ">> Coverage enabled")

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        message(FATAL_ERROR "Coverage only supported with GCC/Clang")
    endif()
endif()

#
# --- GoogleTest lokal einbinden ---
#

# Der Pfad zu deiner lokalen googletest-Installation
set(GTEST_SOURCE_DIR "${CMAKE_SOURCE_DIR}/googletest/googletest")
set(GMOCK_SOURCE_DIR "${CMAKE_SOURCE_DIR}/googletest/googlemock") # optional, falls du GoogleMock nutzt

# GoogleTest als Bibliothek bauen
add_library(gtest STATIC
    ${GTEST_SOURCE_DIR}/src/gtest-all.cc
)
target_include_directories(gtest PUBLIC
    ${GTEST_SOURCE_DIR}/include
	${GTEST_SOURCE_DIR}
)

# gtest_main hinzufügen
add_library(gtest_main STATIC
    ${GTEST_SOURCE_DIR}/src/gtest_main.cc
)

target_link_libraries(gtest_main PUBLIC gtest)
target_include_directories(gtest_main PUBLIC
    ${GTEST_SOURCE_DIR}/include
)


# Optional: GoogleMock ebenfalls einbinden
add_library(gmock STATIC
    ${GMOCK_SOURCE_DIR}/src/gmock-all.cc
)
target_link_libraries(gmock PUBLIC gtest)
target_include_directories(gmock PUBLIC
    ${GMOCK_SOURCE_DIR}/include
	${GMOCK_SOURCE_DIR}
    ${GTEST_SOURCE_DIR}/include
)

#
# --- Dein Code ---
#
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    src/*.c
)
add_library(core STATIC ${PROJECT_SOURCES})
target_include_directories(core PUBLIC src)

#
# --- Tests ---
#
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
message("TEST_SOURCES = ${TEST_SOURCES}")
add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(unit_tests PRIVATE
	core
	gtest
	gtest_main
)

enable_testing()

include(GoogleTest)
gtest_discover_tests(unit_tests)

#
# --- Coverage Target ---
#
if(ENABLE_COVERAGE)
    find_program(GCOVR_EXECUTABLE gcovr)

    if (GCOVR_EXECUTABLE)
        add_custom_target(coverage
            COMMAND ${GCOVR_EXECUTABLE}
                --root ${CMAKE_SOURCE_DIR}
                --object-directory ${CMAKE_BINARY_DIR}
                --html --html-details
                -o coverage.html
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating coverage report via gcovr"
        )
    else()
        message(WARNING "gcovr not found — install via pip install gcovr")
    endif()
endif()