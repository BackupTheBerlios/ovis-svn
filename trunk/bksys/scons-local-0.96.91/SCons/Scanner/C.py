__revision__ = "src/engine/SCons/Scanner/C.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Node.FS
import SCons.Scanner
def CScanner():
    """Return a prototype Scanner instance for scanning source files
    that use the C pre-processor"""
    cs = SCons.Scanner.ClassicCPP("CScanner",
                                  "$CPPSUFFIXES",
                                  "CPPPATH",
                                  '^[ \t]*#[ \t]*(?:include|import)[ \t]*(<|")([^>"]+)(>|")')
    return cs
