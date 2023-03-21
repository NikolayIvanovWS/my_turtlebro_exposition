#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from sensor_msgs.msg import BatteryState
from std_msgs.msg import Empty
import rospy
import subprocess

class Charge():
    def __init__(self):
        self.charge = 16.6
        self.state = 0
        self.text = " "
        rospy.Subscriber('/bat', BatteryState, self.voltage_value)
        rospy.Subscriber('/charge', Empty, self.charge_state)
        
    def say_text(self, text):
        print(f"Ready to Speech: {self.text}")
        subprocess.call('echo '+self.text+'|festival --tts --language russian', shell=True)
        print("Speech end")

    def voltage_value(self, msg):
        self.state = self.charge - msg.voltage
        self.state = 100 - (self.state/0.03)
        
    def charge_state(self, data):
        if data:
            if self.state <= 100 and self.state >= 80:
                self.text = 'Заряд почти полный'
            if self.state <= 80 and self.state >= 50:
                self.text = 'Заряд хороший'
            if self.state <= 50 and self.state >= 30:
                self.text = 'Заряд удовлетворительный'
            if self.state <= 30 and self.state >= 10:
                self.text = 'Заряд низкий'
            if self.state <= 10 and self.state >= 0:
                self.text = 'Заряд почти нулевой'
            self.say_text(self.text)
        if self.state <= 10 and self.state >= 0:
            self.text = 'Заряд почти нулевой'
            self.say_text(self.text)


rospy.init_node('battery_state')
Charge = Charge()
rospy.spin()
