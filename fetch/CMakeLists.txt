message("-- Linking fetch / curl")
add_library(fetch libcurl.dll)
set_target_properties(fetch PROPERTIES LINKER_LANGUAGE C)