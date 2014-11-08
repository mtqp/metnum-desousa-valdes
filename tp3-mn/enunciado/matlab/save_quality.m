function save_quality( imgName, pathToFolder, distanceToBorder )
%SAVE_QUALITY Graba los scores de PSNR y SSIM (RGB y G)

% Folder string path correction
if (pathToFolder(end) ~= '/')
    pathToFolder = strcat(pathToFolder, '/');
end

im_orig = imread(['../images/' imgName '.bmp']);
im_filtered = imread([pathToFolder 'filtered_' imgName '.tif']);

im_orig_borders_cut = im_orig( (1+distanceToBorder):(end-distanceToBorder), (1+distanceToBorder):(end-distanceToBorder), :);

psnr_score_all = psnr(im_filtered, im_orig_borders_cut, false);
psnr_score_green = psnr(im_filtered, im_orig_borders_cut, true);

im_orig_borders_cut_green_channel = im_orig_borders_cut(:,:,2);
im_filtered_green_channel = im_filtered(:,:,2);

ssim_score_all = ssim(im_filtered, im_orig_borders_cut);
ssim_score_green = ssim(im_filtered_green_channel, im_orig_borders_cut_green_channel);

psnr_file_name = [pathToFolder 'psnr_filtered_' imgName];
ssim_file_name = [pathToFolder 'ssim_filtered_' imgName];
file_psnr = fopen(psnr_file_name, 'w');
file_ssim = fopen(ssim_file_name, 'w');

fprintf(file_psnr, 'all channels: %6.15f\ngreen channel: %6.15f', psnr_score_all, psnr_score_green);
fprintf(file_ssim, 'all channels: %6.15f\ngreen channel: %6.15f', ssim_score_all, ssim_score_green);

disp(['PSNR and SSIM quality scores saved in files: ' psnr_file_name ' and ' ssim_file_name]);
fclose(file_psnr);
fclose(file_ssim);

end