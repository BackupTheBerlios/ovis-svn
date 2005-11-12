__revision__ = "src/engine/SCons/Tool/gas.py 0.96.91 2005/10/17 20:04:26 ita"
import as
assemblers = ['as', 'gas']
def generate(env):
    """Add Builders and construction variables for as to an Environment."""
    as.generate(env)
    env['AS']        = env.Detect(assemblers) or 'as'
def exists(env):
    return env.Detect(assemblers)
