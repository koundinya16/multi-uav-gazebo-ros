
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iomanip>
#include <termios.h>
#include <stdio.h>

using namespace cv;
cv_bridge::CvImagePtr cv_ptr_uav1,cv_ptr_uav2,cv_ptr_uav3,cv_ptr_uav4,cv_ptr_uav5,cv_ptr_uav6;

void imageCb_uav1(const sensor_msgs::ImageConstPtr& msg)
{
    try
       {
         cv_ptr_uav1 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
       }
       catch (cv_bridge::Exception& e)
       {
         ROS_ERROR("cv_bridge exception: %s", e.what());
         return;
       }
}
void imageCb_uav3(const sensor_msgs::ImageConstPtr& msg)
{
    try
       {
         cv_ptr_uav3 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
       }
       catch (cv_bridge::Exception& e)
       {
         ROS_ERROR("cv_bridge exception: %s", e.what());
         return;
       }
}
void imageCb_uav4(const sensor_msgs::ImageConstPtr& msg)
{
    try
       {
         cv_ptr_uav4 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
       }
       catch (cv_bridge::Exception& e)
       {
         ROS_ERROR("cv_bridge exception: %s", e.what());
         return;
       }
}
void imageCb_uav5(const sensor_msgs::ImageConstPtr& msg)
{
    try
       {
         cv_ptr_uav5 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
       }
       catch (cv_bridge::Exception& e)
       {
         ROS_ERROR("cv_bridge exception: %s", e.what());
         return;
       }
}
void imageCb_uav6(const sensor_msgs::ImageConstPtr& msg)
{
    try
       {
         cv_ptr_uav6 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
       }
       catch (cv_bridge::Exception& e)
       {
         ROS_ERROR("cv_bridge exception: %s", e.what());
         return;
       }
}
void imageCb_uav2(const sensor_msgs::ImageConstPtr& msg)
{
    try
       {
         cv_ptr_uav2 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
       }
       catch (cv_bridge::Exception& e)
       {
         ROS_ERROR("cv_bridge exception: %s", e.what());
         return;
       }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "viewer");
    ros::NodeHandle nh;
    
    int inp_num=0;
    std::string uav_num="";
    std::cout<<"Enter UAV num :";
    std::cin>>inp_num;
    ROS_INFO("Displaying UAV-%d Image feed",inp_num); 

    image_transport::ImageTransport it_uav1(nh),it_uav2(nh),it_uav3(nh),it_uav4(nh),it_uav5(nh),it_uav6(nh),it(nh);
    image_transport::Subscriber image_sub_uav1,image_sub_uav2,image_sub_uav3,image_sub_uav4,image_sub_uav5,image_sub_uav6;
    //image_transport::Publisher image_pub;

    image_sub_uav1 = it_uav1.subscribe("/iris_1/camera_ground_cam/image_raw", 1,imageCb_uav1);
    image_sub_uav2 = it_uav2.subscribe("/iris_2/camera_ground_cam/image_raw", 1,imageCb_uav2);
    image_sub_uav3 = it_uav3.subscribe("/iris_3/camera_ground_cam/image_raw", 1,imageCb_uav3);
    image_sub_uav4 = it_uav4.subscribe("/iris_4/camera_ground_cam/image_raw", 1,imageCb_uav4);
    
    image_sub_uav5 = it_uav5.subscribe("/iris_5/camera_ground_cam/image_raw", 1,imageCb_uav5);
    image_sub_uav6 = it_uav6.subscribe("/iris_6/camera_ground_cam/image_raw", 1,imageCb_uav6);

    std::stringstream sstm;
    sstm << "UAV-"<<inp_num;
    uav_num=sstm.str();

      

//    image_pub = it.advertise("/swarm/combined_image",1);
std::string OPENCV_WINDOW = uav_num+" Image Feed"; 
    
    cv::namedWindow(OPENCV_WINDOW,0);
    cv::Mat frame_combined,frame_combined_1,frame_combined_2;
    
    ros::Rate rate(30.0);

    while(ros::ok())
    {
        ros::spinOnce();  
        
        if (inp_num==1)
          frame_combined=cv_ptr_uav1->image;
        if (inp_num==2)
          frame_combined=cv_ptr_uav2->image;
        if (inp_num==3)
          frame_combined=cv_ptr_uav3->image;
        if (inp_num==4)
          frame_combined=cv_ptr_uav4->image;
        if (inp_num==5)
          frame_combined=cv_ptr_uav5->image;
        if (inp_num==6)
          frame_combined=cv_ptr_uav6->image;
        cv::imshow(OPENCV_WINDOW, frame_combined);
        if(cv::waitKey(30)>= 0) break;

        //image_pub.publish(cv_ptr->toImageMsg());

        ros::spinOnce();    
       
        
        
    }
    return 0;
}