add_executable( main main.c
                api/fetch.c api/fetch.h)
target_link_libraries(main libcurl)