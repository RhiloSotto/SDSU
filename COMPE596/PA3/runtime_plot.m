close all
clc
data = load('data.txt', '-ascii');
threads = data(:,1);
runtime = data(:,5);
error = data(:,3);

figure;
plot(threads, runtime, 'o-', 'LineWidth', 2);
set(gca, 'XTick', threads);
set(gca, 'XTickLabel', arrayfun(@(x) sprintf('2^{%d}', log2(x)), threads, 'UniformOutput', false));
xlabel('Hardware Threads', 'FontSize', 12);
ylabel('Elapsed (wall clock) time, s', 'FontSize', 12);
title('Programming Assignment 03 (OpenMP)', 'FontSize', 14);
set(gcf,'Color','white');
grid on;
ax = gca;
ax.YMinorGrid = 'on';
ax.XMinorGrid = 'on';
figure(1);
