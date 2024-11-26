# Define source files
set(SOURCES
   auth/email_validator.cpp
   auth/user_session.cpp
   main.cpp
)


set(HEADERS
   ${CMAKE_SOURCE_DIR}/include/auth/email_validator.h
   ${CMAKE_SOURCE_DIR}/include/auth/user_session.h
)


# Create library
add_library(src_lib STATIC ${SOURCES} ${HEADERS})


# Include directories for the library
target_include_directories(src_lib
   PUBLIC
       ${CMAKE_SOURCE_DIR}/include
       ${CMAKE_SOURCE_DIR}/src
)


# Link Qt libraries
target_link_libraries(src_lib
   PUBLIC
       Qt6::Core
       Qt6::Widgets
)

