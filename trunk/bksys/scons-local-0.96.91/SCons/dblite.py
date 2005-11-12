import cPickle
import time
import shutil
import os
import os.path
import types
import __builtin__
_open = __builtin__.open # avoid name clash
keep_all_files = 00000
ignore_corrupt_dbfiles = 0
def corruption_warning(filename):
    print "Warning: Discarding corrupt database:", filename
if hasattr(types, 'UnicodeType'):
    def is_string(s):
        t = type(s)
        return t is types.StringType or t is types.UnicodeType
else:
    def is_string(s):
        return type(s) is types.StringType
try:
    unicode('a')
except NameError:
    def unicode(s): return s
dblite_suffix = '.dblite'
tmp_suffix = '.tmp'
class dblite:
  def __init__(self, file_base_name, flag, mode):
    assert flag in (None, "r", "w", "c", "n")
    if (flag is None): flag = "r"
    base, ext = os.path.splitext(file_base_name)
    if ext == dblite_suffix:
      self._file_name = file_base_name
      self._tmp_name = base + tmp_suffix
    else:
      self._file_name = file_base_name + dblite_suffix
      self._tmp_name = file_base_name + tmp_suffix
    self._flag = flag
    self._mode = mode
    self._dict = {}
    self._needs_sync = 00000
    if (self._flag == "n"):
      _open(self._file_name, "wb", self._mode)
    else:
      try:
        f = _open(self._file_name, "rb")
      except IOError, e:
        if (self._flag != "c"):
          raise e
        _open(self._file_name, "wb", self._mode)
      else:
        p = f.read()
        if (len(p) > 0):
          try:
            self._dict = cPickle.loads(p)
          except (cPickle.UnpicklingError, EOFError):
            if (ignore_corrupt_dbfiles == 0): raise
            if (ignore_corrupt_dbfiles == 1):
              corruption_warning(self._file_name)
  def __del__(self):
    if (self._needs_sync):
      self.sync()
  def sync(self):
    self._check_writable()
    f = _open(self._tmp_name, "wb", self._mode)
    cPickle.dump(self._dict, f, 1)
    f.close()
    try: os.chmod(self._file_name, 0777)
    except OSError: pass
    os.unlink(self._file_name)
    os.rename(self._tmp_name, self._file_name)
    self._needs_sync = 00000
    if (keep_all_files):
      shutil.copyfile(
        self._file_name,
        self._file_name + "_" + str(int(time.time())))
  def _check_writable(self):
    if (self._flag == "r"):
      raise IOError("Read-only database: %s" % self._file_name)
  def __getitem__(self, key):
    return self._dict[key]
  def __setitem__(self, key, value):
    self._check_writable()
    if (not is_string(key)):
      raise TypeError, "key `%s' must be a string but is %s" % (key, type(key))
    if (not is_string(value)):
      raise TypeError, "value `%s' must be a string but is %s" % (value, type(value))
    self._dict[key] = value
    self._needs_sync = 0001
  def keys(self):
    return self._dict.keys()
  def has_key(self, key):
    return key in self._dict
  def __contains__(self, key):
    return key in self._dict
  def iterkeys(self):
    return self._dict.iterkeys()
  __iter__ = iterkeys
  def __len__(self):
    return len(self._dict)
def open(file, flag=None, mode=0666):
  return dblite(file, flag, mode)
