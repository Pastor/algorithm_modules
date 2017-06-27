#ifndef ALGORITHM_SQL_CONTEXT_H
#define ALGORITHM_SQL_CONTEXT_H

#include <plugin_api.h>

class LIBRARY_API SqlContext : public ModuleContext {
public:
    enum Type {
        PostgreSQL,
        SQLite
    };

    SqlContext(Type type);

    SqlContext(Type type, std::shared_ptr<ModuleContext> parent);

    virtual ~SqlContext() {}

    virtual bool is_connected() const = 0;

    virtual bool connect() const = 0;

    bool is_database_context() const override;

    Type type() const {
        return _type;
    }

private:
    Type _type;
};

#endif //ALGORITHM_SQL_CONTEXT_H
