cmake_minimum_required (VERSION 3.1)
project (infinite)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(vendor/libtwittercpp)
add_subdirectory(vendor/verbly)
add_subdirectory(vendor/yaml-cpp EXCLUDE_FROM_ALL)

find_package(PkgConfig)
pkg_check_modules(sqlite3 sqlite3>=3.8.3 REQUIRED)
pkg_check_modules(GraphicsMagick GraphicsMagick++ REQUIRED)

include_directories(${GraphicsMagick_INCLUDE_DIRS} ${sqlite3_INCLUDE_DIR} vendor/verbly/lib vendor/libtwittercpp/src vendor/yaml-cpp/include vendor/libtwittercpp/vendor/json/src vendor/libtwittercpp/vendor/curlcpp/include)
link_directories(${sqlite3_LIBRARY_DIRS})
add_executable(infinite infinite.cpp color.cpp fractal.cpp matrix3x3.cpp vector3d.cpp tinyxml2.cpp triangle.cpp)
set_property(TARGET infinite PROPERTY CXX_STANDARD 11)
set_property(TARGET infinite PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(infinite ${GraphicsMagick_LIBRARIES} verbly ${sqlite3_LIBRARIES} yaml-cpp twitter++ curlcpp)
