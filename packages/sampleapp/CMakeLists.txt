project(SampleApp CXX)

# Executable files
add_executable(SampleApp
    src/main.cpp)

# Linked libraries
target_link_libraries(SampleApp
    PRIVATE Common::Common
    PRIVATE Geometry::Geometry
    PRIVATE Graphics::Graphics)

# Outputs
set_target_properties(SampleApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin")