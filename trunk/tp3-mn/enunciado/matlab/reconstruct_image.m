function [ im ] = reconstruct_image( imgName, pathToFolder, saveToFile )
%RECONSTRUCT_IMAGE Toma los 3 archivos de los 3 canales y los junta en una
%imagen
    % Optional argument saveToFile
    if (~exist('saveToFile', 'var'))
        saveToFile = 0;
    end
    
    % Folder string path correction
    if (pathToFolder(end) ~= '/')
        pathToFolder = strcat(pathToFolder, '/');
    end
    
    % Paths
    extension = '_out_';
    R_file = strcat(pathToFolder, imgName, extension, 'r');
    G_file = strcat(pathToFolder, imgName, extension, 'g');
    B_file = strcat(pathToFolder, imgName, extension, 'b');
    
    % Load images
    R = importdata(R_file);
    G = importdata(G_file);
    B = importdata(B_file);
    
    % Save to a RGB matrix
    im = zeros(size(B, 1), size(B, 2), 3);
    im(:,:,1) = R;
    im(:,:,2) = G;
    im(:,:,3) = B;
    
    % Cast to uint8
    im = uint8(im);
    if saveToFile
        prefix = 'filtered_';
        fmt = '.tif';
        savingPath = strcat(pathToFolder, prefix, imgName, fmt);
        imwrite(im, savingPath, 'Compression', 'none');
        disp(strcat('Image saved! File path: ',savingPath));
    end
end

