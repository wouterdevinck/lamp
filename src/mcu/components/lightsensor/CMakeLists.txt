idf_component_register(SRCS "LightSensor.cpp" "TSL2591.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES lamp-core
                       PRIV_REQUIRES i2c constants)