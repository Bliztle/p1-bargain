add_executable( main main.c
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c )
target_link_libraries(main libcurl)
add_subdirectory(api)
configure_file(settings.conf settings.conf COPYONLY)

add_executable(config config.c)