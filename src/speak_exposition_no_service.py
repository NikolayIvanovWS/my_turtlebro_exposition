#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from geometry_msgs.msg import Point
import rospy
import subprocess

class Speaker():
    def __init__(self):
        rospy.init_node('exposition_speaker')
        rospy.loginfo("Ready to Speak!")
        self.right_button = False
        self.left_button = False
        rospy.Subscriber('/button_state', Point, self.button_value)    
    
    def button_value(self, msg):
        rospy.loginfo("Button")        
        if msg.x == 1.0:
            self.right_button = True
            
        elif msg.y == 1.0:
            self.left_button = True
        else:
            self.right_button = False
            self.left_button = False

    def exp_speaker(self):
        rospy.loginfo("Speak!")
        if self.right_button:
            rospy.loginfo("right_button")
            subprocess.call('echo '"Привет. Я робот TartleBro"'|festival --tts --language russian', shell=True)
        elif self.left_button:
            rospy.loginfo("left_button")
            subprocess.call('echo '"Я умею строить карту помещения, а также озвучивать картины"'|festival --tts --language russian', shell=True)

Speaker = Speaker()

while not rospy.is_shutdown(): 
    rospy.sleep(0.1) 
    Speaker.exp_speaker()