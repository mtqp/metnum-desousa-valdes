function [ im_mosaic im_mosaic_color ] = mosaicing( im )
%MOSAICING Hace el mosaico con Bayer Array
% Primera fila, AZUL VERDE. Segunda fila, VERDE ROJO.
    im_red = im(:,:,1);
    im_green = im(:,:,2);
    im_blue = im(:,:,3);
   
    rgb_size = 3;
    im_mosaic_color = zeros(size(im,1), size(im,2), rgb_size);
    
    % En fila par/col impar y al reves, VERDE
    im_mosaic_color(2:2:end, 1:2:end, 2) = im_green(2:2:end, 1:2:end);
    im_mosaic_color(1:2:end, 2:2:end, 2) = im_green(1:2:end, 2:2:end);
    im_mosaic = im_green;
    
    % En las posiciones impares pone los valores AZULES (para filas y col
    % impares)
    im_mosaic_color(1:2:end, 1:2:end, 3) = im_blue(1:2:end, 1:2:end);
    im_mosaic(1:2:end, 1:2:end) = im_blue(1:2:end, 1:2:end);
    
    % En las posiciones pares pone los valores ROJOS (para filas y col
    % pares)
    im_mosaic_color(2:2:end, 2:2:end, 1) = im_red(2:2:end, 2:2:end);
    im_mosaic(2:2:end, 2:2:end) = im_red(2:2:end, 2:2:end);
    
    % Casteo para que quede todo en double
    im_mosaic = double(im_mosaic);
    
    % Casteo para mostrar la de color (la otra no hace falta)
    im_mosaic_color = uint8(im_mosaic_color);
end

