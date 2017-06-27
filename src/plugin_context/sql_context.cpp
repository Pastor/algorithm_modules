#include "sql_context.h"

SqlContext::SqlContext(Type type)
        : _type(type) {
}

SqlContext::SqlContext(Type type, std::shared_ptr<ModuleContext> parent)
        : ModuleContext(parent), _type(type) {
}

bool
SqlContext::is_database_context() const {
    return true;
}
