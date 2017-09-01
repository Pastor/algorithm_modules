#include <cstdio>
#include <cstdlib>
#include <string>
#include <python_plugin_module.h>

int
main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Script not define in program arguments\n");
        return EXIT_FAILURE;
    }
    PythonFileScriptModule module(argv[1]);
    return module.execute(std::make_shared<ModuleContext>()) ? EXIT_SUCCESS : EXIT_FAILURE;
}