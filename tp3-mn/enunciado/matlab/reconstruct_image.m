function [ im ] = reconstruct_image( R_file, G_file, B_file )
%RECONSTRUCT_IMAGE Toma los 3 archivos de los 3 canales y los junta en una
%imagen

R = importdata(R_file);
G = importdata(G_file);
B = importdata(B_file);
im = zeros(size(B, 1), size(B, 2), 3);
im(:,:,1) = R;
im(:,:,2) = G;
im(:,:,3) = B;
im = uint8(im);
end

