# -----------------------------------------------------------------------------------
# This CMake script is responsible for taking 
# -----------------------------------------------------------------------------------

if(NOT DEFINED INPUT_DIRECTORY OR NOT DEFINED OUTPUT_FILE)
    message(FATAL_ERROR "INPUT_DIRECTORY and OUTPUT_FILE are required")
endif()

set(FACES right left top bottom front back)
get_filename_component(OUTPUT_DIRECTORY "${OUTPUT_FILE}" DIRECTORY)
file(MAKE_DIRECTORY "${OUTPUT_DIRECTORY}")

set(CONTENTS "#pragma once\n#include <cstddef>\n\nnamespace DefaultSkyboxData\n{\n")
foreach(FACE IN LISTS FACES)
    set(INPUT_FILE "${INPUT_DIRECTORY}/${FACE}.jpg")
    if(NOT EXISTS "${INPUT_FILE}")
        message(FATAL_ERROR "Default skybox face not found: ${INPUT_FILE}")
    endif()

    file(READ "${INPUT_FILE}" HEX_DATA HEX)
    string(REGEX REPLACE "([0-9a-fA-F][0-9a-fA-F])" "0x\\1," BYTE_DATA "${HEX_DATA}")
    string(APPEND CONTENTS
        "inline constexpr unsigned char ${FACE}[] = {${BYTE_DATA}};\n"
        "inline constexpr std::size_t ${FACE}Size = sizeof(${FACE});\n")
endforeach()
string(APPEND CONTENTS "}\n")

file(WRITE "${OUTPUT_FILE}" "${CONTENTS}")
