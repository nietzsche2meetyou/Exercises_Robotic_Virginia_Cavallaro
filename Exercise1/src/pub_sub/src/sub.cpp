#include <ros/ros.h>
#include "pub_sub_msg/JointPositions.h"

void callback(const pub_sub_msg::JointPositions& msg){
    for (int i = 0; i< 6; i++)
        ROS_INFO("%f", msg.positions[i]);
}

int main(int argc, char**argv){

    ros::init(argc, argv, "listener");
    ros::NodeHandle nodeHandle;
    ros::Subscriber subscriber=nodeHandle.subscribe("joint_positions", 10, callback);
    ros::spin();
    return 0;

}