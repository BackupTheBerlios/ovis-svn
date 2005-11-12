__revision__ = "src/engine/SCons/Options/ListOption.py 0.96.91 2005/10/17 20:04:26 ita"
# Know Bug: This should behave like a Set-Type, but does not really,
# since elements can occur twice.
__all__ = ('ListOption',)
import string
import UserList
import SCons.Util
class _ListOption(UserList.UserList):
    def __init__(self, initlist=[], allowedElems=[]):
        UserList.UserList.__init__(self, filter(None, initlist))
        self.allowedElems = allowedElems[:]
        self.allowedElems.sort()
        
    def __cmp__(self, other):
        raise NotImplementedError
    def __eq__(self, other):
        raise NotImplementedError
    def __ge__(self, other):
        raise NotImplementedError
    def __gt__(self, other):
        raise NotImplementedError
    def __le__(self, other):
        raise NotImplementedError
    def __lt__(self, other):
        raise NotImplementedError
    def __str__(self):
        if len(self) == 0:
            return 'none'
        self.data.sort()
        if self.data == self.allowedElems:
            return 'all'
        else:
            return string.join(self, ',')
    def __repr__(self):
        return self.__str__()
    
def _converter(val, allowedElems, mapdict):
    """
    """
    if val == 'none':
        val = []
    elif val == 'all':
        val = allowedElems
    else:
        val = filter(None, string.split(val, ','))
        val = map(lambda v, m=mapdict: m.get(v, v), val)
        notAllowed = filter(lambda v, aE=allowedElems: not v in aE, val)
        if notAllowed:
            raise ValueError("Invalid value(s) for option: %s" %
                             string.join(notAllowed, ','))
    return _ListOption(val, allowedElems)
## def _validator(key, val, env):
##     """
##     """
##     # todo: write validater for pgk list
##     return 1
def ListOption(key, help, default, names, map={}):
    """
    The input parameters describe a 'package list' option, thus they
    are returned with the correct converter and validater appended. The
    result is usable for input to opts.Add() .
    A 'package list' option may either be 'all', 'none' or a list of
    package names (separated by space).
    """
    names_str = 'allowed names: %s' % string.join(names, ' ')
    if SCons.Util.is_List(default):
        default = string.join(default, ',')
    help = string.join(
        (help, '(all|none|comma-separated list of names)', names_str),
        '\n    ')
    return (key, help, default,
            None, #_validator,
            lambda val, elems=names, m=map: _converter(val, elems, m))
