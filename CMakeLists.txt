cmake_minimum_required(VERSION 3.16)
project(honors_project VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create assets directory in build
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)

# Source files
set(SOURCES
    src/main.cpp
    src/auth/authenticator.cpp
    src/auth/email_validator.cpp
    src/auth/user_session.cpp
    src/ui/login_window.cpp
    src/ui/registration_window.cpp
    src/ui/mainshop_window.cpp
    src/ui/textbook_page.cpp         # Added
    src/database/database_manager.cpp # Added
    src/database/textbook.cpp        # Added
)

# Header files
set(HEADERS
    include/auth/authenticator.h
    include/auth/email_validator.h
    include/auth/user_session.h
    include/ui/login_window.h
    include/ui/registration_window.h
    include/ui/mainshop_window.h
    include/ui/textbook_page.h       # Added
    include/database/database_manager.h # Added
    include/database/textbook.h        # Added
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Copy assets to build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
)