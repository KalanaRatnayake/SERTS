gaitPeriod = 2.0;

%path described seperately for x,y, and z
x =         [0.0525; 0.0525; 0.0525; 0.0525; 0.0525; 0.0525];
y =         [0.4000; 0.4000; 0.3950; 0.3750; 0.3950; 0.4000];
z =         [0.0500; -0.050; -0.050; 0.0000; 0.0500; 0.0500];
time_pos =  [     0;   1.00;    1.1;    1.5;    1.9;    2.0];

%velocity described seperately for x,y, and z directions
Vx =        [     0;      0;      0;      0;      0;      0;      0;      0;     0];
Vy =        [     0;      0;      0;  -0.04;   -0.1;      0;    0.1;   0.04;     0];
Vz =        [     0;   -0.2;      0;      0;  0.094;   0.25;  0.094;      0;     0];
time_velo = [     0;    0.5;    1.0;    1.1;   1.25;    1.5;   1.75;   1.90;     2];

%force described seperately for x,y, and z directions
Fx =            [     0;      0;      0;      0;      0];
Fy =            [    95;     95;      0;      0;     95];
Fz =            [   -48;    -48;      0;      0;    -48];
time_force =    [     0;      1;   1.01;   1.99;    2.0];