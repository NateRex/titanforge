project(Common CXX)

add_library(Common
    src/exceptions/IllegalArgumentException.cpp
    src/Utils.cpp)
add_library(Common::Common ALIAS Common)

target_include_directories(
    Common PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

target_link_libraries(
    Common)

install(
    TARGETS Common
    EXPORT CommonConfig
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib
    INCLUDES
    DESTINATION include)