__revision__ = "src/engine/SCons/Taskmaster.py 0.96.91 2005/10/17 20:04:26 ita"
import string
import sys
import traceback
import SCons.Node
import SCons.Errors
# A subsystem for recording stats about how different Nodes are handled by
# the main Taskmaster loop.  There's no external control here (no need for
# a --debug= option); enabled it by changing the value of CollectStats.
CollectStats = None
class Stats:
    def __init__(self):
        self.considered  = 0
        self.already_handled  = 0
        self.problem  = 0
        self.child_failed  = 0
        self.not_started  = 0
        self.not_built  = 0
        self.side_effects  = 0
        self.build  = 0
StatsNodes = []
fmt = "%(considered)3d "\
      "%(already_handled)3d " \
      "%(problem)3d " \
      "%(child_failed)3d " \
      "%(not_started)3d " \
      "%(not_built)3d " \
      "%(side_effects)3d " \
      "%(build)3d "
def dump_stats():
    StatsNodes.sort(lambda a, b: cmp(str(a), str(b)))
    for n in StatsNodes:
        print (fmt % n.stats.__dict__) + str(n)
class Task:
    """Default SCons build engine task.
    This controls the interaction of the actual building of node
    and the rest of the engine.
    This is expected to handle all of the normally-customizable
    aspects of controlling a build, so any given application
    *should* be able to do what it wants by sub-classing this
    class and overriding methods as appropriate.  If an application
    needs to customze something by sub-classing Taskmaster (or
    some other build engine class), we should first try to migrate
    that functionality into this class.
    Note that it's generally a good idea for sub-classes to call
    these methods explicitly to update state, etc., rather than
    roll their own interaction with Taskmaster from scratch."""
    def __init__(self, tm, targets, top, node):
        self.tm = tm
        self.targets = targets
        self.top = top
        self.node = node
        self.exc_clear()
    def display(self, message):
        """Allow the calling interface to display a message
        """
        pass
    def prepare(self):
        """Called just before the task is executed.
        This unlinks all targets and makes all directories before
        building anything."""
        self.exception_raise()
        if self.tm.message:
            self.display(self.tm.message)
            self.tm.message = None
        for t in self.targets:
            t.prepare()
            for s in t.side_effects:
                s.prepare()
    def execute(self):
        """Called to execute the task.
        This method is called from multiple threads in a parallel build,
        so only do thread safe stuff here.  Do thread unsafe stuff in
        prepare(), executed() or failed()."""
        try:
            everything_was_cached = 1
            for t in self.targets:
                if not t.retrieve_from_cache():
                    everything_was_cached = 0
                    break
            if not everything_was_cached:
                self.targets[0].build()
        except KeyboardInterrupt:
            raise
        except SystemExit:
            exc_value = sys.exc_info()[1]
            raise SCons.Errors.ExplicitExit(self.targets[0], exc_value.code)
        except SCons.Errors.UserError:
            raise
        except SCons.Errors.BuildError:
            raise
        except:
            exc_type, exc_value, exc_traceback = sys.exc_info()
            raise SCons.Errors.BuildError(self.targets[0],
                                          "Exception",
                                          exc_type,
                                          exc_value,
                                          exc_traceback)
    def get_target(self):
        """Fetch the target being built or updated by this task.
        """
        return self.node
    def executed(self):
        """Called when the task has been successfully executed.
        This may have been a do-nothing operation (to preserve
        build order), so check the node's state before updating
        things.  Most importantly, this calls back to the
        Taskmaster to put any node tasks waiting on this one
        back on the pending list."""
        for t in self.targets:
            if t.get_state() == SCons.Node.executing:
                for side_effect in t.side_effects:
                    side_effect.set_state(SCons.Node.no_state)
                t.set_state(SCons.Node.executed)
                t.built()
            else:
                t.visited()
        self.tm.executed(self.node)
    def failed(self):
        """Default action when a task fails:  stop the build."""
        self.fail_stop()
    def fail_stop(self):
        """Explicit stop-the-build failure."""
        for t in self.targets:
            t.set_state(SCons.Node.failed)
        self.tm.failed(self.node)
        next_top = self.tm.next_top_level_candidate()
        self.tm.stop()
        if next_top:
            self.targets = [next_top]
            self.top = 1
    def fail_continue(self):
        """Explicit continue-the-build failure.
        This sets failure status on the target nodes and all of
        their dependent parent nodes.
        """
        for t in self.targets:
            def set_state(node): node.set_state(SCons.Node.failed)
            t.call_for_all_waiting_parents(set_state)
        self.tm.executed(self.node)
    def make_ready_all(self):
        """Mark all targets in a task ready for execution.
        This is used when the interface needs every target Node to be
        visited--the canonical example being the "scons -c" option.
        """
        self.out_of_date = self.targets[:]
        for t in self.targets:
            for s in t.side_effects:
                s.set_state(SCons.Node.executing)
            t.set_state(SCons.Node.executing)
    def make_ready_current(self):
        """Mark all targets in a task ready for execution if any target
        is not current.
        This is the default behavior for building only what's necessary.
        """
        self.out_of_date = []
        for t in self.targets:
            if t.current():
                t.set_state(SCons.Node.up_to_date)
            else:
                self.out_of_date.append(t)
                for s in t.side_effects:
                    s.set_state(SCons.Node.executing)
                t.set_state(SCons.Node.executing)
    make_ready = make_ready_current
    def postprocess(self):
        """Post process a task after it's been executed."""
        for t in self.targets:
            t.postprocess()
    def exc_info(self):
        return self.exception
    def exc_clear(self):
        self.exception = (None, None, None)
        self.exception_raise = self._no_exception_to_raise
    def exception_set(self, exception=None):
        if not exception:
            exception = sys.exc_info()
        self.exception = exception
        self.exception_raise = self._exception_raise
    def _no_exception_to_raise(self):
        pass
    def _exception_raise(self):
        """Raise a pending exception that was recorded while
        getting a Task ready for execution."""
        self.tm.exception_raise(self.exc_info())
