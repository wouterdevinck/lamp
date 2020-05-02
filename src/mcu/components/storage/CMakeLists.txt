idf_component_register(SRCS "Storage.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES lamp-core nvs_flash
                       PRIV_REQUIRES constants)