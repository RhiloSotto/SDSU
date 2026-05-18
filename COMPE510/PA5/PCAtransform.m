function Xt = PCAtransform(X,W)
% This function performs PCA to reduce the dimension of X
%   PCAtransform(X,W) transforms X to the K-dimensional PC space defined
%   by W; Each column of W is a principle component. 

% ====================== YOUR CODE HERE ======================

Xt = X * W;


% ============================================================



end