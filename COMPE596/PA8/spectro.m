close all; clear; clc;

figure;

% Original
subplot(2,1,1);
[y1, Fs1] = audioread('compe596_mix_48kHz_16bit_mono_Mixdown_1.wav');
spectrogram(y1,128,120,128,Fs1,'yaxis');
title('Original Audio');
ylim([0 24]);

% Filtered
subplot(2,1,2);
[y2, Fs2] = audioread('FFTW_P08_out.wav');
spectrogram(y2,128,120,128,Fs2,'yaxis');
title('Filtered Audio (10 kHz Removed)');
ylim([0 24]);