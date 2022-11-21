typedef enum
{
    CONFIG_SUCCESS,
    CONFIG_FAILLURE
} config_status_e;

typedef enum
{
    SETTINGS,
    TOKENS,
    CONFIG_FILE_E_LENGTH
} config_files_e;

typedef enum
{
    ADDRESS,
    COORDS,
    DEVIANCE,
    DISTANCE,
    PATH,
    CONFIG_SETTINGS_E_LENGTH
} config_settings_e;

typedef enum
{
    SALLING,
    COOP,
    CONFIG_TOKENS_E_LENGTH
} config_tokens_e;

static const char *file_map[CONFIG_FILE_E_LENGTH];
static const char *settings_map[CONFIG_SETTINGS_E_LENGTH];
static const char *tokens_map[CONFIG_TOKENS_E_LENGTH];

config_status_e config_write_value(char *file_path, char *key, char *value);
config_status_e config_write_setting(config_settings_e key, char *value);
config_status_e config_write_token(config_tokens_e key, char *value);