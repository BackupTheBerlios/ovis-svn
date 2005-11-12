__revision__ = "src/engine/SCons/Environment.py 0.96.91 2005/10/17 20:04:26 ita"
import copy
import os
import os.path
import string
from UserDict import UserDict
import SCons.Action
import SCons.Builder
from SCons.Debug import logInstanceCreation
import SCons.Defaults
import SCons.Errors
import SCons.Node
import SCons.Node.Alias
import SCons.Node.FS
import SCons.Node.Python
import SCons.Platform
import SCons.SConsign
import SCons.Sig
import SCons.Sig.MD5
import SCons.Sig.TimeStamp
import SCons.Subst
import SCons.Tool
import SCons.Util
import SCons.Warnings
class _Null:
    pass
_null = _Null
CleanTargets = {}
CalculatorArgs = {}
# Pull UserError into the global name space for the benefit of
# Environment().SourceSignatures(), which has some import statements
# which seem to mess up its ability to reference SCons directly.
UserError = SCons.Errors.UserError
def installFunc(target, source, env):
    """Install a source file into a target using the function specified
    as the INSTALL construction variable."""
    try:
        install = env['INSTALL']
    except KeyError:
        raise SCons.Errors.UserError('Missing INSTALL construction variable.')
    return install(target[0].path, source[0].path, env)
def installString(target, source, env):
    return 'Install file: "%s" as "%s"' % (source[0], target[0])
installAction = SCons.Action.Action(installFunc, installString)
InstallBuilder = SCons.Builder.Builder(action=installAction,
                                       name='InstallBuilder')
def alias_builder(env, target, source):
    pass
AliasBuilder = SCons.Builder.Builder(action = alias_builder,
                                     target_factory = SCons.Node.Alias.default_ans.Alias,
                                     source_factory = SCons.Node.FS.Entry,
                                     multi = 1,
                                     is_explicit = None,
                                     name='AliasBuilder')
def our_deepcopy(x):
   """deepcopy lists and dictionaries, and just copy the reference
   for everything else."""
   if SCons.Util.is_Dict(x):
       copy = {}
       for key in x.keys():
           copy[key] = our_deepcopy(x[key])
   elif SCons.Util.is_List(x):
       copy = map(our_deepcopy, x)
       try:
           copy = x.__class__(copy)
       except AttributeError:
           pass
   else:
       copy = x
   return copy
def apply_tools(env, tools, toolpath):
    if toolpath is not None:
        env['toolpath'] = toolpath
    if not tools:
        return
    for tool in filter(None, tools):
        if SCons.Util.is_List(tool) or type(tool)==type(()):
            toolname = tool[0]
            toolargs = tool[1] # should be a dict of kw args
            tool = apply(env.Tool, [toolname], toolargs)
        else:
            env.Tool(tool)
# These names are controlled by SCons; users should never set or override
# them.  This warning can optionally be turned off, but scons will still
# ignore the illegal variable names even if it's off.
reserved_construction_var_names = \
    ['TARGET', 'TARGETS', 'SOURCE', 'SOURCES']
def copy_non_reserved_keywords(dict):
    result = our_deepcopy(dict)
    for k in result.keys():
        if k in reserved_construction_var_names:
            SCons.Warnings.warn(SCons.Warnings.ReservedVariableWarning,
                                "Ignoring attempt to set reserved variable `%s'" % k)
            del result[k]
    return result
def _set_reserved(env, key, value):
    msg = "Ignoring attempt to set reserved variable `%s'" % key
    SCons.Warnings.warn(SCons.Warnings.ReservedVariableWarning, msg)
def _set_BUILDERS(env, key, value):
    try:
        bd = env._dict[key]
        for k in bd.keys():
            del bd[k]
    except KeyError:
        env._dict[key] = BuilderDict(kwbd, env)
    env._dict[key].update(value)
def _set_SCANNERS(env, key, value):
    env._dict[key] = value
    env.scanner_map_delete()
class BuilderWrapper:
    """Wrapper class that associates an environment with a Builder at
    instantiation."""
    def __init__(self, env, builder):
        self.env = env
        self.builder = builder
    def __call__(self, target=None, source=_null, *args, **kw):
        if source is _null:
            source = target
            target = None
        if not target is None and not SCons.Util.is_List(target):
            target = [target]
        if not source is None and not SCons.Util.is_List(source):
            source = [source]
        return apply(self.builder, (self.env, target, source) + args, kw)
    def execute(self, **kw):
        kw['env'] = self.env
        apply(self.builder.execute, (), kw)
class BuilderDict(UserDict):
    """This is a dictionary-like class used by an Environment to hold
    the Builders.  We need to do this because every time someone changes
    the Builders in the Environment's BUILDERS dictionary, we must
    update the Environment's attributes."""
    def __init__(self, dict, env):
        self.env = env
        UserDict.__init__(self, dict)
    def __setitem__(self, item, val):
        UserDict.__setitem__(self, item, val)
        try:
            self.setenvattr(item, val)
        except AttributeError:
            pass
    def setenvattr(self, item, val):
        """Set the corresponding environment attribute for this Builder.
        If the value is already a BuilderWrapper, we pull the builder
        out of it and make another one, so that making a copy of an
        existing BuilderDict is guaranteed separate wrappers for each
        Builder + Environment pair."""
        try:
            builder = val.builder
        except AttributeError:
            builder = val
        setattr(self.env, item, BuilderWrapper(self.env, builder))
    def __delitem__(self, item):
        UserDict.__delitem__(self, item)
        delattr(self.env, item)
    def update(self, dict):
        for i, v in dict.items():
            self.__setitem__(i, v)
