function [l, grad] = likelihoodFunction(beta, X, y)
%LIKELIHOODFUNCTION Computes log likelihood using beta as the parameter 
%   for logistic regression and the gradient of the log likelihood function
%   w.r.t. to the parameters.

% You need to return the following variables correctly 
l = 0; 
grad = zeros(size(beta)); 

% ====================== YOUR CODE HERE =====================
% Instructions: Compute the log-likelihood (l) of a particular choice of beta
%               averaged over all training examples, i.e., l = 1/num_train * (...) 
%               Compute the partial derivatives and set grad to the partial
%               derivatives of the log-likelihood function w.r.t. each 
%               parameter in beta
%
% Note: grad should have the same dimensions as beta
%


% Number of training examples
m = length(y);

% Compute predicted probabilities using sigmoid
z = X * beta;
h = 1 ./ (1 + exp(-z));  % sigmoid(z)

% Compute the average log-likelihood
l = (1/m) * sum(y .* log(h + eps) + (1 - y) .* log(1 - h + eps));  % eps to avoid log(0)

% Compute the gradient of the average log-likelihood
grad = (1/m) * X' * (y - h);


% =============================================================

end
