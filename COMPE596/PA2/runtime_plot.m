close all
clc
data = load('data.txt', '-ascii');
threads = data(:,1);
nodes = data(:,2);
runtime = data(:,3);

unique_threads = unique(threads);
unique_nodes = unique(nodes);

figure;
hold on;
colors = lines(length(unique_threads)); 

thread_mask = threads == 1;
plot(nodes(thread_mask), runtime(thread_mask), '-o', 'Color', [0,0,0], 'DisplayName', sprintf('Serial'));
idx = find(thread_mask, 1, 'first');
text(nodes(idx), runtime(idx), 'serial', 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left');
for i = 2:length(unique_threads)
    thread_mask = threads == unique_threads(i);
    plot(nodes(thread_mask), runtime(thread_mask), '-o', 'Color', colors(i, :), 'DisplayName', sprintf('%d Threads', unique_threads(i)));
    idx = find(thread_mask, 1, 'first');
    text(nodes(idx), runtime(idx), sprintf('%d Threads', unique_threads(i)), 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'left');
end

xlabel('2^N # of Nodes');
ylabel('Run Time');
title('Runtime vs Number of Nodes with Varying Thread Counts');
set(gca, 'YScale', 'log');
set(gca, 'XScale', 'log');
set(gca, 'XTick', unique_nodes);
set(gca, 'XTickLabel', arrayfun(@(x) sprintf('2^{%d}', log2(x)), unique_nodes, 'UniformOutput', false));

legend('Location', 'northwest');
grid on;
hold off;
