__revision__ = "src/engine/SCons/Options/PackageOption.py 0.96.91 2005/10/17 20:04:26 ita"
__all__ = ('PackageOption', 'True', 'False')
import string
from BoolOption import True, False
import SCons.Errors
__enable_strings  = (str(True), 'yes', 'true',  'on', 'enable', 'search')
__disable_strings = (str(False), 'no',  'false', 'off', 'disable')
def _converter(val):
    """
    """
    lval = string.lower(val)
    if lval in __enable_strings: return True
    if lval in __disable_strings: return False
    return val
def _validator(key, val, env, searchfunc):
    """
    """
    import os
    if env[key] == False:
        pass
    elif env[key] == True:
        if searchfunc:
            env[key] = searchfunc(key, val)
    elif not os.path.exists(val):
        raise SCons.Errors.UserError(
            'Path does not exist for option %s: %s' % (key, val))
def PackageOption(key, help, default, searchfunc=None):
    """
    The input parameters describe a 'package list' option, thus they
    are returned with the correct converter and validator appended. The
    result is usable for input to opts.Add() .
    A 'package list' option may either be 'all', 'none' or a list of
    package names (seperated by space).
    """
    help = string.join(
        (help, '( yes | no | /path/to/%s )' % key),
        '\n    ')
    return (key, help, default,
            lambda k, v, e, f=searchfunc: _validator(k,v,e,f),
            _converter)
