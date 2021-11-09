#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h> 
#include <pcl_conversions/pcl_conversions.h> 
#include <sensor_msgs/PointCloud2.h> 
#include<moveit/planning_scene_interface/planning_scene_interface.h>

// MoveIt
#include <moveit_msgs/PlanningScene.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/GetStateValidity.h>
#include <moveit/planning_interface/planning_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit/kinematic_constraints/utils.h>
#include <moveit_msgs/ApplyPlanningScene.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>
#include <moveit_visual_tools/moveit_visual_tools.h>
using namespace std;
int main(int argc, char** argv)
{
 
    
    /*ros::init(argc,argv,"planning_scene_ros_api_tutorial");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle node_handle;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();
    ros::Publisher planning_scene_diff_publisher = node_handle.advertise<moveit_msgs::PlanningScene>("planning_scene",1);
    ros::WallDuration sleep_t(0.5);
    while (planning_scene_diff_publisher.getNumSubscribers() < 1)
    {
      sleep_t.sleep();
    }
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in(new pcl::PointCloud<pcl::PointXYZ>);
    //pcl::io::loadPCDFile("/home/slishy/working/Code/pcd/test.pcd",*cloud_in);
    cloud_in->width = 100;
    cloud_in->height =1;
    cloud_in->points.resize(100);
    for (size_t i = 0; i < cloud_in->points.size (); ++i)
	 { 
		 cloud_in->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f); 
		 cloud_in->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f); 
		 cloud_in->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f); 
	} 
    ros::Publisher pcl_pub = node_handle.advertise<sensor_msgs::PointCloud2> ("pcl_output", 1); 
    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(*cloud_in,output);
    output.header.frame_id = "panda_link0";
    ros::Rate loop_rate(1); 
	 pcl_pub.publish(output); 
    visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to start the demo");
    moveit_msgs::AttachedCollisionObject attached_object;
    attached_object.link_name = "panda_hand";
    attached_object.object.header.frame_id = "panda_hand";
    attached_object.object.id = "box";
    geometry_msgs::Pose pose;
    pose.position.z = 0.11;
    pose.orientation.w = 1;
    shape_msgs::SolidPrimitive primitive;
    primitive.type = primitive.BOX;
    primitive.dimensions.resize(3);
    primitive.dimensions[0] =0.1;
    primitive.dimensions[1] =0.1;
    primitive.dimensions[2] =0.1;
    attached_object.object.primitives.push_back(primitive);
    attached_object.object.primitive_poses.push_back(pose);
    attached_object.object.operation = attached_object.object.ADD;
    attached_object.touch_links = std::vector<std::string>{"panda_hand","panda_leftfinger","panda_rightfinger"};
    moveit_msgs::PlanningScene planning_scene;
    planning_scene.world.collision_objects.push_back(attached_object.object);
    planning_scene.is_diff = true;
    //add object
    planning_scene_diff_publisher.publish(planning_scene);
    cout << "publisher" <<endl;
    visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");
    ros::ServiceClient planning_scene_diff_client = node_handle.serviceClient<moveit_msgs::ApplyPlanningScene>("apply_planning_scene");
    planning_scene_diff_client.waitForExistence();
    moveit_msgs::ApplyPlanningScene srv;
    srv.request.scene = planning_scene;
    planning_scene_diff_client.call(srv);
    moveit_msgs::CollisionObject remove_object;
    remove_object.id = "box";
    remove_object.header.frame_id = "panda_hand";
    remove_object.operation = remove_object.REMOVE;
    cout <<"remove" <<endl;
    planning_scene.world.collision_objects.clear();
    planning_scene.world.collision_objects.push_back(remove_object);
     //attach object to robot
    planning_scene.robot_state.attached_collision_objects.push_back(attached_object);
    planning_scene.robot_state.is_diff = true;
    
    planning_scene_diff_publisher.publish(planning_scene);
    visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");
    moveit_msgs::AttachedCollisionObject detach_object;
    detach_object.object.id = "box";
    detach_object.link_name = "panda_hand";
    detach_object.object.operation = detach_object.object.REMOVE;
    planning_scene.robot_state.attached_collision_objects.clear();
    planning_scene.robot_state.attached_collision_objects.push_back(detach_object);
    planning_scene.robot_state.is_diff = true;
    planning_scene.world.collision_objects.clear();
    planning_scene.world.collision_objects.push_back(attached_object.object);
    planning_scene_diff_publisher.publish(planning_scene);
     visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");
     planning_scene.robot_state.attached_collision_objects.clear();
     planning_scene.world.collision_objects.clear();
     planning_scene.world.collision_objects.push_back(remove_object);
     planning_scene_diff_publisher.publish(planning_scene);*/
  ros::init (argc, argv, "pcl_create"); 
	ros::NodeHandle nh; 
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();
    ros::Publisher planning_scene_diff_publisher = nh.advertise<moveit_msgs::PlanningScene>("planning_scene",1);
    ros::WallDuration sleep_t(0.5);
    while (planning_scene_diff_publisher.getNumSubscribers() < 1)
    {
      sleep_t.sleep();
    }
	ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2> ("pcl_output", 1);     
	pcl::PointCloud<pcl::PointXYZ> cloud; 
	sensor_msgs::PointCloud2 output; 
	pcl::io::loadPCDFile("/home/slishy/working/Code/pcd/test.pcd",cloud);
  for (size_t i = 0; i < cloud.points.size(); i++)
  {
    cloud.points[i].x = cloud.points[i].x * 0.001 - 0.8;
    cloud.points[i].y = cloud.points[i].y * 0.001;
    cloud.points[i].z = cloud.points[i].z * 0.001;
  }
   vector<moveit_msgs::AttachedCollisionObject> attached_object;
    vector<geometry_msgs::Pose> pose;
    pose.resize(cloud.points.size());
    attached_object.resize(cloud.points.size());
    for (size_t i = 0; i < cloud.points.size(); i++)
    {
    attached_object[i].link_name = "panda_link0";
    attached_object[i].object.header.frame_id = "panda_link0";
    attached_object[i].object.id = "box"+to_string(i);
    pose[i].position.z = cloud.points[i].z;
    pose[i].position.x = cloud.points[i].x;
    pose[i].position.y = cloud.points[i].y;
    pose[i].orientation.w = 1;
    }
    shape_msgs::SolidPrimitive primitive;
    primitive.type = primitive.BOX;
    primitive.dimensions.resize(3);
    primitive.dimensions[0] =0.01;
    primitive.dimensions[1] =0.01;
    primitive.dimensions[2] =0.01;
    moveit_msgs::PlanningScene planning_scene;
    for (size_t i = 0; i < cloud.points.size(); i++)
    {
      attached_object[i].object.primitives.push_back(primitive);
      attached_object[i].object.primitive_poses.push_back(pose[i]);
      attached_object[i].object.operation = attached_object[i].object.ADD;
      attached_object[i].touch_links = std::vector<std::string>{"panda_hand","panda_leftfinger","panda_rightfinger"};
      planning_scene.world.collision_objects.push_back(attached_object[i].object);
    }
    planning_scene.is_diff = true;
    //add object
    planning_scene_diff_publisher.publish(planning_scene);
	return 0; 
  
  /* string node_name = "motion_planning_tutorial";
   ros::init(argc,argv,node_name);
   ros::AsyncSpinner spinner(1);
   spinner.start();
   ros::NodeHandle node_handle("~");  
   string PLANNING_GROUP = "panda_arm";
   robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
   const moveit::core::RobotModelPtr& robot_model = robot_model_loader.getModel();
   moveit::core::RobotStatePtr robot_state(new moveit::core::RobotState(robot_model));
   const moveit::core::JointModelGroup* joint_model_group = robot_state->getJointModelGroup(PLANNING_GROUP);
   planning_scene::PlanningScenePtr planning_scene(new planning_scene::PlanningScene(robot_model));
   planning_scene->getCurrentStateNonConst().setToDefaultValues(joint_model_group,"ready");
   boost::scoped_ptr<pluginlib::ClassLoader<planning_interface::PlannerManager>> planner_pluginlib_loader;
   planning_interface::PlannerManagerPtr planner_instance;
   string planner_plugin_name;
   if (!node_handle.getParam("planning_plugin", planner_plugin_name))
   {
     ROS_FATAL_STREAM("Could not find planner plugin name");
   }
   try
   {
     planner_pluginlib_loader.reset(new pluginlib::ClassLoader<planning_interface::PlannerManager>
     ("moveit_core","planning_interface::plannermanager"));
   }
   catch(const std::exception& ex)
   {
     ROS_FATAL_STREAM("Exception while creating planning plugin loader " << ex.what());
   }
   try
  {
    planner_instance.reset(planner_pluginlib_loader->createUnmanagedInstance(planner_plugin_name));
    if (!planner_instance->initialize(robot_model, node_handle.getNamespace()))
      ROS_FATAL_STREAM("Could not initialize planner instance");
    ROS_INFO_STREAM("Using planning interface '" << planner_instance->getDescription() << "'");
  }
  catch (pluginlib::PluginlibException& ex)
  {
    const std::vector<std::string>& classes = planner_pluginlib_loader->getDeclaredClasses();
    std::stringstream ss;
    for (const auto& cls : classes)
      ss << cls << " ";
    ROS_ERROR_STREAM("Exception while loading planner '" << planner_plugin_name << "': " << ex.what() << std::endl
                                                         << "Available plugins: " << ss.str());
  }
  namespace rvt = rviz_visual_tools;
  moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
  visual_tools.loadRobotStatePub("./display_robot_state");
  visual_tools.enableBatchPublishing();
  visual_tools.deleteAllMarkers();
  visual_tools.trigger();
  visual_tools.loadRemoteControl();
  Eigen::Isometry3d text_pose = Eigen::Isometry3d::Identity();
  text_pose.translation().z() = 1.75;
  visual_tools.publishText(text_pose, "Motion Planning API Demo", rvt::WHITE, rvt::XLARGE);
  visual_tools.trigger();
  visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to start the demo");
  visual_tools.publishRobotState(planning_scene->getCurrentStateNonConst(), rviz_visual_tools::GREEN);
  visual_tools.trigger();
  planning_interface::MotionPlanRequest req;
  planning_interface::MotionPlanResponse res;
  geometry_msgs::PoseStamped pose;
  pose.header.frame_id = "panda_link0";
  pose.pose.position.x = 0.3;
  pose.pose.position.y = 0.4;
  pose.pose.position.z = 0.75;
  pose.pose.orientation.w = 1.0;
  vector<double> tolerance_pose(3,0.01);
  vector<double> tolerance_angle(3,0.01);
  moveit_msgs::Constraints pose_goal = 
          kinematic_constraints::constructGoalConstraints("panda_link8",pose,tolerance_pose,tolerance_angle);
  req.group_name = PLANNING_GROUP;
  req.goal_constraints.push_back(pose_goal);

   planning_interface::PlanningContextPtr context =
      planner_instance->getPlanningContext(planning_scene, req, res.error_code_);
  context->solve(res);
  if (res.error_code_.val != res.error_code_.SUCCESS)
  {
    ROS_ERROR("Could not compute plan successfully");
    return 0;
  }

  // Visualize the result
  // ^^^^^^^^^^^^^^^^^^^^
  ros::Publisher display_publisher =
      node_handle.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);
  moveit_msgs::DisplayTrajectory display_trajectory;

  /* Visualize the trajectory */
 /* moveit_msgs::MotionPlanResponse response;
  res.getMessage(response);

  display_trajectory.trajectory_start = response.trajectory_start;
  display_trajectory.trajectory.push_back(response.trajectory);
  visual_tools.publishTrajectoryLine(display_trajectory.trajectory.back(), joint_model_group);
  visual_tools.trigger();
  display_publisher.publish(display_trajectory);

  /* Set the state in the planning scene to the final state of the last plan */
  /*robot_state->setJointGroupPositions(joint_model_group, response.trajectory.joint_trajectory.points.back().positions);
  planning_scene->setCurrentState(*robot_state.get());

  // Display the goal state
  visual_tools.publishRobotState(planning_scene->getCurrentStateNonConst(), rviz_visual_tools::GREEN);
  visual_tools.publishAxisLabeled(pose.pose, "goal_1");
  visual_tools.publishText(text_pose, "Pose Goal (1)", rvt::WHITE, rvt::XLARGE);
  visual_tools.trigger();

  /* We can also use visual_tools to wait for user input */
 /* visual_tools.prompt("Press 'next' in the RvizVisualToolsGui window to continue the demo");*/

}