
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/stitching.hpp"

#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

static const std::string OPENCV_WINDOW = "stitcher";

Stitcher::Mode mode = Stitcher::PANORAMA;
vector<Mat> imgs;

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
    ros::init(argc, argv, "stitcher");
    ros::NodeHandle nh;

    image_transport::ImageTransport it_uav1(nh),it_uav2(nh),it_uav3(nh),it_uav4(nh),it_uav5(nh),it_uav6(nh),it(nh);
    image_transport::Subscriber image_sub_uav1,image_sub_uav2,image_sub_uav3,image_sub_uav4,image_sub_uav5,image_sub_uav6;
    image_transport::Publisher image_pub;

    image_sub_uav1 = it_uav1.subscribe("/iris_1/camera_ground_cam/image_raw", 1,imageCb_uav1);
    image_sub_uav2 = it_uav2.subscribe("/iris_2/camera_ground_cam/image_raw", 1,imageCb_uav2);
    image_sub_uav3 = it_uav3.subscribe("/iris_3/camera_ground_cam/image_raw", 1,imageCb_uav3);
    image_sub_uav4 = it_uav4.subscribe("/iris_4/camera_ground_cam/image_raw", 1,imageCb_uav4);
    
    /*
    image_sub_uav5 = it_uav5.subscribe("/iris_5/camera_ground_cam/image_raw", 1,imageCb_uav5);
    image_sub_uav6 = it_uav6.subscribe("/iris_6/camera_ground_cam/image_raw", 1,imageCb_uav6);
    */


//    image_pub = it.advertise("/swarm/combined_image",1); 
    
    cv::namedWindow(OPENCV_WINDOW,0);
    cv::Mat frame_combined,frame_combined_1,frame_combined_2;
    ros::Rate rate(30.0);

     Mat pano;
     //Mat img1=imread("/home/koundinya/garuda_ws/src/ddp_v1/src/img04.jpg",CV_LOAD_IMAGE_COLOR);
     //Mat img2=imread("/home/koundinya/garuda_ws/src/ddp_v1/src/img07.jpg",CV_LOAD_IMAGE_COLOR);
     Ptr<Stitcher> stitcher = Stitcher::create(mode, false);
     int i=1;
    while(ros::ok())
    {
         ros::spinOnce();
        // imgs.push_back(img1);
        // imgs.push_back(img2);
        imgs.push_back(cv_ptr_uav1->image);
        //imgs.push_back(cv_ptr_uav2->image);
        imgs.push_back(cv_ptr_uav3->image);
       // imgs.push_back(cv_ptr_uav4->image);


       Stitcher::Status status = stitcher->stitch(imgs, pano);
 
      if (status != Stitcher::OK)
      {
        // Check if images could not be stiched
        // status is OK if images are stiched successfully
        cout <<i<< ": Can't stitch images\n";
       // return -1;
      }
    
      else
      {
       cout <<i<< ":stitching successfull\n";
      imwrite("/home/koundinya/garuda_ws/src/ddp_v1/src/result.jpg", pano);
      imshow(OPENCV_WINDOW,pano);
       waitKey(0);
      }

      ros::spinOnce();    
      rate.sleep();
      i++; 
        
   }
    return 0;
}