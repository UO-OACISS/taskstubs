
#ifndef TASKTIMER_PLUGIN_ENVIRONMENT_VARIABLE_NAME
#define TASKTIMER_PLUGIN_ENVIRONMENT_VARIABLE_NAME "TASKTIMER_EXTERNAL_TOOL_LIBRARY"
#endif

#define GET_SYMBOL(name) (get_symbol(#name))

void* get_library(void);
void* get_symbol(const char * symbol);

