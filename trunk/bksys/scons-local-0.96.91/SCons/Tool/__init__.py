__revision__ = "src/engine/SCons/Tool/__init__.py 0.96.91 2005/10/17 20:04:26 ita"
import imp
import sys
import SCons.Errors
import SCons.Scanner
import SCons.Scanner.C
import SCons.Scanner.Prog
CScanner = SCons.Scanner.C.CScanner()
ProgramScanner = SCons.Scanner.Prog.ProgramScanner()
SourceFileScanner = SCons.Scanner.Scanner({}, name='SourceFileScanner')
CSuffixes = [".c", ".C", ".cxx", ".cpp", ".c++", ".cc",
             ".h", ".H", ".hxx", ".hpp", ".hh",
             ".F", ".fpp", ".FPP",
             ".m", ".mm",
             ".S", ".spp", ".SPP"]
IDLSuffixes = [".idl", ".IDL"]
for suffix in CSuffixes:
    SourceFileScanner.add_scanner(suffix, CScanner)
class Tool:
    def __init__(self, name, toolpath=[], **kw):
        self.name = name
        self.toolpath = toolpath
        self.init_kw = kw
        module = self._tool_module()
        self.generate = module.generate
        self.exists = module.exists
    def _tool_module(self):
        oldpythonpath = sys.path
        sys.path = self.toolpath + sys.path
        try:
            try:
                file, path, desc = imp.find_module(self.name, self.toolpath)
                try:
                    return imp.load_module(self.name, file, path, desc)
                finally:
                    if file:
                        file.close()
            except ImportError, e:
                pass
        finally:
            sys.path = oldpythonpath
        full_name = 'SCons.Tool.' + self.name
        try:
            return sys.modules[full_name]
        except KeyError:
            try:
                smpath = sys.modules['SCons.Tool'].__path__
                file, path, desc = imp.find_module(self.name, smpath)
                try:
                    module = imp.load_module(full_name, file, path, desc)
                    setattr(SCons.Tool, self.name, module)
                    return module
                finally:
                    if file:
                        file.close()
            except ImportError, e:
                m = "No tool named '%s': %s" % (self.name, e)
                raise SCons.Errors.UserError, m
    def __call__(self, env, *args, **kw):
        if self.init_kw is not None:
            if kw is not None:
                call_kw = kw
                kw = self.init_kw.copy()
                kw.update(call_kw)
            else:
                kw = self.init_kw
        env.Append(TOOLS = [ self.name ])
        apply(self.generate, ( env, ) + args, kw)
    def __str__(self):
        return self.name
def createProgBuilder(env):
    """This is a utility function that creates the Program
    Builder in an Environment if it is not there already.
    If it is already there, we return the existing one.
    """
    try:
        program = env['BUILDERS']['Program']
    except KeyError:
        import SCons.Defaults
        program = SCons.Builder.Builder(action = SCons.Defaults.LinkAction,
                                        emitter = '$PROGEMITTER',
                                        prefix = '$PROGPREFIX',
                                        suffix = '$PROGSUFFIX',
                                        src_suffix = '$OBJSUFFIX',
                                        src_builder = 'Object',
                                        target_scanner = ProgramScanner)
        env['BUILDERS']['Program'] = program
    return program
def createStaticLibBuilder(env):
    """This is a utility function that creates the StaticLibrary
    Builder in an Environment if it is not there already.
    If it is already there, we return the existing one.
    """
    try:
        static_lib = env['BUILDERS']['StaticLibrary']
    except KeyError:
        ar_action = SCons.Action.Action("$ARCOM", "$ARCOMSTR")
        static_lib = SCons.Builder.Builder(action = ar_action,
                                           emitter = '$LIBEMITTER',
                                           prefix = '$LIBPREFIX',
                                           suffix = '$LIBSUFFIX',
                                           src_suffix = '$OBJSUFFIX',
                                           src_builder = 'StaticObject')
        env['BUILDERS']['StaticLibrary'] = static_lib
        env['BUILDERS']['Library'] = static_lib
    return static_lib
