    #include <ros/ros.h>
    #include <tf/transform_broadcaster.h>
    #include <tf2_geometry_msgs/tf2_geometry_msgs.h>
    #include</home/slishy/Code/class/lib/computeangle.h>
    #include "std_msgs/String.h"
    int main(int argc, char** argv){
      ros::init(argc, argv, "robot_tf_publisher");
      std::string x_,y_,z_,rx_,ry_,rz_,msg_;
      x_ = argv[1]; 
      float x = std::atof(x_.c_str());
      y_ = argv[2]; 
      float y = std::atof(y_.c_str());
      z_ = argv[3]; 
      float z = std::atof(z_.c_str());
      rx_ = argv[4]; 
      float rx = std::atof(rx_.c_str());
      ry_ = argv[5]; 
      float ry = std::atof(ry_.c_str());
      rz_ = argv[6]; 
      float rz = std::atof(rz_.c_str());
      x = x * 0.001;
      y = y * 0.001;
      z = z  * 0.001;
      msg_ = argv[7];
      float msg = std::atof(msg_.c_str());
      computeangle ca;
      Eigen::Matrix4f tansformition,tansformition2;
      Eigen::Vector3f RPY{rx,ry,rz};
      Eigen::Vector3f RPY2{180,0,180};
      tansformition = ca.eulerAnglesToRotationMatrix(RPY);
      tansformition2 =ca.eulerAnglesToRotationMatrix(RPY2);
      tansformition = tansformition * tansformition2;
      RPY = ca.rotationMatrixToEulerAngles(tansformition);
      
      ros::NodeHandle n;
      ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
      sleep(1);
      ros::Rate r(10);
      ros::Rate sleep(1);
      tf::TransformBroadcaster broadcaster;
        tf::Transform transform;
        int sec = 0;
        tf2::Quaternion orientation;
        geometry_msgs::Pose target_pose1;
        RPY[0] = (M_PI/180) *RPY[0];
         RPY[1] = (M_PI/180) *RPY[1];
         RPY[2] = (M_PI/180) *RPY[2];
        orientation.setRPY(RPY[0],RPY[1],RPY[2]);
        cout <<RPY <<endl;
         target_pose1.orientation = tf2::toMsg(orientation);
         cout << target_pose1.orientation <<endl;
       while(n.ok()&& sec++ < 5){
        transform.setOrigin( tf::Vector3(x, y, z));
        transform.setRotation(tf::Quaternion(target_pose1.orientation.x,target_pose1.orientation.y,target_pose1.orientation.z ,target_pose1.orientation.w));
        broadcaster.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", "PickPoint"));
        r.sleep();
       }
       while (chatter_pub.getNumSubscribers() == 0) {
          sleep.sleep();
        }
       std_msgs::String sendmsg;
        std::stringstream ss;
        if(msg == 1)
        {
          ss << "AddPointCloud";
        }  
        if(msg == 2)
        {
          ss << "ComputePath";
        }
        if(msg == 3)
        {
          ss << "ComputePathlock";
        }
        sendmsg.data = ss.str();
        cout << ss.str() <<endl;
        chatter_pub.publish(sendmsg);
        r.sleep();
   }