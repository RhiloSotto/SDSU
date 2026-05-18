function [beta, l_history] = gradientAscent(X, y, beta, alpha, num_iters)
%GRADIENTASCENT Performs gradient ascent to learn beta. It updates beta by
%   taking num_iters gradient steps with learning rate alpha

l_history = zeros(num_iters, 1); % store log-likelihood value computed at each iteration

for iter = 1:num_iters

    % ====================== YOUR CODE HERE ======================
    % Instructions: Perform a single gradient step to update the parameter vector
    %               beta. 
    %
    % Hint: While debugging, it can be useful to print out the values
    %       of the log likelihood function and gradient here.
    %
    
    % Compute log-likelihood and gradient at current beta
    [l, grad] = likelihoodFunction(beta, X, y);

    % Update beta using gradient ascent
    beta = beta + alpha * grad;

    % Store the log-likelihood
    l_history(iter) = l;
    
    
    % ============================================================
end

end
