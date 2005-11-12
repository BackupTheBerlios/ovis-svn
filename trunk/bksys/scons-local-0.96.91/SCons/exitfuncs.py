__revision__ = "src/engine/SCons/exitfuncs.py 0.96.91 2005/10/17 20:04:26 ita"
_exithandlers = []
def _run_exitfuncs():
    """run any registered exit functions
    _exithandlers is traversed in reverse order so functions are executed
    last in, first out.
    """
    while _exithandlers:
        func, targs, kargs =  _exithandlers.pop()
        apply(func, targs, kargs)
def register(func, *targs, **kargs):
    """register a function to be executed upon normal program termination
    func - function to be called at exit
    targs - optional arguments to pass to func
    kargs - optional keyword arguments to pass to func
    """
    _exithandlers.append((func, targs, kargs))
import sys
try:
    x = sys.exitfunc
    if x != _run_exitfuncs:
        register(x)
except AttributeError:
    pass
# make our exit function get run by python when it exits:    
sys.exitfunc = _run_exitfuncs
del sys
