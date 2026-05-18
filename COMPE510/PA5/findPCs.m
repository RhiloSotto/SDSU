function W = findPCs(X, K)
% This function finds the first K principle components of X

% ====================== YOUR CODE HERE ======================
% Instructions: First, compute the estimated variance of X, denoted as S
%               Second, compute eigenvalues and corresponding 
%               eigenvectors of S
%               Third, sort the eigenvalues in a descend order
%               Four, find the top K principle components with the largest
%               eigenvalues
% Hints:        try functions cov(), eig() 
%

% estimated variance
S = cov(X);
% eigenvalues
[V, D] = eig(S);
% sorteignevalues in descending order
[eigenvalues, idx] = sort(diag(D), 'descend');
% top K principle components
V = V(:, idx);
W = V(:, 1:K);


% ============================================================
end