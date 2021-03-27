# Fanuc Robot

It has been implemented the URDF for Fanuc Robot. In particular, the URDF file (`rm-10ia.urdf`) is in `robot` directory: in this file it has been inserted each joint with its limit and frames have been inserted with `Denavit-Hartenberg` convention (kinematic chain is inserted in `doc` directory).
For launching the simulation, starting from the workspace directory:

```bash
cd src/fanuc_description
roslaunch fanuc_description display.launch model:=robot/rm-10ia.urdf
```

For generating the package `fanuc_moveit_config`, it has been used the `moveit_setup_assistant`:

```bash
roslaunch moveit_setup_assistant setup_assistant.launch
```

In the start window, select "create new moveit configuration package" for loading the urdf file previously created.
It has been generated the self collisions matrix (`ACM`) and in virtual joints window it has been inserted a `virtual joint` because it's necessary a link between the first robot link (`base_link`) and an external frame of reference. It has been created a `planning group` and a home pose for fanuc (http://docs.ros.org/en/melodic/api/moveit_tutorials/html/doc/setup_assistant/setup_assistant_tutorial.html, for more detalis). After this process it has been generated the new package `fanuc_moveit_config`.

For loading it in Rviz, in workspace directory:

```bash
catkin build
source devel/setup.bash
cd src/fanuc_moveit_config/launch
roslaunch demo.launch
```

In `demo.launch` file, it has been modified the parameter that allows to use GUI.

# TF-Parser

Tf-parser node reads from `/tf` topic, using `TransformListener` object. In particular, it uses `lookupTransform` method for evaluating the direct kinematic of each joint to flange.

So, it prints the result for the traslation and orientation (with Rotation Matrix, Axes Angle and Euler Angles representations)
