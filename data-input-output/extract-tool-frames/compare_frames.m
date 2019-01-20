% Compare two color images for equality

im1 = imread('frame-100.jpg');
im2 = imread('frame-100-1.jpg');
diff = im1-im2;
nnz(diff)