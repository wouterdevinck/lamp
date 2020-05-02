# Configure the mbedtls component from esp-idf to use the 
# ATECC608A hardware implementations of ECDH and ECDSA 
# rather than the built-in sofware implementations.

# Diffie-Hellman
add_definitions(-DMBEDTLS_ECDH_GEN_PUBLIC_ALT)
add_definitions(-DMBEDTLS_ECDH_COMPUTE_SHARED_ALT)

# Signatures
add_definitions(-DMBEDTLS_ECDSA_SIGN_ALT)
add_definitions(-DMBEDTLS_ECDSA_VERIFY_ALT)