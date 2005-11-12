__revision__ = "src/engine/SCons/Errors.py 0.96.91 2005/10/17 20:04:26 ita"
class BuildError(Exception):
    def __init__(self, node=None, errstr="Unknown error", *args):
        self.node = node
        self.errstr = errstr
        apply(Exception.__init__, (self,) + args)
class InternalError(Exception):
    pass
class UserError(Exception):
    pass
class StopError(Exception):
    pass
class ExplicitExit(Exception):
    def __init__(self, node=None, status=None, *args):
        self.node = node
        self.status = status
        apply(Exception.__init__, (self,) + args)
