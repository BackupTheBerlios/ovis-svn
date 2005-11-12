__revision__ = "src/engine/SCons/Tool/zip.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import SCons.Builder
import SCons.Node.FS
import SCons.Util
try:
    import zipfile
    internal_zip = 1
except ImportError:
    internal_zip = 0
if internal_zip:
    zipcompression = zipfile.ZIP_DEFLATED
    def zip(target, source, env):
        def visit(arg, dirname, names):
            for name in names:
                path = os.path.join(dirname, name)
                if os.path.isfile(path):
                    arg.write(path)
        compression = env.get('ZIPCOMPRESSION', 0)
        zf = zipfile.ZipFile(str(target[0]), 'w', compression)
        for s in source:
            if s.isdir():
                os.path.walk(str(s), visit, zf)
            else:
                zf.write(str(s))
        zf.close()
else:
    zipcompression = 0
    zip = "$ZIP $ZIPFLAGS ${TARGET.abspath} $SOURCES"
zipAction = SCons.Action.Action(zip, varlist=['ZIPCOMPRESSION'])
ZipBuilder = SCons.Builder.Builder(action = SCons.Action.Action('$ZIPCOM', '$ZIPCOMSTR'),
                                   source_factory = SCons.Node.FS.Entry,
                                   source_scanner = SCons.Defaults.DirScanner,
                                   suffix = '$ZIPSUFFIX',
                                   multi = 1)
def generate(env):
    """Add Builders and construction variables for zip to an Environment."""
    try:
        bld = env['BUILDERS']['Zip']
    except KeyError:
        bld = ZipBuilder
        env['BUILDERS']['Zip'] = bld
    env['ZIP']        = 'zip'
    env['ZIPFLAGS']   = SCons.Util.CLVar('')
    env['ZIPCOM']     = zipAction
    env['ZIPCOMPRESSION'] =  zipcompression
    env['ZIPSUFFIX']  = '.zip'
def exists(env):
    return internal_zip or env.Detect('zip')
