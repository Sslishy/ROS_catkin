#include <ros/ros.h>
// MoveIt
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h> 
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl_conversions/pcl_conversions.h> 
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <pcl/filters/statistical_outlier_removal.h>
// TF2
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf/transform_listener.h>
#include</home/slishy/Code/class/lib/computeangle.h>
#include "std_msgs/String.h"
using namespace std;
const double tau = 2 * M_PI;
std::string loadmsg;
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
   loadmsg = msg->data.c_str();
   ROS_INFO("I heard: [%s]", msg->data.c_str());
}
bool listenTF(const tf::TransformListener& listener,float &px,float& py ,float& pz,float &x,float& y ,float& z,geometry_msgs::Pose &targetRPY)
{
  geometry_msgs::PointStamped pickpoint;
  pickpoint.header.frame_id = "PickPoint";
  //we'll just use the most recent transform available for our simple example
  pickpoint.header.stamp = ros::Time();
  //just an arbitrary point in space
  pickpoint.point.x = 0.00845;
  pickpoint.point.y = -0.13597;
  pickpoint.point.z = -0.281;
  geometry_msgs::PointStamped prepickpoint;
  prepickpoint.header.frame_id = "PickPoint";
  //we'll just use the most recent transform available for our simple example
  prepickpoint.header.stamp = ros::Time();
  //just an arbitrary point in space
  prepickpoint.point.x = 0.00845;
  prepickpoint.point.y = -0.13597;
  prepickpoint.point.z = -0.351;
  geometry_msgs::PoseStamped rpy;
  // change rpy 
  rpy.header.stamp = ros::Time();
  rpy.header.frame_id = "PickPoint";
  rpy.pose.position.x = 0;
  rpy.pose.position.y = 0;
  rpy.pose.position.z = 0;
  rpy.pose.orientation.w = -0.953714;
  rpy.pose.orientation.x = 0.300714;
  rpy.pose.orientation.y = 0;
  rpy.pose.orientation.z = 0;
  try{
    geometry_msgs::PointStamped Link6_point;
    geometry_msgs::PointStamped preLink6_point;
    geometry_msgs::PoseStamped rpy1;
    listener.transformPoint("base_link", pickpoint, Link6_point);
    listener.transformPoint("base_link", prepickpoint, preLink6_point);
   listener.transformPose("base_link", rpy, rpy1);
   /* ROS_INFO("preLink6_point: (%.4f, %.4f. %.4f) -----> Link6_point: (%.4f, %.4f, %.4f)",
        preLink6_point.point.x, preLink6_point.point.y, preLink6_point.point.z,
        Link6_point.point.x, Link6_point.point.y, Link6_point.point.z);*/
         px =  preLink6_point.point.x; py = preLink6_point.point.y;pz = preLink6_point.point.z;
        x =  Link6_point.point.x; y = Link6_point.point.y;z = Link6_point.point.z;
        targetRPY.orientation.w = rpy1.pose.orientation.w;
        targetRPY.orientation.x = rpy1.pose.orientation.x;
        targetRPY.orientation.y = rpy1.pose.orientation.y;
        targetRPY.orientation.z = rpy1.pose.orientation.z;
        return true;
  }
  catch(tf::TransformException& ex){
    return false;
  }
  
}
bool listenTFlock(const tf::TransformListener& listener,float &px,float& py ,float& pz,float &x,float& y ,float& z,geometry_msgs::Pose &targetRPY)
{
  geometry_msgs::PointStamped pickpoint;
  pickpoint.header.frame_id = "PickPoint";
  //we'll just use the most recent transform available for our simple example
  pickpoint.header.stamp = ros::Time();
  //just an arbitrary point in space
  pickpoint.point.x = 0;
  pickpoint.point.y = 0;
  pickpoint.point.z = -0.221;
  geometry_msgs::PointStamped prepickpoint;
  prepickpoint.header.frame_id = "PickPoint";
  //we'll just use the most recent transform available for our simple example
  prepickpoint.header.stamp = ros::Time();
  //just an arbitrary point in space
  prepickpoint.point.x = 0;
  prepickpoint.point.y = 0;
  prepickpoint.point.z = -0.351;
  geometry_msgs::PoseStamped rpy;
  // change rpy 
  rpy.header.stamp = ros::Time();
  rpy.header.frame_id = "PickPoint";
  rpy.pose.position.x = 0;
  rpy.pose.position.y = 0;
  rpy.pose.position.z = 0;
   rpy.pose.orientation.w = 0.029898;
  rpy.pose.orientation.x = -0.0948237;
  rpy.pose.orientation.y = 0.948991;
  rpy.pose.orientation.z = -0.299218;
  try{
    geometry_msgs::PointStamped Link6_point;
    geometry_msgs::PointStamped preLink6_point;
    geometry_msgs::PoseStamped rpy1;
    listener.transformPoint("base_link", pickpoint, Link6_point);
    listener.transformPoint("base_link", prepickpoint, preLink6_point);
   listener.transformPose("base_link", rpy, rpy1);
    /*ROS_INFO("preLink6_point: (%.4f, %.4f. %.4f) -----> Link6_point: (%.4f, %.4f, %.4f)",
        preLink6_point.point.x, preLink6_point.point.y, preLink6_point.point.z,
        Link6_point.point.x, Link6_point.point.y, Link6_point.point.z);*/
         px =  preLink6_point.point.x; py = preLink6_point.point.y;pz = preLink6_point.point.z;
        x =  Link6_point.point.x; y = Link6_point.point.y;z = Link6_point.point.z;
        targetRPY.orientation.w = rpy1.pose.orientation.w;
        targetRPY.orientation.x = rpy1.pose.orientation.x;
        targetRPY.orientation.y = rpy1.pose.orientation.y;
        targetRPY.orientation.z = rpy1.pose.orientation.z;
        return true;
  }
  catch(tf::TransformException& ex){
    return false;
  }
  
}
void addPointCloud(moveit::planning_interface::PlanningSceneInterface &PlanningScenceInterface)
{
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::io::loadPCDFile("/home/slishy/Code/PCD/anchuan/test.pcd",*cloud);
  /*for (size_t i = 0; i < cloud->points.size(); i++)
  {
    cloud->points[i].x =  cloud->points[i].x *0.001;
    cloud->points[i].y =  cloud->points[i].y *0.001;
    cloud->points[i].z =  cloud->points[i].z *0.001;
  }
  pcl::io::savePCDFile("/home/slishy/Code/PCD/anchuan/test.pcd",*cloud);*/
  pcl::PassThrough<pcl::PointXYZ> pass;
  pass.setInputCloud(cloud);         
  pass.setFilterFieldName("z");      
  pass.setFilterLimits(0.27,0.5);    
  pass.filter(*cloud); 
  pcl::VoxelGrid<pcl::PointXYZ> sor;
  sor.setInputCloud(cloud);
  sor.setLeafSize(0.015,0.015,0.015);
  sor.filter(*cloud);
  pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor1;
  sor1.setInputCloud(cloud);
  sor1.setMeanK(50);
  sor1.setStddevMulThresh(1);
  sor1.filter(*cloud);
  std::vector<moveit_msgs::CollisionObject> collisionobjects;
  collisionobjects.resize(cloud->points.size());
  for (size_t i = 0; i < cloud->points.size(); i++)
  {
    collisionobjects[i].id = "box"+std::to_string(i);
    collisionobjects[i].header.frame_id = "base_link";
    collisionobjects[i].primitives.resize(1);
    collisionobjects[i].primitives[0].type = collisionobjects[i].primitives[0].BOX;
    collisionobjects[i].primitives[0].dimensions.resize(3);
    collisionobjects[i].primitives[0].dimensions[0] = 0.001;
    collisionobjects[i].primitives[0].dimensions[1] = 0.001;
    collisionobjects[i].primitives[0].dimensions[2] = 0.001;
    collisionobjects[i].primitive_poses.resize(1);
    collisionobjects[i].primitive_poses[0].position.x = cloud->points[i].x ;
    collisionobjects[i].primitive_poses[0].position.y = cloud->points[i].y ;
    collisionobjects[i].primitive_poses[0].position.z = cloud->points[i].z ;
    collisionobjects[i].primitive_poses[0].orientation.w = 1;
    collisionobjects[i].operation = collisionobjects[i].ADD;
  }
  PlanningScenceInterface.applyCollisionObjects(collisionobjects);
}
void openGripper( moveit::planning_interface::MoveGroupInterface& grippergroup)
{
    const moveit::core::JointModelGroup* joint_model_group =
      grippergroup.getCurrentState()->getJointModelGroup("gripper");
   moveit::core::RobotStatePtr current_state = grippergroup.getCurrentState();
   std::vector<double> joint_group_positions;
    current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);
    joint_group_positions[0] = 0.02;
     joint_group_positions[1] = 0.02;
    grippergroup.setJointValueTarget(joint_group_positions);
    grippergroup.setMaxVelocityScalingFactor(1);
    grippergroup.setMaxAccelerationScalingFactor(1);
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (grippergroup.plan(plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 2 (joint space goal) %s", success ? "" : "FAILED");
    grippergroup.execute(plan);
}
void closeGripper( moveit::planning_interface::MoveGroupInterface& grippergroup)
{
    const moveit::core::JointModelGroup* joint_model_group =
      grippergroup.getCurrentState()->getJointModelGroup("gripper");
   moveit::core::RobotStatePtr current_state = grippergroup.getCurrentState();
   std::vector<double> joint_group_positions;
    current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);
    joint_group_positions[0] = 0;
     joint_group_positions[1] = 0;
    grippergroup.setJointValueTarget(joint_group_positions);
    grippergroup.setMaxVelocityScalingFactor(1);
    grippergroup.setMaxAccelerationScalingFactor(1);
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (grippergroup.plan(plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 2 (joint space goal) %s", success ? "" : "FAILED");
    grippergroup.execute(plan);
}
bool picktrajectory(moveit::planning_interface::MoveGroupInterface& move_group,float px,float py ,float pz,float x,float y ,float z,geometry_msgs::Pose targetRPY )
{
  geometry_msgs::Pose target_pose1;
  target_pose1.orientation = targetRPY.orientation;
  vector<geometry_msgs::Pose> waypoints;
  target_pose1.position.x = px;
  target_pose1.position.y = py;
  target_pose1.position.z = pz;
  waypoints.push_back(target_pose1);
  target_pose1.position.x = x;
  target_pose1.position.y = y;
  target_pose1.position.z = z;
  waypoints.push_back(target_pose1);
  target_pose1.position.x = px;
  target_pose1.position.y = py;
  target_pose1.position.z = pz;
  waypoints.push_back(target_pose1);
  target_pose1.position.x = 1.35;
  target_pose1.position.y = 0;
  target_pose1.position.z = 1.36;
target_pose1.orientation.x=0;
target_pose1.orientation.z =0;
target_pose1.orientation.w =0;
target_pose1.orientation.y =1;
  waypoints.push_back(target_pose1);
 moveit_msgs::RobotTrajectory trajectory;
 
	const double jump_threshold = 0.0;
	const double eef_step = 0.01;
	double fraction = 0.0;
    int maxtries = 100;   //最大尝试规划次数
    int attempts = 0;     //已经尝试规划次数
    while(fraction < 1.0 && attempts < maxtries)
    {
        fraction = move_group.computeCartesianPath(waypoints, eef_step, jump_threshold, trajectory);
        attempts++;
        
        if(attempts % 10 == 0)
            ROS_INFO("Still trying after %d attempts...", attempts);
    }
    if(fraction == 1)
    {   
        ROS_INFO("Path computed successfully. Moving the arm.");
	    // 生成机械臂的运动规划数据
	    moveit::planning_interface::MoveGroupInterface::Plan plan;
	    plan.trajectory_ = trajectory;
      
	    // 执行运动
	    move_group.execute(plan);
      sleep(1);
      return true;
    }
    else
    {
        ROS_INFO("Path planning failed with only %0.6f success after %d attempts.", fraction, maxtries);
        return false;
    }
}
void addCollisionObjects(moveit::planning_interface::PlanningSceneInterface& planning_scene_interface)
{
  std::vector<moveit_msgs::CollisionObject> collision_objects;
  collision_objects.resize(4);
  // Add the first table where the cube will originally be kept.
  collision_objects[0].id = "table1";
  collision_objects[0].header.frame_id = "base_link";
  collision_objects[0].primitives.resize(1);
  collision_objects[0].primitives[0].type = collision_objects[0].primitives[0].BOX;
  collision_objects[0].primitives[0].dimensions.resize(3);
  collision_objects[0].primitives[0].dimensions[0] = 0.54;
  collision_objects[0].primitives[0].dimensions[1] = 0.001;
  collision_objects[0].primitives[0].dimensions[2] = 0.4;
  collision_objects[0].primitive_poses.resize(1);
  collision_objects[0].primitive_poses[0].position.x = 1.2108;
  collision_objects[0].primitive_poses[0].position.y = 0.376140;
  collision_objects[0].primitive_poses[0].position.z = 0.259062 + 0.2;
  collision_objects[0].primitive_poses[0].orientation.w = 1.0;
  collision_objects[0].operation = collision_objects[0].ADD;


  collision_objects[1].id = "table2";
  collision_objects[1].header.frame_id = "base_link";
  collision_objects[1].primitives.resize(1);
  collision_objects[1].primitives[0].type = collision_objects[1].primitives[0].BOX;
  collision_objects[1].primitives[0].dimensions.resize(3);
  collision_objects[1].primitives[0].dimensions[0] = 0.001;
  collision_objects[1].primitives[0].dimensions[1] = 0.7617;
  collision_objects[1].primitives[0].dimensions[2] = 0.4;
  collision_objects[1].primitive_poses.resize(1);
  collision_objects[1].primitive_poses[0].position.x = 1.468637;
  collision_objects[1].primitive_poses[0].position.y = -0.012678;
  collision_objects[1].primitive_poses[0].position.z = 0.259062 + 0.2;
  collision_objects[1].primitive_poses[0].orientation.w = 1.0;
  collision_objects[1].operation = collision_objects[1].ADD;

  collision_objects[2].header.frame_id = "base_link";
  collision_objects[2].id = "table3";
  collision_objects[2].primitives.resize(1);
  collision_objects[2].primitives[0].type = collision_objects[1].primitives[0].BOX;
  collision_objects[2].primitives[0].dimensions.resize(3);
  collision_objects[2].primitives[0].dimensions[0] = 0.54;
  collision_objects[2].primitives[0].dimensions[1] = 0.001;
  collision_objects[2].primitives[0].dimensions[2] = 0.4;
  collision_objects[2].primitive_poses.resize(1);
  collision_objects[2].primitive_poses[0].position.x = 1.2160;
  collision_objects[2].primitive_poses[0].position.y = 0.3785580-0.7617;
  collision_objects[2].primitive_poses[0].position.z = 0.259062 + 0.2;
  collision_objects[2].primitive_poses[0].orientation.w = 1.0;
  collision_objects[2].operation = collision_objects[2].ADD;

  collision_objects[3].id = "table4";
  collision_objects[3].header.frame_id = "base_link";
  collision_objects[3].primitives.resize(1);
  collision_objects[3].primitives[0].type = collision_objects[1].primitives[0].BOX;
  collision_objects[3].primitives[0].dimensions.resize(3);
  collision_objects[3].primitives[0].dimensions[0] = 0.001;
  collision_objects[3].primitives[0].dimensions[1] = 0.7617;
  collision_objects[3].primitives[0].dimensions[2] = 0.4;
  collision_objects[3].primitive_poses.resize(1);
  collision_objects[3].primitive_poses[0].position.x = 0.928868;
  collision_objects[3].primitive_poses[0].position.y = -0.016620;
  collision_objects[3].primitive_poses[0].position.z = 0.259062 + 0.2;
  collision_objects[3].primitive_poses[0].orientation.w = 1.0;
  collision_objects[3].operation = collision_objects[3].ADD;

  planning_scene_interface.applyCollisionObjects(collision_objects);
}
int main(int argc, char** argv)
{

   float x,y,z;
  ros::init(argc, argv, "hg_arm_pick_place");
  ros::NodeHandle nh;
  ros::Publisher publisher = nh.advertise<std_msgs::String>("msg", 1000);
  tf::StampedTransform transform;
  ros::AsyncSpinner spinner(1);
  ros::Rate r(1);
  ros::Rate sleep(1);
  ros::Rate sleep1(500);
  ros::Rate loop_rate(10);
  spinner.start();
  ros::WallDuration(1.0).sleep();
  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
  moveit::planning_interface::MoveGroupInterface group("hg_arm");
  moveit::planning_interface::MoveGroupInterface grippergroup("gripper");
  group.allowReplanning(true);
  group.setGoalPositionTolerance(0.01); 
  //group.setGoalOrientationTolerance(0.001);
  group.setMaxAccelerationScalingFactor(0.9);
  group.setMaxVelocityScalingFactor(0.9);
  addCollisionObjects(planning_scene_interface);
  tf::TransformListener listener(ros::Duration(10));
  float px,py,pz;
  geometry_msgs::Pose targetRPY;
  ros::Subscriber sub = nh.subscribe("chatter", 1000, chatterCallback);
  sleep.sleep();
  while (ros::ok)
  {
  while (ros::ok)
  {
    bool f = listenTF(listener,px,py,pz,x,y,z,targetRPY);
    ros::spinOnce();
    loop_rate.sleep();
   if ( f == true && loadmsg == "AddPointCloud" )
   {
     break;
   }
   if (f == true && loadmsg == "ComputePath" )
   {
     break;
   }
   if (f == true && loadmsg == "ComputePathlock" )
   {
    listenTFlock(listener,px,py,pz,x,y,z,targetRPY);
     break;
   }
  }
  sleep.sleep();
   if (loadmsg == "AddPointCloud")
    {
      addPointCloud(planning_scene_interface);
      std_msgs::String sendmsg;
      std::stringstream ss;
      ss << "addpointcloud done!";
      sendmsg.data = ss.str();
      publisher.publish(sendmsg);
      loadmsg = "nan";
      x = 99999;
    }
     if (loadmsg == "ComputePath")
    {
     closeGripper(grippergroup);
      bool s = picktrajectory(group,px,py,pz,x,y,z,targetRPY);
      if (s == true)
      { 
        loadmsg = "nan";
        vector<string> name(9999);
        for (size_t i = 0; i < 9999; i++)
        {
          name[i] = "box"+ std::to_string(i);
        }
         planning_scene_interface.removeCollisionObjects(name);
      std_msgs::String sendmsg;
      std::stringstream ss;
      ss << "successfully!";
      sendmsg.data = ss.str();
      publisher.publish(sendmsg);
      r.sleep();
    
      }
      if (s == false)
      {
      std_msgs::String sendmsg;
      std::stringstream ss;
      ss << "fail!";
      sendmsg.data = ss.str();
      publisher.publish(sendmsg);
      loadmsg = "nan";
      }
    }
    if (loadmsg == "ComputePathlock")
    {
      openGripper(grippergroup);
      bool s = picktrajectory(group,px,py,pz,x,y,z,targetRPY);
      if (s == true)
      { 
        loadmsg = "nan";
        vector<string> name(9999);
        for (size_t i = 0; i < 9999; i++)
        {
          name[i] = "box"+ std::to_string(i);
        }
         planning_scene_interface.removeCollisionObjects(name);
      std_msgs::String sendmsg;
      std::stringstream ss;
      ss << "successfully!";
      
      sendmsg.data = ss.str();
      publisher.publish(sendmsg);
      r.sleep();
      x = 99999;
      }
      if (s == false)
      {
      std_msgs::String sendmsg;
      std::stringstream ss;
      ss << "fail!";
      sendmsg.data = ss.str();
      publisher.publish(sendmsg);
      loadmsg = "nan";
    
      }
    }
  }
    
  return 0;
}
