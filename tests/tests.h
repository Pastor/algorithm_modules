#ifndef ALGORITHM_TESTS_H
#define ALGORITHM_TESTS_H
#include <string>
#if defined(_MSC_VER)
const std::string library = "dynamic_module_library.dll";
#else
const std::string library = "libdynamic_module_library.dll";
#endif
const std::string configuration_file = "tests_data/configuration.xml";

#endif //ALGORITHM_TESTS_H
