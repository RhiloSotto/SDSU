function [beta, J_history] = gradientDescent(X, y, beta, alpha, num_iters)
%GRADIENTDESCENT Performs gradient descent to learn beta by
%   taking num_iters gradient steps with learning rate alpha

% Initialize some useful values
m = length(y); % number of training examples
J_history = zeros(num_iters, 1); % stores value of the cost function J at each iteration
n = length(beta); % number of features

for iter = 1:num_iters

    % ====================== YOUR CODE HERE ======================
    % Instructions: Perform a single gradient step on the parameter vector
    %               beta. 
    %
    % Hint: While debugging, it can be useful to print out the values
    %       of the cost function (computeCost) and gradient here.
    %

    error = X * beta - y;
    gradient = (1/m) * (X' * error);

    % update beta
    beta = beta - alpha * gradient;


    % ============================================================

    % Save the cost J (J = 1/(2m)*SSE) in every iteration 
    
    % ====================== YOUR CODE HERE ======================
    J_history(iter) = computeCost(X, y, beta);
  

    % ============================================================
end

end
