% Name: Rhilo Sotto
% RedID: 130551574
close all;
clear;
clc;

% Read image
I = imread('iris.jpg', 'jpg');

%% ---------------- ENCODING STAGE ------------------%%
% Convert image from RGB to YCbCr and subsample 4:2:0
I_YCbCr = rgb2ycbcr(I);

% Image dimensions
X_ = size(I,1);
Y_ = size(I,2);

Y = I_YCbCr(:,:,1);
Cb = I_YCbCr(:,:,2);
Cr = I_YCbCr(:,:,3);


% Subsampling
X_sub = floor(X_ / 2);
Y_sub = floor(Y_ / 2);

Cb_sub = zeros(X_sub, Y_sub);
Cr_sub = zeros(X_sub, Y_sub);

for row = 1:X_sub
    for col = 1:Y_sub
        Cb_sub(row, col) = Cb(2 * row - 1, 2 * col - 1);
        Cr_sub(row, col) = Cr(2 * row - 1, 2 * col - 1);
    end
end


% Block size (N x N)
B_size = 8;
% Block array dimensions (rounding down)
X_B = floor(X_ / B_size);
Y_B = floor(Y_ / B_size);
X_B_sub = floor(X_sub / B_size);
Y_B_sub = floor(Y_sub / B_size);

% Array of blocks
% Y blocks
Blocks_Y = zeros(X_B, Y_B, B_size, B_size);
% Cb = 1, Cr = 2 blocks
Blocks_CbCr = zeros(2, X_B_sub, Y_B_sub, B_size, B_size);

% read image into blocks
for row_B = 1:X_B
    for col_B = 1:Y_B
        for row = 1:B_size
            for col = 1:B_size
                % Y component
                Blocks_Y(row_B, col_B, row, col) = Y((row_B - 1)*B_size+row, (col_B - 1)*B_size+col);
            end
        end
    end
end

for row_B = 1:X_B_sub
    for col_B = 1:Y_B_sub
        for row = 1:B_size
            for col = 1:B_size  
                % Cb and Cr Components
                Blocks_CbCr(1, row_B, col_B, row, col) = Cb_sub((row_B - 1)*B_size+row, (col_B - 1)*B_size+col);
                Blocks_CbCr(2, row_B, col_B, row, col) = Cr_sub((row_B - 1)*B_size+row, (col_B - 1)*B_size+col);

            end
        end
    end
end

% DCT on luminance and chrominance blocks
Blocks_Y_DCT = zeros(X_B, Y_B, B_size, B_size);
Blocks_CbCr_DCT = zeros(2, X_B_sub, Y_B_sub, B_size, B_size);

for row_B = 1:X_B
    for col_B = 1:Y_B
        % DCT on the block level
        block = double(squeeze(Blocks_Y(row_B, col_B,:,:))) - 128;
        Blocks_Y_DCT(row_B, col_B,:,:) = dct2(block);
    end
end

for row_B = 1:X_B_sub
    for col_B = 1:Y_B_sub
        % DCT on the block level
        blockCb = double(squeeze(Blocks_CbCr(1, row_B, col_B,:,:))) - 128;
        blockCr = double(squeeze(Blocks_CbCr(2, row_B, col_B,:,:))) - 128;

        Blocks_CbCr_DCT(1, row_B, col_B,:,:) = dct2(blockCb);
        Blocks_CbCr_DCT(2, row_B, col_B,:,:) = dct2(blockCr);

    end
end

% Quantization Matrices
Lum_Q = readmatrix("Quantization - Luminance.csv");
Chrom_Q = readmatrix("Quantization - Chrominance.csv");

% Quantize DCT
Blocks_Y_DCT_Q = zeros(X_B, Y_B, B_size, B_size);
Blocks_CbCr_DCT_Q = zeros(2, X_B_sub, Y_B_sub, B_size, B_size);

