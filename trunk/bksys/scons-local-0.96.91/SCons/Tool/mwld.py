__revision__ = "src/engine/SCons/Tool/mwld.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Tool
import SCons.Tool.mwcc
def generate(env):
    """Add Builders and construction variables for lib to an Environment."""
    SCons.Tool.createStaticLibBuilder(env)
    SCons.Tool.createSharedLibBuilder(env)
    SCons.Tool.createProgBuilder(env)
    env['AR'] = 'mwld'
    env['ARCOM'] = '$AR $ARFLAGS -library -o $TARGET $SOURCES'
    env['LIBDIRPREFIX'] = '-L'
    env['LIBDIRSUFFIX'] = ''
    env['LIBLINKPREFIX'] = '-l'
    env['LIBLINKSUFFIX'] = '.lib'
    env['LINK'] = 'mwld'
    env['LINKCOM'] = '$LINK $LINKFLAGS -o $TARGET $SOURCES $_LIBDIRFLAGS $_LIBFLAGS'
    env['SHLINK'] = '$LINK'
    env['SHLINKFLAGS'] = '$LINKFLAGS'
    env['SHLINKCOM']   = shlib_action
    env['SHLIBEMITTER']= shlib_emitter
def exists(env):
    return mwcc.set_versions(env)
def shlib_generator(target, source, env, for_signature):
    cmd = ['$SHLINK', '$SHLINKFLAGS', '-shared']
    no_import_lib = env.get('no_import_lib', 0)
    if no_import_lib: cmd.extend('-noimplib')
    dll = env.FindIxes(target, 'SHLIBPREFIX', 'SHLIBSUFFIX')
    if dll: cmd.extend(['-o', dll])
    implib = env.FindIxes(target, 'LIBPREFIX', 'LIBSUFFIX')
    if implib: cmd.extend(['-implib', implib.get_string(for_signature)])
    cmd.extend(['$SOURCES', '$_LIBDIRFLAGS', '$_LIBFLAGS'])
    return [cmd]
def shlib_emitter(target, source, env):
    dll = env.FindIxes(target, 'SHLIBPREFIX', 'SHLIBSUFFIX')
    no_import_lib = env.get('no_import_lib', 0)
    if not dll:
        raise SCons.Errors.UserError, "A shared library should have exactly one target with the suffix: %s" % env.subst("$SHLIBSUFFIX")
    if not no_import_lib and \
       not env.FindIxes(target, 'LIBPREFIX', 'LIBSUFFIX'):
        target.append(env.ReplaceIxes(dll,
                                      'SHLIBPREFIX', 'SHLIBSUFFIX',
                                      'LIBPREFIX', 'LIBSUFFIX'))
    return target, source
shlib_action = SCons.Action.Action(shlib_generator, generator=1)
