idf_component_register(SRCS "HttpClient.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES lamp-core interop esp_http_client)
                       
set_source_files_properties(HttpClient.cpp PROPERTIES COMPILE_FLAGS -std=c++14)