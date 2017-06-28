#ifndef ALGORITHM_MODULE_CONTROLLER_H
#define ALGORITHM_MODULE_CONTROLLER_H
#include <plugin_api.h>
#include <sql_module.h>

class MODULE_LIBRARY_API ModuleController final {
public:
    void execute(const PluginSpec &spec, std::shared_ptr<ModuleContext> context);
};

#endif //ALGORITHM_MODULE_CONTROLLER_H
