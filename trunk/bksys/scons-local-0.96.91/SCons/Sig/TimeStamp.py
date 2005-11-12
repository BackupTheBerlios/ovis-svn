__revision__ = "src/engine/SCons/Sig/TimeStamp.py 0.96.91 2005/10/17 20:04:26 ita"
def current(new, old):
    """Return whether a new timestamp is up-to-date with
    respect to an old timestamp.
    """
    return not old is None and new <= old
def collect(signatures):
    """
    Collect a list of timestamps, returning
    the most-recent timestamp from the list 
    signatures - a list of timestamps
    returns - the most recent timestamp
    """
    if len(signatures) == 0:
        return 0
    elif len(signatures) == 1:
        return signatures[0]
    else:
        return max(signatures)
def signature(obj):
    """Generate a timestamp.
    """
    return obj.get_timestamp()
def to_string(signature):
    """Convert a timestamp to a string"""
    return str(signature)
def from_string(string):
    """Convert a string to a timestamp"""
    try:
        return int(string)
    except ValueError:
        return None