class SubstitutionEnvironment:
    """Base class for different flavors of construction environments.
    This class contains a minimal set of methods that handle contruction
    variable expansion and conversion of strings to Nodes, which may or
    may not be actually useful as a stand-alone class.  Which methods
    ended up in this class is pretty arbitrary right now.  They're
    basically the ones which we've empirically determined are common to
    the different construction environment subclasses, and most of the
    others that use or touch the underlying dictionary of construction
    variables.
    Eventually, this class should contain all the methods that we
    determine are necessary for a "minimal" interface to the build engine.
    A full "native Python" SCons environment has gotten pretty heavyweight
    with all of the methods and Tools and construction variables we've
    jammed in there, so it would be nice to have a lighter weight
    alternative for interfaces that don't need all of the bells and
    whistles.  (At some point, we'll also probably rename this class
    "Base," since that more reflects what we want this class to become,
    but because we've released comments that tell people to subclass
    Environment.Base to create their own flavors of construction
    environment, we'll save that for a future refactoring when this
    class actually becomes useful.)
    """
    if SCons.Memoize.use_memoizer:
        __metaclass__ = SCons.Memoize.Memoized_Metaclass
    def __init__(self, **kw):
        """Initialization of an underlying SubstitutionEnvironment class.
        """
        if __debug__: logInstanceCreation(self, 'Environment.SubstitutionEnvironment')
        self.fs = SCons.Node.FS.default_fs or SCons.Node.FS.FS()
        self.ans = SCons.Node.Alias.default_ans
        self.lookup_list = SCons.Node.arg2nodes_lookups
        self._dict = kw.copy()
        self._init_special()
    def _init_special(self):
        """Initial the dispatch table for special handling of
        special construction variables."""
        self._special = {}
        for key in reserved_construction_var_names:
            self._special[key] = _set_reserved
        self._special['BUILDERS'] = _set_BUILDERS
        self._special['SCANNERS'] = _set_SCANNERS
    def __cmp__(self, other):
        return cmp(self._dict, other._dict)
    def __delitem__(self, key):
        "__cache_reset__"
        del self._dict[key]
    def __getitem__(self, key):
        return self._dict[key]
    def __setitem__(self, key, value):
        "__cache_reset__"
        special = self._special.get(key)
        if special:
            special(self, key, value)
        else:
            if not SCons.Util.is_valid_construction_var(key):
                raise SCons.Errors.UserError, "Illegal construction variable `%s'" % key
            self._dict[key] = value
    def get(self, key, default=None):
        "Emulates the get() method of dictionaries."""
        return self._dict.get(key, default)
    def has_key(self, key):
        return self._dict.has_key(key)
    def items(self):
        return self._dict.items()
    def arg2nodes(self, args, node_factory=_null, lookup_list=_null):
        if node_factory is _null:
            node_factory = self.fs.File
        if lookup_list is _null:
            lookup_list = self.lookup_list
        if not args:
            return []
        if SCons.Util.is_List(args):
            args = SCons.Util.flatten(args)
        else:
            args = [args]
        nodes = []
        for v in args:
            if SCons.Util.is_String(v):
                n = None
                for l in lookup_list:
                    n = l(v)
                    if not n is None:
                        break
                if not n is None:
                    if SCons.Util.is_String(n):
                        n = self.subst(n, raw=1)
                        if node_factory:
                            n = node_factory(n)
                    if SCons.Util.is_List(n):
                        nodes.extend(n)
                    else:
                        nodes.append(n)
                elif node_factory:
                    v = node_factory(self.subst(v, raw=1))
                    if SCons.Util.is_List(v):
                        nodes.extend(v)
                    else:
                        nodes.append(v)
            else:
                nodes.append(v)
    
        return nodes
    def gvars(self):
        return self._dict
    def lvars(self):
        return {}
    def subst(self, string, raw=0, target=None, source=None, conv=None):
        """Recursively interpolates construction variables from the
        Environment into the specified string, returning the expanded
        result.  Construction variables are specified by a $ prefix
        in the string and begin with an initial underscore or
        alphabetic character followed by any number of underscores
        or alphanumeric characters.  The construction variable names
        may be surrounded by curly braces to separate the name from
        trailing characters.
        """
        gvars = self.gvars()
        lvars = self.lvars()
        lvars['__env__'] = self
        return SCons.Subst.scons_subst(string, self, raw, target, source, gvars, lvars, conv)
    def subst_kw(self, kw, raw=0, target=None, source=None):
        nkw = {}
        for k, v in kw.items():
            k = self.subst(k, raw, target, source)
            if SCons.Util.is_String(v):
                v = self.subst(v, raw, target, source)
            nkw[k] = v
        return nkw
    def subst_list(self, string, raw=0, target=None, source=None, conv=None):
        """Calls through to SCons.Subst.scons_subst_list().  See
        the documentation for that function."""
        gvars = self.gvars()
        lvars = self.lvars()
        lvars['__env__'] = self
        return SCons.Subst.scons_subst_list(string, self, raw, target, source, gvars, lvars, conv)
    def subst_path(self, path, target=None, source=None):
        """Substitute a path list, turning EntryProxies into Nodes
        and leaving Nodes (and other objects) as-is."""
        if not SCons.Util.is_List(path):
            path = [path]
        def s(obj):
            """This is the "string conversion" routine that we have our
            substitutions use to return Nodes, not strings.  This relies
            on the fact that an EntryProxy object has a get() method that
            returns the underlying Node that it wraps, which is a bit of
            architectural dependence that we might need to break or modify
            in the future in response to additional requirements."""
            try:
                get = obj.get
            except AttributeError:
                pass
            else:
                obj = get()
            return obj
        r = []
        for p in path:
            if SCons.Util.is_String(p):
                p = self.subst(p, target=target, source=source, conv=s)
                if SCons.Util.is_List(p):
                    if len(p) == 1:
                        p = p[0]
                    else:
                        p = string.join(map(SCons.Util.to_String, p), '')
            else:
                p = s(p)
            r.append(p)
        return r
    subst_target_source = subst
    def Override(self, overrides):
        """
        Produce a modified environment whose variables are overriden by
        the overrides dictionaries.  "overrides" is a dictionary that
        will override the variables of this environment.
        This function is much more efficient than Copy() or creating
        a new Environment because it doesn't copy the construction
        environment dictionary, it just wraps the underlying construction
        environment, and doesn't even create a wrapper object if there
        are no overrides.
        """
        if overrides:
            o = copy_non_reserved_keywords(overrides)
            overrides = {}
            for key, value in o.items():
                overrides[key] = SCons.Subst.scons_subst_once(value, self, key)
        if overrides:
            env = OverrideEnvironment(self, overrides)
            return env
        else:
            return self
