%% SDSU Machine Learning Course (CompE510/EE600/CompE596)
%% Programming Assignment:  Probability 
%
%
%  Instructions
%  ------------
%  
%  This file contains code that helps you get started on the
%  probability assignment. 
%
%  For this part of the exercise, you will need to change some
%  parts of the code below for various experiments.
%
% Initialization
clear ; close all; clc

%% ================ Part 1: Toss a coin ===================

% Instructions: Define a random variable called "coin" that represents the coin, 
%               i.e., it can take two possible values, 0 (tail) 
%               and 1 (head), with the probability of taking each 
%               value being 0.5. 
%               Then, complete the following steps. 
%                   Step 1: toss a coin once, and show the value of the
%                           coin
%                   Step 2: toss a coin for 10 times, count the 
%                           number of heads. Store the number of 
%                           heads in variable "c" and print its value
% ============================================================
fprintf('===== Part 1: Toss a coin ===== \n');


% ====================== YOUR CODE HERE ======================
% Step 1: toss a coin, and show the value of the coin
fprintf('Toss a coin ...\n');

% '1' is heads, '0' is tails
coin = round(rand);
% converting integer to a string that can be printed
% with the given fprintf code
coin = int2str(coin);


fprintf('The value of the coin: \n %s\n', coin);





% Step 2: toss a coin for 10 times and count the number of heads
% Store the number of heads in variable "c" and print its value
fprintf('Toss a coin for 10 times ...\n');


c = 0;
for i = 1:10
    coin = int2str(round(rand));
    if isequal(coin, '1')
      c = c + 1;  
    end
end





fprintf('The number of heads: \n %d\n', c);
% ============================================================

fprintf('Program paused. Press enter to continue.\n');
pause;


%% ================ Part 2: Roll a die ================

% Instructions: Define a random variable "die" that represents the die, 
%               i.e., it can take six possible values, 0 - 6, with 
%               the probability of taking each value being 1/6. 
%               Then, complete the following steps. 
%                   Step 1: roll a die once, and show its value
%                   Step 2: roll a die for 10 times and make a histogram 
%                           showing the distribution (Hint: can use 
%                           function "hist()" to plot the histogram)
%                   Step 3: roll a die for 10000 times, make a histogram,
%                           and then plot an empirical cdf (Hint: can use 
%                           function "stairs()" to plot the cdf)             
% ============================================================
fprintf('===== Part 2: Roll a die ===== \n');


% ====================== YOUR CODE HERE ======================
% Step 1: roll a die once
fprintf('Roll a die ...\n');

% rolling a die has 6 outcomes 1,2,3,4,5,6
% rand is between 0 and 1, multiplying by 6 makes the range 0-6
% taking the ceiling constrains the range to integers 1-6
die = int2str(ceil(rand * 6));


fprintf('The value of the die: \n %s\n', die);




% Step 2: roll a die 10 times and make a histogram
fprintf('Roll a die 10 times ...\n');


die_roll = zeros(1, 10);
for i = 1:10
    die_roll(i) = ceil(rand * 6);
end
histogram(die_roll, 6)


fprintf('Showing the histogram of the die...\n');

pause;





% Step 3: roll a die 10000 times, make a histogram, and plot the cdf
fprintf('Roll a die 10000 times ...\n');



die_roll = zeros(1, 10000);
for i = 1:10000
    die_roll(i) = ceil(rand * 6);
end
histogram(die_roll)
%histogram(die_roll, 'Normalization', 'cdf', 'DisplayStyle', 'stairs')

pause;
fprintf('Showing the empiral cdf of the die...\n');
ecdf(die_roll)



% ============================================================
fprintf('Program paused. Press enter to continue.\n');
pause;

%% ================ Part 3: Plot a normal distribution ================

% Instructions: Generate 10000 random samples from the normal distribution
% 
%               with mean = 1 and variance = 4 (Hint: use function "rand()").
%               Then complete the following:
%                   Step 1: Make a histogram to show the distribution.
%                   Step 2: Fit a probability density function (normal distribution) 
%                           to the data and plot the pdf superimposed over a histogram 
%                           of the data (Hint: use function "fitdist()")             
% ============================================================

fprintf('===== Part 3: Plot a normal distribution ===== \n');

% ====================== YOUR CODE HERE ======================
% Step 1: Make a histogram to show the distribution.

fprintf('Showing the histogram of the data ...\n');

mean = 1;
variance = 4;
std_dev = sqrt(variance);

sample = mean + std_dev * randn(1, 10000);
histogram(sample, 'Normalization', 'pdf');
hold;



% Step 2: Fit a probability density function to the data and plot the pdf 
%     superimposed over a histogram of the data (Hint: use function "fitdist()")


fprintf('Showing the pdf created by fitting a normal distribution to the data ...\n');


pdf = fitdist(sample', 'Normal');
plot(pdf);



% ============================================================
