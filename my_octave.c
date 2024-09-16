//Copyright Popescu Mihnea-Gabriel 312CAb 2023-2024
#include <stdio.h>
#include <stdlib.h>
#define MOD 10007

int **alloc_matrix(int n, int m)
{
	int **a;
	a = malloc(n * sizeof(int *));
	if (!a) {
		fprintf(stderr, "Eroare la alocarea memoriei.\n");
		exit(-1);
	}
	for (int i = 0; i < n; i++) {
		a[i] = malloc(m * sizeof(int));
		if (!a[i]) {
			fprintf(stderr, "Eroare la alocarea memoriei.\n");
			exit(-1);
			for (int j = i - 1; j >= 0; j--)
				free(a[j]);
			free(a);
			exit(-1);
		}
	}
	return a;
}

void make_rows_columns(int dim, int **rows, int x, int **columns, int y)
{
	(*rows) = realloc(*rows, (dim + 1) * sizeof(int));
	(*columns) = realloc(*columns, (dim + 1) * sizeof(int));
	if (!rows) {
		fprintf(stderr, "Eroare la realocarea memoriei pentru linii\n");
		exit(-1);
	}
	if (!columns) {
		fprintf(stderr, "Eroare la realocarea memoriei pentru coloane\n");
		exit(-1);
	}
	(*rows)[dim] = x;
	(*columns)[dim] = y;
}

void read_matrix(int n, int m, int **a)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			scanf("%d", &a[i][j]);
	}
}

void print_matrix(int *rows, int *columns, int ***matrix, int index, int dim)
{
	if (index >= 0 && index < dim) {
		for (int i = 0; i < rows[index]; i++) {
			for (int j = 0; j < columns[index]; j++)
				printf("%d ", matrix[index][i][j]);
			printf("\n");
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

int **tsp(int n, int m, int **a, int *rows, int *columns, int index)
{
	int mx;
	if (n > m)
		mx = n;
	else
		mx = m;
	a = realloc(a, mx * sizeof(int *));
	if (!a) {
		fprintf(stderr, "Eroare la realocarea memoriei\n");
		exit(-1);
	}
	for (int i = n; i < mx; i++)
		a[i] = NULL;
	for (int i = 0; i < mx; i++) {
		a[i] = realloc(a[i], mx * sizeof(int));
		if (!a[i]) {
			fprintf(stderr, "Eroare la realocarea memoriei\n");
			exit(-1);
		}
	}
	/*Interschimb elementele de pe linia i coloana j
	cu cele de pe linia j coloana i*/
	for (int i = 0; i < mx; i++) {
		for (int j = i + 1; j < mx; j++) {
			int aux = a[i][j];
			a[i][j] = a[j][i];
			a[j][i] = aux;
		}
	}
	//Interschimb numarul de linii cu numarl de coloane
	int tmp = rows[index];
	rows[index] = columns[index];
	columns[index] = tmp;
	for (int i = rows[index]; i < mx; i++)
		free(a[i]);
	a = realloc(a, rows[index] * sizeof(int *));
	for (int i = 0; i < rows[index]; i++)
		a[i] = realloc(a[i], columns[index] * sizeof(int));
	return a;
}

int **raise(int ***a, int power, int *rows, int index,
			int nrmat, int *columns)
{
	if (index < 0 || index >= nrmat) {
		printf("No matrix with the given index\n");
		return NULL;
	}
	if (power < 0) {
		printf("Power should be positive\n");
		return NULL;
	}
	if (rows[index] != columns[index]) {
		printf("Cannot perform matrix multiplication\n");
		return NULL;
	}
	int n = rows[index];
	int **ans = alloc_matrix(n, n);
	int **temporary = alloc_matrix(n, n);
	//Initializam matricea rezultat cu matricea identitate
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j)
				ans[i][j] = 1;
			else
				ans[i][j] = 0;
		}
	}
	//Aplicam exponentierea in timp logaritmic
	while (power > 0) {
		if (power % 2 == 1) {
			/*Daca puterea este impara, inmultim matricea rezultat
			cu matricea initiala*/
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					temporary[i][j] = 0;
					for (int k = 0; k < n; k++)
						temporary[i][j] += (ans[i][k] * a[index][k][j]) % MOD;
					temporary[i][j] = (temporary[i][j] % MOD + MOD) % MOD;
				}
			}
			//Copiem rezultatul in matricea rezultat
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++)
					ans[i][j] = temporary[i][j];
			}
		}
		//Ridicam matricea la patrat pentru a imparti puterea la 2
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				temporary[i][j] = 0;
				for (int k = 0; k < n; k++)
					temporary[i][j] += (a[index][i][k] * a[index][k][j]) % MOD;
				temporary[i][j] = (temporary[i][j] % MOD + MOD) % MOD;
			}
		}
		//Copiem rezultatul in matricea a
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				a[index][i][j] = temporary[i][j];
		}
		//Impartim puterea la 2
		power /= 2;
	}
	for (int i = 0; i < n; i++)
		free(a[index][i]);
	free(a[index]);
	for (int i = 0; i < n; i++)
		free(temporary[i]);
	free(temporary);
	return ans;
}

