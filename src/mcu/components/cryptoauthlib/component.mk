# https://github.com/MicrochipTech/cryptoauthlib/issues/5#issuecomment-433541966

CRYPTOAUTHLIB_DIR := ./cryptoauthlib/lib

COMPONENT_SRCDIRS := $(CRYPTOAUTHLIB_DIR)/atcacert \
                     $(CRYPTOAUTHLIB_DIR)/basic \
                     $(CRYPTOAUTHLIB_DIR)/crypto \
                     $(CRYPTOAUTHLIB_DIR)/crypto/hashes \
                     $(CRYPTOAUTHLIB_DIR)/host \
                     $(CRYPTOAUTHLIB_DIR)/mbedtls \
                     $(CRYPTOAUTHLIB_DIR)

COMPONENT_ADD_INCLUDEDIRS := $(CRYPTOAUTHLIB_DIR)

CFLAGS += -DESP32 -DATCA_HAL_I2C -DATCA_PRINTF

CFLAGS += -Wno-error=char-subscripts