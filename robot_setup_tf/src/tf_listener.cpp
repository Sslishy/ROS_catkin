#include "ros/ros.h"
#include "std_msgs/String.h"
 std::string load;
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
   load =  msg->data.c_str();
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Subscriber sub = n.subscribe("msg", 1000, chatterCallback);
  int sec = 0;
  ros::spin();
  return 0;
}