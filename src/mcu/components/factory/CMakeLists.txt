idf_component_register(SRCS "Factory.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES cryptoauthlib constants nvs_flash)

# Because of https://github.com/MicrochipTech/cryptoauthlib/issues/43
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")