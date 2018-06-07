#include <stdio.h>
#include <math.h>
#include <iostream>
#include <termios.h>
#include <iomanip>

#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>



int main(int argc, char **argv)
{
    ros::init(argc, argv, "swarm_teleop_controller");
    ros::NodeHandle nh;


    
  
    ros::Publisher uav_swarm_pos_pub = nh.advertise<geometry_msgs::Pose> ("/swarm_pos", 100);
    
    ros::Rate rate(30.0);
    
    geometry_msgs::Pose uav_swarm_pose;

    uav_swarm_pose.position.x = 0;
    uav_swarm_pose.position.y = 0;
    uav_swarm_pose.position.z = 5;

    uav_swarm_pose.orientation.x = 5;
    uav_swarm_pose.orientation.y = 0;    
    
    std::cout<<"   ____________________________________"<<std::endl;
    std::cout<<"  |  U: Unlock/TakeOff   L: Lock/Land  |"<<std::endl;
    std::cout<<"   ____________________________________"<<std::endl;
    std::cout<<"  | W: Forward     A: Left    D: Right |"<<std::endl;     
    std::cout<<"  | S: Backs       Z: Up      X: Down  |"<<std::endl;
    std::cout<<"  --------------------------------------"<<std::endl;
    std::cout<<"  | F :Focus       G: Diverge          |"<<std::endl;
    std::cout<<"  -------------------------------------"<<std::endl; 
    std::cout<<"  | q/e: Increase/Decrease Radius      |"<<std::endl;
    std::cout<<"  | r/t: Increase/Decrease control res.|"<<std::endl;
    std::cout<<"   ____________________________________"<<std::endl<<std::endl;
    
    float res=0.2;

    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);


    while(ros::ok())
    {
       char c = std::cin.get();

        if(c=='w')
           uav_swarm_pose.position.x += res;
        if(c=='d')
           uav_swarm_pose.position.y += res; 
        if(c=='z')
           uav_swarm_pose.position.z += res;
       if(c=='s')
           uav_swarm_pose.position.x -= res;
       if(c=='a')
           uav_swarm_pose.position.y -= res;
       if(c=='x')
           uav_swarm_pose.position.z -= res;

       if(c=='q')
           uav_swarm_pose.orientation.x += res;
       if(c=='e')
           uav_swarm_pose.orientation.x -= res;

         if(c=='u')
         {
           uav_swarm_pose.orientation.y = 0;
           std::cout<<"Unlocked"<<std::endl;
         }
          if(c=='l')
          {
           uav_swarm_pose.orientation.y = 1;
           std::cout<<"Position Lock"<<std::endl;
          }

       if(c=='f')
       {
           //uav_swarm_pose.position.z -= 16*res;
           uav_swarm_pose.orientation.x -=18*res;
        }

        if(c=='g')
       {
           //uav_swarm_pose.position.z += 16*res;
           uav_swarm_pose.orientation.x +=18*res;
        }
       
       if(c=='r')
           res+=0.1;
       if(c=='t')
           res-=0.1;

        uav_swarm_pos_pub.publish(uav_swarm_pose);
        
        printf("\33[2K\r");
        std::cout<<" X :"<<uav_swarm_pose.position.x<<"  Y :"<<uav_swarm_pose.position.y<<" Z :"<<uav_swarm_pose.position.z<<"      Radius :"<<uav_swarm_pose.orientation.x;

        
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}

