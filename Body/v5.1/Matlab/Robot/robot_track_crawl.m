gaitPeriod = 2.0;

%---------------------front right leg---------------------------------%
%path described seperately for x,y, and z
x_rf =         [0.0525; 0.0525; 0.0525; 0.0525; 0.0525; 0.0525];
y_rf =         [0.5000; 0.5000; 0.4800; 0.4800; 0.5000; 0.5000];
z_rf =         [-0.020; -0.050; -0.025; 0.0250; 0.0500; -0.020];
time_pos_rf =  [     0;   1.00;   1.25;   1.75;    2.0;    4.0];

%velocity described seperately for x,y, and z directions
Vx_rf =        [     0;      0;      0;      0;      0;      0;      0;      0;      0;      0;     0];
Vy_rf =        [     0;      0;      0;  -0.16;      0;      0;      0;   0.16;      0;      0;     0];
Vz_rf =        [     0; -0.060;      0;   0.05;    0.1;    0.2;    0.1;   0.05;      0;  -0.07;     0];
time_velo_rf = [     0;    0.5;      1;  1.125;   1.25;    1.5;   1.75;  1.875;    2.0;    3.0;   4.0];

%force described seperately for x,y, and z directions
Fx_rf =            [     0;      0;      0;      0;      0;     0];
Fy_rf =            [    63;     63;      0;      0;     63;    63];
Fz_rf =            [   -32;    -32;      0;      0;    -32;   -32];
time_force_rf =    [     0;      1;   1.01;   1.99;    2.0;   4.0];


%---------------------front left leg---------------------------------%
%path described seperately for x,y, and z
x_lf =         [-0.0525; -0.0525; -0.0525; -0.0525; -0.0525];
y_lf =         [ 0.5000;  0.5000;  0.4800;  0.4800;  0.5000];
z_lf =         [  0.050;  -0.050;  -0.025;   0.025;   0.050];
time_pos_lf =  [   0.00;     3.0;    3.25;    3.75;     4.0];

%velocity described seperately for x,y, and z directions
Vx_lf =        [     0;      0;      0;      0;      0;      0;      0;     0;     0;     0;     0];
Vy_lf =        [     0;      0;      0;      0;      0;  -0.16;      0;     0;     0;  0.16;     0];
Vz_lf =        [     0;  -0.07;      0;  -0.06;      0;   0.05;    0.1;   0.2;   0.1;  0.05;     0];
time_velo_lf = [     0;    1.0;    2.0;    2.5;    3.0;  3.125;   3.25;   3.5;  3.75; 3.875;   4.0];

%force described seperately for x,y, and z directions
Fx_lf =            [     0;      0;      0;      0;       0];
Fy_lf =            [    63;     63;      0;      0;      63];
Fz_lf =            [   -32;    -32;      0;      0;     -32];
time_force_lf =    [     0;    3.0;   3.01;   3.99;     4.0];

%---------------------back right leg---------------------------------%
%path described seperately for x,y, and z
x_lb =         [-0.0525; -0.0525; -0.0525; -0.0525; -0.0525];
y_lb =         [ 0.5000;  0.4800;  0.4800;  0.5000;  0.5000];
z_lb =         [  0.050;   0.025;  -0.025;  -0.050;   0.050];
time_pos_lb =  [   0.00;    0.25;    0.75;     1.0;     4.0];

%velocity described seperately for x,y, and z directions
Vx_lb =        [     0;      0;      0;      0;      0;      0;     0;       0;     0];
Vy_lb =        [     0;  -0.16;      0;      0;      0;   0.16;     0;       0;     0];
Vz_lb =        [     0;  -0.05;   -0.1;   -0.2;   -0.1;  -0.05;     0;   0.066;     0];
time_velo_lb = [     0;  0.125;   0.25;    0.5;   0.75;  0.875;     1;     2.5;   4.0];

%force described seperately for x,y, and z directions
Fx_lb =            [     0;      0;      0;      0;     0];
Fy_lb =            [    63;      0;      0;     63;    63];
Fz_lb =            [    32;      0;      0;     32;    32];
time_force_lb =    [     0;   0.01;   0.99;    1.0;   4.0];


%---------------------back left leg---------------------------------%
%path described seperately for x,y, and z
x_rb =         [ 0.0525;  0.0525;  0.0525;  0.0525;  0.0525;   0.0525];
y_rb =         [ 0.5000;  0.5000;  0.4800;  0.4800;  0.5000;   0.5000];
z_rb =         [ -0.015;   0.050;   0.025;  -0.025; -0.0500;   -0.015];
time_pos_rb =  [      0;    2.00;    2.25;    2.75;     3.0;      4.0];

%velocity described seperately for x,y, and z directions
Vx_rb =        [     0;      0;      0;      0;      0;      0;      0;      0;     0;     0;    0];
Vy_rb =        [     0;      0;      0;  -0.16;      0;      0;      0;   0.16;     0;     0;    0];
Vz_rb =        [     0;  0.065;      0;  -0.05;   -0.1;   -0.2;   -0.1;  -0.05;     0;  0.07;    0];
time_velo_rb = [     0;    1.0;    2.0;  2.125;   2.25;    2.5;   2.75;  2.875;     3;   3.5;    4];

%force described seperately for x,y, and z directions
Fx_rb =            [     0;      0;      0;      0;      0;      0];
Fy_rb =            [    63;     63;      0;      0;     63;     63];
Fz_rb =            [    32;     32;      0;      0;     32;     32];
time_force_rb =    [     0;      2;   2.01;   2.99;    3.0;    4.0];