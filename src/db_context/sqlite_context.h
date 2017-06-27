#ifndef ALGORITHM_SQLITE_CONTEXT_H
#define ALGORITHM_SQLITE_CONTEXT_H
#include <plugin_api.h>
#include <sql_context.h>

struct SqliteContextPrivate;

class LIBRARY_API SqliteContext final: public SqlContext {
    std::shared_ptr<SqliteContextPrivate> d;
public:
    SqliteContext();
    SqliteContext(std::shared_ptr<ModuleContext> parent);

    ~SqliteContext() override;

    bool is_connected() const;
    bool connect() const;
};

#endif //ALGORITHM_SQLITE_CONTEXT_H
