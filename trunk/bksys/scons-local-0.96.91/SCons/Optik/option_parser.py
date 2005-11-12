__revision__ = "src/engine/SCons/Optik/option_parser.py 0.96.91 2005/10/17 20:04:26 ita"
# Original Optik revision this is based on:
__Optik_revision__ = "option_parser.py,v 1.38.2.1 2002/07/23 01:51:14 gward Exp"
# Copyright (c) 2001 Gregory P. Ward.  All rights reserved.
# See the README.txt distributed with Optik for licensing terms.
# created 2001/10/17, GPW (from optik.py)
import sys, os
import string
import types
from SCons.Optik.option import Option, NO_DEFAULT
from SCons.Optik.errors import OptionConflictError, OptionValueError, BadOptionError
def get_prog_name ():
    return os.path.basename(sys.argv[0])
SUPPRESS_HELP = "SUPPRESS"+"HELP"
SUPPRESS_USAGE = "SUPPRESS"+"USAGE"
class Values:
    def __init__ (self, defaults=None):
        if defaults:
            for (attr, val) in defaults.items():
                setattr(self, attr, val)
    def _update_careful (self, dict):
        """
        Update the option values from an arbitrary dictionary, but only
        use keys from dict that already have a corresponding attribute
        in self.  Any keys in dict without a corresponding attribute
        are silently ignored.
        """
        for attr in dir(self):
            if dict.has_key(attr):
                dval = dict[attr]
                if dval is not None:
                    setattr(self, attr, dval)
    def _update_loose (self, dict):
        """
        Update the option values from an arbitrary dictionary,
        using all keys from the dictionary regardless of whether
        they have a corresponding attribute in self or not.
        """
        self.__dict__.update(dict)
    def _update (self, dict, mode):
        if mode == "careful":
            self._update_careful(dict)
        elif mode == "loose":
            self._update_loose(dict)
        else:
            raise ValueError, "invalid update mode: %s" % (repr(mode),)
    def read_module (self, modname, mode="careful"):
        __import__(modname)
        mod = sys.modules[modname]
        self._update(vars(mod), mode)
    def read_file (self, filename, mode="careful"):
        vars = {}
        execfile(filename, vars)
        self._update(vars, mode)
    def ensure_value (self, attr, value):
        if not hasattr(self, attr) or getattr(self, attr) is None:
            setattr(self, attr, value)
        return getattr(self, attr)
