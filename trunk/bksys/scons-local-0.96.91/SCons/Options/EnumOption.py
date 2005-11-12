__revision__ = "src/engine/SCons/Options/EnumOption.py 0.96.91 2005/10/17 20:04:26 ita"
__all__ = ('EnumOption',)
import string
import SCons.Errors
def _validator(key, val, env, vals):
    if not val in vals:
        raise SCons.Errors.UserError(
            'Invalid value for option %s: %s' % (key, val))
def EnumOption(key, help, default, allowed_values, map={}, ignorecase=0):
    """
    The input parameters describe a option with only certain values
    allowed. They are returned with an appropriate converter and
    validator appended. The result is usable for input to
    Options.Add().
    'key' and 'default' are the values to be passed on to Options.Add().
    'help' will be appended by the allowed values automatically
    'allowed_values' is a list of strings, which are allowed as values
    for this option.
    The 'map'-dictionary may be used for converting the input value
    into canonical values (eg. for aliases).
    'ignorecase' defines the behaviour of the validator:
    If ignorecase == 0, the validator/converter are case-sensitive.
    If ignorecase == 1, the validator/converter are case-insensitive.
    If ignorecase == 2, the validator/converter is case-insensitive and
                        the converted value will always be lower-case.
    The 'validator' tests whether the value is in the list of allowed
    values. The 'converter' converts input values according to the
    given 'map'-dictionary (unmapped input values are returned
    unchanged). 
    """
    help = '%s (%s)' % (help, string.join(allowed_values, '|'))
    if ignorecase >= 1:
        validator = lambda key, val, env, vals=allowed_values: \
                    _validator(key, string.lower(val), env, vals)
    else:
        validator = lambda key, val, env, vals=allowed_values: \
                    _validator(key, val, env, vals)
    if ignorecase == 2:
        converter = lambda val, map=map: \
                    string.lower(map.get(string.lower(val), val))
    elif ignorecase == 1:
        converter = lambda val, map=map: \
                    map.get(string.lower(val), val)
    else:
        converter = lambda val, map=map: \
                    map.get(val, val)
    return (key, help, default, validator, converter)
