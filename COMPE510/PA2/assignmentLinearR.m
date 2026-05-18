%% SDSU Machine Learning Course (CompE510/EE600/CompE596)
%% Programming Assignment:  Linear regression 
%
% Dataset comes from: 
% https://archive.ics.uci.edu/ml/datasets/Real+estate+valuation+data+set
%
%  Instructions
%  ------------
% 
%  This file contains code that helps you get started on the
%  linear regression assignment. 
%
%  You will need to complete the following functions in this 
%  exericse:
%
%     loadData.m
%     normalEqn.m
%     evaluateAccuracy.m
%     featureNormalize.m
%     gradientDescent.m
%     computeCost.m
%
%  For this part of the exercise, you will need to change some
%  parts of the code below for various experiments (e.g., changing
%  learning rates).
%
% Initialization
clear ; close all; clc

%% ================ Part 1: Normal Equations ================
fprintf('===== Part 1: Normal Equations ===== \n');

% Instructions: The following code computes the closed form 
%               solution for linear regression using the normal
%               equations. You should complete the code in loadData.m,
%               normalEqn.m
%
%               After doing so, you should complete this code 
%               to predict the price of a house that is 2 year 
%               old, 500 meter to the nearest MRT station, and 
%               has 8 convenience stores in the living circle 
%               on foot
% ============================================================


% Step 1: Load data and split data into a training set and testing set
% ====================== YOUR CODE HERE ======================
[X_train, y_train, X_test, y_test] = loadData();

% ============================================================

% Print out some data points
fprintf('First 10 examples from the training dataset: \n');
fprintf(' x = [%.0f %.0f %.0f], y = %.0f \n', [X_train(1:10,:) y_train(1:10,:)]');
fprintf('\n');

num_train = length(y_train); % number of training examples

% Add intercept term to X_train and X_test
X_train = [ones(num_train, 1) X_train];

% Step 2: Calculate the parameters beta from the normal equation
% ====================== YOUR CODE HERE ======================
beta = normalEqn(X_train, y_train);

% ============================================================

% Display normal equation's result
fprintf('beta computed from the normal equations: \n');
fprintf(' %f \n', beta);
fprintf('\n');

fprintf('Program paused. Press enter to continue.\n');
pause;

%% ========== Part 2: Evaluate performance =============
fprintf('===== Part 2: Evaluate Performance ===== \n');

% Instructions: The following code evaluates the performance of
%               the trained linear regression model. You should 
%               complete code in evaluateAccuracy.m. You should 
%               also predict the price of a house described below.
% ============================================================

num_test = length(y_test); % number of testing examples

% Add intercept term to Xn_test
X_test = [ones(num_test, 1) X_test];

% ====================== YOUR CODE HERE ======================
% Step 1: Evaluate the prediction performance of the trained model
error = evaluateAccuracy(beta, X_test, y_test);

% ============================================================

% Display the average prediction error
fprintf('Average prediction error (using normal equations):\n %f\n', error);
fprintf('\n');

% Step 2: Estimate the price of a house that is 2 year old, 500 meter 
% to the nearest MRT station, has 8 convenience stores in the living circle on foot
% ====================== YOUR CODE HERE ======================

house_features = [2, 500, 8];
house_input = [1, house_features];
price = house_input * beta;  % You should change this.

% ============================================================

fprintf(['Predicted price of the house ' ...
         '(using normal equations):\n $%f\n'], price);
fprintf('\n');


fprintf('Program paused. Press enter to continue.\n');
pause;


%% ================ Part 3: Feature Normalization ================

fprintf('===== Part 3: Feature Normalization ===== \n');

% Instructions: The following code normalizes the data using standardization 
%               and evaluate the performance of the model trained using the
%               normalized data. 
%               You should complete code in featureNormalize.m
% ============================================================

% Step 1: Normalize the features
fprintf('Normalizing Features ...\n');
% ====================== YOUR CODE HERE ======================
[Xn_train mu sigma] = featureNormalize(X_train(:,2:end));
% Add intercept term to X_train
Xn_train = [ones(num_train, 1) Xn_train];
% ============================================================

beta_n = normalEqn(Xn_train, y_train);
% Display value of the parameters beta_n
fprintf('beta computed from the normal equations after feature normalization: \n');
fprintf(' %f \n', beta_n);
fprintf('\n');

% Step 2: Evaluate the performance of the model trained using the
% normalized data
% ====================== YOUR CODE HERE ======================
% Hint: The testing set also needs to be normalized first
Xn_test = (X_test(:, 2:end) - mu) ./ sigma; % you should change this
Xn_test = [ones(num_test, 1) Xn_test];
error_n = evaluateAccuracy(beta_n, Xn_test, y_test); % you should change this


% ============================================================
% Display the average prediction error
fprintf('Average prediction error after normalization (using normal equations):\n %f\n', error_n);
fprintf('\n');

fprintf('Program paused. Press enter to continue.\n');
pause;



%% ================ Part 4: Gradient Descent ================

fprintf('===== Part 4: Gradient Descent ===== \n');

% Instructions: The following code applies gradient descent to 
%               estimate the parameters in a linear regression 
%               model. You should complete code in gradientDescent.m
%
%               Try running gradient descent with 
%               different values of alpha and see which one gives
%               you the best result.
%
%               After that, you should complete the code at the end
%               to predict the price of a house.
% ============================================================


% Step 1: Initialize the hyper-parameters, including 
%         1. alpha: learning rate
%         2. num_iters: number of training iterations
% ====================== YOUR CODE HERE ======================
alpha = 1.0; % you should change this
num_iters = 1000; % you should change this

% ============================================================

% Initialize beta 
beta_g = zeros(4, 1);

% Step 2: Run Gradient Descent 
% ====================== YOUR CODE HERE ======================
[beta_g, J_history] = gradientDescent(Xn_train, y_train, beta_g, alpha, num_iters);

% ============================================================

% Plot the convergence graph
figure;
plot(1:numel(J_history), J_history, '-b', 'LineWidth', 2);
xlabel('Number of iterations');
ylabel('Cost J');

% Display gradient descent's result
fprintf('beta computed from gradient descent: \n');
fprintf(' %f \n', beta_g);
fprintf('\n');


error_g = evaluateAccuracy(beta_g, Xn_test, y_test);

% Display the average prediction error
fprintf('Average prediction error (using gradient descent):\n %f\n', error_g);
fprintf('\n');

% Step 3: Estimate the price of a house 
% that is 2 year old, 500 meter to the nearest MRT station, has 8 
% convenience stores in the living circle on foot
% ====================== YOUR CODE HERE ======================
% Recall that the first column of X is all-ones. Thus, it does
% not need to be normalized. But the other columns need to be normalized
house_features_g = [2, 500, 8];
house_input_n = (house_features_g - mu) ./ sigma;
house_input_n = [1, house_input_n];

price_g = house_input_n * beta_g; % You should change this

% ============================================================

fprintf(['Predicted price of the house ' ...
         '(using gradient descent):\n $%f\n'], price_g);