class OptionParser:
    """
    Class attributes:
      standard_option_list : [Option]
        list of standard options that will be accepted by all instances
        of this parser class (intended to be overridden by subclasses).
    Instance attributes:
      usage : string
        a usage string for your program.  Before it is displayed
        to the user, "%prog" will be expanded to the name of
        your program (os.path.basename(sys.argv[0])).
      option_list : [Option]
        the list of all options accepted on the command-line of
        this program
      _short_opt : { string : Option }
        dictionary mapping short option strings, eg. "-f" or "-X",
        to the Option instances that implement them.  If an Option
        has multiple short option strings, it will appears in this
        dictionary multiple times.
      _long_opt : { string : Option }
        dictionary mapping long option strings, eg. "--file" or
        "--exclude", to the Option instances that implement them.
        Again, a given Option can occur multiple times in this
        dictionary.
      defaults : { string : any }
        dictionary mapping option destination names to default
        values for each destination.
      allow_interspersed_args : boolean = true
        if true, positional arguments may be interspersed with options.
        Assuming -a and -b each take a single argument, the command-line
          -ablah foo bar -bboo baz
        will be interpreted the same as
          -ablah -bboo -- foo bar baz
        If this flag were false, that command line would be interpreted as
          -ablah -- foo bar -bboo baz
        -- ie. we stop processing options as soon as we see the first
        non-option argument.  (This is the tradition followed by
        Python's getopt module, Perl's Getopt::Std, and other argument-
        parsing libraries, but it is generally annoying to users.)
      rargs : [string]
        the argument list currently being parsed.  Only set when
        parse_args() is active, and continually trimmed down as
        we consume arguments.  Mainly there for the benefit of
        callback options.
      largs : [string]
        the list of leftover arguments that we have skipped while
        parsing options.  If allow_interspersed_args is false, this
        list is always empty.
      values : Values
        the set of option values currently being accumulated.  Only
        set when parse_args() is active.  Also mainly for callbacks.
    Because of the 'rargs', 'largs', and 'values' attributes,
    OptionParser is not thread-safe.  If, for some perverse reason, you
    need to parse command-line arguments simultaneously in different
    threads, use different OptionParser instances.
    
    """ 
    standard_option_list = []
    def __init__ (self,
                  usage=None,
                  option_list=None,
                  option_class=Option,
                  version=None,
                  conflict_handler="error"):
        self.set_usage(usage)
        self.option_class = option_class
        self.version = version
        self.set_conflict_handler(conflict_handler)
        self.allow_interspersed_args = 1
        self._create_option_list()
        self._populate_option_list(option_list)
        self._init_parsing_state()
    def _create_option_list (self):
        self.option_list = []
        self._short_opt = {}            # single letter -> Option instance
        self._long_opt = {}             # long option -> Option instance
        self.defaults = {}              # maps option dest -> default value
    def _populate_option_list (self, option_list):
        if self.standard_option_list:
            self.add_options(self.standard_option_list)
        if option_list:
            self.add_options(option_list)
        
    def _init_parsing_state (self):
        self.rargs = None
        self.largs = None
        self.values = None
    def set_usage (self, usage):
        if usage is None:
            self.usage = "usage: %prog [options]"
        elif usage is SUPPRESS_USAGE:
            self.usage = None
        else:
            self.usage = usage
    def enable_interspersed_args (self):
        self.allow_interspersed_args = 1
    def disable_interspersed_args (self):
        self.allow_interspersed_args = 0
    def set_conflict_handler (self, handler):
        if handler not in ("ignore", "error", "resolve"):
            raise ValueError, "invalid conflict_resolution value %s" % (repr(handler),)
        self.conflict_handler = handler
    def set_default (self, dest, value):
        self.defaults[dest] = value
    def set_defaults (self, **kwargs):
        self.defaults.update(kwargs)
    def get_default_values(self):
        return Values(self.defaults)
    def _check_conflict (self, option):
        conflict_opts = []
        for opt in option._short_opts:
            if self._short_opt.has_key(opt):
                conflict_opts.append((opt, self._short_opt[opt]))
        for opt in option._long_opts:
            if self._long_opt.has_key(opt):
                conflict_opts.append((opt, self._long_opt[opt]))
        if conflict_opts:
            handler = self.conflict_handler
            if handler == "ignore":     # behaviour for Optik 1.0, 1.1
                pass
            elif handler == "error":    # new in 1.2
                raise OptionConflictError(
                    "conflicting option string(s): %s"
                    % string.join( map( lambda x: x[0], conflict_opts),", "),
                    option)
            elif handler == "resolve":  # new in 1.2
                for (opt, c_option) in conflict_opts:
                    if len(opt)>2 and opt[:2]=="--":
                        c_option._long_opts.remove(opt)
                        del self._long_opt[opt]
                    else:
                        c_option._short_opts.remove(opt)
                        del self._short_opt[opt]
                    if not (c_option._short_opts or c_option._long_opts):
                        self.option_list.remove(c_option)
    def add_option (self, *args, **kwargs):
        """add_option(Option)
           add_option(opt_str, ..., kwarg=val, ...)
        """
        if type(args[0]) is types.StringType:
            option = apply(self.option_class,args, kwargs)
        elif len(args) == 1 and not kwargs:
            option = args[0]
            if not isinstance(option, Option):
                raise TypeError, "not an Option instance: %s" % (repr(option),)
        else:
            raise TypeError, "invalid arguments"
        self._check_conflict(option)
        self.option_list.append(option)
        for opt in option._short_opts:
            self._short_opt[opt] = option
        for opt in option._long_opts:
            self._long_opt[opt] = option
        if option.dest is not None:     # option has a dest, we need a default
            if option.default is not NO_DEFAULT:
                self.defaults[option.dest] = option.default
            elif not self.defaults.has_key(option.dest):
                self.defaults[option.dest] = None
    def add_options (self, option_list):
        for option in option_list:
            self.add_option(option)
    def get_option (self, opt_str):
        return (self._short_opt.get(opt_str) or
                self._long_opt.get(opt_str))
    def has_option (self, opt_str):
        return (self._short_opt.has_key(opt_str) or
                self._long_opt.has_key(opt_str))
    def remove_option (self, opt_str):
        option = self._short_opt.get(opt_str)
        if option is None:
            option = self._long_opt.get(opt_str)
        if option is None:
            raise ValueError("no such option %s" % (repr(opt_str),))
        for opt in option._short_opts:
            del self._short_opt[opt]
        for opt in option._long_opts:
            del self._long_opt[opt]
        self.option_list.remove(option)
    def _get_args (self, args):
        if args is None:
            return sys.argv[1:]
        else:
            return args[:]              # don't modify caller's list
    def parse_args (self, args=None, values=None):
        """
        parse_args(args : [string] = sys.argv[1:],
                   values : Values = None)
        -> (values : Values, args : [string])
        Parse the command-line options found in 'args' (default:
        sys.argv[1:]).  Any errors result in a call to 'error()', which
        by default prints the usage message to stderr and calls
        sys.exit() with an error message.  On success returns a pair
        (values, args) where 'values' is an Values instance (with all
        your option values) and 'args' is the list of arguments left
        over after parsing options.
        """
        rargs = self._get_args(args)
        if values is None:
            values = self.get_default_values()
        self.rargs = rargs
        self.largs = largs = []
        self.values = values
        try:
            stop = self._process_args(largs, rargs, values)
        except (BadOptionError, OptionValueError), err:
            self.error(err.msg)
        args = largs + rargs
        return self.check_values(values, args)
    def check_values (self, values, args):
        """
        check_values(values : Values, args : [string])
        -> (values : Values, args : [string])
        Check that the supplied option values and leftover arguments are
        valid.  Returns the option values and leftover arguments
        (possibly adjusted, possibly completely new -- whatever you
        like).  Default implementation just returns the passed-in
        values; subclasses may override as desired.
        """
        return (values, args)
    def _process_args (self, largs, rargs, values):
        """_process_args(largs : [string],
                         rargs : [string],
                         values : Values)
        Process command-line arguments and populate 'values', consuming
        options and arguments from 'rargs'.  If 'allow_interspersed_args' is
        false, stop at the first non-option argument.  If true, accumulate any
        interspersed non-option arguments in 'largs'.
        """
        while rargs:
            arg = rargs[0]
            if arg == "--":
                del rargs[0]
                return
            elif arg[0:2] == "--":
                self._process_long_opt(rargs, values)
            elif arg[:1] == "-" and len(arg) > 1:
                self._process_short_opts(rargs, values)
            elif self.allow_interspersed_args:
                largs.append(arg)
                del rargs[0]
            else:
                return                  # stop now, leave this arg in rargs
        
    def _match_long_opt (self, opt):
        """_match_long_opt(opt : string) -> string
        Determine which long option string 'opt' matches, ie. which one
        it is an unambiguous abbrevation for.  Raises BadOptionError if
        'opt' doesn't unambiguously match any long option string.
        """
        return _match_abbrev(opt, self._long_opt)
    def _process_long_opt (self, rargs, values):
        arg = rargs.pop(0)
        if "=" in arg:
            (opt, next_arg) = string.split(arg,"=", 1)
            rargs.insert(0, next_arg)
            had_explicit_value = 1
        else:
            opt = arg
            had_explicit_value = 0
        opt = self._match_long_opt(opt)
        option = self._long_opt[opt]
        if option.takes_value():
            nargs = option.nargs
            if len(rargs) < nargs:
                if nargs == 1:
                    self.error("%s option requires a value" % opt)
                else:
                    self.error("%s option requires %d values"
                               % (opt, nargs))
            elif nargs == 1:
                value = rargs.pop(0)
            else:
                value = tuple(rargs[0:nargs])
                del rargs[0:nargs]
        elif had_explicit_value:
            self.error("%s option does not take a value" % opt)
        else:
            value = None
        option.process(opt, value, values, self)
    def _process_short_opts (self, rargs, values):
        arg = rargs.pop(0)
        stop = 0
        i = 1
        for ch in arg[1:]:
            opt = "-" + ch
            option = self._short_opt.get(opt)
            i = i+1                      # we have consumed a character
            if not option:
                self.error("no such option: %s" % opt)
            if option.takes_value():
                if i < len(arg):
                    rargs.insert(0, arg[i:])
                    stop = 1
                nargs = option.nargs
                if len(rargs) < nargs:
                    if nargs == 1:
                        self.error("%s option requires a value" % opt)
                    else:
                        self.error("%s option requires %s values"
                                   % (opt, nargs))
                elif nargs == 1:
                    value = rargs.pop(0)
                else:
                    value = tuple(rargs[0:nargs])
                    del rargs[0:nargs]
            else:                       # option doesn't take a value
                value = None
            option.process(opt, value, values, self)
            if stop:
                break
    def error (self, msg):
        """error(msg : string)
        Print a usage message incorporating 'msg' to stderr and exit.
        If you override this in a subclass, it should not return -- it
        should either exit or raise an exception.
        """
        self.print_usage(sys.stderr)
        sys.stderr.write("\nSCons error: %s\n" % msg)
        sys.exit(2)
    def print_usage (self, file=None):
        """print_usage(file : file = stdout)
        Print the usage message for the current program (self.usage) to
        'file' (default stdout).  Any occurence of the string "%prog" in
        self.usage is replaced with the name of the current program
        (basename of sys.argv[0]).  Does nothing if self.usage is empty
        or not defined.
        """
        if file is None:
            file = sys.stdout
        if self.usage:
            usage = string.replace(self.usage,"%prog", get_prog_name())
            file.write(usage + "\n")
    def print_version (self, file=None):
        """print_version(file : file = stdout)
        Print the version message for this program (self.version) to
        'file' (default stdout).  As with print_usage(), any occurence
        of "%prog" in self.version is replaced by the current program's
        name.  Does nothing if self.version is empty or undefined.
        """
        if file is None:
            file = sys.stdout
        if self.version:
            version = string.replace(self.version,"%prog", get_prog_name())
            file.write(version+"\n")
    def print_help (self, file=None):
        """print_help(file : file = stdout)
        Print an extended help message, listing all options and any
        help text provided with them, to 'file' (default stdout).
        """
        
        if file is None:
            file = sys.stdout
        self.print_usage(file)
        file.write("Options:\n")
        width = 78                      # assume 80 cols for now
        option_help = []                # list of (string, string) tuples
        lengths = []
        for option in self.option_list:
            takes_value = option.takes_value()
            if takes_value:
                metavar = option.metavar or string.upper(option.dest)
            opts = []               # list of "-a" or "--foo=FILE" strings
            if option.help is SUPPRESS_HELP:
                continue
            if takes_value:
                for sopt in option._short_opts:
                    opts.append(sopt + ' ' + metavar)
                for lopt in option._long_opts:
                    opts.append(lopt + "=" + metavar)
            else:
                for opt in option._short_opts + option._long_opts:
                    opts.append(opt)
            opts = string.join(opts,", ")
            option_help.append((opts, option.help))
            lengths.append(len(opts))
        max_opts = min(max(lengths), 26)
        for (opts, help) in option_help:
            indent_rest = 2 + max_opts + 2 
            help_width = width - indent_rest
            if len(opts) > max_opts:
                opts = "  " + opts + "\n"
                indent_first = indent_rest
            else:                       # start help on same line as opts
                opts = "  %-*s  " % (max_opts, opts)
                indent_first = 0
            file.write(opts)
            if help:
                help_lines = wrap_text(help, help_width)
                file.write( "%*s%s\n" % (indent_first, "", help_lines[0]))
                for line in help_lines[1:]:
                    file.write("  %*s%s\n" % (indent_rest, "", line))
            elif opts[-1] != "\n":
                file.write("\n")