int sum_of_elements(int **a, int n, int m)
{
	int s = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0 ; j < m; j++) {
			s += a[i][j] % MOD;
			s = ((s % MOD) + MOD) % MOD;
		}
	}
	return s;
}

void sort(int ***matrix, int *rows, int *columns, int nr)
{
	int *sume = malloc(nr * sizeof(int));
	if (!sume) {
		fprintf(stderr, "Eroare la alocarea memoriei.\n");
		exit(-1);
	}
	//Ordonam crescator in functie de suma elementelor
	for (int i = 0; i < nr; i++)
		sume[i] = sum_of_elements(matrix[i], rows[i], columns[i]);
	for (int i = 0; i < nr; i++) {
		for (int j = i + 1; j < nr; j++) {
			if (sume[i] > sume[j]) {
				int aux = sume[i];
				sume[i] = sume[j];
				sume[j] = aux;
				int aux1 = rows[i];
				rows[i] = rows[j];
				rows[j] = aux1;
				int aux2 = columns[i];
				columns[i] = columns[j];
				columns[j] = aux2;
				int **tmp = matrix[i];
				matrix[i] = matrix[j];
				matrix[j] = tmp;
			}
		}
	}
	free(sume);
}

void delete(int ****matrix, int *n, int index, int **rows,
			int **columns, int *size, int *dim)
{
	if (index < 0 || index >= (*n)) {
		printf("No matrix with the given index\n");
	} else {
		//Dealocarea memoriei pentru matricea de pe pozitia p
		for (int i = 0; i < (*rows)[index]; i++)
			free((*matrix)[index][i]);
		free((*matrix)[index]);
		/*Mutarea matricelor urmatoare pentru a umple golul
		lasat de matricea stearsa*/
		for (int i = index; i < (*n) - 1; i++) {
			(*matrix)[i] = (*matrix)[i + 1];
			(*rows)[i] = (*rows)[i + 1];  //Actualizare indice linii
			(*columns)[i] = (*columns)[i + 1];  //Actualizare indice coloane
		}
		/*Realocarea memoriei pentru vectorul de matrice
		Redimensionarea sa*/
		*n = *n - 1;
		*dim = *dim - 1;
		if ((*n) <= (*size) / 2) {
			*size = (*size) / 2;
			*matrix = (int ***)realloc(*matrix, (*size) * sizeof(int **));
			*rows = (int *)realloc(*rows, (*size) * sizeof(int));
			*columns = (int *)realloc(*columns, (*size) * sizeof(int));
		}
	}
}

int **crop(int ***matrix, int index, int *rows, int *colo, int nrmat)
{
	int x, y;
	scanf("%d", &x);
	int *liniisel;
	int *coloanesel;
	//Citim vectorii de linii si coloane selectate
	liniisel = (int *)malloc(x * sizeof(int));
	for (int i = 0; i < x; i++)
		scanf("%d", &liniisel[i]);
	scanf("%d", &y);
	coloanesel = (int *)malloc(y * sizeof(int));
	for (int i = 0; i < y; i++)
		scanf("%d", &coloanesel[i]);
	if (index < 0 || index >= nrmat) {
		printf("No matrix with the given index\n");
		free(liniisel);
		free(coloanesel);
		return NULL;
	}
	int **a = matrix[index];
	int **nouamatrice = (int **)malloc(x * sizeof(int *));
	for (int i = 0; i < x; i++)
		nouamatrice[i] = (int *)malloc(y * sizeof(int));
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++)
			nouamatrice[i][j] = a[liniisel[i]][coloanesel[j]];
	}
	for (int i = 0; i < rows[index]; i++)
		free(a[i]);
	free(a);
	rows[index] = x;
	colo[index] = y;
	free(liniisel);
	free(coloanesel);
	return nouamatrice;
}

void sum(int **a, int **b, int **mat, int n)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			mat[i][j] = (a[i][j] + b[i][j]) % MOD;
			mat[i][j] = (mat[i][j] % MOD + MOD) % MOD;
		}
	}
}

void substract(int **a, int **b, int **mat, int n)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			mat[i][j] = (a[i][j] - b[i][j]) % MOD;
			mat[i][j] = (mat[i][j] % MOD + MOD) % MOD;
		}
	}
}

