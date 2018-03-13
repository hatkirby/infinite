cmake_minimum_required (VERSION 3.1)
project (infinite)

add_subdirectory(vendor/libtwittercpp)
add_subdirectory(vendor/verbly)

find_package(PkgConfig)
pkg_check_modules(GraphicsMagick GraphicsMagick++ REQUIRED)
pkg_check_modules(yaml-cpp yaml-cpp REQUIRED)

include_directories(
  ${GraphicsMagick_INCLUDE_DIRS}
  vendor/verbly/lib
  vendor/libtwittercpp/src
  ${yaml-cpp_INCLUDE_DIRS})

link_directories(${GraphicsMagick_LIBRARY_DIRS})
add_executable(infinite infinite.cpp color.cpp fractal.cpp matrix3x3.cpp vector3d.cpp tinyxml2.cpp triangle.cpp)
set_property(TARGET infinite PROPERTY CXX_STANDARD 11)
set_property(TARGET infinite PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(infinite ${GraphicsMagick_LIBRARIES} verbly ${yaml-cpp_LIBRARIES} twitter++)
