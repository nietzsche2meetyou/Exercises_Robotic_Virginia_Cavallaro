#include <ros/ros.h>
#include <moveit/robot_state/conversions.h>
#include <moveit/robot_state/robot_state.h>
#include <geometry_msgs/Pose.h>
#include "forward_kinematic_srv/ForwardKinSrv.h"
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <eigen_conversions/eigen_msg.h>
#include <tf_conversions/tf_eigen.h>
#include <moveit_msgs/GetPositionFK.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

int main(int argc, char **argv){
    ros::init(argc, argv, "forward_kinematic_client");
    ros::NodeHandle node;
    ros::Rate rate(0.1);
    
    /*Instances of client for requesting the forward kinematics. In particular:
    - client is custom one
    - client_moveit is that provided by moveit 
    */ 
    ros::ServiceClient client = node.serviceClient<forward_kinematic_srv::ForwardKinSrv>("evaluate_FK");
    ros::ServiceClient client_moveit = node.serviceClient<moveit_msgs::GetPositionFK>("compute_fk");

    /*
    Servers for handling the client requests. In particular:
    - srv is the custom one
    - srv_moveit is that provided by moveit
    */
    forward_kinematic_srv::ForwardKinSrv srv;
    moveit_msgs::GetPositionFK srv_moveit;

    while(node.ok()){

        std::ostringstream ss;
        /*At first, it starts by instantiating a RobotModelLoader object, 
        which will look up the robot description on the ROS parameter server and construct a 
        RobotModel to use.*/
        robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
        robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();

        /* Using the RobotModel, it's possible to construct a RobotState that maintains the configuration 
        of the robot. It will set all joints in the state to their default values.*/
        robot_state::RobotState kinematic_state(kinematic_model);

        /*it converts the object RobotState into a RobotStateMsg for composing in the correct way 
        the request for the service*/ 
        moveit::core::robotStateToRobotStateMsg(kinematic_state, srv.request.robot_state);
        client.call(srv);
        
        /* Get the planning_group_name from parameter server
        */
        std::string planning_group_name;
        if (!node.getParam("planning_group_name", planning_group_name)){
            ROS_ERROR("ERRORE");
            return false;
        }
        
        /* Start the computing of forward kinematic: 
        - JointModelGroup represents the robot model for a particular group. 
        - link_names contains names of links
        - srv_moviet.request is the request for srv_moveit: it's necessary to set the source and
        target links for evaluating the forward kinematic
        - It's necessary to covert the RobotState in message format
        */
        const robot_state::JointModelGroup * jmg = kinematic_model -> getJointModelGroup(planning_group_name);
        std::vector<std::string> link_names = jmg -> getLinkModelNames();
        srv_moveit.request.header.frame_id = link_names[0];
        srv_moveit.request.fk_link_names = link_names;
        moveit::core::robotStateToRobotStateMsg(kinematic_state, srv_moveit.request.robot_state);
        client_moveit.call(srv_moveit);

        // Printing the results
        ss << srv.response.pose << std::endl;
        ss << srv_moveit.response.pose_stamped[7].pose << std::endl;
        ROS_INFO_STREAM(ss.str());

        rate.sleep();
    }


    ros::shutdown();
    return 0;
}