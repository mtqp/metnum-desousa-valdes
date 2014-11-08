for i = 1:12
    reconstruct_image(['img' int2str(i)], '../images_files/out/closest_neighbor/', 1);
    save_quality(['img' int2str(i)], '../images_files/out/closest_neighbor/', 1);
    reconstruct_image(['img' int2str(i)], '../images_files/out/bilinear_interpolation/', 1);
    save_quality(['img' int2str(i)], '../images_files/out/bilinear_interpolation/', 1);
    reconstruct_image(['img' int2str(i)], '../images_files/out/directional_interpolation/', 1);
    save_quality(['img' int2str(i)], '../images_files/out/directional_interpolation/', 1);
    reconstruct_image(['img' int2str(i)], '../images_files/out/malvar_he_cutler/', 1);
    save_quality(['img' int2str(i)], '../images_files/out/malvar_he_cutler/', 2);
end