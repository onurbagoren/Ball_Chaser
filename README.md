# Udacity Robotics Software Engineer Project 2
## Ball Chaser by Onur Bagoren

#### Clone repo into catkin workspace
- `$ git clone git@github.com:onurbagoren/Ball_Chaser .`
- `$ mv Ball_Chaser/* ~/catkin_ws/src`
- `$ cd ~/catkin_ws/`
- `$ catkin_make`

#### How to run:
- On one terminal run `source devel/setup.bash` and then `roslaunch my_robot world.launch`
This will open a window for RViz and Gazebo with the custom world, robot and white ball that the robot will chase.
- On another terminal, run `source devel/setup.bash` and then `roslaunch ball_chaser ball_chaser.launch`
This will make the robot chase after the white ball and stop once it is close enough.