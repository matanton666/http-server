# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HttpServer VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)




# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create a library with your code
file(GLOB SOURCE_FILES src/mods/*.c inculde/*.h)
add_library(${PROJECT_NAME}_lib
    ${SOURCE_FILES}
)

# Add main executable
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

# Enable testing;
enable_testing()

# Add tests
add_subdirectory(tests)
