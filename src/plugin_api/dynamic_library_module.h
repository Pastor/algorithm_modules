#ifndef ALGORITHM_DYNAMIC_LIBRARY_MODULE_H
#define ALGORITHM_DYNAMIC_LIBRARY_MODULE_H
#include <string>
#include <memory>
#include <plugin_api.h>

struct DynamicLibraryModulePrivate;

class LIBRARY_API DynamicLibraryModule final: public Module {
public:
    DynamicLibraryModule(const std::string &library_path);
    virtual ~DynamicLibraryModule();

    bool execute(std::shared_ptr<ModuleContext> context) override ;
    bool is_loaded() const;

    virtual const PluginSpec &spec() const;
private:
    std::shared_ptr<DynamicLibraryModulePrivate> d;
};

#endif //ALGORITHM_DYNAMIC_LIBRARY_MODULE_H
