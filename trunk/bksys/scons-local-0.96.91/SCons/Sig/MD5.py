__revision__ = "src/engine/SCons/Sig/MD5.py 0.96.91 2005/10/17 20:04:26 ita"
import imp
import string
# Force Python to load the builtin "md5" module.  If we do this with a
# normal import statement, then case-insensitive systems (Win32) get
# confused and thinks there's a case mismatch with *this* MD5.py module.
file, name, desc = imp.find_module('md5')
try:
    md5 = imp.load_module('md5', file, name, desc)
finally:
    if file:
        file.close()
def current(new, old):
    """Return whether a new signature is up-to-date with
    respect to an old signature.
    """
    return new == old
try:
    md5.new('').hexdigest
except AttributeError:
    class new_md5:
        def __init__(self, s):
            self.m = md5.new(str(s))
        def digest(self):
            return self.m.digest()
        def hexdigest(self):
            h = string.hexdigits
            r = ''
            for c in self.m.digest():
                i = ord(c)
                r = r + h[(i >> 4) & 0xF] + h[i & 0xF]
            return r
        def update(self, s):
            return self.m.update(s)
else:
    new_md5 = lambda s: md5.new(str(s))
def collect(signatures):
    """
    Collect a list of signatures into an aggregate signature.
    signatures - a list of signatures
    returns - the aggregate signature
    """
    if len(signatures) == 1:
        return signatures[0]
    else:
        return new_md5(string.join(signatures, ', ')).hexdigest()
def signature(obj):
    """Generate a signature for an object
    """
    try:
        gc = obj.get_contents
    except AttributeError:
        raise AttributeError, "unable to fetch contents of '%s'" % str(obj)
    return new_md5(gc()).hexdigest()
def to_string(signature):
    """Convert a signature to a string"""
    return signature
def from_string(string):
    """Convert a string to a signature"""
    return string
