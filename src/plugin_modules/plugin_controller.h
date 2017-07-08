#ifndef ALGORITHM_MODULE_CONTROLLER_H
#define ALGORITHM_MODULE_CONTROLLER_H
#include <plugin_api.h>
#include <sql_module.h>

struct ModuleControllerPrivate;

class MODULE_LIBRARY_API ModuleController final {
    std::shared_ptr<ModuleControllerPrivate> d;
public:
    ModuleController(const std::shared_ptr<ModuleContext> &context);

    void execute(const PluginSpec &spec, std::shared_ptr<ModuleContext> context);
    void load(const std::string &file_path);
};

#endif //ALGORITHM_MODULE_CONTROLLER_H
