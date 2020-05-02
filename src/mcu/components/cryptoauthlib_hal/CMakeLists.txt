# https://github.com/MicrochipTech/cryptoauthlib/issues/5#issuecomment-433541966

# I2C pins are hardcoded in $(CRYPTOAUTHLIB_DIR)/hal/hal_esp32_i2c.c
# to 16 (sda) and 17 (scl) instead of 20/21, so using our own HAL instead.

set (CRYPTOAUTHLIB_DIR "${CMAKE_CURRENT_LIST_DIR}/../cryptoauthlib/cryptoauthlib/lib")

set (srcs
    "${CRYPTOAUTHLIB_DIR}/hal/atca_hal.c"
    "${CRYPTOAUTHLIB_DIR}/hal/hal_freertos.c"
    "${CRYPTOAUTHLIB_DIR}/hal/hal_esp32_timer.c"
    "hal_esp32_i2c.c"
    )

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${CRYPTOAUTHLIB_DIR}"
                       REQUIRES i2c
                       PRIV_REQUIRES constants)
           
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-but-set-variable -Wno-unused-variable")
add_definitions(-DESP32 -DATCA_HAL_I2C -DATCA_PRINTF -DATCA_USE_RTOS_TIMER)

set_source_files_properties(
    "${CRYPTOAUTHLIB_DIR}/hal/hal_freertos.c"
    PROPERTIES COMPILE_FLAGS
    -I${IDF_PATH}/components/freertos/include/freertos)