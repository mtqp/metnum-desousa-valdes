%% Grabar imágenes en archivos

% Falta agregar tamaño de imagen en el archivo de salida. Se hace
% manualmente por ahora

root = '../images/';
root_to_save = '../images_txt/';
saving_format = ''; % antes era .txt pero el C++ es mas facil maniobrarlo sin extensión

lookup_term = strcat(root, '*.bmp');
listing_bmp_names = ls(lookup_term);

for i = 1:size(listing_bmp_names, 1)
    % Read image
    img_file_name =  listing_bmp_names(i,:);
    img_path = strcat(root, img_file_name);
    im = imread(img_path);
    
    % Mosaicing
    im_mosaic_to_save = mosaicing(im);
    
    % Saving
    file_name_separate = strsplit(img_file_name, '.');
    file_name_without_extension = file_name_separate(1);
    saving_path_for_current_img = char(strcat(root_to_save, file_name_without_extension, saving_format));
    save(saving_path_for_current_img, 'im_mosaic_to_save', '-ascii');
end