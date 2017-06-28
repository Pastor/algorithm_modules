#ifndef ALGORITHM_SQLITE_CONTEXT_H
#define ALGORITHM_SQLITE_CONTEXT_H

#include <plugin_api.h>
#include <sql_module.h>

extern "C" {
bool MODULE_LIBRARY_API plugin_call(ModuleContext *context);
MODULE_LIBRARY_API const char *plugin_name();
double MODULE_LIBRARY_API plugin_version();
PluginSpec::ModuleType MODULE_LIBRARY_API plugin_type();
PluginSpec::ModuleStage MODULE_LIBRARY_API plugin_stage();
}

#endif //ALGORITHM_SQLITE_CONTEXT_H
