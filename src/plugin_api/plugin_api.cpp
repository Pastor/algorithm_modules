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

void
ModuleContext::toXml(tinyxml2::XMLElement *root, tinyxml2::XMLDocument &document) const {
    auto properties_xml = document.NewElement("Properties");
    root->InsertEndChild(properties_xml);
    for (auto it = _properties.begin(); it != _properties.end(); ++it) {
        auto property_xml = document.NewElement((*it).first.c_str());
        property_xml->SetText((*it).second.c_str());
        properties_xml->InsertEndChild(property_xml);
    }
}

void
ModuleContext::fromXml(const tinyxml2::XMLElement *properties_xml) {
    if (properties_xml == nullptr || std::strcmp(properties_xml->Name(), "Properties"))
        return;
    _properties.clear();
    for (auto node = properties_xml->FirstChild();
         node;
         node = node->NextSibling()) {
        const auto element = node->ToElement();
        if (element == nullptr)
            continue;
        const auto value = element->GetText();
        if (value != nullptr && std::strlen(value) > 0) {
            _properties[element->Name()] = value;
        }
        fprintf(stdout, "ModuleContext. %s = %s\n", element->Name(), value);
    }
}

void
ModuleContext::copy_to(ModuleContext &context) const {
    for (auto it = _properties.begin(); it != _properties.end(); ++it) {
        context.set_property((*it).first, (*it).second);
    }
}
