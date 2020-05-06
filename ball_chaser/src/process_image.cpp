#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;
bool too_close = false;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv))
    {
        ROS_ERROR("Failed to call service process_image"); //sus
    }
}

float vector_average( std::vector<int> v )
{
    int size_of = v.size();
    float total = 0.0;
    for( int i = 0; i < size_of; i++)
    {
        total += v.at(i);
    }
    return total / size_of;
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera


    int leftest = img.width / 6;
    int left = img.width / 3;

    int right = img.width * 2 / 3;
    int rightest = img.width * 5 / 6;

    bool ball_found = false;

    // TODO: implement so that the robot stops if within a certain distance of the ball
    // Get area of the ball/frame size. if >65%, stop robot.
    // Find ball position by not the first pixel, but the middle point of the ball

    std::vector<int> values;

    for( int i = 0; i < img.height; i ++)
    {
        // At every point i, which represents the height of the image, go through (r, g, b) of the rows, incrementing by three
        for ( int j = 0; j < img.step; j+=3)
        {
            int position = i * img.step + j;
            if( img.data[position] == white_pixel && img.data[position+1] == white_pixel && img.data[position+2] == white_pixel ){
                ball_found = true;
                values.push_back(position%img.width);
            }

        }

    }

    float average_value = vector_average(values);
    float total_white_pixels = float(values.size());
    float ratio = total_white_pixels / (img.height * img.width);

    if(ratio > 0.3)
    {
        too_close = true;
    }else{
        too_close = false;
    }

    if(!too_close)
    {
        if (ball_found)
        {
            if( average_value < leftest ){
                drive_robot(0.0, 0.5);
            }
            else if( average_value < left){
                drive_robot(0.075, 0.25);
            }
            else if (average_value > rightest){
                drive_robot(0.0, -0.5);
            }
            else if (average_value > right )
            {
                drive_robot(0.075, -0.25);
            }else{
                drive_robot(0.5, 0.0);
            }
        }else{
            // Search for ball by rotating in place
            drive_robot(0.0, 0.5);
        }
    }else{
        ROS_INFO_STREAM("Robot is very close to the ball. Robot stopped.");
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