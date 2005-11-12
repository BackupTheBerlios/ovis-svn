__revision__ = "src/engine/SCons/Scanner/Dir.py 0.96.91 2005/10/17 20:04:26 ita"
import string
import SCons.Node.FS
import SCons.Scanner
def DirScanner(**kw):
    """Return a prototype Scanner instance for scanning
    directories for on-disk files"""
    def only_dirs(nodes):
        return filter(lambda n: isinstance(n.disambiguate(),
                                SCons.Node.FS.Dir),
                      nodes)
    kw['node_factory'] = SCons.Node.FS.Entry
    kw['recursive'] = only_dirs
    ds = apply(SCons.Scanner.Base, (scan, "DirScanner"), kw)
    return ds
skip_entry = {
   '.' : 1,
   '..' : 1,
   '.sconsign' : 1,
   '.sconsign.dblite' : 1,
}
def scan(node, env, path=()):
    """
    This scanner scans a directory for on-disk files and directories therein.
    """
    try:
        flist = node.fs.listdir(node.abspath)
    except (IOError, OSError):
        return []
    dont_scan = lambda k: not skip_entry.has_key(k)
    flist = filter(dont_scan, flist)
    flist.sort()
    return map(lambda f, node=node: node.Entry('./'+f), flist)
