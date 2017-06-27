#ifndef ALGORITHM_PLUGIN_API_H
#define ALGORITHM_PLUGIN_API_H

#include <string>
#include <memory>
#include <map>
#include <plugin_spec.h>

class API_LIBRARY_API ModuleContext {
    std::map<std::string, std::string> _properties;
    std::shared_ptr<ModuleContext> _parent;
public:
    ModuleContext();

    ModuleContext(std::shared_ptr<ModuleContext> parent);

    virtual ~ModuleContext() {}

    virtual const std::string &property(const std::string &key);

    virtual void set_property(const std::string &key, const std::string &value);

    int int_value(const std::string &key);

    bool bool_value(const std::string &key);

    virtual bool is_database_context() const { return false; }

    operator bool() const;
};

class API_LIBRARY_API Module {
public:
    Module() {}

    virtual ~Module() {}

    virtual const PluginSpec &spec() const = 0;

    virtual bool execute(std::shared_ptr<ModuleContext> context) = 0;
};

class API_LIBRARY_API ScriptModule : public Module {
public:
    ScriptModule(const PluginSpec &spec);

    ScriptModule(const std::string name, const std::string &description, double version);

    virtual ~ScriptModule() {}

    virtual const PluginSpec &spec() const {
        return _spec;
    }

private:
    PluginSpec _spec;
};

#endif //ALGORITHM_PLUGIN_API_H
