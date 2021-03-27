# Kinematic

In startup phase, it has been inserted in the current workspace the `fanuc_moveit_config` and `fanuc_description` packages from the previous exercise.

# Forward kinematic

For evaluating a custom forward kinematic, it has been created a custom service, `forward_kinematic_srv`. In particular, it contains in request field a `RobotState` instance and in response field a `Pose` instance.

The server that manages requests evaluates the FK thanks to `getGlobalLinkTransform` method, offered by `RobotState` object.

In workspace directory:

```bash
catkin build kinematics_action_msgs forward_kinematic_srv
catkin build
source devel/setup.bash
cd launch
roslaunch fk_launch.launch
```

In another terminal:

```bash
source devel/setup.bash
rosrun forward_kinematics forward_kinematic_client
```

# Inverse Kinematic

The IK task has been implemented throught Ros action. In particular, the `GetInverseKinematicsSolution.action` is composed by `Pose` goal, `RobotState` array for result and `RobotState` instance for feedback.

It has been istantiated a kinematic solver, basing on the configuration previously setted in `moveit_setup_assistant`. For forcing the algorithm to finish, it has been setted a fixed number of iterations, considering also the maximum amount of time for finding a single solution, setted, as previous, in `moveit_setup_assistant`.

For each iteration:

- it has been generated a randomic seed, for maximizing the probability of finding new solutions for IK problem, using `generateSeedState_` function.
- the process of finding a single solution is performed thanks to `getPositionIK`, offered by solver
- if solution is retured, it's normalized thank to `normalizeJointPositions`
  - for each joint, position angle found is represented in the interval [-pi,pi]
- check if the solution is new with `isSolutionNew` method, the checking is performed basing on the `shortest_angular_distance`
- if the solution is new, it's added to the array of solutions and it's sent to client as feedback
- At the end, the state of action is setted on `SUCCEEDED` and the server send all the solutions found as result

It's possibile to launch two different configurations. Both ones have been generated thanks to moveit setup assistant, but the first URDF used does not enforce bounds on joints, while in the second one it has been considered joints with bounds.

No Limit config:

```bash
roslaunch ik_launch.launch urdf_file:="rm-20ia-no_limit.urdf" moveit_config:="fanuc_moveit_config_no_limit" group:="fanuc_arm_no_limit"
```

Limit config:

```bash
roslaunch ik_launch.launch
```
