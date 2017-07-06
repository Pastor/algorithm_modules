#ifndef ALGORITHM_SYSTEM_PROCESS_MODULE_H
#define ALGORITHM_SYSTEM_PROCESS_MODULE_H
#include <string>
#include <memory>
#include <plugin_api.h>

struct SystemProcessModulePrivate;

class API_LIBRARY_API SystemProcessModule final: public DefaultModule {
public:
    SystemProcessModule(const std::string &file_path,
                        const std::string &name,
                        const std::string &description,
                        double version);
    virtual ~SystemProcessModule();

    bool execute(std::shared_ptr<ModuleContext> context) override ;
private:
    std::shared_ptr<SystemProcessModulePrivate> d;
};
#endif //ALGORITHM_SYSTEM_PROCESS_MODULE_H
