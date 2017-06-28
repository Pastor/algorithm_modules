#ifndef ALGORITHM_SQL_CONTEXT_H
#define ALGORITHM_SQL_CONTEXT_H

#include <plugin_api.h>

#if defined(EXPORT_MODULE_LIBRARY)
#define MODULE_LIBRARY_API __declspec(dllexport)
#else
#define MODULE_LIBRARY_API __declspec(dllimport)
#endif

/*
CREATE USER "User_${ModuleName}_${ModuleVersion}" WITH PASSWORD '${PASSWORD}';
INSERT INTO "processing"."user_module"("user_name", "password", "schema_name")
 VALUES('User_${ModuleName}_${ModuleVersion}', '${PASSWORD}', 'Schema_${ModuleName}_${ModuleVersion}');
CREATE SCHEMA IF NOT EXISTS "Schema_${ModuleName}_${ModuleVersion}" AUTHORIZATION "User_${ModuleName}_${ModuleVersion}";

CREATE TABLE "Schema_${ModuleName}_${ModuleVersion}"."${Table_UUID}"(
   id            BIGSERIAL NOT NULL PRIMARY KEY,
   created_at    TIMESTAMP NOT NULL DEFAULT now()
);
GRANT ALL PRIVILEGES ON TABLE "Schema_${ModuleName}_${ModuleVersion}"."${Table_UUID}"
 TO "User_${ModuleName}_${ModuleVersion}" WITH GRANT OPTION;

INSERT INTO "processing"."module_processing"("stream_input", "schema_name", "table_name")
       VALUES ('UUID', 'Schema_${ModuleName}_${Version}', '${Table_UUID}');

...
UPDATE "processing"."module_processing" SET complete_at = now() WHERE id = ${ID}
*/

#endif //ALGORITHM_SQL_CONTEXT_H
