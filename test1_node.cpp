
/**
 * @file offb_node.cpp
 * @brief offboard example node, written with mavros version 0.14.2, px4 flight
 * stack and tested in Gazebo SITL
 */

#include <stdio.h>
#include <math.h>

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>    
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>



mavros_msgs::State current_state;
double radius=4;
double angular_velocity=M_PI/2;
double theta=0;


void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
    current_state = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "test1_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State> ("/uav2/mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped> ("/uav2/mavros/setpoint_position/local", 10);



    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool> ("/uav2/mavros/cmd/arming");
           
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/uav2/mavros/set_mode");
            

    ROS_INFO("Starting UAV2......");
    
    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    ROS_INFO("Waiting for MAVlink connection");
    
    // wait for FCU connection
    while(ros::ok() && !current_state.connected)
    {
        ros::spinOnce();
        rate.sleep();
    }

    ROS_INFO("FCU connected with MAVROS");
    
    geometry_msgs::PoseStamped pose;

    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = 4;



    ROS_INFO("Streaming Setpoints");
    //send a few setpoints before starting
    
    for(int i = 20; ros::ok() && i > 0; --i)
    {
        local_pos_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();
    ros::Time start_time;
    int a=1;
   
    while(ros::ok())
    {
       if( current_state.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request > ros::Duration(5.0))){
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent){
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        } else {
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0))){
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }


        if(a==1)
        {
            start_time=ros::Time::now();
            a=0;
        }

        theta=angular_velocity*((ros::Time::now()-start_time).toSec());

      

        pose.pose.position.x = radius*sin(theta);
        pose.pose.position.y = radius*cos(theta);
        pose.pose.position.z = 2;

        local_pos_pub.publish(pose);
        
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}

