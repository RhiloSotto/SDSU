function plot_surface(filename)
    if nargin < 1
        filename = 'results.txt';
    end

    fid = fopen(filename, 'r');
    if fid < 0
        error('Could not open %s', filename);
    end

    M_vals = [];
    T_vals = [];
    times = [];

    while ~feof(fid)
        line = fgetl(fid);

        tokens = regexp(line, ...
            'M=(\d+)\s+threads=(\d+).*time=([0-9.eE+-]+)', ...
            'tokens');

        if ~isempty(tokens)
            tok = tokens{1};
            M_vals(end+1) = str2double(tok{1});
            T_vals(end+1) = str2double(tok{2});
            times(end+1) = str2double(tok{3});
        end
    end

    fclose(fid);

    M_unique = unique(M_vals);
    T_unique = unique(T_vals);

    [X, Y] = meshgrid(M_unique, T_unique);
    Z = nan(size(X));

    for k = 1:length(times)
        i = find(T_unique == T_vals(k));
        j = find(M_unique == M_vals(k));
        Z(i, j) = times(k);
    end

    figure;
    surf(X, Y, Z);

    xlabel('MPI ranks M');
    ylabel('CUDA threads per block');
    zlabel('Wall time seconds');
    title('MPI + CUDA Simpson Runtime Surface');

    set(gca, 'YScale', 'log');
    yticks(T_unique);
    yticklabels(string(T_unique));

    colorbar;
    grid on;

    saveas(gcf, 'runtime_surface.png');

    [best_time, idx] = min(times);

    fprintf('Best configuration:\n');
    fprintf('M=%d, threads=%d, time=%g seconds\n', ...
        M_vals(idx), T_vals(idx), best_time);
end