function [ score_in_db ] = psnr( img1, img2 )
%PSNR Peak Signal To Noise Ratio
	mean_square_error = mse(img1, img2);
	max_uint8 = 255;
	score_in_db = 20*log10(max_uint8) - 10*log10(mean_square_error);
end