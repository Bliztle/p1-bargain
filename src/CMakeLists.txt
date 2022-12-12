add_executable( main main.c
                calc.c
                menu.c
                basket.c
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c
                settings.c
                menu.c
                config.c)

target_link_libraries(main libcurl m)
add_subdirectory(api)
configure_file(settings.conf settings.conf COPYONLY)
configure_file(settings.conf api/settings.conf COPYONLY)

# Testing
add_executable(test_items test_items.c items.c)
target_link_libraries(test_items m)
add_test(NAME test_items COMMAND test_items)
add_test(NAME test_config COMMAND test_config)
add_executable( test_config test_config.c
                calc.c
                menu.c
                basket.c
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c
                settings.c
                menu.c
                config.c
)
target_link_libraries(test_config libcurl m)
add_executable(calc_test calc_test.c calc.c)
target_link_libraries(calc_test m)
add_test(NAME calc_test COMMAND calc_test)
add_executable( test_settings 
                test_settings.c
                settings.c 
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c
                menu.c
                calc.c
                config.c)
target_link_libraries(test_settings libcurl m)
add_test(NAME test_settings COMMAND test_settings)
add_executable(basket_test basket_test.c basket.c menu.c api/parse.c api/nxjson/nxjson.c)
target_link_libraries(basket_test m)
add_test(NAME basket_test COMMAND basket_test)
