from pprint import pprint


class Context:
    def __init__(self):
        self.db = "db"
        self.properties = []


def main(context, name):
    print("cwd : %s" % os.getcwd())
    max_ = 0
    name_ = ""
    for param in os.environ.keys():
        len_ = len(os.environ[param])
        if len_ > max_:
            max_ = len_
            name_ = param
    print("length(%s, %d)" % (name_, max_))
    print("value (%s, %s)" % (name_, os.environ[name_]))
    print("called(%s, %s)" % (context, name))
    return 0

if __name__ == '__main__':
    ctx = Context()
    pprint(vars(ctx))
    main(ctx, "name")