def createSharedLibBuilder(env):
    """This is a utility function that creates the SharedLibrary
    Builder in an Environment if it is not there already.
    If it is already there, we return the existing one.
    """
    try:
        shared_lib = env['BUILDERS']['SharedLibrary']
    except KeyError:
        import SCons.Defaults
        action_list = [ SCons.Defaults.SharedCheck,
                        SCons.Defaults.ShLinkAction ]
        shared_lib = SCons.Builder.Builder(action = action_list,
                                           emitter = "$SHLIBEMITTER",
                                           prefix = '$SHLIBPREFIX',
                                           suffix = '$SHLIBSUFFIX',
                                           target_scanner = ProgramScanner,
                                           src_suffix = '$SHOBJSUFFIX',
                                           src_builder = 'SharedObject')
        env['BUILDERS']['SharedLibrary'] = shared_lib
    return shared_lib
def createLoadableModuleBuilder(env):
    """This is a utility function that creates the LoadableModule
    Builder in an Environment if it is not there already.
    If it is already there, we return the existing one.
    """
    try:
        ld_module = env['BUILDERS']['LoadableModule']
    except KeyError:
        import SCons.Defaults
        action_list = [ SCons.Defaults.SharedCheck,
                        SCons.Defaults.LdModuleLinkAction ]
        ld_module = SCons.Builder.Builder(action = action_list,
                                          emitter = "$SHLIBEMITTER",
                                          prefix = '$LDMODULEPREFIX',
                                          suffix = '$LDMODULESUFFIX',
                                          target_scanner = ProgramScanner,
                                          src_suffix = '$SHOBJSUFFIX',
                                          src_builder = 'SharedObject')
        env['BUILDERS']['LoadableModule'] = ld_module
    return ld_module
def createObjBuilders(env):
    """This is a utility function that creates the StaticObject
    and SharedObject Builders in an Environment if they
    are not there already.
    If they are there already, we return the existing ones.
    This is a separate function because soooo many Tools
    use this functionality.
    The return is a 2-tuple of (StaticObject, SharedObject)
    """
    try:
        static_obj = env['BUILDERS']['StaticObject']
    except KeyError:
        static_obj = SCons.Builder.Builder(action = {},
                                           emitter = {},
                                           prefix = '$OBJPREFIX',
                                           suffix = '$OBJSUFFIX',
                                           src_builder = ['CFile', 'CXXFile'],
                                           source_scanner = SourceFileScanner,
                                           single_source = 1)
        env['BUILDERS']['StaticObject'] = static_obj
        env['BUILDERS']['Object'] = static_obj
    try:
        shared_obj = env['BUILDERS']['SharedObject']
    except KeyError:
        shared_obj = SCons.Builder.Builder(action = {},
                                           emitter = {},
                                           prefix = '$SHOBJPREFIX',
                                           suffix = '$SHOBJSUFFIX',
                                           src_builder = ['CFile', 'CXXFile'],
                                           source_scanner = SourceFileScanner,
                                           single_source = 1)
        env['BUILDERS']['SharedObject'] = shared_obj
    return (static_obj, shared_obj)
def createCFileBuilders(env):
    """This is a utility function that creates the CFile/CXXFile
    Builders in an Environment if they
    are not there already.
    If they are there already, we return the existing ones.
    This is a separate function because soooo many Tools
    use this functionality.
    The return is a 2-tuple of (CFile, CXXFile)
    """
    try:
        c_file = env['BUILDERS']['CFile']
    except KeyError:
        c_file = SCons.Builder.Builder(action = {},
                                       emitter = {},
                                       suffix = {None:'$CFILESUFFIX'})
        env['BUILDERS']['CFile'] = c_file
        env['CFILESUFFIX'] = '.c'
    try:
        cxx_file = env['BUILDERS']['CXXFile']
    except KeyError:
        cxx_file = SCons.Builder.Builder(action = {},
                                         emitter = {},
                                         suffix = {None:'$CXXFILESUFFIX'})
        env['BUILDERS']['CXXFile'] = cxx_file
        env['CXXFILESUFFIX'] = '.cc'
    return (c_file, cxx_file)
def FindTool(tools, env):
    for tool in tools:
        t = Tool(tool)
        if t.exists(env):
            return tool
    return None
