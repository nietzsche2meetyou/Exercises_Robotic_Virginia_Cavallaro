#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "tf_parser");
    ros::NodeHandle nh;
    // Istatiantion of buffer to fill through reading tf topic 
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tf_listener(tfBuffer);
    ros::Rate rate(1.0);

    while (nh.ok()) {
        std::vector<geometry_msgs::TransformStamped> transform(7);
        rate.sleep();
        try {
            // Taking the trasformations from last link to all the others
            transform[0] = tfBuffer.lookupTransform("base_link", "flange", ros::Time(0));
            transform[1] = tfBuffer.lookupTransform("link1", "flange", ros::Time(0));
            transform[2] = tfBuffer.lookupTransform("link2", "flange", ros::Time(0));
            transform[3] = tfBuffer.lookupTransform("link3", "flange", ros::Time(0));
            transform[4] = tfBuffer.lookupTransform("link4", "flange", ros::Time(0));
            transform[5] = tfBuffer.lookupTransform("link5", "flange", ros::Time(0));
            transform[6] = tfBuffer.lookupTransform("link6", "flange", ros::Time(0));
            
        } catch (tf2::TransformException &exception) {
            ROS_WARN("%s", exception.what());
            ros::Duration(1.0).sleep();
        }

        for(int i=0; i < transform.size(); i++)
        {
            tf2::Quaternion quaternion;
            tf2::fromMsg(transform[i].transform.rotation, quaternion);
            tf2::Vector3 rotation_axis = quaternion.getAxis();
            std::ostringstream ss;
            ss << std::endl << std::endl << "************ Transformation " << transform[i].header.frame_id << " -> " << transform[i].child_frame_id << " ************" << std::endl;

            // Taking the traslation vector
            ss << std::endl << "------- Translation -------" << std::endl;
            ss << transform[i].transform.translation << std::endl;
            
            // Orientation represented with rotation matrix
            // Crete a Rotation matric starting from quaternion representation
            tf2::Matrix3x3 matrix(quaternion);

            ss << std::endl << "------- Rotation matrix -------" << std::endl;
            ss << "[ " << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << " ]" << std::endl;
            ss << "[ " << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << " ]" << std::endl;
            ss << "[ " << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << " ]" << std::endl;

            // Orientation represented in Euler angles in RPY form
            tf2Scalar roll, pitch, yaw;
            // Evaluate the roll, pitch and yaw angles starting from the rotation matrix
            matrix.getRPY(roll, pitch, yaw);

            ss << std::endl << "------- Euler angles (RPY) -------" << std::endl;
            ss << "[ " << roll << ", " << pitch << ", " << yaw << " ]" << std::endl;

            // Orientation represented in Axis/Angle form
            ss << "------- Axis/angle -------" << std::endl;
            // Axis and Angle derive from rotation matrix
            ss << "Axis = [" << rotation_axis.getX() << ", " << rotation_axis.getY() << ", " << rotation_axis.getZ() << "]" << std::endl;
            ss << "Angle = " << quaternion.getAngle() << std::endl;

            ROS_INFO_STREAM(ss.str());
        }

        rate.sleep();
    }

    ros::shutdown();
    return 0;
}
