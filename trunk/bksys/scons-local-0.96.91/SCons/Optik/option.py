__revision__ = "src/engine/SCons/Optik/option.py 0.96.91 2005/10/17 20:04:26 ita"
# Original Optik revision this is based on:
__Optik_revision__ = "option.py,v 1.19.2.1 2002/07/23 01:51:14 gward Exp" 
# Copyright (c) 2001 Gregory P. Ward.  All rights reserved.
# See the README.txt distributed with Optik for licensing terms.
# created 2001/10/17, GPW (from optik.py)
import sys
import string
from types import TupleType, ListType, DictType
from SCons.Optik.errors import OptionError, OptionValueError
_builtin_cvt = { "int" : (int, "integer"),
                 "long" : (long, "long integer"),
                 "float" : (float, "floating-point"),
                 "complex" : (complex, "complex") }
def check_builtin (option, opt, value):
    (cvt, what) = _builtin_cvt[option.type]
    try:
        return cvt(value)
    except ValueError:
        raise OptionValueError(
            "option %s: invalid %s value: %s" % (opt, what, repr(value)))
def check_choice(option, opt, value):
    if value in option.choices:
        return value
    else:
        choices = string.join(map(repr, option.choices),", ")
        raise OptionValueError(
            "option %s: invalid choice: %s (choose from %s)"
            % (opt, repr(value), choices))