% divide DCT by quantization matrix and round to nearest integer
for row_B = 1:X_B
    for col_B = 1:Y_B
        for row = 1:B_size
            for col = 1:B_size
                % Y component
                Blocks_Y_DCT_Q(row_B, col_B, row, col) = round(Blocks_Y_DCT(row_B, col_B, row, col) / Lum_Q(row, col));
            end
        end
    end
end

for row_B = 1:X_B_sub
    for col_B = 1:Y_B_sub
        for row = 1:B_size
            for col = 1:B_size  
                % Cb and Cr Components
                Blocks_CbCr_DCT_Q(1, row_B, col_B, row, col) = round(Blocks_CbCr_DCT(1, row_B, col_B, row, col) / Chrom_Q(row, col));
                Blocks_CbCr_DCT_Q(2, row_B, col_B, row, col) = round(Blocks_CbCr_DCT(2, row_B, col_B, row, col) / Chrom_Q(row, col));
            end
        end
    end
end



%% ---------------- DECODING STAGE --------------- %%
% Inverse quantize
Blocks_Y_DCT_iQ = zeros(X_B, Y_B, B_size, B_size);
Blocks_CbCr_DCT_iQ = zeros(2, X_B_sub, Y_B_sub, B_size, B_size);

% multiply by quantization matrix
for row_B = 1:X_B
    for col_B = 1:Y_B
        for row = 1:B_size
            for col = 1:B_size
                % Y component
                Blocks_Y_DCT_iQ(row_B, col_B, row, col) = Blocks_Y_DCT_Q(row_B, col_B, row, col) * Lum_Q(row, col);
            end
        end
    end
end

for row_B = 1:X_B_sub
    for col_B = 1:Y_B_sub
        for row = 1:B_size
            for col = 1:B_size  
                % Cb and Cr Components
                Blocks_CbCr_DCT_iQ(1, row_B, col_B, row, col) = Blocks_CbCr_DCT_Q(1, row_B, col_B, row, col) * Chrom_Q(row, col);
                Blocks_CbCr_DCT_iQ(2, row_B, col_B, row, col) = Blocks_CbCr_DCT_Q(2, row_B, col_B, row, col) * Chrom_Q(row, col);
            end
        end
    end
end

% Inverse DCT
Blocks_Y_iDCT_iQ = zeros(X_B, Y_B, B_size, B_size);
Blocks_CbCr_iDCT_iQ = zeros(2, X_B_sub, Y_B_sub, B_size, B_size);

% add up all basis frequencies
for row_B = 1:X_B
    for col_B = 1:Y_B
        % iDCT on the block level
        % Blocks_Y_iDCT_iQ(row_B, col_B,:,:) = idct2(Blocks_Y_DCT_iQ(row_B, col_B), B_size, B_size);
        Blocks_Y_iDCT_iQ(row_B, col_B,:,:) = idct2(squeeze(Blocks_Y_DCT_iQ(row_B, col_B,:,:)));
    end
end

for row_B = 1:X_B_sub
    for col_B = 1:Y_B_sub
        % iDCT on the block level
        Blocks_CbCr_iDCT_iQ(1, row_B, col_B,:,:) = idct2(squeeze(Blocks_CbCr_DCT_iQ(1, row_B, col_B,:,:)));
        Blocks_CbCr_iDCT_iQ(2, row_B, col_B,:,:) = idct2(squeeze(Blocks_CbCr_DCT_iQ(2, row_B, col_B,:,:)));
    end
end

% Reconstruct image
Y_rec = zeros(X_, Y_);
Cb_rec = zeros(X_, Y_);
Cr_rec = zeros(X_, Y_);

% Reassemble Y
for row_B = 1:X_B
    for col_B = 1:Y_B
        for row = 1:B_size
            for col = 1:B_size
                Y_rec((row_B-1)*B_size + row, (col_B-1)*B_size + col) = Blocks_Y_iDCT_iQ(row_B, col_B, row, col) + 128;
            end
        end
    end
end

% Reassemble Cb and Cr
Cb_low = zeros(X_sub, Y_sub);
Cr_low = zeros(X_sub, Y_sub);

