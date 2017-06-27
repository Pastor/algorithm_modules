#ifndef ALGORITHM_SQLITE_CONTEXT_H
#define ALGORITHM_SQLITE_CONTEXT_H
#include <plugin_api.h>
#include <sql_module.h>

extern "C" {
CONTEXT_LIBRARY_API bool plugin_call(ModuleContext *context);
CONTEXT_LIBRARY_API const char *plugin_name();
CONTEXT_LIBRARY_API double plugin_version();
}

#endif //ALGORITHM_SQLITE_CONTEXT_H
