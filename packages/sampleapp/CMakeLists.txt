project(SampleApp CXX)

# Executable files
add_executable(SampleApp src/main.cpp)

# Linked libraries
target_link_libraries(SampleApp
    PRIVATE Geometry::Geometry)

# Executable installation
install(TARGETS SampleApp DESTINATION bin)
