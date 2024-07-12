cmake_minimum_required(VERSION 3.10)

project(CS2Cht VERSION 1.0)

# Set C++ standard

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(IMGUI_DIR "/media/jerryyy/Jerryyy/CS2Cht/include/imgui")

#IMGUI-things
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/include/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

set(LIBRARIES "glfw")

include_directories(${GLFW_DIR}/deps)
file(GLOB SOURCES *.cpp)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# GLFW
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/include/glfw) # Set this to point to an up-to-date GLFW repo
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

# Define your source files
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/memoryread.cpp
    ${SRC_DIR}/renderer/imguimain.cpp
)

# Platform-specific configurations
if(WIN32)
    # Windows specific libraries or settings
    message(STATUS "Configuring for Windows")
    # Define your Windows-specific source files if any
    #list(APPEND SOURCES windows_specific.cpp)
elseif(UNIX)
    # UNIX specific libraries or settings (Linux, macOS, etc.)
    message(STATUS "Configuring for UNIX (Linux, macOS, etc.)")
    # Define your UNIX-specific source files if any
    #list(APPEND SOURCES unix_specific.cpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

if (UNIX AND NOT APPLE)
    message("Linux")
    find_package(PkgConfig REQUIRED)
    find_package(OpenGL REQUIRED)

    pkg_check_modules(GLFW REQUIRED glfw3)
elseif (WIN32)
    message("MinGW")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    find_package(OpenGL REQUIRED)
endif()

# Add executable target
add_executable(CS2Cht ${SOURCES} ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
target_compile_definitions(CS2Cht PUBLIC -DImTextureID=ImU64)
# Additional platform-specific settings or libraries
if(WIN32)
    # Link any Windows-specific libraries if needed
    target_link_libraries(CS2Cht ${LIBRARIES} ${OPENGL_LIBRARY})
elseif(UNIX)
    # Link any UNIX-specific libraries if needed (e.g., dl for Linux)
    target_link_libraries(CS2Cht dl ${LIBRARIES} ${OPENGL_LIBRARY})
endif()

# Include the toolchain file for cross-compilation
#if(WIN32)
#    include(toolchain-mingw.cmake)
#endif()
