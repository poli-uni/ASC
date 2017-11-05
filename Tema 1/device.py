"""
This module represents a device.

Computer Systems Architecture Course
Assignment 1
March 2016
"""

from math import ceil
from threading import Event, Thread, Lock
from multiprocessing.pool import ThreadPool
from barrier import ReusableBarrier


class Device(object):
    """
    Class that represents a device.
    """

    def __init__(self, device_id, sensor_data, supervisor):
        """
        Constructor.

        @type device_id: Integer
        @param device_id: the unique id of this node; between 0 and N-1

        @type sensor_data: List of (Integer, Float)
        @param sensor_data: a list containing (location, data) as measured by this device

        @type supervisor: Supervisor
        @param supervisor: the testing infrastructure's control and validation component
        """
        self.device_id = device_id
        self.sensor_data = sensor_data
        self.supervisor = supervisor
        self.script_received = Event()
        self.scripts = []
        self.timepoint_done = Event()
        self.barrier = None
        self.lock = None
        self.thread = DeviceThread(self)
        self.thread.start()

    def __str__(self):
        """
        Pretty prints this device.

        @rtype: String
        @return: a string containing the id of this device
        """
        return "Device %d" % self.device_id

    def setup_devices(self, devices):
        """
        Setup the devices before simulation begins.

        @type devices: List of Device
        @param devices: list containing all devices
        """
        # we don't need no stinkin' setup

        if self.device_id == 0:
            barrier = ReusableBarrier(len(devices))
            lock = {}

            for device in devices:
                device.barrier = barrier
                device.lock = lock


    def assign_script(self, script, location):
        """
        Provide a script for the device to execute.

        @type script: Script
        @param script: the script to execute from now on at each timepoint; None if the
            current timepoint has ended

        @type location: Integer
        @param location: the location for which the script is interested in
        """
        if script is not None:
            self.scripts.append((script, location))
        else:
            self.script_received.set()

    def get_data(self, location):
        """
        Returns the pollution value this device has for the given location.

        @type location: Integer
        @param location: a location for which obtain the data

        @rtype: Float
        @return: the pollution value
        """
        return self.sensor_data[location] if location in self.sensor_data else None

    def set_data(self, location, data):
        """
        Sets the pollution value stored by this device for the given location.

        @type location: Integer
        @param location: a location for which to set the data

        @type data: Float
        @param data: the pollution value
        """
        if location in self.sensor_data:
            self.sensor_data[location] = data

    def shutdown(self):
        """
        Instructs the device to shutdown (terminate all threads). This method
        is invoked by the tester. This method must block until all the threads
        started by this device terminate.
        """
        self.thread.join()


class DeviceThread(Thread):
    """
    Class that implements the device's worker thread.
    """

    def __init__(self, device):
        """
        Constructor.

        @type device: Device
        @param device: the device which owns this thread
        """
        Thread.__init__(self, name="Device Thread %d" % device.device_id)
        self.device = device

    def run(self):
        # hope there is only one timepoint, as multiple iterations of the loop are not supported
        while True:
            # get the current neighbourhood
            limit = 8 # multiprocessing.cpu_count() for any pc
            num = float(limit) # float format

            neighbours = self.device.supervisor.get_neighbours()
            if neighbours is None:
                break

            self.device.script_received.wait()

            # add current device to neighbours
            if self.device not in neighbours:
                neighbours.append(self.device)

            scripts = self.device.scripts
            iterations = int(ceil(len(scripts) / num))

            pos = 0

            # divide the scripts between threads
            for i in range(iterations):
                threads = []

                if (i == iterations - 1) and (len(scripts) % limit > 0):
                    limit = len(scripts) % limit

                for j in range(limit):
                    threads.append(MyThread(self.device, scripts[pos][0], scripts[pos][1], neighbours))
                    threads[-1].start()
                    pos += 1

                for thread in threads:
                    thread.join()

            self.device.script_received.clear()

            self.device.barrier.wait()


class MyThread(Thread):
    """
    Class that simulates a thread
    """
    def __init__(self, device, script, location, neighbours):
        """
        Constructor.

        @type device: Device
        @param device: the device which owns this thread

        @type script: Scripts
        @param script: the script that will run

        @type location: List
        @param location: list of pairs: (id, sensor_data)

        @type neighbours: List
        @param neighbours: list of devices
        """
        Thread.__init__(self)
        self.device = device
        self.script = script
        self.location = location
        self.neighbours = neighbours

    def run(self):
        script_data = []

        # create locks
        for device in self.neighbours:
            if self.location not in device.lock:
                device.lock[self.location] = Lock()

        # set lock
        device.lock[self.location].acquire()

        # collect data from all neighbours in this location
        for device in self.neighbours:
            data = device.get_data(self.location)
            if data is not None:
                script_data.append(data)


        if script_data != []:
            # run script on data
            result = self.script.run(script_data)

            # update data of all neighbours in this location
            for device in self.neighbours:
                device.set_data(self.location, result)

        # release lock
        device.lock[self.location].release()