# Not supplying a default is different from a default of None,
# so we need an explicit "not supplied" value.
NO_DEFAULT = "NO"+"DEFAULT"
class Option:
    """
    Instance attributes:
      _short_opts : [string]
      _long_opts : [string]
      action : string
      type : string
      dest : string
      default : any
      nargs : int
      const : any
      choices : [string]
      callback : function
      callback_args : (any*)
      callback_kwargs : { string : any }
      help : string
      metavar : string
    """
    ATTRS = ['action',
             'type',
             'dest',
             'default',
             'nargs',
             'const',
             'choices',
             'callback',
             'callback_args',
             'callback_kwargs',
             'help',
             'metavar']
    ACTIONS = ("store",
               "store_const",
               "store_true",
               "store_false",
               "append",
               "count",
               "callback",
               "help",
               "version")
    STORE_ACTIONS = ("store",
                     "store_const",
                     "store_true",
                     "store_false",
                     "append",
                     "count")
    TYPED_ACTIONS = ("store",
                     "append",
                     "callback")
    TYPES = ("string", "int", "long", "float", "complex", "choice")
    TYPE_CHECKER = { "int"    : check_builtin,
                     "long"   : check_builtin,
                     "float"  : check_builtin,
                     "complex"  : check_builtin,
                     "choice" : check_choice,
                   }
    CHECK_METHODS = None
                    
    def __init__ (self, *opts, **attrs):
        opts = self._check_opt_strings(opts)
        self._set_opt_strings(opts)
        self._set_attrs(attrs)
        for checker in self.CHECK_METHODS:
            checker(self)
    def _check_opt_strings (self, opts):
        opts = filter(None, opts)
        if not opts:
            raise OptionError("at least one option string must be supplied",
                              self)
        return opts
    def _set_opt_strings (self, opts):
        self._short_opts = []
        self._long_opts = []
        for opt in opts:
            if len(opt) < 2:
                raise OptionError(
                    "invalid option string %s: "
                    "must be at least two characters long" % (`opt`,), self)
            elif len(opt) == 2:
                if not (opt[0] == "-" and opt[1] != "-"):
                    raise OptionError(
                        "invalid short option string %s: "
                        "must be of the form -x, (x any non-dash char)" % (`opt`,),
                        self)
                self._short_opts.append(opt)
            else:
                if not (opt[0:2] == "--" and opt[2] != "-"):
                    raise OptionError(
                        "invalid long option string %s: "
                        "must start with --, followed by non-dash" % (`opt`,),
                        self)
                self._long_opts.append(opt)
    def _set_attrs (self, attrs):
        for attr in self.ATTRS:
            if attrs.has_key(attr):
                setattr(self, attr, attrs[attr])
                del attrs[attr]
            else:
                if attr == 'default':
                    setattr(self, attr, NO_DEFAULT)
                else:
                    setattr(self, attr, None)
        if attrs:
            raise OptionError(
                "invalid keyword arguments: %s" % string.join(attrs.keys(),", "),
                self)
    def _check_action (self):
        if self.action is None:
            self.action = "store"
        elif self.action not in self.ACTIONS:
            raise OptionError("invalid action: %s" % (`self.action`,), self)
    def _check_type (self):
        if self.type is None:
            if self.action in ("store", "append"):
                if self.choices is not None:
                    self.type = "choice"
                else:
                    self.type = "string"
        else:
            if self.type not in self.TYPES:
                raise OptionError("invalid option type: %s" % (`self.type`,), self)
            if self.action not in self.TYPED_ACTIONS:
                raise OptionError(
                    "must not supply a type for action %s" % (`self.action`,), self)
    def _check_choice(self):
        if self.type == "choice":
            if self.choices is None:
                raise OptionError(
                    "must supply a list of choices for type 'choice'", self)
            elif type(self.choices) not in (TupleType, ListType):
                raise OptionError(
                    "choices must be a list of strings ('%s' supplied)"
                    % string.split(str(type(self.choices)),"'")[1], self)
        elif self.choices is not None:
            raise OptionError(
                "must not supply choices for type %s" % (repr(self.type),), self)
    def _check_dest (self):
        if self.action in self.STORE_ACTIONS and self.dest is None:
            if self._long_opts:
                self.dest = string.replace(self._long_opts[0][2:],'-', '_')
            else:
                self.dest = self._short_opts[0][1]
    def _check_const (self):
        if self.action != "store_const" and self.const is not None:
            raise OptionError(
                "'const' must not be supplied for action %s" % (repr(self.action),),
                self)
        
    def _check_nargs (self):
        if self.action in self.TYPED_ACTIONS:
            if self.nargs is None:
                self.nargs = 1
        elif self.nargs is not None:
            raise OptionError(
                "'nargs' must not be supplied for action %s" % (repr(self.action),),
                self)
    def _check_callback (self):
        if self.action == "callback":
            if not callable(self.callback):
                raise OptionError(
                    "callback not callable: %s" % (repr(self.callback),), self)
            if (self.callback_args is not None and
                type(self.callback_args) is not TupleType):
                raise OptionError(
                    "callback_args, if supplied, must be a tuple: not %s"
                    % (repr(self.callback_args),), self)
            if (self.callback_kwargs is not None and
                type(self.callback_kwargs) is not DictType):
                raise OptionError(
                    "callback_kwargs, if supplied, must be a dict: not %s"
                    % (repr(self.callback_kwargs),), self)
        else:
            if self.callback is not None:
                raise OptionError(
                    "callback supplied (%s) for non-callback option"
                    % (repr(self.callback),), self)
            if self.callback_args is not None:
                raise OptionError(
                    "callback_args supplied for non-callback option", self)
            if self.callback_kwargs is not None:
                raise OptionError(
                    "callback_kwargs supplied for non-callback option", self)
    CHECK_METHODS = [_check_action,
                     _check_type,
                     _check_choice,
                     _check_dest,
                     _check_const,
                     _check_nargs,
                     _check_callback]
        
    def __str__ (self):
        if self._short_opts or self._long_opts:
            return string.join(self._short_opts + self._long_opts,"/")
        else:
            raise RuntimeError, "short_opts and long_opts both empty!"
    def takes_value (self):
        return self.type is not None
    def check_value (self, opt, value):
        checker = self.TYPE_CHECKER.get(self.type)
        if checker is None:
            return value
        else:
            return checker(self, opt, value)
    def process (self, opt, value, values, parser):
        if value is not None:
            if self.nargs == 1:
                value = self.check_value(opt, value)
            else:
                def cv(v,check=self.check_value,o=opt):
                    return check(o,v)
                value = tuple(map(cv,value))
        return self.take_action(
            self.action, self.dest, opt, value, values, parser)
    def take_action (self, action, dest, opt, value, values, parser):
        if action == "store":
            setattr(values, dest, value)
        elif action == "store_const":
            setattr(values, dest, self.const)
        elif action == "store_true":
            setattr(values, dest, 1)
        elif action == "store_false":
            setattr(values, dest, 0)
        elif action == "append":
            values.ensure_value(dest, []).append(value)
        elif action == "count":
            setattr(values, dest, values.ensure_value(dest, 0) + 1)
        elif action == "callback":
            args = self.callback_args or ()
            kwargs = self.callback_kwargs or {}
            apply( self.callback, (self, opt, value, parser,)+ args, kwargs)
        elif action == "help":
            parser.print_help()
            sys.exit(0)
        elif action == "version":
            parser.print_version()
            sys.exit(0)
        else:
            raise RuntimeError, "unknown action %s" % (repr(self.action),)
        return 1
# class Option
