% Read tools present stats file
% Annihilate lower diagonal part (including diagonal)
% Sort remaining and save text file showing most frequently occuring
% together pairs

M = dlmread('toolspresenttogether-No-Heading.txt');
M1 = triu(M);
M2 = M1-diag(diag(M1)); % Annihlate diagonal elements

[A,I] = sort(M2(:),'descend');

A = find(A>0);
I1 = I(A);

% reshape to get [m,n] values
[R,C] = ind2sub(size(M2),I1);

% Read names of tools as strings
fileID = fopen('toolheadings.txt');
toolnames=textscan(fileID, '%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s','delimiter', ',');
fclose(fileID);

% write tool pairs occuring together in descending order
fileID = fopen('tool-together-frequencies.txt','w');
for i = 1:size(R)
    tool1 = char(toolnames{R(i)});
    tool2 = char(toolnames{C(i)});
    freq = M(R(i),C(i));
    fprintf(fileID,'%s,%s,%i\n',tool1,tool2,freq);
end
fclose(fileID);

% Rank individual tools and write frequencies
B = diag(diag(M));
[A,I] = sort(B(:),'descend');
A = find(A>0);
I1 = I(A);
[R,C] = ind2sub(size(B),I1);

fileID = fopen('tool-rankings.txt','w');
for i = 1:size(R)
    tool = char(toolnames{R(i)});
    freq = M(R(i),C(i));
    fprintf(fileID,'%s,%i\n',tool,freq);
end
fclose(fileID);















