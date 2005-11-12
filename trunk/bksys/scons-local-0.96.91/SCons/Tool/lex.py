__revision__ = "src/engine/SCons/Tool/lex.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Action
import SCons.Tool
import SCons.Util
LexAction = SCons.Action.Action("$LEXCOM", "$LEXCOMSTR")
def generate(env):
    """Add Builders and construction variables for lex to an Environment."""
    c_file, cxx_file = SCons.Tool.createCFileBuilders(env)
    c_file.add_action('.l', LexAction)
    c_file.add_action('.lex', LexAction)
    cxx_file.add_action('.ll', LexAction)
    env['LEX']      = env.Detect('flex') or 'lex'
    env['LEXFLAGS'] = SCons.Util.CLVar('')
    env['LEXCOM']   = '$LEX $LEXFLAGS -t $SOURCES > $TARGET'
    
def exists(env):
    return env.Detect(['flex', 'lex'])
