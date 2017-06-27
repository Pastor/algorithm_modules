#ifndef ALGORITHM_POSTGRESQL_CONTEXT_H
#define ALGORITHM_POSTGRESQL_CONTEXT_H
#include <plugin_api.h>
#include <sql_context.h>

struct PostgreSqlContextPrivate;

class LIBRARY_API PostgreSqlContext final: public SqlContext {
    std::shared_ptr<PostgreSqlContextPrivate> d;
public:
    PostgreSqlContext();
    PostgreSqlContext(std::shared_ptr<ModuleContext> parent);

    ~PostgreSqlContext() override;

    bool is_connected() const;
    bool connect() const;
};

#endif //ALGORITHM_POSTGRESQL_CONTEXT_H
