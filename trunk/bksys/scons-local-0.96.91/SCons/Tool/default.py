__revision__ = "src/engine/SCons/Tool/default.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Tool
def generate(env):
    """Add default tools."""
    for t in SCons.Tool.tool_list(env['PLATFORM'], env):
        SCons.Tool.Tool(t)(env)
def exists(env):
    return 1