class Base(SubstitutionEnvironment):
    """Base class for "real" construction Environments.  These are the
    primary objects used to communicate dependency and construction
    information to the build engine.
    Keyword arguments supplied when the construction Environment
    is created are construction variables used to initialize the
    Environment.
    """
    if SCons.Memoize.use_memoizer:
        __metaclass__ = SCons.Memoize.Memoized_Metaclass
    def __init__(self,
                 platform=None,
                 tools=None,
                 toolpath=None,
                 options=None,
                 **kw):
        """
        Initialization of a basic SCons construction environment,
        including setting up special construction variables like BUILDER,
        PLATFORM, etc., and searching for and applying available Tools.
        Note that we do *not* call the underlying base class
        (SubsitutionEnvironment) initialization, because we need to
        initialize things in a very specific order that doesn't work
        with the much simpler base class initialization.
        """
        if __debug__: logInstanceCreation(self, 'Environment.Base')
        self.fs = SCons.Node.FS.default_fs or SCons.Node.FS.FS()
        self.ans = SCons.Node.Alias.default_ans
        self.lookup_list = SCons.Node.arg2nodes_lookups
        self._dict = our_deepcopy(SCons.Defaults.ConstructionEnvironment)
        self._init_special()
        self._dict['BUILDERS'] = BuilderDict(self._dict['BUILDERS'], self)
        if platform is None:
            platform = self._dict.get('PLATFORM', None)
            if platform is None:
                platform = SCons.Platform.Platform()
        if SCons.Util.is_String(platform):
            platform = SCons.Platform.Platform(platform)
        self._dict['PLATFORM'] = str(platform)
        platform(self)
        apply(self.Replace, (), kw)
        if options:
            options.Update(self)
        if tools is None:
            tools = self._dict.get('TOOLS', None)
            if tools is None:
                tools = ['default']
        apply_tools(self, tools, toolpath)
        apply(self.Replace, (), kw)
        if options:
            options.Update(self)
    def get_builder(self, name):
        """Fetch the builder with the specified name from the environment.
        """
        try:
            return self._dict['BUILDERS'][name]
        except KeyError:
            return None
    def get_calculator(self):
        "__cacheable__"
        try:
            module = self._calc_module
            c = apply(SCons.Sig.Calculator, (module,), CalculatorArgs)
        except AttributeError:
            c = SCons.Defaults.DefaultEnvironment().get_calculator()
        return c
    def get_factory(self, factory, default='File'):
        """Return a factory function for creating Nodes for this
        construction environment.
        __cacheable__
        """
        name = default
        try:
            is_node = issubclass(factory, SCons.Node.Node)
        except TypeError:
            pass
        else:
            if is_node:
                try: name = factory.__name__
                except AttributeError: pass
                else: factory = None
        if not factory:
            factory = getattr(self.fs, name)
        return factory
    def _gsm(self):
        "__cacheable__"
        try:
            scanners = self._dict['SCANNERS']
        except KeyError:
            return None
        sm = {}
        if not SCons.Util.is_List(scanners):
            scanners = [scanners]
        else:
            scanners = scanners[:] # copy so reverse() doesn't mod original
        scanners.reverse()
        for scanner in scanners:
            for k in scanner.get_skeys(self):
                sm[k] = scanner
        return sm
        
    def get_scanner(self, skey):
        """Find the appropriate scanner given a key (usually a file suffix).
        """
        sm = self._gsm()
        try: return sm[skey]
        except (KeyError, TypeError): return None
    def _smd(self):
        "__reset_cache__"
        pass
    
    def scanner_map_delete(self, kw=None):
        """Delete the cached scanner map (if we need to).
        """
        if not kw is None and not kw.has_key('SCANNERS'):
            return
        self._smd()
    def _update(self, dict):
        """Update an environment's values directly, bypassing the normal
        checks that occur when users try to set items.
        __cache_reset__
        """
        self._dict.update(dict)
    def use_build_signature(self):
        try:
            return self._build_signature
        except AttributeError:
            b = SCons.Defaults.DefaultEnvironment()._build_signature
            self._build_signature = b
            return b
    def Append(self, **kw):
        """Append values to existing construction variables
        in an Environment.
        """
        kw = copy_non_reserved_keywords(kw)
        for key, val in kw.items():
            try:
                orig = self._dict[key]
            except KeyError:
                self._dict[key] = val
            else:
                try:
                    self._dict[key] = orig + val
                except TypeError:
                    try:
                        orig.update(val)
                    except AttributeError:
                        try:
                            add_to_orig = orig.append
                        except AttributeError:
                            if orig:
                                val.insert(0, orig)
                            self._dict[key] = val
                        else:
                            if val:
                                add_to_orig(val)
        self.scanner_map_delete(kw)
    def AppendENVPath(self, name, newpath, envname = 'ENV', sep = os.pathsep):
        """Append path elements to the path 'name' in the 'ENV'
        dictionary for this environment.  Will only add any particular
        path once, and will normpath and normcase all paths to help
        assure this.  This can also handle the case where the env
        variable is a list instead of a string.
        """
        orig = ''
        if self._dict.has_key(envname) and self._dict[envname].has_key(name):
            orig = self._dict[envname][name]
        nv = SCons.Util.AppendPath(orig, newpath, sep)
            
        if not self._dict.has_key(envname):
            self._dict[envname] = {}
        self._dict[envname][name] = nv
    def AppendUnique(self, **kw):
        """Append values to existing construction variables
        in an Environment, if they're not already there.
        """
        kw = copy_non_reserved_keywords(kw)
        for key, val in kw.items():
            if not self._dict.has_key(key) or not self._dict[key]:
                self._dict[key] = val
            elif SCons.Util.is_Dict(self._dict[key]) and \
                 SCons.Util.is_Dict(val):
                self._dict[key].update(val)
            elif SCons.Util.is_List(val):
                dk = self._dict[key]
                if not SCons.Util.is_List(dk):
                    dk = [dk]
                val = filter(lambda x, dk=dk: x not in dk, val)
                self._dict[key] = dk + val
            else:
                dk = self._dict[key]
                if SCons.Util.is_List(dk):
                    if not val in dk:
                        self._dict[key] = dk + val
                else:
                    self._dict[key] = self._dict[key] + val
        self.scanner_map_delete(kw)
    def Copy(self, tools=[], toolpath=None, **kw):
        """Return a copy of a construction Environment.  The
        copy is like a Python "deep copy"--that is, independent
        copies are made recursively of each objects--except that
        a reference is copied when an object is not deep-copyable
        (like a function).  There are no references to any mutable
        objects in the original Environment.
        """
        clone = copy.copy(self)
        clone._dict = our_deepcopy(self._dict)
        try:
            cbd = clone._dict['BUILDERS']
            clone._dict['BUILDERS'] = BuilderDict(cbd, clone)
        except KeyError:
            pass
        
        apply_tools(clone, tools, toolpath)
        kw = copy_non_reserved_keywords(kw)
        new = {}
        for key, value in kw.items():
            new[key] = SCons.Subst.scons_subst_once(value, self, key)
        apply(clone.Replace, (), new)
        if __debug__: logInstanceCreation(self, 'Environment.EnvironmentCopy')
        return clone
    def Detect(self, progs):
        """Return the first available program in progs.  __cacheable__
        """
        if not SCons.Util.is_List(progs):
            progs = [ progs ]
        for prog in progs:
            path = self.WhereIs(prog)
            if path: return prog
        return None
    def Dictionary(self, *args):
        if not args:
            return self._dict
        dlist = map(lambda x, s=self: s._dict[x], args)
        if len(dlist) == 1:
            dlist = dlist[0]
        return dlist
    def Dump(self, key = None):
        """
        Using the standard Python pretty printer, dump the contents of the
        scons build environment to stdout.
        If the key passed in is anything other than None, then that will
        be used as an index into the build environment dictionary and
        whatever is found there will be fed into the pretty printer. Note
        that this key is case sensitive.
        """
        import pprint
        pp = pprint.PrettyPrinter(indent=2)
        if key:
            dict = self.Dictionary(key)
        else:
            dict = self.Dictionary()
        return pp.pformat(dict)
    def FindIxes(self, paths, prefix, suffix):
        """
        Search a list of paths for something that matches the prefix and suffix.
        paths - the list of paths or nodes.
        prefix - construction variable for the prefix.
        suffix - construction variable for the suffix.
        """
        suffix = self.subst('$'+suffix)
        prefix = self.subst('$'+prefix)
        for path in paths:
            dir,name = os.path.split(str(path))
            if name[:len(prefix)] == prefix and name[-len(suffix):] == suffix: 
                return path
    def ParseConfig(self, command, function=None, unique=1):
        """
        Use the specified function to parse the output of the command
        in order to modify the current environment. The 'command' can
        be a string or a list of strings representing a command and
        it's arguments. 'Function' is an optional argument that takes
        the environment and the output of the command. If no function is
        specified, the output will be treated as the output of a typical
        'X-config' command (i.e. gtk-config) and used to append to the
        ASFLAGS, CCFLAGS, CPPFLAGS, CPPPATH, LIBPATH, LIBS, LINKFLAGS
        and CCFLAGS variables.
        """
        def parse_conf(env, output, fs=self.fs, unique=unique):
            dict = {
                'ASFLAGS'       : [],
                'CCFLAGS'       : [],
                'CPPFLAGS'      : [],
                'CPPPATH'       : [],
                'LIBPATH'       : [],
                'LIBS'          : [],
                'LINKFLAGS'     : [],
            }
    
            params = string.split(output)
            append_next_arg_to=''       # for multi-word args
            for arg in params:
                if append_next_arg_to:
                    dict[append_next_arg_to].append(arg)
                    append_next_arg_to = ''
                elif arg[0] != '-':
                    dict['LIBS'].append(fs.File(arg))
                elif arg[:2] == '-L':
                    if arg[2:]:
                        dict['LIBPATH'].append(arg[2:])
                    else:
                        append_next_arg_to = 'LIBPATH'
                elif arg[:2] == '-l':
                    if arg[2:]:
                        dict['LIBS'].append(arg[2:])
                    else:
                        append_next_arg_to = 'LIBS'
                elif arg[:2] == '-I':
                    if arg[2:]:
                        dict['CPPPATH'].append(arg[2:])
                    else:
                        append_next_arg_to = 'CPPPATH'
                elif arg[:4] == '-Wa,':
                    dict['ASFLAGS'].append(arg)
                elif arg[:4] == '-Wl,':
                    dict['LINKFLAGS'].append(arg)
                elif arg[:4] == '-Wp,':
                    dict['CPPFLAGS'].append(arg)
                elif arg == '-framework':
                    dict['LINKFLAGS'].append(arg)
                    append_next_arg_to='LINKFLAGS'
                elif arg == '-mno-cygwin':
                    dict['CCFLAGS'].append(arg)
                    dict['LINKFLAGS'].append(arg)
                elif arg == '-mwindows':
                    dict['LINKFLAGS'].append(arg)
                elif arg == '-pthread':
                    dict['CCFLAGS'].append(arg)
                    dict['LINKFLAGS'].append(arg)
                else:
                    dict['CCFLAGS'].append(arg)
            if unique:
                appender = env.AppendUnique
            else:
                appender = env.Append
            apply(appender, (), dict)
    
        if function is None:
            function = parse_conf
        if type(command) is type([]):
            command = string.join(command)
        command = self.subst(command)
        return function(self, os.popen(command).read())
    def ParseDepends(self, filename, must_exist=None, only_one=0):
        """
        Parse a mkdep-style file for explicit dependencies.  This is
        completely abusable, and should be unnecessary in the "normal"
        case of proper SCons configuration, but it may help make
        the transition from a Make hierarchy easier for some people
        to swallow.  It can also be genuinely useful when using a tool
        that can write a .d file, but for which writing a scanner would
        be too complicated.
        """
        filename = self.subst(filename)
        try:
            fp = open(filename, 'r')
        except IOError:
            if must_exist:
                raise
            return
        lines = SCons.Util.LogicalLines(fp).readlines()
        lines = filter(lambda l: l[0] != '#', lines)
        tdlist = []
        for line in lines:
            try:
                target, depends = string.split(line, ':', 1)
            except (AttributeError, TypeError, ValueError):
                pass
            else:
                tdlist.append((string.split(target), string.split(depends)))
        if only_one:
            targets = reduce(lambda x, y: x+y, map(lambda p: p[0], tdlist))
            if len(targets) > 1:
                raise SCons.Errors.UserError, "More than one dependency target found in `%s':  %s" % (filename, targets)
        for target, depends in tdlist:
            self.Depends(target, depends)
    def Platform(self, platform):
        platform = self.subst(platform)
        return SCons.Platform.Platform(platform)(self)
    def Prepend(self, **kw):
        """Prepend values to existing construction variables
        in an Environment.
        """
        kw = copy_non_reserved_keywords(kw)
        for key, val in kw.items():
            try:
                orig = self._dict[key]
            except KeyError:
                self._dict[key] = val
            else:
                try:
                    self._dict[key] = val + orig
                except TypeError:
                    try:
                        orig.update(val)
                    except AttributeError:
                        try:
                            add_to_val = val.append
                        except AttributeError:
                            if val:
                                orig.insert(0, val)
                        else:
                            if orig:
                                add_to_val(orig)
                            self._dict[key] = val
        self.scanner_map_delete(kw)
    def PrependENVPath(self, name, newpath, envname = 'ENV', sep = os.pathsep):
        """Prepend path elements to the path 'name' in the 'ENV'
        dictionary for this environment.  Will only add any particular
        path once, and will normpath and normcase all paths to help
        assure this.  This can also handle the case where the env
        variable is a list instead of a string.
        """
        orig = ''
        if self._dict.has_key(envname) and self._dict[envname].has_key(name):
            orig = self._dict[envname][name]
        nv = SCons.Util.PrependPath(orig, newpath, sep)
            
        if not self._dict.has_key(envname):
            self._dict[envname] = {}
        self._dict[envname][name] = nv
    def PrependUnique(self, **kw):
        """Append values to existing construction variables
        in an Environment, if they're not already there.
        """
        kw = copy_non_reserved_keywords(kw)
        for key, val in kw.items():
            if not self._dict.has_key(key) or not self._dict[key]:
                self._dict[key] = val
            elif SCons.Util.is_Dict(self._dict[key]) and \
                 SCons.Util.is_Dict(val):
                self._dict[key].update(val)
            elif SCons.Util.is_List(val):
                dk = self._dict[key]
                if not SCons.Util.is_List(dk):
                    dk = [dk]
                val = filter(lambda x, dk=dk: x not in dk, val)
                self._dict[key] = val + dk
            else:
                dk = self._dict[key]
                if SCons.Util.is_List(dk):
                    if not val in dk:
                        self._dict[key] = val + dk
                else:
                    self._dict[key] = val + dk
        self.scanner_map_delete(kw)
    def Replace(self, **kw):
        """Replace existing construction variables in an Environment
        with new construction variables and/or values.
        """
        try:
            kwbd = our_deepcopy(kw['BUILDERS'])
            del kw['BUILDERS']
            self.__setitem__('BUILDERS', kwbd)
        except KeyError:
            pass
        kw = copy_non_reserved_keywords(kw)
        self._update(our_deepcopy(kw))
        self.scanner_map_delete(kw)
    def ReplaceIxes(self, path, old_prefix, old_suffix, new_prefix, new_suffix):
        """
        Replace old_prefix with new_prefix and old_suffix with new_suffix.
        env - Environment used to interpolate variables.
        path - the path that will be modified.
        old_prefix - construction variable for the old prefix.
        old_suffix - construction variable for the old suffix.
        new_prefix - construction variable for the new prefix.
        new_suffix - construction variable for the new suffix.
        """
        old_prefix = self.subst('$'+old_prefix)
        old_suffix = self.subst('$'+old_suffix)
        new_prefix = self.subst('$'+new_prefix)
        new_suffix = self.subst('$'+new_suffix)
        dir,name = os.path.split(str(path))
        if name[:len(old_prefix)] == old_prefix:
            name = name[len(old_prefix):]
        if name[-len(old_suffix):] == old_suffix:
            name = name[:-len(old_suffix)]
        return os.path.join(dir, new_prefix+name+new_suffix)
    def SetDefault(self, **kw):
        for k in kw.keys():
            if self._dict.has_key(k):
                del kw[k]
        apply(self.Replace, (), kw)
    def Tool(self, tool, toolpath=None, **kw):
        if SCons.Util.is_String(tool):
            tool = self.subst(tool)
            if toolpath is None:
                toolpath = self.get('toolpath', [])
            toolpath = map(self.subst, toolpath)
            tool = apply(SCons.Tool.Tool, (tool, toolpath), kw)
        tool(self)
    def WhereIs(self, prog, path=None, pathext=None, reject=[]):
        """Find prog in the path.  __cacheable__
        """
        if path is None:
            try:
                path = self['ENV']['PATH']
            except KeyError:
                pass
        elif SCons.Util.is_String(path):
            path = self.subst(path)
        if pathext is None:
            try:
                pathext = self['ENV']['PATHEXT']
            except KeyError:
                pass
        elif SCons.Util.is_String(pathext):
            pathext = self.subst(pathext)
        path = SCons.Util.WhereIs(prog, path, pathext, reject)
        if path: return path
        return None
    def Action(self, *args, **kw):
        nargs = self.subst(args)
        nkw = self.subst_kw(kw)
        return apply(SCons.Action.Action, nargs, nkw)
    def AddPreAction(self, files, action):
        nodes = self.arg2nodes(files, self.fs.Entry)
        action = SCons.Action.Action(action)
        uniq = {}
        for executor in map(lambda n: n.get_executor(), nodes):
            uniq[executor] = 1
        for executor in uniq.keys():
            executor.add_pre_action(action)
        return nodes
    def AddPostAction(self, files, action):
        nodes = self.arg2nodes(files, self.fs.Entry)
        action = SCons.Action.Action(action)
        uniq = {}
        for executor in map(lambda n: n.get_executor(), nodes):
            uniq[executor] = 1
        for executor in uniq.keys():
            executor.add_post_action(action)
        return nodes
    def Alias(self, target, source=[], action=None, **kw):
        tlist = self.arg2nodes(target, self.ans.Alias)
        if not SCons.Util.is_List(source):
            source = [source]
        source = filter(None, source)
        if not action:
            if not source:
                return tlist
            result = []
            for t in tlist:
                bld = t.get_builder(AliasBuilder)
                result.extend(bld(self, t, source))
            return result
        nkw = self.subst_kw(kw)
        nkw.update({
            'action'            : SCons.Action.Action(action),
            'source_factory'    : self.fs.Entry,
            'multi'             : 1,
            'is_explicit'       : None,
        })
        bld = apply(SCons.Builder.Builder, (), nkw)
        result = []
        for t in tlist:
            b = t.get_builder()
            if b is None or b is AliasBuilder:
                b = bld
            else:
                nkw['action'] = b.action + action
                b = apply(SCons.Builder.Builder, (), nkw)
            t.convert()
            result.extend(b(self, t, t.sources + source))
        return result
    def AlwaysBuild(self, *targets):
        tlist = []
        for t in targets:
            tlist.extend(self.arg2nodes(t, self.fs.File))
        for t in tlist:
            t.set_always_build()
        return tlist
    def BuildDir(self, build_dir, src_dir, duplicate=1):
        build_dir = self.arg2nodes(build_dir, self.fs.Dir)[0]
        src_dir = self.arg2nodes(src_dir, self.fs.Dir)[0]
        self.fs.BuildDir(build_dir, src_dir, duplicate)
    def Builder(self, **kw):
        nkw = self.subst_kw(kw)
        return apply(SCons.Builder.Builder, [], nkw)
    def CacheDir(self, path):
        self.fs.CacheDir(self.subst(path))
    def Clean(self, targets, files):
        global CleanTargets
        tlist = self.arg2nodes(targets, self.fs.Entry)
        flist = self.arg2nodes(files, self.fs.Entry)
        for t in tlist:
            try:
                CleanTargets[t].extend(flist)
            except KeyError:
                CleanTargets[t] = flist
    def Configure(self, *args, **kw):
        nargs = [self]
        if args:
            nargs = nargs + self.subst_list(args)[0]
        nkw = self.subst_kw(kw)
        nkw['_depth'] = kw.get('_depth', 0) + 1
        try:
            nkw['custom_tests'] = self.subst_kw(nkw['custom_tests'])
        except KeyError:
            pass
        return apply(SCons.SConf.SConf, nargs, nkw)
    def Command(self, target, source, action, **kw):
        """Builds the supplied target files from the supplied
        source files using the supplied action.  Action may
        be any type that the Builder constructor will accept
        for an action."""
        bkw = {
            'action' : action,
            'source_factory' : self.fs.Entry,
        }
        try: bkw['source_scanner'] = kw['source_scanner']
        except KeyError: pass
        else: del kw['source_scanner']
        bld = apply(SCons.Builder.Builder, (), bkw)
        return apply(bld, (self, target, source), kw)
    def Depends(self, target, dependency):
        """Explicity specify that 'target's depend on 'dependency'."""
        tlist = self.arg2nodes(target, self.fs.Entry)
        dlist = self.arg2nodes(dependency, self.fs.Entry)
        for t in tlist:
            t.add_dependency(dlist)
        return tlist
    def Dir(self, name, *args, **kw):
        """
        """
        return apply(self.fs.Dir, (self.subst(name),) + args, kw)
    def Entry(self, name, *args, **kw):
        """
        """
        return apply(self.fs.Entry, (self.subst(name),) + args, kw)
    def Environment(self, **kw):
        return apply(SCons.Environment.Environment, [], self.subst_kw(kw))
    def Execute(self, action, *args, **kw):
        """Directly execute an action through an Environment
        """
        action = apply(self.Action, (action,) + args, kw)
        return action([], [], self)
    def File(self, name, *args, **kw):
        """
        """
        return apply(self.fs.File, (self.subst(name),) + args, kw)
    def FindFile(self, file, dirs):
        file = self.subst(file)
        nodes = self.arg2nodes(dirs, self.fs.Dir)
        return SCons.Node.FS.find_file(file, tuple(nodes))
    def Flatten(self, sequence):
        return SCons.Util.flatten(sequence)
    def GetBuildPath(self, files):
        result = map(str, self.arg2nodes(files, self.fs.Entry))
        if SCons.Util.is_List(files):
            return result
        else:
            return result[0]
    def Ignore(self, target, dependency):
        """Ignore a dependency."""
        tlist = self.arg2nodes(target, self.fs.Entry)
        dlist = self.arg2nodes(dependency, self.fs.Entry)
        for t in tlist:
            t.add_ignore(dlist)
        return tlist
    def Install(self, dir, source):
        """Install specified files in the given directory."""
        try:
            dnodes = self.arg2nodes(dir, self.fs.Dir)
        except TypeError:
            raise SCons.Errors.UserError, "Target `%s' of Install() is a file, but should be a directory.  Perhaps you have the Install() arguments backwards?" % str(dir)
        try:
            sources = self.arg2nodes(source, self.fs.File)
        except TypeError:
            if SCons.Util.is_List(source):
                raise SCons.Errors.UserError, "Source `%s' of Install() contains one or more non-files.  Install() source must be one or more files." % repr(map(str, source))
            else:
                raise SCons.Errors.UserError, "Source `%s' of Install() is not a file.  Install() source must be one or more files." % str(source)
        tgt = []
        for dnode in dnodes:
            for src in sources:
                target = self.fs.File(src.name, dnode)
                tgt.extend(InstallBuilder(self, target, src))
        return tgt
    def InstallAs(self, target, source):
        """Install sources as targets."""
        sources = self.arg2nodes(source, self.fs.File)
        targets = self.arg2nodes(target, self.fs.File)
        result = []
        for src, tgt in map(lambda x, y: (x, y), sources, targets):
            result.extend(InstallBuilder(self, tgt, src))
        return result
    def Literal(self, string):
        return SCons.Subst.Literal(string)
    def Local(self, *targets):
        ret = []
        for targ in targets:
            if isinstance(targ, SCons.Node.Node):
                targ.set_local()
                ret.append(targ)
            else:
                for t in self.arg2nodes(targ, self.fs.Entry):
                   t.set_local()
                   ret.append(t)
        return ret
    def Precious(self, *targets):
        tlist = []
        for t in targets:
            tlist.extend(self.arg2nodes(t, self.fs.Entry))
        for t in tlist:
            t.set_precious()
        return tlist
    def Repository(self, *dirs, **kw):
        dirs = self.arg2nodes(list(dirs), self.fs.Dir)
        apply(self.fs.Repository, dirs, kw)
    def Scanner(self, *args, **kw):
        nargs = []
        for arg in args:
            if SCons.Util.is_String(arg):
                arg = self.subst(arg)
            nargs.append(arg)
        nkw = self.subst_kw(kw)
        return apply(SCons.Scanner.Scanner, nargs, nkw)
    def SConsignFile(self, name=".sconsign", dbm_module=None):
        if not name is None:
            name = self.subst(name)
            if not os.path.isabs(name):
                name = os.path.join(str(self.fs.SConstruct_dir), name)
        SCons.SConsign.File(name, dbm_module)
    def SideEffect(self, side_effect, target):
        """Tell scons that side_effects are built as side 
        effects of building targets."""
        side_effects = self.arg2nodes(side_effect, self.fs.Entry)
        targets = self.arg2nodes(target, self.fs.Entry)
        for side_effect in side_effects:
            if side_effect.multiple_side_effect_has_builder():
                raise SCons.Errors.UserError, "Multiple ways to build the same target were specified for: %s" % str(side_effect)
            side_effect.add_source(targets)
            side_effect.side_effect = 1
            self.Precious(side_effect)
            for target in targets:
                target.side_effects.append(side_effect)
        return side_effects
    def SourceCode(self, entry, builder):
        """Arrange for a source code builder for (part of) a tree."""
        entries = self.arg2nodes(entry, self.fs.Entry)
        for entry in entries:
            entry.set_src_builder(builder)
        return entries
    def SourceSignatures(self, type):
        type = self.subst(type)
        if type == 'MD5':
            import SCons.Sig.MD5
            self._calc_module = SCons.Sig.MD5
        elif type == 'timestamp':
            import SCons.Sig.TimeStamp
            self._calc_module = SCons.Sig.TimeStamp
        else:
            raise UserError, "Unknown source signature type '%s'"%type
    def Split(self, arg):
        """This function converts a string or list into a list of strings
        or Nodes.  This makes things easier for users by allowing files to
        be specified as a white-space separated list to be split.
        The input rules are:
            - A single string containing names separated by spaces. These will be
              split apart at the spaces.
            - A single Node instance
            - A list containing either strings or Node instances. Any strings
              in the list are not split at spaces.
        In all cases, the function returns a list of Nodes and strings."""
        if SCons.Util.is_List(arg):
            return map(self.subst, arg)
        elif SCons.Util.is_String(arg):
            return string.split(self.subst(arg))
        else:
            return [self.subst(arg)]
    def TargetSignatures(self, type):
        type = self.subst(type)
        if type == 'build':
            self._build_signature = 1
        elif type == 'content':
            self._build_signature = 0
        else:
            raise SCons.Errors.UserError, "Unknown target signature type '%s'"%type
    def Value(self, value):
        """
        """
        return SCons.Node.Python.Value(value)
