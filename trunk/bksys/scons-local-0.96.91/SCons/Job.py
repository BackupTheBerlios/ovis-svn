__revision__ = "src/engine/SCons/Job.py 0.96.91 2005/10/17 20:04:26 ita"
class Jobs:
    """An instance of this class initializes N jobs, and provides
    methods for starting, stopping, and waiting on all N jobs.
    """
    def __init__(self, num, taskmaster):
        """
        create 'num' jobs using the given taskmaster.
        If 'num' is 1 or less, then a serial job will be used,
        otherwise a parallel job with 'num' worker threads will
        be used.
        The 'num_jobs' attribute will be set to the actual number of jobs
        allocated.  If more than one job is requested but the Parallel
        class can't do it, it gets reset to 1.  Wrapping interfaces that
        care should check the value of 'num_jobs' after initialization.
        """
        self.job = None
        if num > 1:
            try:
                self.job = Parallel(taskmaster, num)
                self.num_jobs = num
            except NameError:
                pass
        if self.job is None:
            self.job = Serial(taskmaster)
            self.num_jobs = 1
    def run(self):
        """run the job"""
        try:
            self.job.start()
        except KeyboardInterrupt:
            import signal
            signal.signal(signal.SIGINT, signal.SIG_IGN)
            raise
class Serial:
    """This class is used to execute tasks in series, and is more efficient
    than Parallel, but is only appropriate for non-parallel builds. Only
    one instance of this class should be in existence at a time.
    This class is not thread safe.
    """
    def __init__(self, taskmaster):
        """Create a new serial job given a taskmaster. 
        The taskmaster's next_task() method should return the next task
        that needs to be executed, or None if there are no more tasks. The
        taskmaster's executed() method will be called for each task when it
        is successfully executed or failed() will be called if it failed to
        execute (e.g. execute() raised an exception). The taskmaster's
        is_blocked() method will not be called.  """
        
        self.taskmaster = taskmaster
    def start(self):
        """Start the job. This will begin pulling tasks from the taskmaster
        and executing them, and return when there are no more tasks. If a task
        fails to execute (i.e. execute() raises an exception), then the job will
        stop."""
        
        while 1:
            task = self.taskmaster.next_task()
            if task is None:
                break
            try:
                task.prepare()
                task.execute()
            except KeyboardInterrupt:
                raise
            except:
                task.exception_set()
                task.failed()
            else:
                task.executed()
            task.postprocess()
# Trap import failure so that everything in the Job module but the
# Parallel class (and its dependent classes) will work if the interpreter
# doesn't support threads.
try:
    import Queue
    import threading
except ImportError:
    pass
else:
    class Worker(threading.Thread):
        """A worker thread waits on a task to be posted to its request queue,
        dequeues the task, executes it, and posts a tuple including the task
        and a boolean indicating whether the task executed successfully. """
        def __init__(self, requestQueue, resultsQueue):
            threading.Thread.__init__(self)
            self.setDaemon(1)
            self.requestQueue = requestQueue
            self.resultsQueue = resultsQueue
            self.start()
        def run(self):
            while 1:
                task = self.requestQueue.get()
                try:
                    task.execute()
                except KeyboardInterrupt:
                    ok = False
                except:
                    task.exception_set()
                    ok = 0
                else:
                    ok = 1
                self.resultsQueue.put((task, ok))
    class ThreadPool:
        """This class is responsible for spawning and managing worker threads."""
        def __init__(self, num):
            """Create the request and reply queues, and 'num' worker threads."""
            self.requestQueue = Queue.Queue(0)
            self.resultsQueue = Queue.Queue(0)
            for i in range(num):
                Worker(self.requestQueue, self.resultsQueue)
        def put(self, obj):
            """Put task into request queue."""
            self.requestQueue.put(obj)
        def get(self, block = 1):
            """Remove and return a result tuple from the results queue."""
            return self.resultsQueue.get(block)
        def preparation_failed(self, obj):
            self.resultsQueue.put((obj, 0))
    class Parallel:
        """This class is used to execute tasks in parallel, and is somewhat 
        less efficient than Serial, but is appropriate for parallel builds.
        This class is thread safe.
        """
        def __init__(self, taskmaster, num):
            """Create a new parallel job given a taskmaster.
            The taskmaster's next_task() method should return the next task
            that needs to be executed, or None if there are no more tasks. The
            taskmaster's executed() method will be called for each task when it
            is successfully executed or failed() will be called if the task
            failed to execute (i.e. execute() raised an exception).  The
            taskmaster's is_blocked() method should return true iff there are
            more tasks, but they can't be executed until one or more other
            tasks have been executed. next_task() will be called iff
            is_blocked() returned false.
            Note: calls to taskmaster are serialized, but calls to execute() on
            distinct tasks are not serialized, because that is the whole point
            of parallel jobs: they can execute multiple tasks
            simultaneously. """
            self.taskmaster = taskmaster
            self.tp = ThreadPool(num)
            self.maxjobs = num
        def start(self):
            """Start the job. This will begin pulling tasks from the
            taskmaster and executing them, and return when there are no
            more tasks. If a task fails to execute (i.e. execute() raises
            an exception), then the job will stop."""
            jobs = 0
            
            while 1:
                while jobs < self.maxjobs:
                    task = self.taskmaster.next_task()
                    if task is None:
                        break
                    try:
                        task.prepare()
                    except KeyboardInterrupt:
                        raise
                    except:
                        task.exception_set()
                        self.tp.preparation_failed(task)
                        jobs = jobs + 1
                        continue
                    self.tp.put(task)
                    jobs = jobs + 1
                if not task and not jobs: break
                while 1:
                    task, ok = self.tp.get()
                    jobs = jobs - 1
                    if ok:
                        task.executed()
                    else:
                        task.failed()
                    task.postprocess()
                    if self.tp.resultsQueue.empty():
                        break
