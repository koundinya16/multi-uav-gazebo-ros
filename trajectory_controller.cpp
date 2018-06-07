#include <iostream>
#include <vector>
#include <math.h>

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <visualization_msgs/Marker.h>

mavros_msgs::State current_state;
geometry_msgs::Pose current_waypoint;
geometry_msgs::PoseStamped current_position;

int speed=10;  // m/s


geometry_msgs::PoseArray trajectory_waypoints;

double distance(geometry_msgs::PoseStamped p1 , geometry_msgs::Pose p2)
{
   return (sqrt(pow(p1.pose.position.x-p2.position.x,2)+pow(p1.pose.position.y-p2.position.y,2)+pow(p1.pose.position.y-p2.position.y,2)));
}
void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
    current_state = *msg;
}

void position_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    current_position = *msg;
}

void trajectory_cb(const geometry_msgs::Pose::ConstPtr& msg)
{
    current_waypoint = *msg;
    trajectory_waypoints.poses.push_back(current_waypoint);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "trajectory_controller");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/uav1/mavros/state", 10, state_cb);
    ros::Subscriber trajectory_sub = nh.subscribe<geometry_msgs::Pose>("/trajectory/waypoints", 10, trajectory_cb);
    ros::Subscriber position_sub = nh.subscribe<geometry_msgs::PoseStamped>("/uav1/mavros/local_position/pose", 10, position_cb);

    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("/uav1/mavros/setpoint_position/local", 10);

    ros::Publisher vel_pub = nh.advertise<geometry_msgs::TwistStamped>("/uav1/mavros/setpoint_velocity/cmd_vel", 10);
    ros::Publisher accel_pub = nh.advertise<geometry_msgs::Vector3Stamped>("/uav1/mavros/setpoint_accel/accel", 10);

    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/uav1/mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/uav1/mavros/set_mode");

    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 10);
    double look_ahead=10;
    int choice=1;
    std::cout<<"Select Controller 1-Pure Pursuit. 2-PID : ";
    std::cin>>choice;
    std::cout<<"Enter look-ahead : ";
    std::cin>>look_ahead;
    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(50.0);

    // wait for FCU connection
    while(ros::ok() && !current_state.connected)
    {
        ros::spinOnce();
        rate.sleep();
    }
     
     visualization_msgs::Marker points,setpoints;
     points.header.frame_id =setpoints.header.frame_id ="/my_frame"; 
     points.ns=points.ns = "vis_node";

     points.action =setpoints.action = visualization_msgs::Marker::ADD;
     points.type = visualization_msgs::Marker::LINE_STRIP;
     setpoints.type = visualization_msgs::Marker::POINTS;

     points.scale.x = 0.1;
     points.scale.y = 0.1;

     setpoints.scale.x = 0.2;
     setpoints.scale.y = 0.2;



     points.pose.orientation.w = 1.0;
     setpoints.pose.orientation.w = 1.0;

     points.color.r = 1.0;
     points.color.a = 1.0;

     setpoints.color.g = 1.0f;
     setpoints.color.a = 1.0;
     
     points.id = 1;
     setpoints.id = 1;


    geometry_msgs::Point p;


    geometry_msgs::PoseStamped pose;
    geometry_msgs::TwistStamped velocity;
    geometry_msgs::Vector3Stamped accel;

    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = 3;

    velocity.twist.linear.x=0;
    velocity.twist.linear.y=0;
    velocity.twist.linear.z=0;

    accel.vector.x=0.1;
    accel.vector.y=0;
    accel.vector.z=0.568;

    //send a few setpoints before starting
    for(int j = 100; ros::ok() && j > 0; --j)
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

    while(ros::ok())
    {
        if( current_state.mode != "OFFBOARD" &&(ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if( set_mode_client.call(offb_set_mode) &&offb_set_mode.response.mode_sent)
            {
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        } 
        else 
        {
            if( !current_state.armed &&(ros::Time::now() - last_request > ros::Duration(5.0)))
            {
                if( arming_client.call(arm_cmd) &&arm_cmd.response.success)
                {
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }

        local_pos_pub.publish(pose);

        ros::spinOnce();
        rate.sleep();

        if(current_state.mode=="OFFBOARD" && current_state.armed)
            break;
    }

    ROS_INFO("Waiting for trajectory.........");

    while(ros::ok())
    {
        local_pos_pub.publish(pose);
        //accel_pub.publish(accel);
        if(trajectory_waypoints.poses.size()>=10)
           break;
        ros::spinOnce();
        rate.sleep();
 
    }
   // std::cout<<"Speed? : ";
     //std::cin>>speed;
    speed=10;
    ROS_INFO("Starting Control Loop.....");
    int i=0;

    double v_i,v_j,v_k,v,error,prev_error=0,pid_speed;
    double t=0;
    double kp=10,ki=0.1,kd=1;
    ros::Time pid_t2,pid_t1; 
    while(ros::ok())
    {
        if(i<trajectory_waypoints.poses.size()-1)
        {
         v_i=(trajectory_waypoints.poses[i].position.x-current_position.pose.position.x);
         v_j=(trajectory_waypoints.poses[i].position.y-current_position.pose.position.y);
         v_k=(trajectory_waypoints.poses[i].position.z-current_position.pose.position.z);
         
         v=sqrt(pow(v_i,2)+pow(v_j,2)+pow(v_k,2));

        if(choice==1)
        {
         velocity.twist.linear.x=(v_i*speed)/v;
         velocity.twist.linear.y=(v_j*speed)/v;
         velocity.twist.linear.z=(v_k*speed)/v;
         }

         else
         {
            pid_t1=ros::Time::now();
            error=distance(current_position,trajectory_waypoints.poses[i]);
            t=(pid_t1-pid_t2).toSec();
            pid_speed=kp*error + (kd*(error-prev_error)/t) + (ki*0.5*t*(error+prev_error));
            if(pid_speed<0)
            pid_speed=-pid_speed;

            velocity.twist.linear.x=(v_i*pid_speed)/v;
            velocity.twist.linear.y=(v_j*pid_speed)/v;
            velocity.twist.linear.z=(v_k*pid_speed)/v;

            prev_error=error;
            pid_t2=pid_t1;

         }
        }

        else
        {
         velocity.twist.linear.x=0;
         velocity.twist.linear.y=0;
         velocity.twist.linear.z=0;
        }

        std::cout<<"i:"<<i<<" N:"<<trajectory_waypoints.poses.size()<<" V-x:"<<velocity.twist.linear.x<<" V-y:"<<velocity.twist.linear.y<<" V-z:"<<velocity.twist.linear.z;


        vel_pub.publish(velocity);
        //pose.pose.position.x=trajectory_waypoints.poses[i].position.x;
        //pose.pose.position.y=trajectory_waypoints.poses[i].position.y;
        //pose.pose.position.z=trajectory_waypoints.poses[i].position.z;
        //local_pos_pub.publish(pose);
        points.header.stamp = ros::Time::now();

      
        p.x = current_position.pose.position.x;
        p.y = current_position.pose.position.y;
        p.z = current_position.pose.position.z;

        points.points.push_back(p);

        p.x = trajectory_waypoints.poses[i].position.x;
        p.y = trajectory_waypoints.poses[i].position.y;
        p.z = trajectory_waypoints.poses[i].position.z;

        setpoints.points.push_back(p);
    


        marker_pub.publish(points);
        marker_pub.publish(setpoints);

        ros::spinOnce();
        
        std::cout<<" Setpoint Error:"<<distance(current_position,trajectory_waypoints.poses[i])<<std::endl;
        
        
        if(distance(current_position,trajectory_waypoints.poses[i])<look_ahead && i<trajectory_waypoints.poses.size()-1)
        i++;
        


        rate.sleep();
    }

    return 0;
}
