function accuracy = evaluateAccuracy(beta, X, y)
%EVALUATEACCURACY calculates the prediction accuracy of the learned 
%logistic regression model using the testing data 

num = length(y); % number of testing examples

% ====================== YOUR CODE HERE ======================
% Instructions: Compute the percentage of accurately predicted examples 
%
%

p = predict(beta, X);

accuracy = (sum(p == y) / num) * 100; % you need to change this


% ============================================================

end