for row_B = 1:X_B_sub
    for col_B = 1:Y_B_sub
        for row = 1:B_size
            for col = 1:B_size
                Cb_low((row_B-1)*B_size + row, (col_B-1)*B_size + col) = Blocks_CbCr_iDCT_iQ(1,row_B,col_B,row,col) + 128;

                Cr_low((row_B-1)*B_size + row, (col_B-1)*B_size + col) = Blocks_CbCr_iDCT_iQ(2,row_B,col_B,row,col) + 128;
            end
        end
    end
end

% Upsample Cb and Cr
Cb_rec = imresize(Cb_low, [X_, Y_], 'bilinear');
Cr_rec = imresize(Cr_low, [X_, Y_], 'bilinear');


%% ---------------- DISPLAY DCT COEFFICIENTS ---------------- %%
% DCT matrices of 88th and 89th blocks in 63rd row of Y channel
block88_DCT = squeeze(Blocks_Y_DCT(63, 88, :, :));
block89_DCT = squeeze(Blocks_Y_DCT(63, 89, :, :));

% Display as matrix (numeric output)
disp('DCT Coefficients for Block 88 (63rd row):');
disp(block88_DCT);
disp('DCT Coefficients for Block 89 (63rd row):');
disp(block89_DCT);

% Display as images
figure;
imagesc(block88_DCT); colormap gray; colorbar;
title('DCT Coefficients Image - Block 88 (Y channel)');

figure;
imagesc(block89_DCT); colormap gray; colorbar;
title('DCT Coefficients Image - Block 89 (Y channel)');


% Apply zigzag to quantized Y blocks
block88_zz = zigzag_scan(squeeze(Blocks_Y_DCT_Q(63,88,:,:)));
block89_zz = zigzag_scan(squeeze(Blocks_Y_DCT_Q(63,89,:,:)));

fprintf('Zigzag Scanned DCT Coefficients - Block 88:\n[');
fprintf('%d, ', block88_zz(1:end-1));
fprintf('%d]\n', block88_zz(end));

fprintf('Zigzag Scanned DCT Coefficients - Block 89:\n[');
fprintf('%d, ', block89_zz(1:end-1));
fprintf('%d]\n', block89_zz(end));

%% ---------------- ORIGINAL IMAGE ------------------%%
figure;
imshow(I);
title('Original RGB Image');


%% ---------------- RECONSTRUCTED IMAGE ---------------- %%
% Assuming Y_rec, Cb_rec, Cr_rec are reconstructed and upsampled
YCbCr_rec = cat(3, Y_rec, Cb_rec, Cr_rec);
I_rec = ycbcr2rgb(uint8(min(max(YCbCr_rec,0),255)));

figure;
imshow(I_rec);
title('Reconstructed RGB Image');

%% ---------------- ERROR IMAGE ---------------- %%
% Luminance error
Error_Y = double(Y) - double(Y_rec);

figure;
imshow(uint8(abs(Error_Y)));
title('Error Image (Y channel)');

%% ---------------- PSNR ---------------- %%
MSE_Y = mean((double(Y(:)) - double(Y_rec(:))).^2);
PSNR_Y = 10 * log10(255^2 / MSE_Y);
fprintf('MSE for Luminance (Y) = %.2f\n', MSE_Y);
fprintf('PSNR for Luminance (Y) = %.2f dB\n', PSNR_Y);


%% ---------------- ZIGZAG SCAN ---------------- %%
% Zigzag function
function zz = zigzag_scan(block)
    index = [1 2 6 7 15 16 28 29;
             3 5 8 14 17 27 30 43;
             4 9 13 18 26 31 42 44;
             10 12 19 25 32 41 45 54;
             11 20 24 33 40 46 53 55;
             21 23 34 39 47 52 56 61;
             22 35 38 48 51 57 60 62;
             36 37 49 50 58 59 63 64];
    zz = block(index);
    zz = zz(:).';
end