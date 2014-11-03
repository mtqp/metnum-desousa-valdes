function [ error ] = mse( img1, img2 )
%MSE Mean Squared Error
	% Casteos para trabajar con doubles y que no limiten los uint8
	img1 = double(img1);
	img2 = double(img2);
	
	% MSE
	m = size(img1, 1);
	n = size(img1, 2);
	substract_and_square_matrix = (img1 - img2) .^ 2;
	error = (1/(m*n)) * sum(substract_and_square_matrix(:));
end