def order(dependencies):
    """Re-order a list of dependencies (if we need to)."""
    return dependencies
class Taskmaster:
    """A generic Taskmaster for handling a bunch of targets.
    Classes that override methods of this class should call
    the base class method, so this class can do its thing.
    """
    def __init__(self, targets=[], tasker=Task, order=order):
        self.targets = targets # top level targets
        self.candidates = targets[:] # nodes that might be ready to be executed
        self.candidates.reverse()
        self.executing = [] # nodes that are currently executing
        self.pending = [] # nodes that depend on a currently executing node
        self.tasker = tasker
        self.ready = None # the next task that is ready to be executed
        self.order = order
        self.message = None
        for node in self.targets:
            alt, message = node.alter_targets()
            if alt:
                self.message = message
                self.candidates.extend(self.order(alt))
                continue
    def _find_next_ready_node(self):
        """Find the next node that is ready to be built"""
        if self.ready:
            return
        self.ready_exc = None
        while self.candidates:
            node = self.candidates.pop()
            state = node.get_state()
            if CollectStats:
                if not hasattr(node, 'stats'):
                    node.stats = Stats()
                    StatsNodes.append(node)
                S = node.stats
                S.considered = S.considered + 1
            else:
                S = None
            if not state in [ SCons.Node.no_state, SCons.Node.stack ]:
                if S: S.already_handled = S.already_handled + 1
                continue
            node.set_state(SCons.Node.stack)
            try:
                childinfo = map(lambda N: (N.get_state(),
                                           N.is_derived() or N.is_pseudo_derived(),
                                           N), node.children())
            except SystemExit:
                exc_value = sys.exc_info()[1]
                e = SCons.Errors.ExplicitExit(node, exc_value.code)
                self.ready_exc = (SCons.Errors.ExplicitExit, e)
                self.ready = node
                break
            except KeyboardInterrupt:
                raise
            except:
                self.ready_exc = sys.exc_info()
                self.ready = node
                if S: S.problem = S.problem + 1
                break
            if filter(lambda I: I[0] == SCons.Node.failed, childinfo):
                node.set_state(SCons.Node.failed)
                if S: S.child_failed = S.child_failed + 1
                continue
            cycle = filter(lambda I: I[0] == SCons.Node.stack, childinfo)
            if cycle:
                self.candidates.append(node)
                nodes = filter(lambda N: N.get_state() == SCons.Node.stack,
                               self.candidates) + \
                               map(lambda I: I[2], cycle)
                nodes.reverse()
                desc = "Dependency cycle: " + string.join(map(str, nodes), " -> ")
                raise SCons.Errors.UserError, desc
            derived_children = filter(lambda I: I[1], childinfo)
            not_started = filter(lambda I: not I[0], derived_children)
            if not_started:
                not_started = map(lambda I: I[2], not_started)
                map(lambda n, P=node: n.add_to_waiting_parents(P), not_started)
                self.candidates.append(node)
                not_started.reverse()
                self.candidates.extend(self.order(not_started))
                if S: S.not_started = S.not_started + 1
                continue
            not_built = filter(lambda I: I[0] <= SCons.Node.executing, derived_children)
            if not_built:
                map(lambda I, P=node: I[2].add_to_waiting_parents(P), not_built)
                self.pending.append(node)
                node.set_state(SCons.Node.pending)
                if S: S.not_built = S.not_built + 1
                continue
            if reduce(lambda E,N:
                      E or N.get_state() == SCons.Node.executing,
                      node.side_effects,
                      0):
                self.pending.append(node)
                node.set_state(SCons.Node.pending)
                if S: S.side_effects = S.side_effects + 1
                continue
            self.ready = node
            if S: S.build = S.build + 1
            break
    def next_task(self):
        """Return the next task to be executed."""
        self._find_next_ready_node()
        node = self.ready
        if node is None:
            return None
        try:
            tlist = node.builder.targets(node)
        except AttributeError:
            tlist = [node]
        self.executing.extend(tlist)
        self.executing.extend(node.side_effects)
        task = self.tasker(self, tlist, node in self.targets, node)
        try:
            task.make_ready()
        except KeyboardInterrupt:
            raise
        except:
            self.ready_exc = sys.exc_info()
        if self.ready_exc:
            task.exception_set(self.ready_exc)
        self.ready = None
        self.ready_exc = None
        return task
    def is_blocked(self):
        self._find_next_ready_node()
        return not self.ready and (self.pending or self.executing)
    def next_top_level_candidate(self):
        candidates = self.candidates[:]
        candidates.reverse()
        for c in candidates:
            if c in self.targets:
                return c
        return None
    def stop(self):
        """Stop the current build completely."""
        self.candidates = []
        self.ready = None
        self.pending = []
    def failed(self, node):
        try:
            tlist = node.builder.targets(node)
        except AttributeError:
            tlist = [node]
        for t in tlist:
            self.executing.remove(t)
        for side_effect in node.side_effects:
            self.executing.remove(side_effect)
    def executed(self, node):
        try:
            tlist = node.builder.targets(node)
        except AttributeError:
            tlist = [node]
        for t in tlist:
            self.executing.remove(t)
        for side_effect in node.side_effects:
            self.executing.remove(side_effect)
        for node in self.pending:
            node.set_state(SCons.Node.no_state)
        self.pending.reverse()
        self.candidates.extend(self.pending)
        self.pending = []
    def exception_raise(self, exception):
        exc = exception[:]
        try:
            exc_type, exc_value, exc_traceback = exc
        except ValueError:
            exc_type, exc_value = exc
            exc_traceback = None
        raise exc_type, exc_value, exc_traceback
