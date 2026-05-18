function p = predict(beta1, beta2, X)
%PREDICT Predict the label of an input given a trained neural network
%   p = PREDICT(beta1, beta2, X) outputs the probability of the output to 
%   1, given input X and trained weights of a neural network (beta1, beta2)

% You need to return the following variables correctly 
p = zeros(size(X, 1), 1); % 

% ====================== YOUR CODE HERE ======================
% Instructions: Complete the following code to make predictions using
%               your learned neural network. 
%
%


m = size(X, 1);

for i = 1:m
    x = X(i, :)';
    
    a1 = [1; x];
    
    z2 = beta1 * a1;
    a2 = sigmoid(z2);
    a2 = [1; a2];
    
    z3 = beta2 * a2;
    a3 = sigmoid(z3);
    
    p(i) = (a3 >= 0.5);
end


% =========================================================================


end
