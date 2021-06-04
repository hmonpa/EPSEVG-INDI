Modificación de la función viewTransform para realizar el cambio en la posición de la cámara (OBS) y objetivo (VRP).
También se modifican zNear y zFar, los cuales son:
	- zN: 0.1
	- zF: 0.1 + (zF - zN)


Se modifica correctamente el ra, en los ejercicios anteriores la formula era subóptima:
  if (ra < 1.0) FOV = 2.0 * atan(tan(0.5*FOV_orig)/ra);
  else FOV = FOV_orig;
