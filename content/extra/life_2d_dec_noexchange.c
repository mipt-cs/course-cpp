/*
 * Author: Nikolay Khokhlov <k_h@inbox.ru>, 2016
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>

#define ind(i, j) (((i + l->nx) % l->nx) + ((j + l->ny) % l->ny) * (l->nx))

#define gs 1
#define LIFE_SIZE ((l->lx + 2 * gs) * (l->ly + 2 * gs))
#define LIFE_CHECK_X(i) (((i) >= l->ox) && ((i) < l->ox + l->lx))
#define LIFE_CHECK_Y(j) (((j) >= l->oy) && ((j) < l->oy + l->ly))
#define LIFE_CHECK(i, j) LIFE_CHECK_X(i) && LIFE_CHECK_Y(j)
#define lind(i, j) (i + gs + (j + gs) * l->lx)
#define gind(i, j) lind(i - l->ox, j - l->oy)


typedef struct {
	int nx, ny;
	int *u0;
	int *u1;
	int steps;
	int save_steps;
	
	/* Переменные для MPI. */
	int ox, oy;
	int lx, ly;

	int rank;
	int coords[2];
	MPI_Comm comm;
	MPI_Comm cartcomm;
	int dims[2];
} life_t;

void life_mpi_init(const char *path, life_t *l, int dims[2], MPI_Comm c);
void life_mpi_collect(life_t *l, int **u, int rank_flag);
void life_init(const char *path, life_t *l);
void life_free(life_t *l);
void life_step(life_t *l);
void life_save_vtk(const char *path, life_t *l);
void life_data_save_vtk(const char *path, life_t *l, int *data);