def FindAllTools(tools, env):
    def ToolExists(tool, env=env):
        return Tool(tool).exists(env)
    return filter (ToolExists, tools)
             
def tool_list(platform, env):
    if str(platform) == 'win32':
        "prefer Microsoft tools on Windows"
        linkers = ['mslink', 'gnulink', 'ilink', 'linkloc', 'ilink32' ]
        c_compilers = ['msvc', 'mingw', 'gcc', 'intelc', 'icl', 'icc', 'cc', 'bcc32' ]
        cxx_compilers = ['msvc', 'intelc', 'icc', 'g++', 'c++', 'bcc32' ]
        assemblers = ['masm', 'nasm', 'gas', '386asm' ]
        ars = ['mslib', 'ar', 'tlib']
    elif str(platform) == 'os2':
        "prefer IBM tools on OS/2"
        linkers = ['ilink', 'gnulink', 'mslink']
        c_compilers = ['icc', 'gcc', 'msvc', 'cc']
        cxx_compilers = ['icc', 'g++', 'msvc', 'c++']
        assemblers = ['nasm', 'masm', 'gas']
        ars = ['ar', 'mslib']
    elif str(platform) == 'irix':
        "prefer MIPSPro on IRIX"
        linkers = ['sgilink', 'gnulink']
        c_compilers = ['sgicc', 'gcc', 'cc']
        cxx_compilers = ['sgic++', 'g++', 'c++']
        assemblers = ['as', 'gas']
        ars = ['sgiar']
    elif str(platform) == 'sunos':
        "prefer Forte tools on SunOS"
        linkers = ['sunlink', 'gnulink']
        c_compilers = ['suncc', 'gcc', 'cc']
        cxx_compilers = ['sunc++', 'g++', 'c++']
        assemblers = ['as', 'gas']
        ars = ['sunar']
    elif str(platform) == 'hpux':
        "prefer aCC tools on HP-UX"
        linkers = ['hplink', 'gnulink']
        c_compilers = ['hpcc', 'gcc', 'cc']
        cxx_compilers = ['hpc++', 'g++', 'c++']
        assemblers = ['as', 'gas']
        ars = ['ar']
    elif str(platform) == 'aix':
        "prefer AIX Visual Age tools on AIX"
        linkers = ['aixlink', 'gnulink']
        c_compilers = ['aixcc', 'gcc', 'cc']
        cxx_compilers = ['aixc++', 'g++', 'c++']
        assemblers = ['as', 'gas']
        ars = ['ar']
    elif str(platform) == 'darwin':
        "prefer GNU tools on Mac OS X, except for some linkers and IBM tools"
        linkers = ['applelink', 'gnulink']
        c_compilers = ['gcc', 'cc']
        cxx_compilers = ['g++', 'c++']
        assemblers = ['as']
        ars = ['ar']
    else:
        "prefer GNU tools on all other platforms"
        linkers = ['gnulink', 'mslink', 'ilink']
        c_compilers = ['gcc', 'msvc', 'intelc', 'icc', 'cc']
        cxx_compilers = ['g++', 'msvc', 'intelc', 'icc', 'c++']
        assemblers = ['gas', 'nasm', 'masm']
        ars = ['ar', 'mslib']
    c_compiler = FindTool(c_compilers, env) or c_compilers[0]
 
    if c_compiler and c_compiler == 'mingw':
        cxx_compiler = None
        linker = None
        assembler = None
        fortran_compiler = None
        ar = None
    else:
        if c_compiler in ('msvc', 'intelc', 'icc'):
            cxx_compiler = None
        else:
            cxx_compiler = FindTool(cxx_compilers, env) or cxx_compilers[0]
        linker = FindTool(linkers, env) or linkers[0]
        assembler = FindTool(assemblers, env) or assemblers[0]
        ar = FindTool(ars, env) or ars[0]
    other_tools = FindAllTools(['lex', 'rpcgen', 'swig', 'tar', 'yacc', 'zip'], env)
    tools = ([linker, c_compiler, cxx_compiler,assembler, ar]+other_tools)
    
    return filter(lambda x: x, tools)