class OverrideEnvironment(Base):
    """A proxy that overrides variables in a wrapped construction
    environment by returning values from an overrides dictionary in
    preference to values from the underlying subject environment.
    This is a lightweight (I hope) proxy that passes through most use of
    attributes to the underlying Environment.Base class, but has just
    enough additional methods defined to act like a real construction
    environment with overridden values.  It can wrap either a Base
    construction environment, or another OverrideEnvironment, which
    can in turn nest arbitrary OverrideEnvironments...
    Note that we do *not* call the underlying base class
    (SubsitutionEnvironment) initialization, because we get most of those
    from proxying the attributes of the subject construction environment.
    But because we subclass SubstitutionEnvironment, this class also
    has inherited arg2nodes() and subst*() methods; those methods can't
    be proxied because they need *this* object's methods to fetch the
    values from the overrides dictionary.
    """
    if SCons.Memoize.use_memoizer:
        __metaclass__ = SCons.Memoize.Memoized_Metaclass
    def __init__(self, subject, overrides={}):
        if __debug__: logInstanceCreation(self, 'Environment.OverrideEnvironment')
        self.__dict__['__subject'] = subject
        self.__dict__['overrides'] = overrides
    def __getattr__(self, name):
        return getattr(self.__dict__['__subject'], name)
    def __setattr__(self, name, value):
        return setattr(self.__dict__['__subject'], name, value)
    def __getitem__(self, key):
        try:
            return self.__dict__['overrides'][key]
        except KeyError:
            return self.__dict__['__subject'].__getitem__(key)
    def __setitem__(self, key, value):
        if not SCons.Util.is_valid_construction_var(key):
            raise SCons.Errors.UserError, "Illegal construction variable `%s'" % key
        self.__dict__['overrides'][key] = value
    def __delitem__(self, key):
        try:
            del self.__dict__['overrides'][key]
        except KeyError:
            deleted = 0
        else:
            deleted = 1
        try:
            result = self.__dict__['__subject'].__delitem__(key)
        except KeyError:
            if not deleted:
                raise
            result = None
        return result
    def get(self, key, default=None):
        """Emulates the get() method of dictionaries."""
        try:
            return self.__dict__['overrides'][key]
        except KeyError:
            return self.__dict__['__subject'].get(key, default)
    def has_key(self, key):
        try:
            self.__dict__['overrides'][key]
            return 1
        except KeyError:
            return self.__dict__['__subject'].has_key(key)
    def Dictionary(self):
        """Emulates the items() method of dictionaries."""
        d = self.__dict__['__subject'].Dictionary().copy()
        d.update(self.__dict__['overrides'])
        return d
    def items(self):
        """Emulates the items() method of dictionaries."""
        return self.Dictionary().items()
    def _update(self, dict):
        """Update an environment's values directly, bypassing the normal
        checks that occur when users try to set items.
        """
        self.__dict__['overrides'].update(dict)
    def gvars(self):
        return self.__dict__['__subject'].gvars()
    def lvars(self):
        lvars = self.__dict__['__subject'].lvars()
        lvars.update(self.__dict__['overrides'])
        return lvars
    def Replace(self, **kw):
        kw = copy_non_reserved_keywords(kw)
        self.__dict__['overrides'].update(our_deepcopy(kw))
