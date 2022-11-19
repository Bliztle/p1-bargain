add_executable( fetch fetch_main.c
                fetch.c
                parse.c
                nxjson/nxjson.c )
target_link_libraries(fetch libcurl)
configure_file(tokens.conf ../tokens.conf COPYONLY)