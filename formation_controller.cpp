
#include <stdio.h>
#include <math.h>

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>    
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>



mavros_msgs::State current_state_uav1,current_state_uav2,current_state_uav3,current_state_uav4,current_state_uav5,current_state_uav6;
double radius=4;
double angular_velocity=M_PI/2;
double velocity=10;
double theta=0;
double altitude=4;


void state_cb_uav1(const mavros_msgs::State::ConstPtr& msg)
{
    current_state_uav1 = *msg;
}

void state_cb_uav2(const mavros_msgs::State::ConstPtr& msg)
{
    current_state_uav2 = *msg;
}

void state_cb_uav3(const mavros_msgs::State::ConstPtr& msg)
{
    current_state_uav3 = *msg;
}
void state_cb_uav4(const mavros_msgs::State::ConstPtr& msg)
{
    current_state_uav4 = *msg;
}
void state_cb_uav5(const mavros_msgs::State::ConstPtr& msg)
{
    current_state_uav5 = *msg;
}
void state_cb_uav6(const mavros_msgs::State::ConstPtr& msg)
{
    current_state_uav6 = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "formation_controller");
    ros::NodeHandle nh;
    
    ros::Subscriber state_sub_uav1 = nh.subscribe<mavros_msgs::State> ("/uav1/mavros/state", 10, state_cb_uav1);
    ros::Publisher local_pos_pub_uav1 = nh.advertise<geometry_msgs::PoseStamped> ("/uav1/mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client_uav1 = nh.serviceClient<mavros_msgs::CommandBool> ("/uav1/mavros/cmd/arming");     
    ros::ServiceClient set_mode_client_uav1 = nh.serviceClient<mavros_msgs::SetMode>("/uav1/mavros/set_mode");

    ros::Subscriber state_sub_uav2 = nh.subscribe<mavros_msgs::State> ("/uav2/mavros/state", 10, state_cb_uav2);
    ros::Publisher local_pos_pub_uav2 = nh.advertise<geometry_msgs::PoseStamped> ("/uav2/mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client_uav2 = nh.serviceClient<mavros_msgs::CommandBool> ("/uav2/mavros/cmd/arming");     
    ros::ServiceClient set_mode_client_uav2 = nh.serviceClient<mavros_msgs::SetMode>("/uav2/mavros/set_mode");
    
    ros::Subscriber state_sub_uav3 = nh.subscribe<mavros_msgs::State> ("/uav3/mavros/state", 10, state_cb_uav3);
    ros::Publisher local_pos_pub_uav3 = nh.advertise<geometry_msgs::PoseStamped> ("/uav3/mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client_uav3 = nh.serviceClient<mavros_msgs::CommandBool> ("/uav3/mavros/cmd/arming");     
    ros::ServiceClient set_mode_client_uav3 = nh.serviceClient<mavros_msgs::SetMode>("/uav3/mavros/set_mode");

    ros::Subscriber state_sub_uav4 = nh.subscribe<mavros_msgs::State> ("/uav4/mavros/state", 10, state_cb_uav4);
    ros::Publisher local_pos_pub_uav4 = nh.advertise<geometry_msgs::PoseStamped> ("/uav4/mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client_uav4 = nh.serviceClient<mavros_msgs::CommandBool> ("/uav4/mavros/cmd/arming");     
    ros::ServiceClient set_mode_client_uav4 = nh.serviceClient<mavros_msgs::SetMode>("/uav4/mavros/set_mode");

    ros::Subscriber state_sub_uav5 = nh.subscribe<mavros_msgs::State> ("/uav5/mavros/state", 10, state_cb_uav5);
    ros::Publisher local_pos_pub_uav5 = nh.advertise<geometry_msgs::PoseStamped> ("/uav5/mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client_uav5 = nh.serviceClient<mavros_msgs::CommandBool> ("/uav5/mavros/cmd/arming");     
    ros::ServiceClient set_mode_client_uav5 = nh.serviceClient<mavros_msgs::SetMode>("/uav5/mavros/set_mode");

    ros::Subscriber state_sub_uav6 = nh.subscribe<mavros_msgs::State> ("/uav6/mavros/state", 10, state_cb_uav6);
    ros::Publisher local_pos_pub_uav6 = nh.advertise<geometry_msgs::PoseStamped> ("/uav6/mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client_uav6 = nh.serviceClient<mavros_msgs::CommandBool> ("/uav6/mavros/cmd/arming");     
    ros::ServiceClient set_mode_client_uav6 = nh.serviceClient<mavros_msgs::SetMode>("/uav6/mavros/set_mode");
            

    ROS_INFO("Establishing Connection........");
    
    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(10.0);

    
    // wait for FCU connection
    while(ros::ok() && !current_state_uav1.connected)
    {
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("UAV1 Online");

    while(ros::ok() && !current_state_uav2.connected)
    {
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("UAV2 Online");

    while(ros::ok() && !current_state_uav3.connected)
    {
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("UAV3 Online");

    while(ros::ok() && !current_state_uav4.connected)
    {
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("UAV4 Online");

     while(ros::ok() && !current_state_uav5.connected)
    {
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("UAV5 Online");

     while(ros::ok() && !current_state_uav6.connected)
    {
        ros::spinOnce();
        rate.sleep();
    }

    ROS_INFO("UAV6 Online");
    
    geometry_msgs::PoseStamped pose_uav1,pose_uav2,pose_uav3,pose_uav4,pose_uav5,pose_uav6;

    pose_uav1.pose.position.x = 0;
    pose_uav1.pose.position.y = radius;
    pose_uav1.pose.position.z = altitude;

    pose_uav2.pose.position.x = 0;
    pose_uav2.pose.position.y = -radius;
    pose_uav2.pose.position.z = altitude;

    pose_uav3.pose.position.x =  radius*cos(theta);
    pose_uav3.pose.position.y =  radius*sin(theta);
    pose_uav3.pose.position.z = altitude;

    pose_uav4.pose.position.x =  radius*cos(theta);
    pose_uav4.pose.position.y = -radius*sin(theta);
    pose_uav4.pose.position.z = altitude;

    pose_uav5.pose.position.x = -radius*cos(theta);
    pose_uav5.pose.position.y =  radius*sin(theta);
    pose_uav5.pose.position.z = altitude;

    pose_uav6.pose.position.x = -radius*cos(theta);
    pose_uav6.pose.position.y = -radius*sin(theta);
    pose_uav6.pose.position.z = altitude;



    ROS_INFO("Preparing for Flight");
    //send a few setpoints before starting
    
    for(int i = 20; ros::ok() && i > 0; --i)
    {
        local_pos_pub_uav1.publish(pose_uav1);
        local_pos_pub_uav2.publish(pose_uav2);
        local_pos_pub_uav3.publish(pose_uav3);
        local_pos_pub_uav4.publish(pose_uav4);
        local_pos_pub_uav5.publish(pose_uav5);
        local_pos_pub_uav6.publish(pose_uav6);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode_uav1,offb_set_mode_uav2,offb_set_mode_uav3,offb_set_mode_uav4,offb_set_mode_uav5,offb_set_mode_uav6;
    offb_set_mode_uav1.request.custom_mode = "OFFBOARD";
    offb_set_mode_uav2.request.custom_mode = "OFFBOARD";
    offb_set_mode_uav3.request.custom_mode = "OFFBOARD";
    offb_set_mode_uav4.request.custom_mode = "OFFBOARD";
    offb_set_mode_uav5.request.custom_mode = "OFFBOARD";
    offb_set_mode_uav6.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd_uav1,arm_cmd_uav2,arm_cmd_uav3,arm_cmd_uav4,arm_cmd_uav5,arm_cmd_uav6;
    arm_cmd_uav1.request.value = true;
    arm_cmd_uav2.request.value = true;
    arm_cmd_uav3.request.value = true;
    arm_cmd_uav4.request.value = true;
    arm_cmd_uav5.request.value = true;
    arm_cmd_uav6.request.value = true;

    ros::Time last_request_uav1 = ros::Time::now();
    ros::Time last_request_uav2 = ros::Time::now();
    ros::Time last_request_uav3 = ros::Time::now();
    ros::Time last_request_uav4 = ros::Time::now();
    ros::Time last_request_uav5 = ros::Time::now();
    ros::Time last_request_uav6 = ros::Time::now();
    ros::Time start_time;
    int flag[6],m=0;
    for(m=0;m<6;m++)
        flag[m]=0;
    flag[3]=1;
    flag[4]=1;
    flag[5]=1;
    

   start_time=ros::Time::now();
    while(ros::ok())
    {
        if( current_state_uav1.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request_uav1 > ros::Duration(5.0))){
            if( set_mode_client_uav1.call(offb_set_mode_uav1) &&
                offb_set_mode_uav1.response.mode_sent){
                ROS_INFO("UAV1 Offboard Control enabled");
            }
            last_request_uav1 = ros::Time::now();
        } else {
            if( !current_state_uav1.armed &&
                (ros::Time::now() - last_request_uav1 > ros::Duration(5.0))){
                if( arming_client_uav1.call(arm_cmd_uav1) &&
                    arm_cmd_uav1.response.success){
                    ROS_INFO("UAV1 Vehicle armed");
                }
                last_request_uav1 = ros::Time::now();
            }
        }

        if( current_state_uav2.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request_uav2 > ros::Duration(5.0))){
            if( set_mode_client_uav2.call(offb_set_mode_uav2) &&
                offb_set_mode_uav2.response.mode_sent){
                ROS_INFO("UAV2 Offboard Control enabled");
            }
            last_request_uav2 = ros::Time::now();
        } else {
            if( !current_state_uav2.armed &&
                (ros::Time::now() - last_request_uav2 > ros::Duration(5.0))){
                if( arming_client_uav2.call(arm_cmd_uav2) &&
                    arm_cmd_uav2.response.success){
                    ROS_INFO("UAV2 Vehicle armed");
                }
                last_request_uav2 = ros::Time::now();
            }
        }

        if( current_state_uav3.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request_uav3 > ros::Duration(5.0))){
            if( set_mode_client_uav3.call(offb_set_mode_uav3) &&
                offb_set_mode_uav3.response.mode_sent){
                ROS_INFO("UAV3 Offboard Control enabled");
            }
            last_request_uav3 = ros::Time::now();
        } else {
            if( !current_state_uav3.armed &&
                (ros::Time::now() - last_request_uav3 > ros::Duration(5.0))){
                if( arming_client_uav3.call(arm_cmd_uav3) &&
                    arm_cmd_uav3.response.success){
                    ROS_INFO("UAV3 Vehicle armed");
                }
                last_request_uav3 = ros::Time::now();
            }
        }


         if( current_state_uav4.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request_uav4 > ros::Duration(5.0))){
            if( set_mode_client_uav4.call(offb_set_mode_uav4) &&
                offb_set_mode_uav4.response.mode_sent){
                ROS_INFO("UAV4 Offboard Control enabled");
            }
            last_request_uav4 = ros::Time::now();
        } else {
            if( !current_state_uav4.armed &&
                (ros::Time::now() - last_request_uav4 > ros::Duration(5.0))){
                if( arming_client_uav4.call(arm_cmd_uav4) &&
                    arm_cmd_uav4.response.success){
                    ROS_INFO("UAV4 Vehicle armed");
                }
                last_request_uav4 = ros::Time::now();
            }
        }

        if( current_state_uav5.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request_uav5 > ros::Duration(5.0))){
            if( set_mode_client_uav5.call(offb_set_mode_uav5) &&
                offb_set_mode_uav5.response.mode_sent){
                ROS_INFO("UAV5 Offboard Control enabled");
            }
            last_request_uav5 = ros::Time::now();
        } else {
            if( !current_state_uav5.armed &&
                (ros::Time::now() - last_request_uav5 > ros::Duration(5.0))){
                if( arming_client_uav5.call(arm_cmd_uav5) &&
                    arm_cmd_uav5.response.success){
                    ROS_INFO("UAV5 Vehicle armed");
                }
                last_request_uav5 = ros::Time::now();
            }
        }

        if( current_state_uav6.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request_uav6 > ros::Duration(5.0))){
            if( set_mode_client_uav6.call(offb_set_mode_uav6) &&
                offb_set_mode_uav6.response.mode_sent){
                ROS_INFO("UAV6 Offboard Control enabled");
            }
            last_request_uav6 = ros::Time::now();
        } else {
            if( !current_state_uav6.armed &&
                (ros::Time::now() - last_request_uav6 > ros::Duration(5.0))){
                if( arming_client_uav6.call(arm_cmd_uav6) &&
                    arm_cmd_uav6.response.success){
                    ROS_INFO("UAV6 Vehicle armed");
                }
                last_request_uav6 = ros::Time::now();
            }
        }


        theta=angular_velocity*((ros::Time::now()-start_time).toSec());

      
        // Diverge
        /*
        pose_uav1.pose.position.x = radius*sin(theta);
        pose_uav1.pose.position.y = radius*cos(theta);
        pose_uav1.pose.position.z = 2;

        pose_uav2.pose.position.x = radius*sin(theta);
        pose_uav2.pose.position.y = radius*cos(theta);
        pose_uav2.pose.position.z = 2;

        pose_uav3.pose.position.x = radius*sin(theta);
        pose_uav3.pose.position.y = radius*cos(theta);
        pose_uav3.pose.position.z = 2;

        pose_uav4.pose.position.x = radius*sin(theta);
        pose_uav4.pose.position.y = radius*cos(theta);
        pose_uav4.pose.position.z = 2;

        pose_uav5.pose.position.x = radius*sin(theta);
        pose_uav5.pose.position.y = radius*cos(theta);
        pose_uav5.pose.position.z = 2;

        pose_uav6.pose.position.x = radius*sin(theta);
        pose_uav6.pose.position.y = radius*cos(theta);
        pose_uav6.pose.position.z = 2;
*/
        local_pos_pub_uav1.publish(pose_uav1);
        local_pos_pub_uav2.publish(pose_uav2);
        local_pos_pub_uav3.publish(pose_uav3);
        local_pos_pub_uav4.publish(pose_uav4);
        local_pos_pub_uav5.publish(pose_uav5);
        local_pos_pub_uav6.publish(pose_uav6);
        
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}

