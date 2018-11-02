L1 = 0.1225;
L2 = 0.2000;
L3 = -0.0625;

%DH Factors
links(1) = Link([    0       0     0    0 ], 'standard');
links(2) = Link([    0       L3    L1   -pi/2], 'standard');
links(3) = Link([    0       0     L2   pi   ], 'standard');

%a single leg
leg = SerialLink(links, 'name', 'leg', 'offset', [0 0 0]);