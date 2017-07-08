#ifndef ALGORITHM_PLUGIN_MODEL_H
#define ALGORITHM_PLUGIN_MODEL_H

#if defined(EXPORT_MODEL_LIBRARY)
#define MODEL_LIBRARY_API __declspec(dllexport)
#else
#define MODEL_LIBRARY_API __declspec(dllimport)
#endif

struct MODEL_LIBRARY_API PluginModel {

};

#endif //ALGORITHM_PLUGIN_MODEL_H
