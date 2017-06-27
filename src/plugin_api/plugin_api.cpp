#include <string>
#include <plugin_api.h>

ScriptModule::ScriptModule(const std::string name, const std::string &description, double version) {
    _spec.plugin_name = name;
    _spec.plugin_type = PluginSpec::PythonScript;
    _spec.plugin_description = description;
    _spec.plugin_version = version;
}

ScriptModule::ScriptModule(const PluginSpec &spec)
        : _spec(spec) {}

const std::string &
ModuleContext::property(const std::string &key) {
    auto find = _properties.find(key);
    if (find == _properties.end() && _parent)
        return _parent->property(key);
    return (*find).second;
}

void
ModuleContext::set_property(const std::string &key, const std::string &value) {
    _properties[key] = value;
}

int
ModuleContext::int_value(const std::string &key) {
    return std::stoi(property(key), nullptr, 10);
}

bool
ModuleContext::bool_value(const std::string &key) {
    return property(key) == std::string("true");
}

ModuleContext::operator bool() const {
    return true;
}

ModuleContext::ModuleContext() {
}

ModuleContext::ModuleContext(std::shared_ptr<ModuleContext> parent)
        : _parent(parent) {
}

const std::map<std::string, std::string> &
ModuleContext::entries() const {
    return _properties;
}
