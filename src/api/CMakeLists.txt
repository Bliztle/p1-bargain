configure_file(tokens.conf ../tokens.conf COPYONLY)

# Tests
add_executable( fetch_test fetch_test.c 
                fetch.c
                parse.c
                nxjson/nxjson.c )
target_link_libraries(fetch_test libcurl)
add_test(NAME fetch_test COMMAND fetch_test)

add_executable( parse_test parse_test.c 
                parse.c
                nxjson/nxjson.c)
target_link_libraries(parse_test libcurl m)
add_test(NAME parse_test COMMAND parse_test)