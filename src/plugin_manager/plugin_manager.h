#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H
#include <string>
#include <memory>
#include <map>
#include <plugin_api.h>
#include <plugin_spec.h>

#if defined(EXPORT_MANAGER_LIBRARY)
#define MANAGER_LIBRARY_API __declspec(dllexport)
#else
#define MANAGER_LIBRARY_API __declspec(dllimport)
#endif

struct PluginManagerPrivate;

class MANAGER_LIBRARY_API PluginManager {
public:
    PluginManager();

    void load(const std::string &file_name);

    void register_module(const PluginSpec &spec);
    void register_module(std::shared_ptr<Module> module);
    void register_dynamic(const std::string &file_name);
    void register_script(const std::string &name, const std::string &description,
                         double version, const std::string &file_name);

    void execute(const std::string &name, std::shared_ptr<ModuleContext> context);
    bool contains(const std::string &name) const;
private:
    std::shared_ptr<PluginManagerPrivate> d;
};

#endif //PLUGIN_MANAGER_H
