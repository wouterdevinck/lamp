idf_component_register(SRCS "HttpServer.cpp" "mongoose.c"
                       INCLUDE_DIRS "."
                       REQUIRES lamp-core)

add_definitions(-DCS_PLATFORM=15 -DMG_DISABLE_DIRECTORY_LISTING=1 -DMG_DISABLE_DAV=1 -DMG_DISABLE_CGI=1)