__revision__ = "src/engine/SCons/Options/BoolOption.py 0.96.91 2005/10/17 20:04:26 ita"
__all__ = ('BoolOption', 'True', 'False')
import string
import SCons.Errors
__true_strings  = ('y', 'yes', 'true', 't', '1', 'on' , 'all' )
__false_strings = ('n', 'no', 'false', 'f', '0', 'off', 'none')
# we need this since SCons should work version indepentant
True, False = 1, 0
def _text2bool(val):
    """
    Converts strings to True/False depending on the 'truth' expressed by
    the string. If the string can't be converted, the original value
    will be returned.
    See '__true_strings' and '__false_strings' for values considered
    'true' or 'false respectivly.
    This is usable as 'converter' for SCons' Options.
    """
    lval = string.lower(val)
    if lval in __true_strings: return True
    if lval in __false_strings: return False
    raise ValueError("Invalid value for boolean option: %s" % val)
def _validator(key, val, env):
    """
    Validates the given value to be either '0' or '1'.
    
    This is usable as 'validator' for SCons' Options.
    """
    if not env[key] in (True, False):
        raise SCons.Errors.UserError(
            'Invalid value for boolean option %s: %s' % (key, env[key]))
def BoolOption(key, help, default):
    """
    The input parameters describe a boolen option, thus they are
    returned with the correct converter and validator appended. The
    'help' text will by appended by '(yes|no) to show the valid
    valued. The result is usable for input to opts.Add().
    """
    return (key, '%s (yes|no)' % help, default,
            _validator, _text2bool)