void strassen(int **A, int **B, int **C, int m)
{
	if (m == 1) {
		C[0][0] = (A[0][0] * B[0][0]) % MOD;
		C[0][0] = ((C[0][0] % MOD) + MOD) % MOD;
	} else {
		int n = m / 2;
		int ***stras;
		stras = (int ***)malloc(21 * sizeof(int **));
		for (int i = 0; i < 21; i++)
			stras[i] = alloc_matrix(n, n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				stras[0][i][j] = A[i][j]; //A1
				stras[1][i][j] = A[i][n + j];//A2
				stras[2][i][j] = A[i + n][j];//A3
				stras[3][i][j] = A[i + n][j + n];//A4
				stras[4][i][j] = B[i][j];//B1
				stras[5][i][j] = B[i][n + j];//B2
				stras[6][i][j] = B[i + n][j];//B3
				stras[7][i][j] = B[i + n][j + n];//B4
			}
		}
		/*Suma o stochez in 8 sau 9
		Diferenta o stochez in 8
		Inmultirile M1 M2 M3 M4 M5 M6 M7 in (10, 11, 12, 13, 14, 15, 16)
		Blocurile finale C1,C2,C3,C4 in (17, 18, 19, 20);*/
		sum(stras[0], stras[3], stras[8], n);//stras[8] -> (A1+A4)
		sum(stras[4], stras[7], stras[9], n);//stras[9] -> (B1+B4)
		strassen(stras[8], stras[9], stras[10], n);
		//stras[10]->(A1+A4)*(B1*B2)	M1
		sum(stras[2], stras[3], stras[8], n);//stras[8] -> (A3+A4)
		strassen(stras[8], stras[4], stras[11], n);
		//stras[11] -> (A3+A4)*B1	M2
		substract(stras[5], stras[7], stras[9], n);//stras[9] -> (B2-B4)
		strassen(stras[0], stras[9], stras[12], n);
		//stras[12] -> A1*(B2-B4)	M3
		substract(stras[6], stras[4], stras[9], n);//stras[9] -> (B3-B1)
		strassen(stras[3], stras[9], stras[13], n);
		//stras[13] -> A4*(B3-B1)	M4
		sum(stras[0], stras[1], stras[8], n);//stras[8] -> (A1+A2)
		strassen(stras[8], stras[7], stras[14], n);
		//stras[14] -> (A1+A2)*B4	M5
		sum(stras[4], stras[5], stras[9], n);//stras[9]] -> (B1+B2)
		substract(stras[2], stras[0], stras[8], n); //stras[8]->A3-A1
		strassen(stras[8], stras[9], stras[15], n);
		//stras[10] -> (A3-A1)*(B1+B2)	M6
		substract(stras[1], stras[3], stras[8], n);//stras[8] -> (A2-A4)
		sum(stras[6], stras[7], stras[9], n);//stras[9] -> (B3+B4)
		strassen(stras[8], stras[9], stras[16], n);
		//stras[17] -> (A2-A4)*(B3+B4)	M7
		sum(stras[10], stras[13], stras[9], n);//M1+M4
		substract(stras[14], stras[16], stras[8], n);//M5-M7
		substract(stras[9], stras[8], stras[17], n);//M1+M4-(M5-M7) C1
		sum(stras[12], stras[14], stras[18], n);//M3+M5			C2
		sum(stras[11], stras[13], stras[19], n);//M2+M4			C3
		substract(stras[10], stras[11], stras[9], n);//M1-M2
		sum(stras[12], stras[15], stras[8], n);//M3+M6
		sum(stras[9], stras[8], stras[20], n);//M1-M2+M3+M6		C4
		for (int i = 0 ; i < n; i++) {
			for (int j = 0; j < n; j++) {
				C[i][j] =  stras[17][i][j];
				C[i][j + n] = stras[18][i][j];
				C[i + n][j] = stras[19][i][j];
				C[i + n][j + n] = stras[20][i][j];
			}
		}
		for (int i = 0; i < 21; i++) {
			for (int j = 0; j < n; j++)
				free(stras[i][j]);
			free(stras[i]);
		}
		free(stras);
	}
}

void verify_redimens(int nrmat, int *size, int **rows,
					 int **columns, int ****matrix)
{
	if (nrmat >= (*size)) {
		(*size) = (*size) * 2 + 1;
		*rows = realloc(*rows, (*size) * sizeof(int));
		*columns = realloc(*columns, (*size) * sizeof(int));
		(*matrix) = realloc((*matrix), (*size) * sizeof(int **));
	}
}

void dimensions(int *rows, int *columns, int index, int dim)
{
	if (index >= 0 && index < dim)
		printf("%d %d\n", rows[index], columns[index]);
	else
		printf("No matrix with the given index\n");
}