# class OptionParser
def _match_abbrev (s, wordmap):
    """_match_abbrev(s : string, wordmap : {string : Option}) -> string
    Return the string key in 'wordmap' for which 's' is an unambiguous
    abbreviation.  If 's' is found to be ambiguous or doesn't match any of
    'words', raise BadOptionError.
    """
    if wordmap.has_key(s):
        return s
    else:
        possibilities = []
        ls = len(s)
        for word in wordmap.keys():
            if len(word)>=ls and word[:ls]==s:
                possibilities.append(word)
        if len(possibilities) == 1:
            return possibilities[0]
        elif not possibilities:
            raise BadOptionError("no such option: %s" % s)
        else:
            raise BadOptionError("ambiguous option: %s (%s?)"
                                 % (s, string.join(possibilities,", ")))
# SCons:  Include a snarfed copy of wrap_text(), so we're not dependent
# on the right version of distutils being installed.
import re
WS_TRANS = string.maketrans(string.whitespace, ' ' * len(string.whitespace))
def wrap_text (text, width):
    """wrap_text(text : string, width : int) -> [string]
    Split 'text' into multiple lines of no more than 'width' characters
    each, and return the list of strings that results.
    """
    if text is None:
        return []
    if len(text) <= width:
        return [text]
    text = string.expandtabs(text)
    text = string.translate(text, WS_TRANS)
    chunks = re.split(r'( +|-+)', text)
    chunks = filter(None, chunks)      # ' - ' results in empty strings
    lines = []
    while chunks:
        cur_line = []                   # list of chunks (to-be-joined)
        cur_len = 0                     # length of current line
        while chunks:
            l = len(chunks[0])
            if cur_len + l <= width:    # can squeeze (at least) this chunk in
                cur_line.append(chunks[0])
                del chunks[0]
                cur_len = cur_len + l
            else:                       # this line is full
                if cur_line and cur_line[-1][0] == ' ':
                    del cur_line[-1]
                break
        if chunks:                      # any chunks left to process?
            if cur_len == 0:
                cur_line.append(chunks[0][0:width])
                chunks[0] = chunks[0][width:]
            if chunks[0][0] == ' ':
                del chunks[0]
        lines.append(string.join(cur_line, ''))
    return lines
# wrap_text ()
