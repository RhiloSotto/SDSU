% Read in original RGB image.
I = imread('peppers.png');
[m,n,c]=size(I);
fileID = fopen('input.dat','w');

% include image dimensions
fprintf(fileID, '%d %d %d\n', m, n, c);
for i = 1:m,
    for j = 1:n,
        for k = 1:c,
            fprintf(fileID,'%d\n', I(i,j,k));
        end
    end
end
fclose(fileID);