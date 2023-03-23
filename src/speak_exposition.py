#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from turtlebro_speech.srv import Speech, SpeechResponse, SpeechRequest

import rospy
import toml
from pathlib import Path
from geometry_msgs.msg import Point

speech_service = rospy.ServiceProxy('festival_speech', Speech)
rospy.loginfo(f"Waiting for festival_speech service")
speech_service.wait_for_service()
rospy.loginfo(f"Have festival_speech service")

button_data_file = rospy.get_param('~button_data_file', str('/home/pi/catkin_ws/src/my_turtlebro_exposition/data/button_text.toml'))
button_data = toml.load(button_data_file)

rospy.loginfo(f"Loading button file {button_data_file}")

class Speaker():
    def __init__(self):
        rospy.init_node('exposition_speaker')
        rospy.loginfo("Ready to Speak!")
        self.right_button = False
        self.left_button = False
        rospy.Subscriber('/button_state', Point, self.button_value)    
    
    def button_value(self, msg):
        rospy.loginfo("Button")
        text = " "        
        if msg.x == 1.0:
            self.right_button = True
            type_button = "right"
            text = ' ' + button_data[str(type_button)]['text']
            
        elif msg.y == 1.0:
            self.left_button = True
            type_button = "left"
            text += ' ' + button_data[str(type_button)]['text']
        else:
            self.right_button = False
            self.left_button = False

        result: SpeechResponse = speech_service.call(SpeechRequest(text))
    
"""
    def exp_speaker(self):
        rospy.loginfo("Speak!")

        text = ""

        if self.right_button:
            rospy.loginfo("right_button")
            type_button = "right"
            text = button_data[str(type_button)]['text']
        elif self.left_button:
            rospy.loginfo("left_button")
            type_button = "left"
            text = button_data[str(type_button)]['text']

        result: SpeechResponse = speech_service.call(SpeechRequest(text))
"""

Speaker = Speaker()
rospy.spin()