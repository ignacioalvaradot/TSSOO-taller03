# TSSOO-taller03

##### Autor: Ignacio Alvarado Torreblanca - Correo: ignacio.alvarado@alumnos.uv.cl

## 1. Diseño de Solución

En la siguiente figura se puede observar un diagrama de secuencia que explica el funcionamiento de los procesos que se ejecutarán para llegar a la solución del problema.
![Ialvarado](http://imgfz.com/i/3h680Hk.png)

Para resolver el problema será necesario conocer el lenguaje de programación c ++ junto a las librerías que se encarguen de la implementación threads, como también los comandos para medir los tiempos de ejecución de las ejecuciones del programa los cuales serán necesarios para el posterior análisis de estos.

Una vez ejecutado el programa este debe comenzar a crear el arreglo vacío, con la cantidad de hilos y el total de elementos agregados en los parámetros al momento de la ejecución. Para crear los hilos por cada módulo, se van a añadir, con un ciclo For, los distintos segmentos de los hilos para luego unirlos. El llenado se debe hacer mediante el encargado de generar números aleatorios en base a los límites propuestos a la hora de ejecutar el programa. Tras realizar el llenado del arreglo, se debe ejecutar el módulo de suma del arreglo compuesto de la otra mitad de hilos instanciados.
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
### 2.2 Modulo de llenado en paralelo

 Para llenar el arreglo con números randomicos en los limites establecidos en los parámetros de entrada del programa, tenemos la siguiente función:

```
void fillArray(uint32_t l_superior, uint32_t l_inferior, size_t beginIndex,
size_t endIndex)
{
std::random_device device;
std::mt19937 rng(device());
std::uniform_int_distribution<> unif(l_inferior, l_superior);
for (size_t i = beginIndex; i < endIndex; ++i){
	paralelo[i] = unif(rng);
	}
}
```


### 2.3 Modulo de suma en paralelo.


