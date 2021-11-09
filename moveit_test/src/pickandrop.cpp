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
#include"/home/slishy/Code/class/lib/computeangle.h" 
#include"/home/slishy/Code/class/lib/PointProcess.h" 

// The circle constant tau = 2*pi. One tau is one rotation in radians.
using namespace std;
const double tau = 2 * M_PI;
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
  pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor1;
  sor1.setInputCloud(cloud);
  sor1.setMeanK(300);
  sor1.setStddevMulThresh(1);
  sor1.filter(*cloud);
  pcl::VoxelGrid<pcl::PointXYZ> sor;
  sor.setInputCloud(cloud);
  sor.setLeafSize(0.009,0.009,0.009);
  sor.filter(*cloud);
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
void openGripper(trajectory_msgs::JointTrajectory& posture)
{
   posture.joint_names.resize(2);
  posture.joint_names[0] = "panda_finger_joint1";
  posture.joint_names[1] = "panda_finger_joint2";

  /* Set them as open, wide enough for the object to fit. */
  posture.points.resize(1);
  posture.points[0].positions.resize(2);
  posture.points[0].positions[0] = 0.04;
  posture.points[0].positions[1] = 0.04;
  posture.points[0].time_from_start = ros::Duration(0.5);
  // END_SUB_TUTORIAL
}
void closeGripper(trajectory_msgs::JointTrajectory& posture)
{
  posture.joint_names.resize(2);
  posture.joint_names[0] = "panda_finger_joint1";
  posture.joint_names[1] = "panda_finger_joint2";

  /* Set them as closed. */
  posture.points.resize(1);
  posture.points[0].positions.resize(2);
  posture.points[0].positions[0] = 0.00;
  posture.points[0].positions[1] = 0.00;
  posture.points[0].time_from_start = ros::Duration(0.5);
}
void pick(moveit::planning_interface::MoveGroupInterface& move_group,float x,float y ,float z, float rx ,float ry,float rz)
{
  tf2::Quaternion orientation;
  rx = (M_PI/180) *rx;
  ry = (M_PI/180) *ry;
  rz = (M_PI/180) *rz;
  computeangle ca;
  orientation.setRPY(rx,ry,rz);
  geometry_msgs::Pose target_pose1;
  target_pose1.orientation = tf2::toMsg(orientation);
  tf::Quaternion q3 = {target_pose1.orientation.x, target_pose1.orientation.y, target_pose1.orientation.z, target_pose1.orientation.w};
  tf::Matrix3x3 matri;
  tf::Vector3 v6;
  matri.setRotation(q3);
  vector<pcl::Normal> normalz(1);
  v6 =  matri[2];
  normalz[0].normal_x = v6[0];
  normalz[0].normal_y = v6[1];
  normalz[0].normal_z = v6[2];
  vector<pcl::PointXYZ> center(1);
  center[0].x = x;
  center[0].y = y;
  center[0].z = z;
  float ofz = 100;
  ca.Setoffset_z_value(ofz);
  ca.offsetZ(center,normalz);
  //target_pose1.orientation.w =1;
  target_pose1.position.x = center[0].x;
  target_pose1.position.y = center[0].y;
  target_pose1.position.z = center[0].z;
  move_group.setPoseTarget(target_pose1,"gripper");
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  if (success == true)
  {
    move_group.execute(my_plan);
    cout<< my_plan.trajectory_ <<endl;
  }
  else{
    std::cout << "planning fail"<<std::endl;
  }

}
void picktrajectory(moveit::planning_interface::MoveGroupInterface& move_group,float x,float y ,float z, float rx ,float ry,float rz)
{
  tf2::Quaternion orientation;
  rx = (M_PI/180) *rx;
  ry = (M_PI/180) *ry;
  rz = (M_PI/180) *rz;
  computeangle ca;
  orientation.setRPY(rx,ry,rz);
  geometry_msgs::Pose target_pose1;
  target_pose1.orientation = tf2::toMsg(orientation);
  tf::Quaternion q3 = {target_pose1.orientation.x, target_pose1.orientation.y, target_pose1.orientation.z, target_pose1.orientation.w};
  tf::Matrix3x3 matri;
  tf::Vector3 v6;
  matri.setRotation(q3);
  vector<pcl::Normal> normalz(1);
  v6 =  matri[2];
  normalz[0].normal_x = v6[0];
  normalz[0].normal_y = v6[1];
  normalz[0].normal_z = v6[2];
  vector<pcl::PointXYZ> center(1);
  center[0].x = x;
  center[0].y = y;
  center[0].z = z;
  float ofz = 50;
  ca.Setoffset_z_value(ofz);
  ca.offsetZ(center,normalz);
  float pretatget_x,pretatget_y,pretatget_z,tatget_x,tatget_y,tatget_z;
  //target_pose1.orientation.w =1;
  tatget_x = center[0].x;
  tatget_y=  center[0].y;
  tatget_z = center[0].z;
  ofz = 150.0;
  ca.Setoffset_z_value(ofz);
  ca.offsetZ(center,normalz);
  pretatget_x = center[0].x;
  pretatget_y=  center[0].y;
  pretatget_z = center[0].z;
  target_pose1.position.x = pretatget_x;
  target_pose1.position.y = pretatget_y;
  target_pose1.position.z = pretatget_z;
  target_pose1.orientation = tf2::toMsg(orientation);
  move_group.setPlanningTime(10.0);
  move_group.setPlannerId("RRTConnectkConfigDefault");
  move_group.setPoseTarget(target_pose1,"gripper");
  vector<geometry_msgs::Pose> waypoints;
  waypoints.push_back(target_pose1);
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  if (success == true)
  {
    move_group.execute(my_plan);
  }
  else{
    std::cout << "pretarget planning fail"<<std::endl;
  }
  move_group.clearPathConstraints();
  target_pose1.position.x = tatget_x;
  target_pose1.position.y = tatget_y;
  target_pose1.position.z = tatget_z;
  target_pose1.orientation = tf2::toMsg(orientation);
   move_group.setPoseTarget(target_pose1,"gripper");
     waypoints.push_back(target_pose1);
 success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  if (success == true)
  {
    move_group.execute(my_plan);
  }
  else{
    std::cout << "target planning fail"<<std::endl;
  }
  target_pose1.position.x = pretatget_x;
  target_pose1.position.y = pretatget_y;
  target_pose1.position.z = pretatget_z;
  target_pose1.orientation = tf2::toMsg(orientation);\
  move_group.setPoseTarget(target_pose1,"gripper");
    waypoints.push_back(target_pose1);
  success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
  if (success == true)
  {
    move_group.execute(my_plan);
  }
  else{
    std::cout << "pretargetout planning fail"<<std::endl;
  }

}

