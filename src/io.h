typedef enum
{
    CONFIG_SUCCESS,
    CONFIG_FAILLURE
} config_status_e;

config_status_e config_write_value(char *file_path, char *key, char *value);