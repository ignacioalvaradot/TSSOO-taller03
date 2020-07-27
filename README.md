# TSSOO-taller03

##### Autor: Ignacio Alvarado Torreblanca - Correo: ignacio.alvarado@alumnos.uv.cl

## 1. Diseño de Solución

En la siguiente figura se puede observar un diagrama de secuencia que explica el funcionamiento de los procesos que se ejecutarán para llegar a la solución del problema.
![Ialvarado](http://imgfz.com/i/3h680Hk.png)

Para resolver el problema será necesario conocer el lenguaje de programación c ++ junto a OpenMP que se encarga de la paralelización del problema, como también los comandos para medir los tiempos de ejecución de las ejecuciones del programa los cuales serán necesarios para el posterior análisis de estos.

Una vez ejecutado el programa este debe comenzar a crear el arreglo vacío, con la cantidad de hilos y el total de elementos agregados en los parámetros al momento de la ejecución. Para la paralelización de los módulos se usarán funciones acorde a lo que pide cada módulo. El llenado se debe hacer mediante el encargado de generar números aleatorios en base a los límites propuestos a la hora de ejecutar el programa. Tras realizar el llenado del arreglo, se debe ejecutar el módulo de suma del arreglo
## 2. Estructura del Código

### 2.1 Parametros del script
Al usar el script desarrollado en bash, se pasan por parámetro distintos elementos que se necesitan para la ejecución del programa, para ingresar el tamaño del arreglo se ingresa después de "-N", para el numero de threads se ingresa después de  "-t", para el limite inferior del rango aleatorio se ingresa después de "-l", para el limite superior del rango aleatorio se ingresa después de "-L" y opcionalmente tenemos  "-h" para mostrar la forma de uso. A continuación se puede ver la variable que contiene la forma de uso del programa: 
```
const std::string descripcion = "Párametros:\n"
"\t-N : tamaño del arreglo.\n"
"\t-t : número de threads.\n"
"\t-l : limite inferior rango aleatorio.\n"
"\t-L : límite superior rango aleatorio.\n"
"\t[-h] : muestra la ayuda de uso y termina. \n";
```
En el archivo "checkArgs.hpp" están condicionadas todos los parámetros y condiciones de uso. En la siguientes lineas de código se puede ver como se almacenan los parámetros nombrados anteriormente :
```
checkArgs::checkArgs(int _argc , char **_argv){
parametros.tamProblema = 0;
parametros.numThreads = 0;
parametros.l_inferior = 0;
parametros.l_superior = 0;
argc = _argc;
argv = _argv;
}
```
### 2.2 Librerias
En el archivo "global.hh" se encuentran las librerias que son usadas para el diseño de los modulos que seran explicados luego.
```
#include <chrono>
#include <thread>
#include <omp.h>
```
Estas tres librerías son las fundamentales para que el programa funcione correctamente, la librería `chrono` permite medir el tiempo para realizar estadísticas de los módulos, `thread` permite generar hilos para luego comparar los tiempos de ejecución con la API OpenMP, y por ultimo esta  `omp.h`  que provee las funcionalidades de OpenMP al código.
### 2.3 Modulo de llenado en paralelo con OpenMP

 Para llenar el arreglo con números randomicos en los limites establecidos en los parámetros de entrada del programa con OpenMP, tenemos el siguiente codigo:

```
paralelo_openmp = new uint64_t[totalElementos];
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<> unif(l_inferior, l_superior);
	#pragma omp parallel for  num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){	
		paralelo_openmp[i] = unif(rng);
	}
	
```
Se tiene  el arreglo `paralelo_openmp` el cual contiene los números que son generados de forma randomica, estos números tienen como argumento a los limites que son ingresados al ejecutar el programa (`l_inferior`, `l_superior`), y se tiene  la funcion `#pragma omp parallel` que permite que el código dentro de el se ejecute de forma paralela con los hilos indicados con la variable `numThreads` que es pasada por parámetro al momento de ejecución.
### 2.4 Modulo de suma en paralelo con OpenMP.
En este modulo se pide sumar todos los numeros almacenados en un arreglo de forma paralela usando OpenMP, esto de puede ver en el siguiente código:
```
uint64_t sum_openmp = 0;
#pragma omp parallel for reduction(+:sum_openmp) num_threads(numThreads)
for(size_t i = 0; i < totalElementos; ++i){
	sum_openmp += paralelo[i];
}
```
Se tiene la variable `sum_openmp` la que contiene la suma de los valores del arreglo llamado `paralelo`. Finalmente se tiene la clausula `reduction`  que divide de forma atómica la variable `sum_openmp` para asociarla a los hilos dados por `numthreads` y luego de eso el for que recorrerá el arreglo y sumar los valores de este en la variable ya mencionada.

