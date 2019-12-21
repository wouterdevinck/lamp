# https://github.com/MicrochipTech/cryptoauthlib/issues/5#issuecomment-433541966

CRYPTOAUTHLIB_DIR := ../cryptoauthlib/cryptoauthlib/lib

COMPONENT_SRCDIRS := . $(CRYPTOAUTHLIB_DIR)/hal

COMPONENT_OBJS := $(CRYPTOAUTHLIB_DIR)/hal/atca_hal.o \
                  $(CRYPTOAUTHLIB_DIR)/hal/hal_freertos.o \
				  $(CRYPTOAUTHLIB_DIR)/hal/hal_esp32_timer.o \
				  hal_esp32_i2c.o 

# I2C pins are hardcoded in $(CRYPTOAUTHLIB_DIR)/hal/hal_esp32_i2c.c
# to 16 (sda) and 17 (scl) instead of 20/21, so using our own HAL instead.

COMPONENT_ADD_INCLUDEDIRS := ../platform $(CRYPTOAUTHLIB_DIR)

CFLAGS += -DESP32 -DATCA_HAL_I2C -DATCA_PRINTF -DATCA_USE_RTOS_TIMER
CFLAGS += -Wno-unused-but-set-variable -Wno-unused-variable

cryptoauthlib/cryptoauthlib/lib/hal/hal_freertos.o: CFLAGS+= -I$(IDF_PATH)/components/freertos/include/freertos