#ifndef ALGORITHM_DYNAMIC_LIBRARY_MODULE_H
#define ALGORITHM_DYNAMIC_LIBRARY_MODULE_H
#include <string>
#include <memory>
#include <plugin_api.h>

class DynamicLibraryModulePrivate;

class DynamicLibraryModule final: public Module {
public:
    DynamicLibraryModule(const std::string &library_path);
    virtual ~DynamicLibraryModule();

    bool execute(std::shared_ptr<ModuleContext> context) override ;
    bool is_loaded() const;

    std::string name() const override;
    double version() const override;
private:
    std::shared_ptr<DynamicLibraryModulePrivate> d;
};

#endif //ALGORITHM_DYNAMIC_LIBRARY_MODULE_H
