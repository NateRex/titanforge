project(SampleApp CXX)

add_executable(SampleApp src/main.cpp)

target_link_libraries(SampleApp
    PRIVATE Geometry::Geometry)

install(TARGETS SampleApp DESTINATION bin)
