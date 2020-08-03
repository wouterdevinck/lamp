idf_component_register(SRCS "Updater.cpp"
    INCLUDE_DIRS "." "${CMAKE_CURRENT_BINARY_DIR}"
    REQUIRES lamp-core app_update spi storage
    EMBED_FILES ${COMPONENT_DIR}/../../../../build/fpga/lamp.bin)

get_filename_component(version_header
    "version.h"
    ABSOLUTE BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(OUTPUT "${version_header}"
    COMMAND ${COMPONENT_DIR}/genver.sh "${COMPONENT_DIR}/../../../../build/fpga/lamp.hash" "${version_header}"
    VERBATIM)

add_custom_target(version DEPENDS "${version_header}")
add_dependencies("${COMPONENT_LIB}" version)

set_property(DIRECTORY "${COMPONENT_DIR}" APPEND PROPERTY
    ADDITIONAL_MAKE_CLEAN_FILES version.h)