#include <ros/ros.h>
#include "pub_sub_msg/JointPositions.h"

int main(int argc, char**argv) {

    ros::init(argc, argv, "publisher");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<pub_sub_msg::JointPositions>("joint_positions", 1);
    ros::Rate loopRate(10);

    pub_sub_msg::JointPositions joint_positions_msg;
    joint_positions_msg.positions.resize(6);

    srand(567890);

    while(ros::ok()) {

        for(int i = 0; i<6; i++){
           joint_positions_msg.positions[i] = float(rand())/float(RAND_MAX/100) ;
        }

        ROS_INFO_STREAM(joint_positions_msg);
        pub.publish(joint_positions_msg);
        ros::spinOnce();
        loopRate.sleep();
        
    }

    return 0;
}