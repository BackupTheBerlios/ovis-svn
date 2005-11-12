__revision__ = "src/engine/SCons/Node/Alias.py 0.96.91 2005/10/17 20:04:26 ita"
import UserDict
import SCons.Errors
import SCons.Node
import SCons.Util
class AliasNameSpace(UserDict.UserDict):
    def Alias(self, name, **kw):
        if isinstance(name, SCons.Node.Alias.Alias):
            return name
        try:
            a = self[name]
        except KeyError:
            a = apply(SCons.Node.Alias.Alias, (name,), kw)
            self[name] = a
        return a
    def lookup(self, name, **kw):
        try:
            return self[name]
        except KeyError:
            return None
class Alias(SCons.Node.Node):
    def __init__(self, name):
        SCons.Node.Node.__init__(self)
        self.name = name
    def __str__(self):
        return self.name
    really_build = SCons.Node.Node.build
    current = SCons.Node.Node.children_are_up_to_date
    def is_under(self, dir):
        return 1
    def get_contents(self):
        """The contents of an alias is the concatenation
        of all the contents of its sources"""
        contents = ""
        for kid in self.children(None):
            contents = contents + kid.get_contents()
        return contents
    def sconsign(self):
        """An Alias is not recorded in .sconsign files"""
        pass
    def build(self):
        """A "builder" for aliases."""
        pass
    def convert(self):
        try: del self.builder
        except AttributeError: pass
        self.reset_executor()
        self.build = self.really_build
default_ans = AliasNameSpace()
SCons.Node.arg2nodes_lookups.append(default_ans.lookup)
