__revision__ = "src/engine/SCons/Options/PathOption.py 0.96.91 2005/10/17 20:04:26 ita"
import os
import os.path
import SCons.Errors
class _PathOptionClass:
    def PathAccept(self, key, val, env):
        """Accepts any path, no checking done."""
        pass
    
    def PathIsDir(self, key, val, env):
        """Validator to check if Path is a directory."""
        if not os.path.isdir(val):
            if os.path.isfile(val):
                m = 'Directory path for option %s is a file: %s'
            else:
                m = 'Directory path for option %s does not exist: %s'
            raise SCons.Errors.UserError(m % (key, val))
    def PathIsDirCreate(self, key, val, env):
        """Validator to check if Path is a directory,
           creating it if it does not exist."""
        if os.path.isfile(val):
            m = 'Path for option %s is a file, not a directory: %s'
            raise SCons.Errors.UserError(m % (key, val))
        if not os.path.isdir(val):
            os.makedirs(val)
    def PathIsFile(self, key, val, env):
        """validator to check if Path is a file"""
        if not os.path.isfile(val):
            if os.path.isdir(val):
                m = 'File path for option %s is a directory: %s'
            else:
                m = 'File path for option %s does not exist: %s'
            raise SCons.Errors.UserError(m % (key, val))
    def PathExists(self, key, val, env):
        """validator to check if Path exists"""
        if not os.path.exists(val):
            m = 'Path for option %s does not exist: %s'
            raise SCons.Errors.UserError(m % (key, val))
    def __call__(self, key, help, default, validator=None):
        """
        The input parameters describe a 'path list' option, thus they
        are returned with the correct converter and validator appended. The
        result is usable for input to opts.Add() .
        The 'default' option specifies the default path to use if the
        user does not specify an override with this option.
        validator is a validator, see this file for examples
        """
        if validator is None:
            validator = self.PathExists
        return (key, '%s ( /path/to/%s )' % (help, key), default,
                validator, None)
PathOption = _PathOptionClass()
