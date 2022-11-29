add_executable( main main.c
                calc.c
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c )
target_link_libraries(main libcurl m)
add_subdirectory(api)
add_subdirectory(bargain)
configure_file(settings.conf settings.conf COPYONLY)

add_executable(settings settings.c)

add_executable(bargain bargain/bargain.c stores.c menu.c items.c)
target_link_libraries(bargain m)

# Testing
add_executable(test_items test_items.c items.c)
target_link_libraries(test_items m)
add_test(NAME test_items COMMAND test_items)
add_executable(calc_test calc_test.c calc.c)
target_link_libraries(calc_test m)
add_test(NAME calc_test COMMAND calc_test)

