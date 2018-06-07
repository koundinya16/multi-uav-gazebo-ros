#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseStamped.h>


#include <cmath>

geometry_msgs::PoseStamped current_pose;

void position_cb(const geometry_msgs::PoseStamped::ConstPtr& pose)
{
    current_pose = *pose;
}



int main( int argc, char** argv )
{
  ros::init(argc, argv, "vis_node");
  ros::NodeHandle n;

  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
  ros::Subscriber position_sub =n.subscribe<geometry_msgs::PoseStamped>("/uav1/mavros/local_position/pose",10,position_cb);

  ros::Rate r(30);

 
  visualization_msgs::Marker points;
  
  points.header.frame_id ="/my_frame"; 
  points.ns  = "vis_node";

  points.action = visualization_msgs::Marker::ADD;
  points.type = visualization_msgs::Marker::POINTS;

  points.scale.x = 0.2;
  points.scale.y = 0.2;

  points.pose.orientation.w = 1.0;

  points.color.g = 1.0f;
  points.color.a = 1.0;
  points.id = 0;


  geometry_msgs::Point p;

  while (ros::ok())
  {
    
    points.header.stamp = ros::Time::now();

      
    p.x = current_pose.pose.position.x;
    p.y = current_pose.pose.position.y;
    p.z = current_pose.pose.position.z;

    points.points.push_back(p);
    


    marker_pub.publish(points);

    ros::spinOnce();
    r.sleep();
  }


}