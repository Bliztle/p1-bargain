# add_subdirectory(curl-7.86.0)
message("-- Linking librarie folder")
add_subdirectory(curl-t-2)
include_directories(curl-t-2)