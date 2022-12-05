add_executable( main main.c
                calc.c
                menu.c
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c
                config.c )
target_link_libraries(main libcurl m)
add_subdirectory(api)
configure_file(settings.conf settings.conf COPYONLY)

add_executable(settings settings.c)

# Testing
add_executable(test_items test_items.c items.c)
target_link_libraries(test_items m)
add_test(NAME test_items COMMAND test_items)
add_test(NAME test_config COMMAND test_config)
add_executable(test_config test_config.c config.c)
add_executable(calc_test calc_test.c calc.c)
target_link_libraries(calc_test m)
add_test(NAME calc_test COMMAND calc_test)

