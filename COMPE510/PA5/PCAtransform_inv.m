function X_inv = PCAtransform_inv(Xt,W)
% This function reverses the PCA process 
%   PCAtransform_inv(X,W) transforms Xt back to its original space; 
%   Each column of W is a principle component. 

% ====================== YOUR CODE HERE ======================

X_inv = Xt * W';


% ============================================================
end