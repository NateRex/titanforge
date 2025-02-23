project(Common CXX)

# Library files
add_library(Common
    src/exceptions/IllegalArgumentException.cpp
    src/exceptions/OutOfBoundsException.cpp
    src/Utils.cpp)
add_library(Common::Common ALIAS Common)

# Include directories
target_include_directories(
    Common PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

# Linked libraries
target_link_libraries(
    Common)

# Library installation
install(
    TARGETS Common
    EXPORT CommonConfig
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib
    INCLUDES
    DESTINATION include)