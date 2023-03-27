#include "logging.h"

enum us_log_level_t us_g_log_level;

bool us_g_log_colored;

pthread_mutex_t us_g_log_mutex;
