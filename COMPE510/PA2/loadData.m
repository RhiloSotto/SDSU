function [X_train, y_train, X_test, y_test] = loadData()
%   LOADDATA imports data downloaded from 
%   https://archive.ics.uci.edu/ml/datasets/Real+estate+valuation+data+set
%   and splits the dataset into two sets: training set and testing set
%
%   We only use three features as the input X: 
%       X2=the house age (unit:year)
%       X3=the distance to the nearest MRT station (unit:degree)
%       X4=the number of convenience stores in the living circle on foot (integer)
%   The output y is:
%       y=house price of unit area (10000 New Taiwan Dollar/Ping, where 
%         Ping is a local unit, 1 Ping = 3.3 meter squared)

 % ====================== YOUR CODE HERE ======================
    % Instructions: Import spreadsheets data and extract the columns
    % corresponding to X2, X3, X4 and store them as X. Extract the last
    % column and store it as y. 
    %
    % Randomly pick 70% of the data examples as the training set and the 
    % the rest as the testing set
    %
    % Hint: You might find the 'readtable' and 'table2array' functions useful.
    %

    
 % ============================================================   

T = readtable("housePriceData.xlsx");

X2 = T.X2HouseAge;
X3 = T.X3DistanceToTheNearestMRTStation;
X4 = T.X4NumberOfConvenienceStores;

X = table(X2, X3, X4);
X = table2array(X);
y = table(T.YHousePriceOfUnitArea);
y = table2array(y);

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

end