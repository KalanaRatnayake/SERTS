gaitPeriod = 2.0;

%---------------------front right leg---------------------------------%
%path described seperately for x,y, and z
x_rf =         [0.0525; 0.0525; 0.0525; 0.0525; 0.0525; 0.0525];
y_rf =         [0.5000; 0.5000; 0.4950; 0.4750; 0.4950; 0.5000];
z_rf =         [0.0500; -0.050; -0.050; 0.0000; 0.0500; 0.0500];
time_pos_rf =  [     0;   1.00;    1.1;    1.5;    1.9;    2.0];

%velocity described seperately for x,y, and z directions
Vx_rf =        [     0;      0;      0;      0;      0;      0;      0;      0;     0];
Vy_rf =        [     0;      0;      0;  -0.04;   -0.1;      0;    0.1;   0.04;     0];
Vz_rf =        [     0;   -0.2;      0;      0;  0.094;   0.25;  0.094;      0;     0];
time_velo_rf = [     0;    0.5;    1.0;    1.1;   1.25;    1.5;   1.75;   1.90;     2];

%force described seperately for x,y, and z directions
Fx_rf =            [     0;      0;      0;      0;      0];
Fy_rf =            [    95;     95;      0;      0;     95];
Fz_rf =            [   -48;    -48;      0;      0;    -48];
time_force_rf =    [     0;      1;   1.01;   1.99;    2.0];


%---------------------front left leg---------------------------------%
%path described seperately for x,y, and z
x_lf =         [-0.0525; -0.0525; -0.0525; -0.0525; -0.0525;  -0.0525];
y_lf =         [ 0.5000;  0.4950;  0.4750;  0.4950;  0.5000;   0.5000];
z_lf =         [ -0.050;  -0.050;  0.0000;  0.0500;  0.0500;  -0.0500];
time_pos_lf =  [   0.00;     0.1;     0.5;     0.9;     1.0;      2.0];

%velocity described seperately for x,y, and z directions
Vx_lf =        [     0;      0;      0;      0;      0;      0;     0;     0;    0];
Vy_lf =        [     0;  -0.04;   -0.1;      0;    0.1;   0.04;     0;     0;    0];
Vz_lf =        [     0;      0;  0.094;   0.25;  0.094;      0;     0;  -0.2;    0];
time_velo_lf = [     0;    0.1;   0.25;    0.5;   0.75;   0.90;     1;   1.5;  2.0];

%force described seperately for x,y, and z directions
Fx_lf =            [     0;      0;      0;      0;       0];
Fy_lf =            [    95;      0;      0;     95;      95];
Fz_lf =            [   -48;      0;      0;    -48;     -48];
time_force_lf =    [     0;   0.01;   0.99;    1.0;     2.0];

%---------------------back left leg---------------------------------%
%path described seperately for x,y, and z
x_lb =         [-0.0525; -0.0525; -0.0525; -0.0525; -0.0525; -0.0525];
y_lb =         [ 0.5000;  0.4950;  0.4750;  0.4950;  0.5000;  0.5000];
z_lb =         [  0.050;   0.050;  0.0000;  -0.050; -0.0500;   0.050];
time_pos_lb =  [   0.00;     0.1;     0.5;     0.9;     1.0;     2.0];

%velocity described seperately for x,y, and z directions
Vx_lb =        [     0;      0;      0;      0;      0;      0;     0;       0;     0];
Vy_lb =        [     0;  -0.04;   -0.1;      0;    0.1;   0.04;     0;       0;     0];
Vz_lb =        [     0;      0; -0.094;  -0.25; -0.094;      0;     0;     0.2;     0];
time_velo_lb = [     0;    0.1;   0.25;    0.5;   0.75;   0.90;     1;     1.5;   2.0];

%force described seperately for x,y, and z directions
Fx_lb =            [     0;      0;      0;      0;     0];
Fy_lb =            [    95;      0;      0;     95;    95];
Fz_lb =            [    48;      0;      0;     48;    48];
time_force_lb =    [     0;   0.01;   0.99;    1.0;   2.0];


%---------------------back right leg---------------------------------%
%path described seperately for x,y, and z
x_rb =         [ 0.0525;  0.0525;  0.0525;  0.0525;  0.0525;   0.0525];
y_rb =         [ 0.5000;  0.5000;  0.4950;  0.4750;  0.4950;   0.5000];
z_rb =         [-0.0500;   0.050;   0.050;  0.0000; -0.0500;  -0.0500];
time_pos_rb =  [      0;    1.00;     1.1;     1.5;     1.9;      2.0];

%velocity described seperately for x,y, and z directions
Vx_rb =        [     0;      0;      0;      0;      0;      0;      0;      0;     0];
Vy_rb =        [     0;      0;      0;  -0.04;   -0.1;      0;    0.1;   0.04;     0];
Vz_rb =        [     0;    0.2;      0;      0; -0.094;  -0.25; -0.094;      0;     0];
time_velo_rb = [     0;    0.5;    1.0;    1.1;   1.25;    1.5;   1.75;   1.90;     2];

%force described seperately for x,y, and z directions
Fx_rb =            [     0;      0;      0;      0;      0];
Fy_rb =            [    95;     95;      0;      0;     95];
Fz_rb =            [    48;     48;      0;      0;     48];
time_force_rb =    [     0;      1;   1.01;   1.99;    2.0];