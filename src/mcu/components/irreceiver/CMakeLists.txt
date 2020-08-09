idf_component_register(SRCS "IrReceiver.cpp"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES driver constants
                       REQUIRES lamp-core)