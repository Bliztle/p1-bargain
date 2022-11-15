add_executable(main main.c)
target_link_libraries(main fetch)

# set(CURL_DIR ../build/libraries/curl-7.86.0/generated)
# find_package(CURL CONFIG REQUIRED)
# target_link_libraries(main PRIVATE CURL::libcurl)