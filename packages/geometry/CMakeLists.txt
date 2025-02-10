project(Geometry CXX)

add_library(Geometry
    src/Utils.cpp
    src/Vector2.cpp
    src/Vector3.cpp)
add_library(Geometry::Geometry ALIAS Geometry)

target_include_directories(
    Geometry PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

target_link_libraries(
    Geometry)

install(
    TARGETS Geometry
    EXPORT GeometryConfig
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib
    INCLUDES
    DESTINATION include)
install(
    EXPORT GeometryConfig
    FILE GeometryConfig.cmake
    NAMESPACE Geometry::
    DESTINATION lib/cmake/Geometry)
install(FILES include/Vector2.h DESTINATION include)