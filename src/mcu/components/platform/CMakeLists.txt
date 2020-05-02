idf_component_register(SRCS "Platform.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES lamp-core httpclient httpserver irreceiver 
                                ledboardchain lightsensor logger rgbled 
                                storage updater wificlient azureiot spi
                       PRIV_REQUIRES constants memmon)