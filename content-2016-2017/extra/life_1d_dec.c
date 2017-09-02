/*
 * Author: Nikolay Khokhlov <k_h@inbox.ru>, 2016
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>

#define ind(i, j) (((i + l->nx) % l->nx) + ((j + l->ny) % l->ny) * (l->nx))
#define indp(i, j) (((i + l.nx) % l.nx) + ((j + l.ny) % l.ny) * (l.nx))

#define kp(k) ((k+l.p) % l.p)

typedef struct {
	int nx, ny;
	int *u0;
	int *u1;
	int steps;
	int save_steps;
	
	int sk, fk; /* Зона ответственности k-го процесса, по оси y */
	int k; /* Номер текущего процесса. */
	int p; /* Число процессов. */
} life_t;

void life_init(const char *path, life_t *l);
void life_free(life_t *l);
void life_step(life_t *l);
void life_save_vtk(const char *path, life_t *l);
void dec(const int k, const int n, const int p, int *sk, int *fk);

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	if (argc != 2) {
		printf("Usage: %s input file.\n", argv[0]);
		return 0;
	}
	
	life_t l;
	life_init(argv[1], &l);
	
	
	MPI_Comm_size(MPI_COMM_WORLD, &l.p);
	MPI_Comm_rank(MPI_COMM_WORLD, &l.k);
	dec(l.k, l.ny, l.p, &l.sk, &l.fk);
	printf("k=%d, p=%d, ny=%d, sk=%d, fk=%d\n", l.k, l.p, l.ny, l.sk, l.fk);
	
	int i, k, sk, fk;
	char buf[100];
	MPI_Request rq[4];
	for (i = 0; i < l.steps; i++) {
		if (i % l.save_steps == 0) {
			if (l.k > 0) {
				MPI_Send(&l.u0[indp(0, l.sk)], l.nx*(l.fk-l.sk), MPI_INT, 0, 0, MPI_COMM_WORLD);
			} else {
				for (k = 1; k < l.p; k++) {
					dec(k, l.ny, l.p, &sk, &fk);
					MPI_Recv(&l.u0[indp(0, sk)], l.nx*(fk-sk), MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
				sprintf(buf, "life_%06d.vtk", i);
				printf("Saving step %d to '%s'.\n", i, buf);
				life_save_vtk(buf, &l);
			}
		}
		
		/* Обмен. */
		MPI_Irecv(&l.u0[indp(0, l.sk-1)], l.nx, MPI_INT, kp(l.k-1), 0, MPI_COMM_WORLD, &rq[0]);
		MPI_Isend(&l.u0[indp(0, l.sk)], l.nx, MPI_INT, kp(l.k-1), 1, MPI_COMM_WORLD, &rq[1]);
		MPI_Isend(&l.u0[indp(0, l.fk-1)], l.nx, MPI_INT, kp(l.k+1), 0, MPI_COMM_WORLD, &rq[2]);
		MPI_Irecv(&l.u0[indp(0, l.fk)], l.nx, MPI_INT, kp(l.k+1), 1, MPI_COMM_WORLD, &rq[3]);
		MPI_Waitall(4, rq, MPI_STATUSES_IGNORE);
		
		life_step(&l);
	}
	
	life_free(&l);
	MPI_Finalize();
	return 0;
}

/**
 * Загрузить входную конфигурацию.
 * Формат файла, число шагов, как часто сохранять, размер поля, затем идут координаты заполненых клеток:
 * steps
 * save_steps
 * nx ny
 * i1 j2
 * i2 j2
 */
void life_init(const char *path, life_t *l)
{
	FILE *fd = fopen(path, "r");
	assert(fd);
	assert(fscanf(fd, "%d\n", &l->steps));
	assert(fscanf(fd, "%d\n", &l->save_steps));
	printf("Steps %d, save every %d step.\n", l->steps, l->save_steps);
	assert(fscanf(fd, "%d %d\n", &l->nx, &l->ny));
	printf("Field size: %dx%d\n", l->nx, l->ny);

	l->u0 = (int*)calloc(l->nx * l->ny, sizeof(int));
	l->u1 = (int*)calloc(l->nx * l->ny, sizeof(int));
	
	int i, j, r, cnt;
	cnt = 0;
	while ((r = fscanf(fd, "%d %d\n", &i, &j)) != EOF) {
		l->u0[ind(i, j)] = 1;
		cnt++;
	}
	printf("Loaded %d life cells.\n", cnt);
	fclose(fd);
}

void life_free(life_t *l)
{
	free(l->u0);
	free(l->u1);
	l->nx = l->ny = 0;
}

void life_save_vtk(const char *path, life_t *l)
{
	FILE *f;
	int i1, i2, j;
	f = fopen(path, "w");
	assert(f);
	fprintf(f, "# vtk DataFile Version 3.0\n");
	fprintf(f, "Created by write_to_vtk2d\n");
	fprintf(f, "ASCII\n");
	fprintf(f, "DATASET STRUCTURED_POINTS\n");
	fprintf(f, "DIMENSIONS %d %d 1\n", l->nx+1, l->ny+1);
	fprintf(f, "SPACING %d %d 0.0\n", 1, 1);
	fprintf(f, "ORIGIN %d %d 0.0\n", 0, 0);
	fprintf(f, "CELL_DATA %d\n", l->nx * l->ny);
	
	fprintf(f, "SCALARS life int 1\n");
	fprintf(f, "LOOKUP_TABLE life_table\n");
	for (i2 = 0; i2 < l->ny; i2++) {
		for (i1 = 0; i1 < l->nx; i1++) {
			fprintf(f, "%d\n", l->u0[ind(i1, i2)]);
		}
	}
	fclose(f);
}

void life_step(life_t *l)
{
	int i, j;
	for (j = l->sk; j < l->fk; j++) {
		for (i = 0; i < l->nx; i++) {
			int n = 0;
			n += l->u0[ind(i+1, j)];
			n += l->u0[ind(i+1, j+1)];
			n += l->u0[ind(i,   j+1)];
			n += l->u0[ind(i-1, j)];
			n += l->u0[ind(i-1, j-1)];
			n += l->u0[ind(i,   j-1)];
			n += l->u0[ind(i-1, j+1)];
			n += l->u0[ind(i+1, j-1)];
			l->u1[ind(i,j)] = 0;
			if (n == 3 && l->u0[ind(i,j)] == 0) {
				l->u1[ind(i,j)] = 1;
			}
			if ((n == 3 || n == 2) && l->u0[ind(i,j)] == 1) {
				l->u1[ind(i,j)] = 1;
			}
		}
	}
	int *tmp;
	tmp = l->u0;
	l->u0 = l->u1;
	l->u1 = tmp;
}

void dec(const int k, const int n, const int p, int *sk, int *fk)
{
	*sk = (n / p) * k;
	*fk = *sk + n / p;
	if (k == p - 1) *fk = n;
}


