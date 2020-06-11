%-- This file contains variables required by robot_joint_controlled.slx --%

%-- Initial parameters that depends on the position of the leg --%
Left = 1;
Right = -1;

LF_RH = -1;
RF_LH =  1;

%-- Parameters that are common to legs and not relevent to path planner --%

width = 0.375;
weight = 19;

k_swing = [02000,     0,       0;
                0, 2000,       0; 
                0,     0,  10000];
           
c_swing = [100,   0,    0;
             0, 100,    0; 
             0,   0, 500];
         
k_stance = [    0,    0,       0;
                0,    0,       0; 
                0,    0,   20000];
           
c_stance = [30,   0,    0;
              0, 30,    0; 
              0,   0, 3000];
          
k_roll = 700;
c_roll = 50;

k_yaw = 300;
c_yaw =  30;

k_x = 0.03;
k_y = 0.03;

stance_time = 0.2;
time_sequence = [0 0.2 0.4];
value_sequence = [0 0.2 0];

%-- Parameters that are relevent to path planner (considered fixed)--%

desired_yaw = 0;
desired_body_height = 0.6;
lift_height = 0.05;
desired_body_x_velocity = 0;
desired_body_y_velocity = 1;