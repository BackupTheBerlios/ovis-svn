__revision__ = "src/engine/SCons/Tool/yacc.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import SCons.Defaults
import SCons.Tool
import SCons.Util
YaccAction = SCons.Action.Action("$YACCCOM", "$YACCCOMSTR")
def _yaccEmitter(target, source, env, ysuf, hsuf):
    if len(source) and '-d' in SCons.Util.CLVar(env.subst("$YACCFLAGS")):
        base, ext = os.path.splitext(SCons.Util.to_String(source[0]))
        if ext in ysuf:
            base, ext = os.path.splitext(SCons.Util.to_String(target[0]))
            target.append(base + env.subst(hsuf))
    return (target, source)
def yEmitter(target, source, env):
    return _yaccEmitter(target, source, env, ['.y', '.yacc'], '$YACCHFILESUFFIX')
def yyEmitter(target, source, env):
    return _yaccEmitter(target, source, env, ['.yy'], '$YACCHXXFILESUFFIX')
def generate(env):
    """Add Builders and construction variables for yacc to an Environment."""
    c_file, cxx_file = SCons.Tool.createCFileBuilders(env)
    
    c_file.add_action('.y', YaccAction)
    c_file.add_action('.yacc', YaccAction)
    cxx_file.add_action('.yy', YaccAction)
    c_file.add_emitter('.y', yEmitter)
    c_file.add_emitter('.yacc', yEmitter)
    cxx_file.add_emitter('.yy', yyEmitter)
    env['YACC']      = env.Detect('bison') or 'yacc'
    env['YACCFLAGS'] = SCons.Util.CLVar('')
    env['YACCCOM']   = '$YACC $YACCFLAGS -o $TARGET $SOURCES'
    env['YACCHFILESUFFIX'] = '.h'
    env['YACCHXXFILESUFFIX'] = '.hpp'
def exists(env):
    return env.Detect(['bison', 'yacc'])
