idf_component_register(SRCS "Spi.cpp" "SpiFlash.cpp"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES spi_flash)