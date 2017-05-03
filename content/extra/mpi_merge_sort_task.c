/*
 * Николай Хохлов, k_h@inbox.ru, 2012.
 * Реализация алгоритма сортировки слиянием.
 * Изначально массив распределен между процессами, после работы сбор у 0.
 * Сбор всех данных и обработка у одного процесса.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

/*Вычисление i-й степени числа 2 с помощью побитового сдвига - нужно для слияния гиперкубом*/
int pow2(int i)
{
	return 1 << i;
}


/*виртуальные объявления функций, использующихся в main до из определения*/
void merge(int *a, int *b, int *c, int na, int nb);
void merge_sort(int *a, int na);
void print(int *a, int na);
int check_sort(int *a, int n);
double timer();
int n_count(int n, int rank, int size);

int main(int argc, char *argv[])
{
	int i, n, nlocal, npartner, *a, *b, *c;
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	double t;
	if (argc < 2) {
		if (rank == 0) {
			printf("Usage: %s num_elements.\n", argv[0]);
		}
		return 1;
	}
	n = atoi(argv[1]);//перевод строки в число
	nlocal = n_count(n, rank, size);
	a = (int*)malloc(sizeof(int) * nlocal);
	
	srand(time(NULL));
	for (i = 0; i < nlocal; i++) a[i] = rand() % 10;
	
	t = timer();
	merge_sort(a, nlocal);


	/* Начало сбора. */

	/*!!!Сбор фрагментов массива дописать самостоятельно!!!*/

	/* Конец сбора. */
	if (rank == 0) {
		t = timer() - t;
		if (n < 11) print(a, n);
		printf("Time: %f sec, sorted: %d\n", t, check_sort(a, n));
	}
	free(a);
	MPI_Finalize();
	return 0;
}

int n_count(int n, int rank, int size)
{
	return (n / size) + (n % size > rank);
}

double timer()//для измерения времени сортировки
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	return (double)ts.tv_sec + 1e-6 * (double)ts.tv_usec;
}

int check_sort(int *a, int n)//проверяет , отсортирован ли массив
{
	int i;
	for (i = 0; i < n - 1; i++) {
		if (a[i] > a[i+1]) {
			return 0;
		}
	}
	return 1;
}

void print(int *a, int na)//распечатывает массив
{
	int i;
	for (i = 0; i < na; i++) printf("%d ", a[i]);
	printf("\n");
}

/*
 * Процедура слияния массивов a и b в массив c.
 */



void merge(int *a, int *b, int *c, int na, int nb)
{
        int i = 0, j = 0;
        while (i < na && j < nb) {
                if (a[i] <= b[j]) {
                        c[i + j] = a[i];
                        i++;
                } else {
                        c[i + j] = b[j];
                        j++;
                }
        }
        if (i < na) {
                memcpy(c + i + j, a + i, (na - i) * sizeof(int));
        } else {
                memcpy(c + i + j, b + j, (nb - j) * sizeof(int));
        }
}


/*
 * Процедура сортировки слиянием.
 */
void merge_sort(int *a, int na)
{
/*!!! дописать самостоятельно !!!*/
}
