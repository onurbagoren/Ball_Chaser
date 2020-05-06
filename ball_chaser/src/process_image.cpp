#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Moving robot");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    std::cout << srv.request << std::endl;

    if (client.call(srv))
    {
        ROS_ERROR("Failed to call service process_image"); //sus
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera


    int left = img.width / 3;
    int forward = img.width * 2 / 3;

    std::cout << "image w: " << img.width << std::endl;
    int ball_position;
    bool ball_found = false;

    //
    for( int i = 0; i < img.height * img.step; i+=3 ) 
    {
        if( img.data[i] == white_pixel && img.data[i+1] == white_pixel && img.data[i+2] == white_pixel )
        {
            ball_position = i % img.width;
            ball_found = true;
            std::cout << ball_position << std::endl;
            break;
        }
    }
    if (ball_found)
    {
        if( ball_position < left ){
            drive_robot(0.0, 1.0);
        }
        else if( ball_position < forward){
            drive_robot(1.0, 0.0);
        }
        else{
            drive_robot(0.0, -1.0);
        }
    }else{
        drive_robot(0.0, 0.0);
    }

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}