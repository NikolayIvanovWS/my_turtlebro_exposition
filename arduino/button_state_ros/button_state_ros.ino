#include <ros.h>
#include <geometry_msgs/Point.h>

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

void setup()
{
  pinMode(left_limit_switchPin, INPUT);
  pinMode(right_limit_switchPin, INPUT);
  nh.initNode();
  nh.advertise(button_state);
}

void loop()
{
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
  delay(500);
}