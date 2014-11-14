Para compilar este TP se utiliza la misma herramienta brindada por la c�tedra METNUM.PY. Solo se necesita hacer python metnum.py build y ya tendremos nuestro archivo ejecutable tp o tp.exe seg�n el sistema operativo que utilicemos.

MODO DE UTILIZACI�N DEL PROGRAMA:
./tp <ruta_a_archivo_de_imagen_con_mosaicing_pasado_a_texto> <tipo_de_filtro> <alpha>

Donde:
- ruta_a_archivo_de_imagen_con_mosaicing_pasado_a_texto hace referencia a la imagen que sale utilizando la funci�n de MATLAB "mosaicing" y grabando con imwrite en .tif sin compresi�n. Tambi�n se debe agregar a esta salida de MATLAB el alto y ancho en las primeras dos filas.
- tipo_de_filtro hace referencia al tipo de filtro a utilizar siguiendo el ENUM: 
enum FilterAlgorithmType { CLOSEST_NEIGHBOR, BILINEAR_INTERPOLATION, DIRECTIONAL_INTERPOLATION, MALVAR_HE_CUTLER }; // CLOSEST_NEIGHBOR = 0, BILINEAR_INTERPOLATION = 1, DIRECTIONAL_INTERPOLATION = 2, MALVAR_HE_CUTLER = 3
-alpha: par�metro que utiliza MALVAR_HE_CUTLER �nicamente. En el resto de los casos ser� ignorado si est� seteado.


NOTAS APARTE:

El an�lisis cuantitativo de la calidad se realiza tambi�n con MATLAB. La funci�n PSNR fue programada por nosotros y recibe dos im�genes y si deseamos analizar todas las capas o la capa verde. La funci�n de SSIM no fue programada por nosotros, y pueden ver los detalles en el informe o mismo en el c�digo de ssim.m. Originalmente quer�amos utilizar las funciones del toolbox de MATLAB que daban PSNR y SSIM pero estas est�n habilitadas �nicamente en MATLAB R2014a, una versi�n con la que no contamos.


M�S SOBRE MATLAB:
 - mosaicing.m realiza el mosaicing de una imagen. Devuelve una imagen en una capa con la informaci�n de los 3 canales toda superpuesta y una imagen con 3 canales para poder ver el efecto del bayer array.
 - mseerror.m calcula el error cuadr�tico m�ximo. Fue programada por nosotros.
 
 - save_quality.m graba las calidades PSNR Y SSIM de la imagen en dos archivos para la capa verde y todas las capas. Recibe el nombre de la imagen, la carpeta donde se encuentra, y a cuantos p�xeles de distancia del borde comienza la imagen filtrada.
 EJ: save_quality('img1', '../images_files/out/bilinear_interpolation/', 1);  (el primer pixel de la imagen filtrada est� en el (1,1) de la original)
	 save_quality('img1', '../images_files/out/malvar_he_cutler/', 2);  (el primer pixel de la imagen filtrada est� en el (2,2) de la original, ya que Malvar He Cutler necesita una ventana de 5x5 m�nimo para realizar la correcci�n del gradiente)
 
 - reconstruct_image.m es la funci�n que va a tomar la salida de nuestro programa para hacer de ella una imagen tif. Recibe el nombre de la imagen, la carpeta donde se encuentra, y si queremos guardarla a un archivo o simplemente devolverla.
 EJ: reconstruct_image('img1', '../images_files/out/closest_neighbor/', 1);
 
 - save_images_as_txt.m y reconstruct_and_save_quality_of_all.m son simplemente scripts que aceleran los procesos en batch de hacer mosaicing grabar imagen o reconstruir imagen a partir de 3 capas y grabar calidades.