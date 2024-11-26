cmake_minimum_required(VERSION 3.16)
project(honors_project VERSION 1.0.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)


# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)


# Source files
set(SOURCES
   src/main.cpp
   src/auth/authenticator.cpp
   src/auth/email_validator.cpp
   src/auth/user_session.cpp
   src/ui/login_window.cpp
   src/ui/registration_window.cpp
)


# Header files
set(HEADERS
   include/auth/authenticator.h
   include/auth/email_validator.h
   include/auth/user_session.h
   include/ui/login_window.h
   include/ui/registration_window.h
)


# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
   Qt6::Core
   Qt6::Widgets
)

