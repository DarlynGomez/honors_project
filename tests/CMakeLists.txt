cmake_minimum_required(VERSION 3.16)

# Find required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Sql)

# Create test executable
add_executable(db_test database_test.cpp)

# Link Qt libraries
target_link_libraries(db_test PRIVATE
    Qt6::Core
    Qt6::Sql
)

# Set output directory
set_target_properties(db_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)