#ifndef ALGORITHM_PLUGIN_API_H
#define ALGORITHM_PLUGIN_API_H
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

    operator bool() const;
};

class Module {
public:
    Module() {}
    virtual ~Module() {}

    virtual std::string name() const = 0;
    virtual double version() const = 0;

    virtual bool execute(std::shared_ptr<ModuleContext> context) = 0;
};

class ScriptModule: public Module {
public:
    ScriptModule(const std::string name, double version);
    virtual ~ScriptModule() {}

    std::string name() const override {
        return _name;
    }
    double version() const override {
        return _version;
    }

private:
    const std::string _name;
    double _version;
};

#endif //ALGORITHM_PLUGIN_API_H
