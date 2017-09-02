DROP SCHEMA "processing" CASCADE;
CREATE SCHEMA "processing";

DROP TABLE IF EXISTS "processing"."user_module" CASCADE;
CREATE TABLE "processing"."user_module"(
  id            BIGSERIAL NOT NULL PRIMARY KEY,

  user_name     TEXT NOT NULL,
  schema_name   TEXT NOT NULL,
  module_name    TEXT NOT NULL,
  module_version TEXT NOT NULL,
  user_password TEXT NOT NULL,
  created_at    TIMESTAMP NOT NULL DEFAULT now()
);

DROP TABLE IF EXISTS "processing"."processing_module" CASCADE;
CREATE TABLE "processing"."processing_module"(
  id            BIGSERIAL NOT NULL PRIMARY KEY,

  stream_input  TEXT NOT NULL,
  stage         TEXT NOT NULL,
  module_id     BIGINT NOT NULL REFERENCES "processing"."user_module"(id) ON DELETE RESTRICT,
  table_name    TEXT NOT NULL,
  status        TEXT NOT NULL DEFAULT 'STARTING',
  complete_at   TIMESTAMP DEFAULT NULL,
  created_at    TIMESTAMP NOT NULL DEFAULT now()
);
DROP SCHEMA "Module_v1.02_DynamicLibraryModule" CASCADE;
DROP SCHEMA "Module_v1.02_PostgreSqlModule" CASCADE;
DROP SCHEMA "Module_v1.02_PythonModule" CASCADE;
DROP SCHEMA "Module_v1.02_SqliteModule" CASCADE;
DROP SCHEMA "Module_v1.02_SystemProcessModule" CASCADE;

DROP USER IF EXISTS "Module_v1.02_DynamicLibraryModule";
DROP USER IF EXISTS "Module_v1.02_PostgreSqlModule";
DROP USER IF EXISTS "Module_v1.02_PythonModule";
DROP USER IF EXISTS "Module_v1.02_SqliteModule";
DROP USER IF EXISTS "Module_v1.02_SystemProcessModule";
--INSERT INTO "processing"."processing_module"(stream_input, stage, module_id, table_name) VALUES() RETURNING id
