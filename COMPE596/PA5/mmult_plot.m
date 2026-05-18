close all
clc
data = load('data.txt', '-ascii');
threads = data(:,1);
runtime_basic = data(:,2);
runtime_tiled = data(:,3);


figure;
hold on;
colors = lines(2); 

plot(threads, runtime_basic, '-o', 'Color', [0,0,1.0], 'DisplayName', sprintf('Basic'));
plot(threads, runtime_tiled, '-o', 'Color', [1.0,0,0], 'DisplayName', sprintf('Tiled'));

xlabel('n = m = 2^N');
ylabel('Run Time');
title('Runtime vs Different Sized Matrices');
set(gca, 'YScale', 'log');
set(gca, 'XScale', 'linear');
set(gca, 'XTick', threads);

legend('Location', 'northwest');
grid on;
hold off;
