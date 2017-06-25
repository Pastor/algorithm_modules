#ifndef ALGORITHM_PLUGIN_MODULE_H
#define ALGORITHM_PLUGIN_MODULE_H
#include <string>
#include <memory>
#include <map>

class ModuleContext {
    std::map<std::string, std::string> _properties;
public:
    virtual const std::string &property(const std::string &key);
    virtual void set_property(const std::string &key, const std::string &value);

    int int_value(const std::string &key);
    bool bool_value(const std::string &key);
};

class ScriptModule {
public:
    ScriptModule();

    virtual bool execute(std::shared_ptr<ModuleContext> context) = 0;
};

#endif //ALGORITHM_PLUGIN_MODULE_H
