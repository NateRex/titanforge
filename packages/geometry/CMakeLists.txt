project(Geometry CXX)

add_library(Geometry
    src/Matrix3.cpp
    src/Vector2.cpp
    src/Vector3.cpp)
add_library(Geometry::Geometry ALIAS Geometry)

target_include_directories(
    Geometry PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

target_link_libraries(Geometry
    PRIVATE Common::Common)

install(
    TARGETS Geometry
    EXPORT GeometryConfig
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib
    INCLUDES
    DESTINATION include)