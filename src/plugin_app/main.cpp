#include <cstdio>
#include <cstdlib>
#include <string>
#include <python_plugin_module.h>
#include <plugin_manager/plugin_manager.h>

int
main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "configuration module_name input_file\n");
        return EXIT_FAILURE;
    }
    PluginManager manager;
    auto context = std::make_shared<ModuleContext>();
    context->set_property(Constants::Stream_Input, argv[3]);
    context->set_property(Constants::SystemProcessModule_PutEnvironment, "true");
    manager.load(argv[1]);
    manager.execute(argv[2], context);
    return EXIT_SUCCESS;
}