void addCollisionObjects(moveit::planning_interface::PlanningSceneInterface& planning_scene_interface)
{
  std::vector<moveit_msgs::CollisionObject> collision_objects;
  collision_objects.resize(3);

  // Add the first table where the cube will originally be kept.
  collision_objects[0].id = "table1";
  collision_objects[0].header.frame_id = "panda_link0";

  /* Define the primitive and its dimensions. */
  collision_objects[0].primitives.resize(1);
  collision_objects[0].primitives[0].type = collision_objects[0].primitives[0].BOX;
  collision_objects[0].primitives[0].dimensions.resize(3);
  collision_objects[0].primitives[0].dimensions[0] = 0.2;
  collision_objects[0].primitives[0].dimensions[1] = 0.4;
  collision_objects[0].primitives[0].dimensions[2] = 0.4;

  /* Define the pose of the table. */
  collision_objects[0].primitive_poses.resize(1);
  collision_objects[0].primitive_poses[0].position.x = 0.5;
  collision_objects[0].primitive_poses[0].position.y = 0;
  collision_objects[0].primitive_poses[0].position.z = 0.2;
  collision_objects[0].primitive_poses[0].orientation.w = 1.0;
  // END_SUB_TUTORIAL

  collision_objects[0].operation = collision_objects[0].ADD;

  // BEGIN_SUB_TUTORIAL table2
  // Add the second table where we will be placing the cube.
  collision_objects[1].id = "table2";
  collision_objects[1].header.frame_id = "panda_link0";

  /* Define the primitive and its dimensions. */
  collision_objects[1].primitives.resize(1);
  collision_objects[1].primitives[0].type = collision_objects[1].primitives[0].BOX;
  collision_objects[1].primitives[0].dimensions.resize(3);
  collision_objects[1].primitives[0].dimensions[0] = 0.4;
  collision_objects[1].primitives[0].dimensions[1] = 0.2;
  collision_objects[1].primitives[0].dimensions[2] = 0.4;

  /* Define the pose of the table. */
  collision_objects[1].primitive_poses.resize(1);
  collision_objects[1].primitive_poses[0].position.x = 0;
  collision_objects[1].primitive_poses[0].position.y = 0.5;
  collision_objects[1].primitive_poses[0].position.z = 0.2;
  collision_objects[1].primitive_poses[0].orientation.w = 1.0;
  // END_SUB_TUTORIAL

  collision_objects[1].operation = collision_objects[1].ADD;

  // BEGIN_SUB_TUTORIAL object
  // Define the object that we will be manipulating
  collision_objects[2].header.frame_id = "panda_link0";
  collision_objects[2].id = "object";

  /* Define the primitive and its dimensions. */
  collision_objects[2].primitives.resize(1);
  collision_objects[2].primitives[0].type = collision_objects[1].primitives[0].BOX;
  collision_objects[2].primitives[0].dimensions.resize(3);
  collision_objects[2].primitives[0].dimensions[0] = 0.02;
  collision_objects[2].primitives[0].dimensions[1] = 0.02;
  collision_objects[2].primitives[0].dimensions[2] = 0.2;

  /* Define the pose of the object. */
  collision_objects[2].primitive_poses.resize(1);
  collision_objects[2].primitive_poses[0].position.x = 0.5;
  collision_objects[2].primitive_poses[0].position.y = 0;
  collision_objects[2].primitive_poses[0].position.z = 0.5;
  collision_objects[2].primitive_poses[0].orientation.w = 1.0;
  // END_SUB_TUTORIAL
  collision_objects[2].operation = collision_objects[2].ADD;
  planning_scene_interface.applyCollisionObjects(collision_objects);
}
int main(int argc, char** argv)
{
  
  std::string x_,y_,z_,rx_,ry_,rz_;
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
  if(rx >0)
  {
     rx = 180 - rx;
  }
  if (rx<0)
  {
    rx = 180 + rx;
  }
  ros::init(argc, argv, "panda_arm_pick_place");
  ros::NodeHandle nh;
  tf::TransformListener listener;
  tf::StampedTransform transform;
  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::WallDuration(1.0).sleep();
  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
  moveit::planning_interface::MoveGroupInterface group("hg_arm");
  string end_effector_link = "gripper";
  group.allowReplanning(true);
  group.setGoalPositionTolerance(0.001);
  group.setGoalOrientationTolerance(0.001);
  group.setMaxAccelerationScalingFactor(0.5);
  group.setMaxVelocityScalingFactor(0.5);
  addPointCloud(planning_scene_interface);
  for (size_t i = 0; i < 50; i++)
  {
   picktrajectory(group,x,y,z,rx,ry,rz);
   group.setNamedTarget("home");
   group.move();
  }
  
  
  return 0;
}