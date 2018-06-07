#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <cmath>
#include <visualization_msgs/Marker.h>

double radius = 10;
double min_distance=0.1;
double altitude=5;
double num=100;

double angular_velocity=M_PI/2;

int main( int argc, char** argv )
{
  ros::init(argc, argv, "trajectory_generator");
  ros::NodeHandle n;

  ros::Publisher trajectory_pub = n.advertise<geometry_msgs::Pose>("/trajectory/waypoints", 10);
 //ros::Publisher marker_setpoint_pub = n.advertise<visualization_msgs::Marker>("visualization_marker_setpoint", 10);


  ros::Rate r(10);

  geometry_msgs::Pose waypoint;
  waypoint.position.x=0;
  waypoint.position.y=0;
  waypoint.position.z = altitude;
  double theta=0;
  int i=1;
  int a;
  double k=0;
  std::cout<<"1-circle 2-helix 3-line3D 4-line2D";
  std::cin>>a;
  
  if(a==1)
  ROS_INFO("Streaming waypoints for circle");

  if(a==2)
  ROS_INFO("Streaming waypoints for  helix");

  if(a==3)
  ROS_INFO("Streaming waypoints for 3D straight line");

  if(a==4)
  ROS_INFO("Streaming waypoints for 2D straight line");
  
   ros::Time start_time = ros::Time::now();

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
     points.id = 1;


    geometry_msgs::Point p;
    
    float m=0.1;
    if(a==2 || a==3)
      k=0.1;
     waypoint.position.z = altitude;
  
  while (ros::ok())
  {
    //theta=angular_velocity*((ros::Time::now()-start_time).toSec());
    
    if(a==1 || a==2)
   {
    waypoint.position.x = radius*sin(theta);
    waypoint.position.y = radius*cos(theta);
   }
   else
   {
    waypoint.position.x +=m;
    waypoint.position.y +=m; 
   }

    waypoint.position.z += k;

    trajectory_pub.publish(waypoint);
    i++;
    theta=theta+(2*M_PI/num);
    
    points.header.stamp = ros::Time::now();

      
    p.x = waypoint.position.x;
    p.y = waypoint.position.y;
    p.z = waypoint.position.z;

    points.points.push_back(p);
    


    //marker_setpoint_pub.publish(points);

    ros::spinOnce();
    r.sleep();
  }

}
