Hay que añadir un integer uniform en el .cpp, que envie un 1 cuando pinte la vaca, y un 0 el resto del tiempo.
En el VS, se definen los colores para un gris brillante en las nuevas variables de material:
        matspecF = vec3(1,1,1);
        matdiffF = vec3(0.3, 0.3, 0.3);
        matambF = vec3(0.3, 0.3, 0.3);

Se crean nuevas variables para las componentes de material porque las originales son read-only.
También hay que modificar Lambert y Phong para que utilice estas nuevas.
Solo pinta la vaca, si el uniform integer pintaVaca recibido en el VS, es igual a 1.