void make_decomposition(const int n, const int size, const int rank, int *o, int *l);

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (argc != 2) {
		if (!rank) printf("Usage: %s input file.\n", argv[0]);
		MPI_Finalize();
		return 0;
	}
	if (!rank) printf("Running on %d CPUs.\n", size);
	int dims[2] = {0, 0};
	MPI_Dims_create(size, 2, dims);
	if (!rank) printf("Creating cart topology (%d, %d).\n", dims[0], dims[1]);
	
	life_t l;
	life_mpi_init(argv[1], &l, dims, MPI_COMM_WORLD);
	int i;
	char buf[100];
	for (i = 0; i < l.steps; i++) {
		if (i % l.save_steps == 0) {
			sprintf(buf, "life_%06d.vtk", i);
			if (!l.rank) printf("Saving step %d to '%s'.\n", i, buf);
			int *u = NULL;
			life_mpi_collect(&l, &u, 0);
			if (!l.rank) life_data_save_vtk(buf, &l, u);
			if (u) free(u);
		}
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

void life_mpi_init(const char *path, life_t *l, int dims[2], MPI_Comm c)
{
	l->comm = c;
	l->dims[0] = dims[0];
	l->dims[1] = dims[1];
	FILE *fd = fopen(path, "r");
	assert(fd);
	assert(fscanf(fd, "%d\n", &l->steps));
	assert(fscanf(fd, "%d\n", &l->save_steps));
	assert(fscanf(fd, "%d %d\n", &l->nx, &l->ny));


	/* Создаем декартову топологию. */
	int periods[2] = {1, 1};
	int reorder = 0;
	MPI_Cart_create(l->comm, 2, dims, periods, reorder, &l->cartcomm);
	
	MPI_Comm_rank(l->cartcomm, &l->rank);
	MPI_Cart_coords(l->cartcomm, l->rank, 2, l->coords);
	if (!l->rank) printf("Steps %d, save every %d step.\n", l->steps, l->save_steps);
	if (!l->rank) printf("Field size: %dx%d\n", l->nx, l->ny);
	
	make_decomposition(l->nx, dims[0], l->coords[0], &l->ox, &l->lx);
	make_decomposition(l->ny, dims[1], l->coords[1], &l->oy, &l->ly);
	printf("My rank %d, my coords (%d, %d), my origin (%d, %d), my local size (%d, %d).\n", l->rank, l->coords[0], l->coords[1], l->ox, l->oy, l->lx, l->ly);
	
	l->u0 = (int*)calloc(LIFE_SIZE, sizeof(int));
	l->u1 = (int*)calloc(LIFE_SIZE, sizeof(int));
	
	int i, j, r, lcnt, cnt;
	lcnt = 0;
	while ((r = fscanf(fd, "%d %d\n", &i, &j)) != EOF) {
		if (LIFE_CHECK(i, j)) {
			l->u0[gind(i, j)] = 1;
			lcnt++;
		}
	}
	MPI_Reduce(&lcnt, &cnt, 1, MPI_INT, MPI_SUM, 0, l->cartcomm);
	if (!l->rank) printf("Loaded %d life cells.\n", cnt);
	
	fclose(fd);
}

void life_mpi_collect(life_t *l, int **u, int rank_flag)
{
	int *ul = l->u0;
	if (rank_flag) {
		ul = (int*)malloc(LIFE_SIZE * sizeof(int));
		int i;
		for (i = 0; i < LIFE_SIZE; i++) ul[i] = l->rank;
	}
	if (l->rank) {
		MPI_Send(ul, LIFE_SIZE, MPI_INT, 0, 0, l->cartcomm);
	} else {
		(*u) = (int*)malloc(sizeof(int) * l->nx * l->ny);
		int i, j, p1, p2;
		for (j = 0; j < l->ly; j++)
			for (i = 0; i < l->lx; i++)
				(*u)[ind(i+l->ox, j+l->oy)] = ul[lind(i, j)];
		for (p1 = 0; p1 < l->dims[0]; p1++)
		for (p2 = 0; p2 < l->dims[1]; p2++) {
			int c[2] = {p1, p2};
			int r, ox, oy, lx, ly;
			MPI_Cart_rank(l->cartcomm, c, &r);
			if (r != l->rank) {
				make_decomposition(l->nx, l->dims[0], p1, &ox, &lx);
				make_decomposition(l->ny, l->dims[1], p2, &oy, &ly);
				int ls = ((lx+2)*(ly+2));
				int *t = (int*)malloc(sizeof(int) * ls);
				MPI_Recv(t, ls, MPI_INT, r, 0, l->cartcomm, MPI_STATUS_IGNORE);
				for (j = 0; j < ly; j++)
				for (i = 0; i < lx; i++)
					(*u)[ind(i+ox, j+oy)] = t[i + gs + (j + gs) * lx];
				free(t);
			}
		}
	}
	if (rank_flag) free(ul);
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

void life_data_save_vtk(const char *path, life_t *l, int *data)
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
			fprintf(f, "%d\n", data[ind(i1, i2)]);
		}
	}
	fclose(f);
}

void life_step(life_t *l)
{
	int i, j;
	for (j = 0; j < l->ly; j++) {
		for (i = 0; i < l->lx; i++) {
			int n = 0;
			n += l->u0[lind(i+1, j)];
			n += l->u0[lind(i+1, j+1)];
			n += l->u0[lind(i,   j+1)];
			n += l->u0[lind(i-1, j)];
			n += l->u0[lind(i-1, j-1)];
			n += l->u0[lind(i,   j-1)];
			n += l->u0[lind(i-1, j+1)];
			n += l->u0[lind(i+1, j-1)];
			l->u1[lind(i,j)] = 0;
			if (n == 3 && l->u0[lind(i,j)] == 0) {
				l->u1[lind(i,j)] = 1;
			}
			if ((n == 3 || n == 2) && l->u0[lind(i,j)] == 1) {
				l->u1[lind(i,j)] = 1;
			}
		}
	}
	int *tmp;
	tmp = l->u0;
	l->u0 = l->u1;
	l->u1 = tmp;
}

void make_decomposition(const int n, const int size, const int rank, int *o, int *l)
{
	int pp = n / size;
	*l = pp + (n % size > rank);
	*o = pp * rank;
	if (rank > n % size) {
		*o += n % size;
	} else {
		*o += rank;
	}
}

