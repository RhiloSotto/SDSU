close all; clear; clc;

data = readmatrix('data.csv');

density = data(:,1);
rocSPARSE = data(:,5);
rocBLAS   = data(:,6);

figure;
loglog(density, rocSPARSE, '-o', 'LineWidth', 2); hold on;
loglog(density, rocBLAS, '-s', 'LineWidth', 2);

xlabel('Density');
ylabel('Time (seconds)');
title('rocSPARSE vs rocBLAS');
legend('rocSPARSE','rocBLAS','Location','northwest');
grid on;