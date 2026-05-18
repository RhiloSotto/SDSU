% EXECUTE THIS FILE
% CompE565 Homework 2
% Mar. 20, 2026
% Name: Rhilo Sotto
% ID: 130551574
% email: rsotto4586@sdsu.edu  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
% Homework 2: JPEG-based Image Compression
% Implementation:
%   - YCbCr to RGB   
%   - RGB to YCbCr conversion
%   - 4:2:0 chroma subsampling
%   - 8×8 block DCT transformation
%   - Quantization using JPEG luminance and chrominance matrices
%   - Zigzag scanning of coefficients
%   - Inverse quantization and inverse DCT
%   - Image reconstruction
% M-file name: HW2_130551574.m  
% Usage: README
% Location of output image: figure windows, command window
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
disp('Executing hw2.m'); 
HW2_130551574;
disp('Figures displayed and MSE/PSNR printed');  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 