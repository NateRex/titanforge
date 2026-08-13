project(SampleApp CXX)

# Executable files
add_executable(SampleApp
    src/main.cpp)

# Linked libraries
target_link_libraries(SampleApp
    PRIVATE Common::Common
    PRIVATE Math::Math
    PRIVATE Graphics::Graphics)

# Outputs
set_target_properties(SampleApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
add_custom_target(SampleAppAssets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/bin/assets"
    COMMENT "Copying SampleApp assets"
    VERBATIM
)
add_dependencies(SampleApp SampleAppAssets)
