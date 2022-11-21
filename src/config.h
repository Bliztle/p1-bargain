typedef enum
{
    CONFIG_SUCCESS,
    CONFIG_FAILLURE
} config_status_e;

typedef enum
{
    SETTINGS,
    TOKENS
} config_files_e;

typedef enum
{
    ADDRESS,
    COORDS,
    DEVIANCE,
    DISTANCE,
    PATH
} config_settings_e;

typedef enum
{
    SALLING,
    COOP
} config_tokens_e;

static const char *file_map[];
static const char *settings_map[];
static const char *tokens_map[];

config_status_e config_write_value(char *file_path, char *key, char *value);
config_status_e config_write_setting(config_settings_e key, char *value);
config_status_e config_write_token(config_tokens_e key, char *value);