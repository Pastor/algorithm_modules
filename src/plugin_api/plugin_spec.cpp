#include <plugin_spec.h>
#include <plugin_api.h>

static PluginSpec::ModuleType
parse_type(const char *type) {
    if (type != nullptr && std::strcmp(type, "DynamicLibrary") == 0)
        return PluginSpec::DynamicLibrary;
    if (type != nullptr && std::strcmp(type, "PythonScript") == 0)
        return PluginSpec::PythonScript;
    return PluginSpec::UnknownModule;
}

static const char *
to_type(PluginSpec::ModuleType type) {
    switch (type) {
        case PluginSpec::DynamicLibrary:
            return "DynamicLibrary";
        case PluginSpec::PythonScript:
            return "PythonScript";
        default:
            return "UnknownModule";
    }
}

static PluginSpec::ModuleStage
parse_stage(const char *type) {
    if (type != nullptr && std::strcmp(type, "FirstInput") == 0)
        return PluginSpec::FirstInput;
    if (type != nullptr && std::strcmp(type, "ProcessingInput") == 0)
        return PluginSpec::ProcessingInput;
    return PluginSpec::UnknownInput;
}

static const char *
to_stage(PluginSpec::ModuleStage type) {
    switch (type) {
        case PluginSpec::FirstInput:
            return "FirstInput";
        case PluginSpec::ProcessingInput:
            return "ProcessingInput";
        default:
            return "UnknownInput";
    }
}

static const char *
child_element_value(const tinyxml2::XMLElement *element, const char *name) {
    auto child_element = element->FirstChildElement(name);
    if (child_element == nullptr)
        return nullptr;
    return child_element->GetText();
}

static double
child_element_double(const tinyxml2::XMLElement *element, const char *name) {
    auto child_element = element->FirstChildElement(name);
    if (child_element == nullptr)
        return 0.0;
    return child_element->DoubleText(0.0);
}

void
PluginSpec::toXml(tinyxml2::XMLElement *root, tinyxml2::XMLDocument &document) const {
    auto plugin_spec = document.NewElement("PluginSpec");
    auto plugin_name_text = document.NewElement("Name");
    plugin_name_text->SetText(plugin_name.c_str());
    auto plugin_file_path_text = document.NewElement("FilePath");
    plugin_file_path_text->SetText(plugin_file_path.c_str());
    auto plugin_description_text = document.NewElement("Description");
    plugin_description_text->SetText(plugin_description.c_str());
    auto plugin_version_double = document.NewElement("Version");
    plugin_version_double->SetText(plugin_version);
    auto plugin_type_text = document.NewElement("Type");
    plugin_type_text->SetText(to_type(plugin_type));
    auto plugin_stage_text = document.NewElement("Stage");
    plugin_stage_text->SetText(to_stage(plugin_stage));

    if (plugin_context) {
        plugin_context->toXml(plugin_spec, document);
    }

    root->InsertEndChild(plugin_spec);
    plugin_spec->InsertEndChild(plugin_name_text);
    plugin_spec->InsertEndChild(plugin_file_path_text);
    plugin_spec->InsertEndChild(plugin_description_text);
    plugin_spec->InsertEndChild(plugin_version_double);
    plugin_spec->InsertEndChild(plugin_type_text);
}

void
PluginSpec::fromXml(const tinyxml2::XMLElement *element) {
    if (std::strcmp(element->Name(), "PluginSpec"))
        return;
    plugin_name = child_element_value(element, "Name");
    plugin_file_path = child_element_value(element, "FilePath");
    plugin_description = child_element_value(element, "Description");
    plugin_version = child_element_double(element, "Version");
    plugin_type = parse_type(child_element_value(element, "Type"));
    plugin_stage = parse_stage(child_element_value(element, "Stage"));
    auto properties_xml = element->FirstChildElement("Properties");
    if (properties_xml != nullptr) {
        plugin_context.reset(new ModuleContext);
        plugin_context->fromXml(properties_xml);
    }
}

bool
PluginSpec::is_valid() const {
    return !plugin_name.empty() && plugin_type != UnknownModule && plugin_version > 0.;
}

std::string
PluginSpec::plugin_stage_text() const {
    return to_stage(plugin_stage);
}

void
PluginSpecController::write(tinyxml2::XMLElement *root, tinyxml2::XMLDocument &document) {
    auto specs = document.NewElement("PluginSpecs");
    root->InsertEndChild(specs);
    for (auto it = _list.begin(); it != _list.end(); ++it) {
        (*it).toXml(specs, document);
    }
}

void
PluginSpecController::write() {
    tinyxml2::XMLDocument document;

    auto configuration = document.NewElement("Configuration");
    document.InsertFirstChild(configuration);
    write(configuration, document);
    auto declaration = document.NewDeclaration();
    document.InsertFirstChild(declaration);
    document.SaveFile(_file_name.c_str());
}

void
PluginSpecController::read(const tinyxml2::XMLDocument &document) {
    _list.clear();

    auto configuration = document.FirstChildElement("Configuration");
    if (configuration == nullptr)
        return;
    auto specs = configuration->FirstChildElement("PluginSpecs");
    if (specs == nullptr)
        return;
    for (auto plugin_spec = specs->FirstChildElement("PluginSpec");
         plugin_spec;
         plugin_spec = plugin_spec->NextSiblingElement("PluginSpec")) {
        PluginSpec spec;

        spec.fromXml(plugin_spec);
        _list.push_back(spec);
    }
}

void
PluginSpecController::read() {
    tinyxml2::XMLDocument document;

    document.LoadFile(_file_name.c_str());
    read(document);
}


void
PluginSpecController::add(const PluginSpec &spec) {
    _list.push_back(spec);
}
