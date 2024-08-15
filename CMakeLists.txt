# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HttpServer VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_GENERATOR "Unix Makefiles")



# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Add debug symbols
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Disable optimizations for Debug build
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# # If using GCC or Clang, you can add sanitizers
# if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
#   # Check if ASan is available
#   include(CheckCSourceRuns)
#   set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
#   check_c_source_runs("int main() { return 0; }" HAVE_ASAN)
#   set(CMAKE_REQUIRED_FLAGS "")

#   if(HAVE_ASAN)
#     set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
#     set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
#     set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
#     message(STATUS "AddressSanitizer enabled")
#   else()
#     message(WARNING "AddressSanitizer is not available")
#   endif()
# endif()


include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create a library with your code
file(GLOB SOURCE_FILES src/mods/*.c inculde/*.h)
add_library(${PROJECT_NAME}_lib
    ${SOURCE_FILES}
)

# Add main executable
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)



enable_testing()

# Add tests
add_subdirectory(tests)