# The entry point that will be used by the external world
# to refer to a construction environment.  This allows the wrapper
# interface to extend a construction environment for its own purposes
# by subclassing SCons.Environment.Base and then assigning the
# class to SCons.Environment.Environment.
Environment = Base
# An entry point for returning a proxy subclass instance that overrides
# the subst*() methods so they don't actually perform construction
# variable substitution.  This is specifically intended to be the shim
# layer in between global function calls (which don't want construction
# variable substitution) and the DefaultEnvironment() (which would
# substitute variables if left to its own devices)."""
#
# We have to wrap this in a function that allows us to delay definition of
# the class until it's necessary, so that when it subclasses Environment
# it will pick up whatever Environment subclass the wrapper interface
# might have assigned to SCons.Environment.Environment.
def NoSubstitutionProxy(subject):
    class _NoSubstitutionProxy(Environment):
        def __init__(self, subject):
            self.__dict__['__subject'] = subject
        def __getattr__(self, name):
            return getattr(self.__dict__['__subject'], name)
        def __setattr__(self, name, value):
            return setattr(self.__dict__['__subject'], name, value)
        def raw_to_mode(self, dict):
            try:
                raw = dict['raw']
            except KeyError:
                pass
            else:
                del dict['raw']
                dict['mode'] = raw
        def subst(self, string, *args, **kwargs):
            return string
        def subst_kw(self, kw, *args, **kwargs):
            return kw
        def subst_list(self, string, *args, **kwargs):
            nargs = (string, self,) + args
            nkw = kwargs.copy()
            nkw['gvars'] = {}
            self.raw_to_mode(nkw)
            return apply(SCons.Subst.scons_subst_list, nargs, nkw)
        def subst_target_source(self, string, *args, **kwargs):
            nargs = (string, self,) + args
            nkw = kwargs.copy()
            nkw['gvars'] = {}
            self.raw_to_mode(nkw)
            return apply(SCons.Subst.scons_subst, nargs, nkw)
    return _NoSubstitutionProxy(subject)
if SCons.Memoize.use_old_memoization():
    _Base = Base
    class Base(SCons.Memoize.Memoizer, _Base):
        def __init__(self, *args, **kw):
            SCons.Memoize.Memoizer.__init__(self)
            apply(_Base.__init__, (self,)+args, kw)
    Environment = Base
