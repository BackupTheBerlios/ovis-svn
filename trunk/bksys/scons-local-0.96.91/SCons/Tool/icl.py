__revision__ = "src/engine/SCons/Tool/icl.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Tool.intelc
# This has been completely superceded by intelc.py, which can
# handle both Windows and Linux versions.
def generate(*args, **kw):
    """Add Builders and construction variables for icl to an Environment."""
    return apply(SCons.Tool.intelc.generate, args, kw)
def exists(*args, **kw):
    return apply(SCons.Tool.intelc.exists, args, kw)
