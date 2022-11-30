add_executable( main main.c
                calc.c
                menu.c
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c )
target_link_libraries(main libcurl m)
add_subdirectory(api)
configure_file(settings.conf settings.conf COPYONLY)

add_executable(settings settings.c)


# Testing
add_executable(test_bargain test_bargain.c bargain.c stores.c menu.c items.c menu.c test_items.c items.c test_bargain.c)
target_link_libraries(test_bargain m)
add_test(test_bargain test_bargain)

add_executable(calc_test calc_test.c calc.c)
target_link_libraries(calc_test m)
add_test(NAME calc_test COMMAND calc_test)

