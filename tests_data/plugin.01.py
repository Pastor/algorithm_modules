from pprint import pprint


class Context:
    def __init__(self):
        self.db = "db"
        self.properties = []


def main(context, name):
    print("cwd: %s" % os.getcwd())
    print("called(%s, %s)" % (context, name))
    return 0

if __name__ == '__main__':
    ctx = Context()
    pprint(vars(ctx))
    main(ctx, "name")
