function [X_train, y_train, X_test, y_test] = loadData()
%   LOADDATA imports data downloaded from 
%   http://networkrepository.com/pima-indians-diabetes.php
%   and splits the dataset into two sets: training set and testing set
%

 % ====================== YOUR CODE HERE ======================
    % Instructions: Import spreadsheets data, extract the first
    % 8 columns and store them as X. Extract the last column and 
    % store it as y. 
    %
    % Randomly pick 70% of the data examples as the training set and the 
    % the rest as the testing set
    %
    % Hint: You might find the 'readtable' and 'table2array' functions useful.
    %

    
    
    data = readtable('pima-indians-diabetes.csv', 'ReadVariableNames', false);
    data = table2array(data);
    
    X = data(:, 1:8);
    y = data(:, 9);
    
    m = size(X, 1);
    
    rng(42);
    
    idx = randperm(m);
    X = X(idx, :);
    y = y(idx, :);
    
    train_size = floor(0.7 * m);
    
    X_train = X(1:train_size, :);
    y_train = y(1:train_size, :);
    X_test = X(train_size+1:end, :);
    y_test = y(train_size+1:end, :);


% ============================================================
end