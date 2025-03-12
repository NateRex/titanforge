project(Common CXX)

# Library files
add_library(Common
    src/exceptions/IllegalArgumentException.cpp
    src/exceptions/IllegalStateException.cpp
    src/exceptions/InstantiationException.cpp
    src/exceptions/NullPointerException.cpp
    src/exceptions/OutOfBoundsException.cpp
    src/Assertions.cpp
    src/Utils.cpp)
add_library(Common::Common ALIAS Common)

# Include directories
target_include_directories(
    Common PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

# Linked libraries
target_link_libraries(
    Common)

# Outputs
set_target_properties(Common PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/bin"
)