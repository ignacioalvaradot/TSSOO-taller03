#include <global.hh>
#include <checkArgs.hpp>

uint64_t *paralelo = nullptr;
uint64_t *paralelo_openmp = nullptr;
uint64_t *sum_parciales = nullptr;
uint64_t *mismo = nullptr;
uint64_t sum_secuencial1 = 0;

void sum_parcial(size_t pos,
				 size_t beginIndex,
				 size_t endIndex)
{
	for (size_t i = beginIndex; i < endIndex; ++i)
	{
		sum_parciales[pos] += paralelo[i];
	}
}

void sum_secuencial(size_t beginIndex,
				 size_t endIndex)
{
	for (size_t i = beginIndex; i < endIndex; ++i)
	{
		sum_secuencial1 += paralelo[i];
	}
}
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
int main(int argc, char **argv)
{
	uint64_t band = 0;
	uint64_t totalElementos;
	uint32_t numThreads;
	uint32_t l_superior;
	uint32_t l_inferior;
	std::vector<std::thread *> threads;
	std::vector<std::thread *> threads2;
	auto argumentos = (std::shared_ptr<checkArgs>)new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	l_superior = argumentos->getArgs().l_superior;
	l_inferior = argumentos->getArgs().l_inferior;

	// LLENADO DEL ARREGLO DE FORMA SECUENCIAL
	paralelo = new uint64_t[totalElementos];
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<> unif(l_inferior, l_superior);
	auto start1 = std::chrono::system_clock::now();
	
	fillArray(l_superior, l_inferior, 0, totalElementos);
	
	auto end1 = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed = end1 - start1;
	auto totalTimeFill_secuencial = elapsed.count();

	delete[] paralelo;

	//LLENADO DEL ARREGLO EN PARALELO OCUPANDO THREADS
	paralelo = new uint64_t[totalElementos];

	for (size_t i = 0; i < numThreads; ++i)
	{

		threads.push_back(new std::thread(fillArray,
										  l_superior, l_inferior,
										  i * (totalElementos) / numThreads,
										  (i + 1) * (totalElementos) / numThreads));
	}
	auto start = std::chrono::system_clock::now();
	for (auto &ths : threads)
	{
		ths->join();
	}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> duration = end - start;
	auto tiempo_arreglo = duration.count();

		//SUMA EN PARALELO DEL ARREGLO OCUPANDO THREADS
	sum_parciales = new uint64_t[totalElementos];
	for (size_t i = 0; i < numThreads; ++i)
	{

		threads2.push_back(new std::thread(sum_parcial, i,
										   i * (totalElementos) / numThreads,
										   (i + 1) * (totalElementos) / numThreads));
	}
	start = std::chrono::system_clock::now();
	for (auto &th : threads2)
	{
		th->join();
	}
	// AGREGANDO CADA SUMA DE LOS THREADS A UNA VARIABLE
	for (size_t i = 0; i < numThreads; ++i)
	{
		band += sum_parciales[i];
	}
	end = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> duration6 = end - start;
	auto tiempo_suma = duration6.count();
	

		// LLENADO DEL ARREGLO CON OPENMP
	paralelo_openmp = new uint64_t[totalElementos];
	auto start3 = std::chrono::system_clock::now();
	#pragma omp parallel for  num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){	
		paralelo_openmp[i] = unif(rng);
	}
	
	auto end3 = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed4 = end3 - start3;
	auto totalTimeFill_OPENMP = elapsed4.count();



		//SUMA SECUENCIAL
	auto start2 = std::chrono::system_clock::now();
	sum_secuencial(0, totalElementos);
	auto end2 = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed2 = end2 - start2;
	auto tiempo_suma_secuencial = elapsed2.count();

	//SUMA PARALELA CON OPENMP
	uint64_t sum_openmp = 0;
	auto start5 = std::chrono::system_clock::now();
	#pragma omp parallel for reduction(+:sum_openmp) num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){
		sum_openmp += paralelo[i];
	}
	auto end5 = std::chrono::system_clock::now(); 
	std::chrono::duration<float, std::milli> elapsed5 = end5 - start5;
	auto tiempo_suma_openmp = elapsed5.count();
	
	std::cout << "Elementos: " << totalElementos << std::endl;
	std::cout << "Threads: " << numThreads << std::endl;
	std::cout << "limite inferior: " << l_inferior << std::endl;
	std::cout << "limite Superior: " << l_superior << std::endl;
	std::cout << "Suma de elementos del arreglo con OpenMP: " <<  sum_openmp<< std::endl;
	std::cout << "Suma de elementos del arreglo con threads: " << band << std::endl;
	std::cout << "Suma de elementos del arreglo secuencial: " <<  sum_secuencial1<< std::endl;
	std::cout << "-------Tiempos modulo suma ------" << std::endl;
	std::cout << "Tiempo de suma de los elementos con OpenMP: " <<  tiempo_suma_openmp << std::endl;
	std::cout << "Tiempo de suma de los elementos con threads: " << tiempo_suma << std::endl;
	std::cout << "Tiempo de suma de los elementos secuencial: " << tiempo_suma_secuencial << std::endl;
	std::cout << "Aceleracion  etapa de Suma `Threads v/s Secuencial`: " << (double)tiempo_suma_secuencial/tiempo_suma << std::endl;
	std::cout << "Aceleracion  etapa de Suma `OpenMP v/s Secuencial`: " << (double)tiempo_suma_secuencial/tiempo_suma_openmp << std::endl;
	std::cout << "-------Tiempos modulo Llenado -------" << std::endl;
	std::cout << "Tiempo de llenado con OpenMP: " << totalTimeFill_OPENMP << std::endl;
	std::cout << "Tiempo de llenado con threads: " << tiempo_arreglo << std::endl;
	std::cout << "Tiempo llenado secuencial: " << totalTimeFill_secuencial << std::endl;
	std::cout << "Aceleracion etapa de Llenado ´Threads v/s Secuencial´: " << (double)totalTimeFill_secuencial/tiempo_arreglo << std::endl;
	std::cout << "Aceleracion etapa de Llenado ´OpenMP v/s Secuencial´: " << (double)totalTimeFill_secuencial/totalTimeFill_OPENMP << std::endl;
	return (EXIT_SUCCESS);
}

