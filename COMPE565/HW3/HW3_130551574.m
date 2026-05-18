close all; clear; clc;

% search window: 60x60
% 5 frames: 16 - 20
% GOP 5: IPPPP
% SAD
% exhaustive search 

video_file = 'foreman_cif.avi';
mb_size = 16;     % macroblock
sw_size = 60;     % search window 
w = sw_size/2; 
start_frame = 16;       
num_frames = 5; 

% read video
vid = VideoReader(video_file);
frame_width  = vid.Width;   % 352
frame_height = vid.Height;  % 288

% frames 16 to 20
frames_y = cell(1, num_frames);
for n = 1:num_frames
    frame_idx = start_frame + n - 1;
    vid.CurrentTime = (frame_idx - 1) / vid.FrameRate;
    frame_rgb = readFrame(vid);
    
    frame_ycbcr = rgb2ycbcr(frame_rgb);
    frames_y{n} = double(frame_ycbcr(:,:,1));
end

I_frame = frames_y{1}; % I frame

figure('Name', 'I-Frame');
imshow(uint8(I_frame));
title('I-Frame');

num_mb_row = frame_height / mb_size;
num_mb_col = frame_width  / mb_size;
num_mb = num_mb_row * num_mb_col;

% frames
for n = 1:4
    frame_num = start_frame + n; 
    current_frame = frames_y{n+1};
    
    fprintf('P-Frame %d\n', frame_num);

    motion_vectors = zeros(num_mb_row, num_mb_col, 2);  % dx and dy
    error_frame = zeros(frame_height, frame_width);
    recon_frame = zeros(frame_height, frame_width);
    
    % mb position and displacement
    mv_x1 = zeros(num_mb, 1);
    mv_y1 = zeros(num_mb, 1);
    mv_x2 = zeros(num_mb, 1);
    mv_y2 = zeros(num_mb, 1);
    
    total_mult_count = 0;
    total_additions = 0;
    total_comparisons = 0;
    mb_index = 0;
    
    % mb
    for mb_row = 1:num_mb_row
        for mb_col = 1:num_mb_col
            mb_index = mb_index + 1;
            
            y_pos = (mb_row - 1) * mb_size + 1;
            x_pos = (mb_col - 1) * mb_size + 1;
            
            current_mb = current_frame(y_pos:y_pos+mb_size-1, x_pos:x_pos+mb_size-1);
            
            % sw bounds
            search_y_min = max(1, y_pos - w);
            search_y_max = min(frame_height - mb_size + 1, y_pos + w);
            search_x_min = max(1, x_pos - w);
            search_x_max = min(frame_width  - mb_size + 1, x_pos + w);
            
            min_sad = Inf;
            best_y = y_pos;
            best_x = x_pos;
            
            % exhaustive search
            for sy = search_y_min:search_y_max
                for sx = search_x_min:search_x_max
                    ref_block = I_frame(sy:sy+mb_size-1, sx:sx+mb_size-1);
                    
                    difference_matrix = current_mb - ref_block;
                    sad_val = sum(abs(difference_matrix(:)));
                    
                    total_additions = total_additions + (2 * mb_size^2 - 1);
                    total_comparisons = total_comparisons + 1;
                    
                    if sad_val < min_sad
                        min_sad = sad_val;
                        best_y  = sy;
                        best_x  = sx;
                    end
                end
            end
            
            % mv
            dy = best_y - y_pos;
            dx = best_x - x_pos;
            motion_vectors(mb_row, mb_col, 1) = dy;
            motion_vectors(mb_row, mb_col, 2) = dx;
            
            mv_x1(mb_index) = x_pos;
            mv_y1(mb_index) = y_pos;
            mv_x2(mb_index) = dx;
            mv_y2(mb_index) = dy;
            
            % error
            best_match_block = I_frame(best_y:best_y+mb_size-1, best_x:best_x+mb_size-1);
            error_frame(y_pos:y_pos+mb_size-1, x_pos:x_pos+mb_size-1) = ...
                current_mb - best_match_block;
            
            % reconstruct
            recon_frame(y_pos:y_pos+mb_size-1, x_pos:x_pos+mb_size-1) = ...
                best_match_block + error_frame(y_pos:y_pos+mb_size-1, x_pos:x_pos+mb_size-1);
        end
    end
    
    % stats
    fprintf('  Multiplications: %d\n', total_mult_count);
    fprintf('  Additions: %d\n', total_additions);
    fprintf('  Comparisons: %d\n\n', total_comparisons);
    
    % mv plot
    figure('Name', sprintf('MV: Frame %d', frame_num));
    quiver(mv_x1, mv_y1, mv_x2, mv_y2, 0);
    set(gca, 'YDir', 'reverse');  
    axis([1 frame_width 1 frame_height]);
    title(sprintf('Motion Vectors: Frame %d', frame_num));

    % error
    figure('Name', sprintf('Error Frame %d', frame_num));
    error_display = error_frame + 128;
    error_display = max(0, min(255, error_display));
    imshow(uint8(error_display));
    title(sprintf('Error Frame %d', frame_num));

    % target
    figure('Name', sprintf('Target Frame %d', frame_num));
    imshow(uint8(current_frame));
    title(sprintf('Target P-Frame %d', frame_num));
    
    % reconstructed
    figure('Name', sprintf('Reconstructed Frame %d', frame_num));
    imshow(uint8(recon_frame));
    title(sprintf('Reconstructed P-Frame %d', frame_num));
    
end