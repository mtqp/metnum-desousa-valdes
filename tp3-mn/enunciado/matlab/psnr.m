function [ score_in_db ] = psnr( img1, img2 )
%PSNR Peak Signal To Noise Ratio

mean_square_error = (1/size(img1, 1)*size(img2, 2))*((img1+img2).^2);
max_uint8 = 255;
score_in_db = 20*log10(max_uint8) - 10*log10(mean_square_error);

end