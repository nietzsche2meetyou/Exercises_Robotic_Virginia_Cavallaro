# Planning through move_group

In startup phase, it has been inserted in the current workspace the `fanuc_moveit_config` and `fanuc_description` packages from the previous exercise.

In workspace directory:

```bash
catkin build
source devel/setup.bash
roslaunch fanuc_move_group_interface ex4.launch
```

It has been used visual tool for an interactive code execution and for showing the result on `Rviz`. So, at first, it has been implemented the communication with `Rviz`, thanks to `MoveItVisualTools` object.

## Evaluate Trajectory

The task was to create a circular trajectory in the workspace, in front of robot. So:

- at first, it has been setted the circle parameters, considering also joint limits.
- the robot start position is setted in a way that its flange is already positioned on a path point.
- it has been created the orientation constraint, in a way that force the end effector orientation to be constant during the path execution
- The desired path is created using a for loop and an array for storing the computed waypoints that compose the circular path.
- The trajectory is created by `computeCartesianPath` method, offered by `MoveGroup`
- On this trajectory it has been setted the continuity constraints on velocity and acceleration, using `RobotState` and `RobotTrajectory`.
- It starts the simulation on `Rviz`, sending information on `DISPLAY_PATH_TOPIC`
- Finally, it publish joint space solution to plot in rqt_multiplot, using the configuration file in `config` folder
