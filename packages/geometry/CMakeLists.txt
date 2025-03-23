project(Geometry CXX)

# Library files
add_library(Geometry
    src/Matrix3.cpp
    src/Matrix4.cpp
    src/Vector2.cpp
    src/Vector3.cpp)
add_library(Geometry::Geometry ALIAS Geometry)

# Include directories
target_include_directories(
    Geometry PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

# Linked libraries
target_link_libraries(Geometry
    PRIVATE Common::Common)

# Outputs
set_target_properties(Geometry PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/bin"
)