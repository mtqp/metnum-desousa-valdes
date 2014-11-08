@echo off

SET alpha=0.5
REM si anduvieran los dos fors............
FOR /L %%G IN (1,1,12) DO (
	tp.exe "images_files/img%%G" 0 0.5
	move images_files\img%%G_out_* images_files\out\closest_neighbor\
	tp.exe "images_files/img%%G" 1 0.5
	move images_files\img%%G_out_* images_files\out\bilinear_interpolation\
	tp.exe "images_files/img%%G" 2 0.5
	move images_files\img%%G_out_* images_files\out\directional_interpolation\
	tp.exe "images_files/img%%G" 3 0.5
	move images_files\img%%G_out_* images_files\out\malvar_he_cutler\
)
GOTO :eof