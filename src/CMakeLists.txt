add_executable( main main.c
                calc.c
                menu.c
                basket.c
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c 
                settings.c
                menu.c
                bargain.c 
                stores.c 
                items.c
                config.c
                mock_functions.c)

target_link_libraries(main libcurl m)
add_subdirectory(api)
configure_file(settings.conf settings.conf COPYONLY)
configure_file(settings.conf api/settings.conf COPYONLY)

add_executable(test_items 
               test_items.c 
               items.c 
               api/parse.c
               api/fetch.c 
               api/nxjson/nxjson.c
               config.c
               mock_functions.c 
               basket.c
               menu.c
               calc.c 
               settings.c)
target_link_libraries(test_items m libcurl)
add_test(NAME test_items COMMAND test_items)

add_executable(test_bargain 
               test_bargain.c bargain.c stores.c menu.c items.c menu.c items.c api/fetch.c api/parse.c api/nxjson/nxjson.c calc.c config.c basket.c mock_functions.c settings.c)
target_link_libraries(test_bargain m libcurl)
add_test(test_bargain test_bargain)

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
add_test(NAME test_config COMMAND test_config)

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
                config.c
                mock_functions.c 
                basket.c 
                menu.c)
target_link_libraries(test_settings libcurl m)
add_test(NAME test_settings COMMAND test_settings)

add_executable(basket_test basket_test.c basket.c menu.c api/parse.c api/nxjson/nxjson.c api/fetch.c config.c calc.c settings.c)
target_link_libraries(basket_test m libcurl)
add_test(NAME basket_test COMMAND basket_test)

add_executable( fetch_test 
                api/fetch_test.c 
                api/fetch.c
                api/parse.c
                api/nxjson/nxjson.c
                calc.c
                config.c 
                mock_functions.c
                basket.c
                menu.c
                settings.c)
target_link_libraries(fetch_test libcurl m)
add_test(NAME fetch_test COMMAND fetch_test)

add_executable( parse_test 
                api/parse_test.c 
                api/parse.c
                api/fetch.c
                calc.c
                config.c 
                api/nxjson/nxjson.c
                mock_functions.c
                basket.c
                menu.c
                settings.c)
target_link_libraries(parse_test libcurl m)
add_test(NAME parse_test COMMAND parse_test)