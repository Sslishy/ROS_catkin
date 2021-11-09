#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>
int main(int argc, char **argv)
{
 
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  sleep(2);
   while (chatter_pub.getNumSubscribers() == 0) {
    // wait for a connection to publisher
     sleep(1);
    // you can do whatever you like here or simply do nothing
}
  ros::Rate loop_rate(10);
  int count = 0;
    std_msgs::String msg;
    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());
    chatter_pub.publish(msg);
    loop_rate.sleep();
    ++count;
  return 0;
}