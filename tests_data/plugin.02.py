#!/usr/bin/python
import os
import pprint

import psycopg2


def main(context, name):
    try:
        pprint.PrettyPrinter(indent=2).pprint(context)
        hostname = context["Database.Connection.Hostname"]
        database = context["Database.Connection.Database"]
        username = context["Database.Connection.Username"]
        password = context["Database.Connection.Password"]
        schema = context["Database.Connection.Schema"]
        table_name = context["Database.Stream.Output"]
        #input_stream = context.get('Stream.Input', default=None)
        conn_string = "host='{}' dbname='{}' user='{}' password='{}'".format(hostname, database, username, password)
        print "Connecting to database\n	->{}\n table_name ->{}".format(conn_string, table_name)
        conn = psycopg2.connect(conn_string)
        cursor = conn.cursor()
        cursor.execute('ALTER TABLE "{}"."{}" ADD COLUMN "{}" TEXT'.format(schema, table_name, 'column_name_01'))
        print "SQL: {} Result: {}".format(cursor.query, cursor.statusmessage)
        cursor.execute('ALTER TABLE "{}"."{}" ADD COLUMN "{}" TEXT'.format(schema, table_name, 'column_name_02'))
        print "SQL: {} Result: {}".format(cursor.query, cursor.statusmessage)
        cursor.execute('ALTER TABLE "{}"."{}" ADD COLUMN "{}" TEXT'.format(schema, table_name, 'column_name_03'))
        print "SQL: {} Result: {}".format(cursor.query, cursor.statusmessage)
        cursor.execute('ALTER TABLE "{}"."{}" ADD COLUMN "{}" TEXT'.format(schema, table_name, 'column_name_04'))
        print "SQL: {} Result: {}".format(cursor.query, cursor.statusmessage)
        conn.commit()
        cursor.close()
        conn.close()
    except Exception as ex:
        print("Exception: {}".format(ex.message))
        return -1
    return 0


def single():
    hostname = os.environ["CONTEXT_Database.Connection.Hostname"]
    database = os.environ["CONTEXT_Database.Connection.Database"]
    username = os.environ["CONTEXT_Database.Connection.Username"]
    password = os.environ["CONTEXT_Database.Connection.Password"]
    schema = os.environ["CONTEXT_Database.Connection.Schema"]
    table_name = os.environ["CONTEXT_Database.Stream.Output"]
    input_stream = os.environ["CONTEXT_Stream.Input"]
    return main({
        'Database.Connection.Hostname': hostname,
        'Database.Connection.Database': database,
        'Database.Connection.Username': username,
        'Database.Connection.Password': password,
        'Database.Connection.Schema': schema,
        'Database.Stream.Output': table_name,
        'Stream.Input': input_stream,
    }, 'NAME')


if __name__ == "__main__":
    single()
