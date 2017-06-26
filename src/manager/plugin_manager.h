#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H
#include <string>
#include <memory>
#include <map>
#include <plugin_api.h>

class PluginManagerPrivate;

class PluginManager {
public:
    PluginManager();
    void register_module(std::shared_ptr<Module> module);
    void register_dynamic(const std::string &file_name);
    void register_script(const std::string &name, double version, const std::string &file_name);

    void execute(const std::string &name, std::shared_ptr<ModuleContext> context);
    bool contains(const std::string &name) const;
private:
    std::shared_ptr<PluginManagerPrivate> d;
};

#endif //PLUGIN_MANAGER_H
