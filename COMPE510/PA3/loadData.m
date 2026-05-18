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
    
T = readtable("pima-indians-diabetes.csv");

X = table2array(T(:, 1:8));
y = table2array(T(:, 9));

numObservations = size(X, 1);
trainingRatio = 0.7; % 70%
numTraining = round(numObservations * trainingRatio);

% randomly picks data examples
indices = randperm(numObservations);

trainingIndices = indices(1:numTraining);
testingIndices = indices(numTraining+1:end);

X_train = X(trainingIndices, :);
y_train = y(trainingIndices, :);
X_test = X(testingIndices, :);
y_test = y(testingIndices, :);



% ============================================================
end