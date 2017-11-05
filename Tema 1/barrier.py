"""
This module represents a barrier.
"""

from threading import Lock, Semaphore

class ReusableBarrier(object):
    """
    Class that implements a reusable barrier
    """
    def __init__(self, num_threads):
        """
        Constructor.

        @type num_threads: Integer
        @param num_threads: The number of threads
        """
        self.num_threads = num_threads
        self.count_threads1 = [self.num_threads]
        self.count_threads2 = [self.num_threads]
        self.count_lock = Lock()
        self.threads_sem1 = Semaphore(0)
        self.threads_sem2 = Semaphore(0)

    def wait(self):
        """
        Phases of the barrier
        """
        self.phase(self.count_threads1, self.threads_sem1)
        self.phase(self.count_threads2, self.threads_sem2)

    def phase(self, count_threads, threads_sem):
        """
        Moves to next phase

        @type count_threads: List
        @param count_threads: List of threads

        @type threads_sem: Semaphore
        @param threads_sem: Semaphore
        """
        with self.count_lock:
            count_threads[0] -= 1
            if count_threads[0] == 0:
                for i in range(self.num_threads):
                    threads_sem.release()
                count_threads[0] = self.num_threads
        threads_sem.acquire()
