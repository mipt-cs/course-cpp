Параллельные сортировки 
#######################

:date: 2017-03-09 09:00


.. default-role:: code
.. contents:: Содержание


Постановка задачи
=================

На прошлом занятии вам было предложено распараллелить сортировку пузырьком как пример алгоритма, для которого распараллеливание приводит к снижению быстродействия, а не к возрастанию(!). В этот раз рассмотрим варианты <<правильного>> распараллеливания сортировок.
Итак, требуется расположить в порядке не убывания n элементов массива чисел, равномерно
размещенных на p процессорах. По окончании сортировки на процессорах с меньшими
номерами должны быть размещены элементы массива с меньшими значениями. 
Рассматриваемые далее параллельные алгоритмы предполагают двухэтапную
сортировку: 

* последовательную сортировку фрагментов массива, распределенных по процессорам системы;
* объединение упорядоченных фрагментов массива – перемещение элементов массива между процессорами.

Для уменьшения общего времени выполнения сортировки следует по
возможности сократить время выполнения каждого из указанных этапов, поэтому в
начале обсуждается последовательный алгоритм, показавший наименьшее время
сортировки тестовых массивов. Именно относительно этого алгоритма определяется
эффективность алгоритмов параллельной сортировки. Таким образом, в работе
используется метод определения эффективности относительно «наилучшего» из
имеющихся в распоряжении последовательных алгоритмов сортировки.


Параллельные алгоритмы сортировки
=================================

Один из примеров сортировки без использование обмена сообщениями.

.. code-block:: c

	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <mpi.h>

	void merge(int *, int *, int, int, int);
	void mergeSort(int *, int *, int, int);

	int main(int argc, char** argv) {
	
		/********** Create and populate the array **********/
		int n = atoi(argv[1]);
		int *original_array = malloc(n * sizeof(int));
	
		int c;
		srand(time(NULL));
		printf("This is the unsorted array: ");
		for(c = 0; c < n; c++) {
		
			original_array[c] = rand() % n;
			printf("%d ", original_array[c]);
		
			}

		printf("\n");
		printf("\n");
	
		/********** Initialize MPI **********/
		int world_rank;
		int world_size;
	
		MPI_INIT(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		
		/********** Divide the array in equal-sized chunks **********/
		int size = n/world_size;
	
		/********** Send each subarray to each process **********/
		int *sub_array = malloc(size * sizeof(int));
		MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);
	
		/********** Perform the mergesort on each process **********/
		int *tmp_array = malloc(size * sizeof(int));
		mergeSort(sub_array, tmp_array, 0, (size - 1));
	
		/********** Gather the sorted subarrays into one **********/
		int *sorted = NULL;
		if(world_rank == 0) {
		
			sorted = malloc(n * sizeof(int));
		
			}
	
		MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);
	
		/********** Make the final mergeSort call **********/
		if(world_rank == 0) {
		
			int *other_array = malloc(n * sizeof(int));
			mergeSort(sorted, other_array, 0, (n - 1));
		
			/********** Display the sorted array **********/
			printf("This is the sorted array: ");
			for(c = 0; c < n; c++) {
			
				printf("%d ", sorted[c]);
			
				}
			
			printf("\n");
			printf("\n");
			
			/********** Clean up root **********/
			free(sorted);
			free(other_array);
			
			}
	
		/********** Clean up rest **********/
		free(original_array);
		free(sub_array);
		free(tmp_array);
	
		/********** Finalize MPI **********/
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Finalize();
	
		}

	/********** Merge Function **********/
	void merge(int *a, int *b, int l, int m, int r) {
	
		int h, i, j, k;
		h = l;
		i = l;
		j = m + 1;
	
		while((h <= m) && (j <= r)) {
		
			if(a[h] <= a[j]) {
			
				b[i] = a[h];
				h++;
			
				}
			
			else {
			
				b[i] = a[j];
				j++;
			
				}
			
			i++;
		
			}
		
		if(m < h) {
		
			for(k = j; k <= r; k++) {
			
				b[i] = a[k];
				i++;
			
				}
			
			}
		
		else {
		
			for(k = h; k <= m; k++) {
			
				b[i] = a[k];
				i++;
			
				}
			
			}
		
		for(k = l; k <= r; k++) {
		
			a[k] = b[k];
		
			}
		
		}

	/********** Recursive Merge Function **********/
	void mergeSort(int *a, int *b, int l, int r) {
	
		int m;
	
		if(l < r) {
		
			m = (l + r)/2;
		
			mergeSort(a, b, l, m);
			mergeSort(a, b, (m + 1), r);
			merge(a, b, l, m, r);
		
			}
		
		}


Рассмотрим два параллельных алгоритма сортировки массивов. Первый
разработан на основе метода сдваивания, второй - на основе «обменной сортировки со
слиянием» Бэтчера.


Их описание см. в следующей статье__.

.. __: {filename}/extra/ParallelSort.pdf


(см. стр. 9-16)

Задание 1
=========

Реализовать алгоритм сдваивания, вычислить время работы на 4,16,28 процессах. Сравнить с qsort на разных входах:

* Случайная последовательность
* Последовательность чисел, отсортированных в обратном порядке

Задание 2
=========

То же самое для алгоритма обменной сортировки со слиянием. 
Найти для него оптимальную сеть для 4, 16, 28 процессов.


