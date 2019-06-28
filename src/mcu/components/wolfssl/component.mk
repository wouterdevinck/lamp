COMPONENT_ADD_INCLUDEDIRS := . ./wolfssl ./wolfssl/include
COMPONENT_EXTRA_INCLUDES += $(IDF_PATH)/components/freertos/include/freertos

CFLAGS += -DWOLFSSL_USER_SETTINGS 
#CFLAGS += -DWOLFSSL_ESPWROOM32SE
#CFLAGS += -DNO_MAIN_DRIVER
CFLAGS += -Wno-error=maybe-uninitialized

COMPONENT_SRCDIRS := wolfssl/src \
                     wolfssl/wolfcrypt/src \
                     wolfssl/wolfcrypt/src/port/Espressif \
                     wolfssl/wolfcrypt/src/port/atmel

COMPONENT_OBJEXCLUDE := wolfssl/wolfcrypt/src/aes_asm.o \
                        wolfssl/wolfcrypt/src/sha256_asm.o \
                        wolfssl/wolfcrypt/src/sha512_asm.o \
                        wolfssl/wolfcrypt/src/poly1305_asm.o \
                        wolfssl/wolfcrypt/src/chacha_asm.o \
                        wolfssl/wolfcrypt/src/sp_x86_64_asm.o \
                        wolfssl/wolfcrypt/src/aes_gcm_asm.o \
                        wolfssl/wolfcrypt/src/evp.o \
                        wolfssl/wolfcrypt/src/misc.o \
                        wolfssl/src/bio.o