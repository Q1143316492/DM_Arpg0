

if __name__ == '__main__':
    import importlib
    Module = importlib.import_module("doc.doc")
    MyClass = getattr(Module, "TestClass")
    a = MyClass()
    a.TestFunc()