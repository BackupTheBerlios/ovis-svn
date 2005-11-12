__revision__ = "src/engine/SCons/Sig/__init__.py 0.96.91 2005/10/17 20:04:26 ita"
try:
    import MD5
    default_module = MD5
except ImportError:
    import TimeStamp
    default_module = TimeStamp
class Calculator:
    """
    Encapsulates signature calculations and .sconsign file generating
    for the build engine.
    """
    def __init__(self, module=default_module):
        """
        Initialize the calculator.
        module - the signature module to use for signature calculations
        """
        self.module = module
default_calc = Calculator()
