add_executable( main main.c
                api/fetch.c )
target_link_libraries(main libcurl)