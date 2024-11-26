cmake_minimum_required(VERSION 3.16)
project(honors_project VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set Qt path
set(CMAKE_PREFIX_PATH "/Users/darlyngomez/Qt/6.6.3/macos")

# Find the required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql Network)

# Add src subdirectory
add_subdirectory(src)

# Specify the executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Link against our library and Qt
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        src_lib
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        Qt6::Network
)

# Include directories for the main executable
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)