#include <ros/ros.h>
#include <moveit/robot_state/conversions.h>
#include <moveit/robot_state/robot_state.h>
#include <geometry_msgs/Pose.h>
#include "forward_kinematic_srv/ForwardKinSrv.h"
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <eigen_conversions/eigen_msg.h>
#include <tf_conversions/tf_eigen.h>

bool forwardKinematic(forward_kinematic_srv::ForwardKinSrvRequest & req, forward_kinematic_srv::ForwardKinSrvResponse & res){
    
    // Create a RobotState object for handling the request
    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    moveit::core::RobotState rs(kinematic_model);

    // Convert the request into a RobotState object
    moveit::core::robotStateMsgToRobotState(req.robot_state, rs);

    /* Update the reference frame transforms for links. 
    This call is needed before using the transforms of links for coordinate transforms.*/
    rs.updateLinkTransforms();
    
    ros::NodeHandle node;
    std::string planning_group_name;
    if (!node.getParam("planning_group_name", planning_group_name)){
        ROS_ERROR("ERRORE");
        return false;
    }
    /*Start the computing of forward kinematic: 
    - JointModelGroup represents the robot model for a particular group. 
    - link_names contains names of links
    - robotState.getGlobalLinkTransform evaluate the forward kinematic for computing the flange pose 
    - Eigen::Isometry3d allows to represent a pose in 3d space 
    */
    const robot_state::JointModelGroup * jmg = kinematic_model -> getJointModelGroup(planning_group_name);
    std::vector<std::string> link_names = jmg -> getLinkModelNames();
    Eigen::Isometry3d forward_kinematics = rs.getGlobalLinkTransform(link_names.back());
    
    // Convert Eigen::Isometry3d into response msg
    tf::poseEigenToMsg(forward_kinematics, res.pose);
    return true;
}

int main(int argc, char **argv){

    ros::init(argc, argv, "forward_kinematic_server");
    ros::NodeHandle node;

    ros::ServiceServer service = node.advertiseService("evaluate_FK", forwardKinematic);
    ROS_INFO("Ready to evaluate the forward kinematic");
    ros::spin();

    return 0;

}



