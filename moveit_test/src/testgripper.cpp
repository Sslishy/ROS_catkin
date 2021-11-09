/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2012, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Ioan Sucan, Ridhwan Luthra*/

// ROS
#include <ros/ros.h>

// MoveIt
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/move_group_interface/move_group_interface.h>
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

// The circle constant tau = 2*pi. One tau is one rotation in radians.
const double tau = 2 * M_PI;

void openGripper(moveit::planning_interface::MoveGroupInterface &group)
{
 std::vector<moveit_msgs::Grasp> grasps;
 
  grasps.resize(1);
  grasps[0].grasp_posture.header.frame_id = "base_link";
   /* moveit_msgs::RobotTrajectory trajectory;
  trajectory_msgs::JointTrajectory posture;
  trajectory.joint_trajectory.header.frame_id = "base_link";
  trajectory.joint_trajectory.joint_names.resize(2);
  trajectory.joint_trajectory.joint_names[0] = "finger1";
  trajectory.joint_trajectory.joint_names[1] = "finger2";
  trajectory.joint_trajectory.points.resize(1);
  trajectory.joint_trajectory.points[0].positions.resize(2);
  trajectory.joint_trajectory.points[0].positions[0] = 0.04;
  trajectory.joint_trajectory.points[0].positions[1] = 0.04;
  trajectory.joint_trajectory.points[0].time_from_start = ros::Duration(0.5);  
  moveit::planning_interface::MoveGroupInterface::Plan plan;
  plan.trajectory_ = trajectory;
  group.execute(plan);*/
  grasps[0].grasp_posture.joint_names.resize(2);
  grasps[0].grasp_posture.joint_names[0] = "finger1";
  grasps[0].grasp_posture.joint_names[1] = "finger2";

  /* Set them as open, wide enough for the object to fit. */
  grasps[0].grasp_posture.points.resize(1);
  grasps[0].grasp_posture.points[0].positions.resize(2);
  grasps[0].grasp_posture.points[0].positions[0] = 0.04;
  grasps[0].grasp_posture.points[0].positions[1] = 0.04;
  grasps[0].grasp_posture.points[0].time_from_start = ros::Duration(0.5);
  moveit::planning_interface::MoveGroupInterface::Plan plan;
  group.execute(plan);
  
  // END_SUB_TUTORIAL

}

void closedGripper()
{
    trajectory_msgs::JointTrajectory posture;
  // BEGIN_SUB_TUTORIAL closed_gripper
  /* Add both finger joints of panda robot. */
  posture.joint_names.resize(2);
  posture.joint_names[0] = "finger1";
  posture.joint_names[1] = "finger2";

  /* Set them as closed. */
  posture.points.resize(1);
  posture.points[0].positions.resize(2);
  posture.points[0].positions[0] = 0.00;
  posture.points[0].positions[1] = 0.00;
  posture.points[0].time_from_start = ros::Duration(0.5);
  // END_SUB_TUTORIAL
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "panda_arm_pick_place");
  ros::NodeHandle nh;
  ros::AsyncSpinner spinner(1);
  spinner.start();

  ros::WallDuration(1.0).sleep();
  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
  moveit::planning_interface::MoveGroupInterface group("gripper");
    const moveit::core::JointModelGroup* joint_model_group =
      group.getCurrentState()->getJointModelGroup("gripper");
   moveit::core::RobotStatePtr current_state = group.getCurrentState();
   std::vector<double> joint_group_positions;
    current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);
    for (size_t i = 0; i < joint_group_positions.size(); i++)
    {
      std::cout << joint_group_positions[i] <<std::endl;
    }
     group.setPlanningTime(45.0);
    joint_group_positions[0] = 0.02;
     joint_group_positions[1] = 0.02;
    group.setJointValueTarget(joint_group_positions);
    group.setMaxVelocityScalingFactor(0.05);
    group.setMaxAccelerationScalingFactor(0.05);
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (group.plan(plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 2 (joint space goal) %s", success ? "" : "FAILED");
    group.execute(plan);

 
  return 0;
}

