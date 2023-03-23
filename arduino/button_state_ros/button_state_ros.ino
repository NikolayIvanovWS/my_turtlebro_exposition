#include <FastLED.h>
#include <ros.h>
#include <geometry_msgs/Point.h>


#define NUM_LEDS 24 
#define DATA_PIN 30

CRGB leds[NUM_LEDS];

class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};
ros::NodeHandle_<NewHardware>  nh;

geometry_msgs::Point point_msg;

ros::Publisher button_state("/button_state", &point_msg);

const int left_limit_switchPin = A14;

const int right_limit_switchPin = A12;

int left_limit_switchState = 0;
int right_limit_switchState = 0;

void setup() { 
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(60);

  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);  

  pinMode(22, INPUT);
  pinMode(23, INPUT);
  pinMode(24, INPUT);
  pinMode(25, INPUT);
  pinMode(left_limit_switchPin, INPUT);
  pinMode(right_limit_switchPin, INPUT);
  nh.initNode();
  nh.advertise(button_state); 
}


void loop() { 
  
  if (digitalRead(22) == HIGH) {
    digitalWrite(29,HIGH);
    lamps();
  } else { 
    digitalWrite(29,LOW);
    offlamps();  
  }

  if (digitalRead(24) == HIGH) digitalWrite(28,HIGH); else digitalWrite(28,LOW);
  if (digitalRead(23) == HIGH) digitalWrite(27,HIGH); else digitalWrite(27,LOW);
  if (digitalRead(25) == HIGH) digitalWrite(26,HIGH); else digitalWrite(26,LOW);  
    
  left_limit_switchState = digitalRead(left_limit_switchPin);
  right_limit_switchState = digitalRead(right_limit_switchPin);
  
  if (left_limit_switchState == HIGH) {
    point_msg.x = 1;
    point_msg.y = 0;
    button_state.publish(&point_msg);
    delay(100);
    point_msg.x = 0;
    point_msg.y = 0;
    button_state.publish(&point_msg);
  }
  else if (right_limit_switchState == HIGH) {
    point_msg.x = 0;
    point_msg.y = 1;   
    button_state.publish(&point_msg);
    delay(100);
    point_msg.x = 0;
    point_msg.y = 0;
    button_state.publish(&point_msg); 
  }
  else{
    point_msg.x = 0;
    point_msg.y = 0;
  }

  nh.spinOnce();
  delay(200);
 
}

void offlamps() { 
  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = CHSV(0, 0, 0); 
    FastLED.show();
    delay(20);
    } 
}


void lamps() {
    static uint8_t hue = 0; 
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show(); 
    fadeall();
    delay(20);
  }
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }
