cmake_minimum_required(VERSION 3.13.4) # CMake 最低版本号要求

option(USE_CLANG "use clang as compiler" true)

if (USE_CLANG) 
  set(CMAKE_ASM_COMPILER "clang") 
  set(CMAKE_C_COMPILER "clang") 
  set(CMAKE_CXX_COMPILER "clang++") 
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall -O0") # -save-temps 保存编译中间文件

if (USE_CLANG) 
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-variable -Wno-unused-function -Wno-unused-private-field -fno-limit-debug-info")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++") # gnu libstdc++  clang libc++
  add_compile_options(
      -fsanitize=address
      -fno-omit-frame-pointer
      -fno-optimize-sibling-calls
  )
  add_link_options(-fsanitize=address) # 内存泄漏检测不能和gdb公用，在退出前必须gdb detach
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines -fexceptions") # -fno-exceptions
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-02 -DNDEBUG")

set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

project(recipes C CXX) # 项目名

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 这个设置放到 project() 命令之后才生效

message(STATUS "${PROJECT_SOURCE_DIR}")

include_directories(
  "${PROJECT_SOURCE_DIR}/include/fmt/include/"
  "${PROJECT_SOURCE_DIR}/recipes/include/"
)

link_directories(
  "${PROJECT_SOURCE_DIR}/libs/"
)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/arc")

add_subdirectory(recipes)

# MESSAGE(STATUS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ")
