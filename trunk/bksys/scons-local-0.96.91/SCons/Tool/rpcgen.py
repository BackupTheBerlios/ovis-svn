__revision__ = "src/engine/SCons/Tool/rpcgen.py 0.96.91 2005/10/17 20:04:26 ita"
from SCons.Builder import Builder
import SCons.Util
cmd = "cd ${SOURCE.dir} && $RPCGEN -%s $RPCGENFLAGS %s -o ${TARGET.abspath} ${SOURCE.file}"
rpcgen_client   = cmd % ('l', '$RPCGENCLIENTFLAGS')
rpcgen_header   = cmd % ('h', '$RPCGENHEADERFLAGS')
rpcgen_service  = cmd % ('m', '$RPCGENSERVICEFLAGS')
rpcgen_xdr      = cmd % ('c', '$RPCGENXDRFLAGS')
def generate(env):
    "Add RPCGEN Builders and construction variables for an Environment."
    
    client  = Builder(action=rpcgen_client,  suffix='_clnt.c', src_suffix='.x')
    header  = Builder(action=rpcgen_header,  suffix='.h',      src_suffix='.x')
    service = Builder(action=rpcgen_service, suffix='_svc.c',  src_suffix='.x')
    xdr     = Builder(action=rpcgen_xdr,     suffix='_xdr.c',  src_suffix='.x')
    env.Append(BUILDERS={'RPCGenClient'  : client,
                         'RPCGenHeader'  : header,
                         'RPCGenService' : service,
                         'RPCGenXDR'     : xdr})
    env['RPCGEN'] = 'rpcgen'
    env['RPCGENFLAGS'] = SCons.Util.CLVar('')
    env['RPCGENCLIENTFLAGS'] = SCons.Util.CLVar('')
    env['RPCGENHEADERFLAGS'] = SCons.Util.CLVar('')
    env['RPCGENSERVICEFLAGS'] = SCons.Util.CLVar('')
    env['RPCGENXDRFLAGS'] = SCons.Util.CLVar('')
def exists(env):
    return env.Detect('rpcgen')