void exit_program(int nrmat, int ****matrix, int **rows, int **columns)
{
	for (int i = 0; i < nrmat; i++) {
		for (int j = 0; j < (*rows)[i]; j++)
			free((*matrix)[i][j]);
		free((*matrix)[i]);
	}
	free(*matrix); free(*rows); free(*columns);
}

int **multiply(int ****matrix, int x, int y, int **rows,
			   int **columns, int *nrmat, int *dim, int *size)
{
	if (x < 0 || x >= (*nrmat) || y < 0 || y >= (*nrmat)) {
		printf("No matrix with the given index\n");
		return NULL;
	}
	if ((*columns)[x] != (*rows)[y]) {
		printf("Cannot perform matrix multiplication\n");
		return NULL;
	}
	if ((*nrmat) >= (*size)) {
		(*size) = (*size) * 2 + 1;
		*rows = realloc(*rows, (*size) * sizeof(int));
		*columns = realloc(*columns, (*size) * sizeof(int));
		(*matrix) = realloc((*matrix), (*size) * sizeof(int **));
	}
	make_rows_columns((*dim), &(*rows), (*rows)[x],
					  &(*columns), (*columns)[y]);
	int **m;
	m = (int **)calloc((*rows)[x], sizeof(int *));
	for (int i = 0; i < (*rows)[x]; i++) {
		m[i] = (int *)calloc((*columns)[y], sizeof(int));
		for (int j = 0; j < (*columns)[y]; j++) {
			m[i][j] = 0;
			for (int k = 0; k < (*columns)[x]; k++) {
				m[i][j] += ((*matrix)[x][i][k] * (*matrix)[y][k][j]) % MOD;
				m[i][j] = ((m[i][j] % MOD) + MOD) % MOD;
			}
		}
	}
	return m;
}

int main(void)
{
	int ***matrix = NULL; char character;
	int *rows = NULL, *columns = NULL;
	int size = 0, nrmat = 0, dim = 0, n, m, ok, power, x, y, index;
	scanf(" %c", &character);
	while (character != 'Q') {
		ok = 0;
		if (character == 'L') {
			ok = 1; scanf("%d%d", &n, &m); int **a = alloc_matrix(n, m);
			make_rows_columns(dim, &rows, n, &columns, m);
			verify_redimens(nrmat, &size, &rows, &columns, &matrix);
			matrix[nrmat] = a; nrmat++; dim++; read_matrix(n, m, a);
		}
		if (character == 'D') {
			ok = 1; scanf("%d", &index);
			dimensions(rows, columns, index, dim);
		}
		if (character == 'P') {
			ok = 1; scanf("%d", &index);
			print_matrix(rows, columns, matrix, index, dim);
		}
		if (character == 'M') {
			ok = 1; scanf("%d%d", &x, &y);
			int **tmp = multiply(&matrix, x, y, &rows, &columns,
								 &nrmat, &dim, &size);
			if (tmp) {
				matrix[nrmat] = tmp; nrmat++; dim++;
			}
		}
		if (character == 'T') {
			ok = 1; scanf("%d", &index);
			if (index < 0 || index >= nrmat)
				printf("No matrix with the given index\n");
			else
				matrix[index] = tsp(rows[index], columns[index],
									matrix[index], rows, columns, index);
		}
		if (character == 'R') {
			ok = 1; scanf("%d%d", &index, &power);
			int **tmp = raise(matrix, power, rows, index, nrmat, columns);
			if (tmp)
				matrix[index] = tmp;
		}
		if (character == 'O') {
			ok = 1; sort(matrix, rows, columns, nrmat);
		}
		if (character == 'F') {
			ok = 1; scanf("%d", &index);
			delete(&matrix, &nrmat, index, &rows, &columns, &size, &dim);
		}
		if (character == 'C') {
			ok = 1; scanf("%d", &index);
			int **tmp = crop(matrix, index, rows, columns, nrmat);
			if (tmp)
				matrix[index] = tmp;
		}
		if (character == 'S') {
			ok = 1; scanf("%d%d", &x, &y);
			if (x < 0 || x >= nrmat || y < 0 || y >= nrmat) {
				printf("No matrix with the given index\n");
			} else {
				if (rows[x] != columns[x] || rows[x] != rows[y] ||
					rows[y] != columns[y]) {
					printf("Cannot perform matrix multiplication\n");
				} else {
					verify_redimens(nrmat, &size, &rows, &columns, &matrix);
					make_rows_columns(dim, &rows, rows[x],
									  &columns, columns[y]);
					matrix[nrmat] = alloc_matrix(rows[x], columns[x]);
					strassen(matrix[x], matrix[y], matrix[nrmat], rows[x]);
					dim++; nrmat++;
				}
			}
		}
		if (ok == 0)
			printf("Unrecognized command\n");
		scanf(" %c", &character);
	}
	exit_program(nrmat, &matrix, &rows, &columns);
	return 0;
}
