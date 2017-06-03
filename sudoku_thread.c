
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
	int row;
	int col;
	int value;
} sudokuCells[9][9];

typedef struct {
	int values[9];
	int rowId;
} threadParams;

int randomDelay;
int maxDelay;
sudokuCells *cells;
int numValidSubgrids = 0;
int numValidColumns = 0;
int numValidSquares = 0;
int currentRow = 0;

void loadData(char *argv[]);
void printData();
void setRowValues(threadParams **param, int rowId);
void *validateRow(void *params);
void *validateColumns(void **param);
void *validateSquares(void **param);

int main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("\nInvalid Run. Please provide a proper input file name and a MaxDelay\n\n");
		return 0;
	}

	srand(time(NULL));

	maxDelay = atoi(argv[2]);
	randomDelay = (rand() % maxDelay) + 1;

	cells = malloc(sizeof(sudokuCells[9]));

	// Load and Print Sudoku Data
	loadData(argv);
	printData();

	// Thread variables & responses
	pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9, thread10, thread11;
	void *res1, *res2, *res3, *res4, *res5, *res6, *res7, *res8, *res9, *res10, *res11;

	threadParams *row1, *row2, *row3, *row4, *row5, *row6, *row7, *row8, *row9;
	row1 = malloc(sizeof(threadParams));
	row2 = malloc(sizeof(threadParams));
	row3 = malloc(sizeof(threadParams));
	row4 = malloc(sizeof(threadParams));
	row5 = malloc(sizeof(threadParams));
	row6 = malloc(sizeof(threadParams));
	row7 = malloc(sizeof(threadParams));
	row8 = malloc(sizeof(threadParams));
	row9 = malloc(sizeof(threadParams));

	// Assign Values
	setRowValues(&row1, 1);
	setRowValues(&row2, 2);
	setRowValues(&row3, 3);
	setRowValues(&row4, 4);
	setRowValues(&row5, 5);
	setRowValues(&row6, 6);
	setRowValues(&row7, 7);
	setRowValues(&row8, 8);
	setRowValues(&row9, 9);

	// Create threads
	pthread_create(&thread1, NULL, validateRow, (void *)row1);
	sleep(1);
	pthread_create(&thread2, NULL, validateRow, (void *)row2);
	sleep(1);
	pthread_create(&thread3, NULL, validateRow, (void *)row3);
	sleep(1);
	pthread_create(&thread4, NULL, validateRow, (void *)row4);
	sleep(1);
	pthread_create(&thread5, NULL, validateRow, (void *)row5);
	sleep(1);
	pthread_create(&thread6, NULL, validateRow, (void *)row6);
	sleep(1);
	pthread_create(&thread7, NULL, validateRow, (void *)row7);
	sleep(1);
	pthread_create(&thread8, NULL, validateRow, (void *)row8);
	sleep(1);
	pthread_create(&thread9, NULL, validateRow, (void *)row9);
	sleep(1);
	pthread_create(&thread10, NULL, validateColumns, (void *)&cells);
	sleep(1);
	pthread_create(&thread11, NULL, validateSquares, (void *)&cells);

	// Wait for the threads to finish
	pthread_join(thread1, &res1);
	pthread_join(thread2, &res2);
	pthread_join(thread3, &res3);
	pthread_join(thread4, &res4);
	pthread_join(thread5, &res5);
	pthread_join(thread6, &res6);
	pthread_join(thread7, &res7);
	pthread_join(thread8, &res8);
	pthread_join(thread9, &res9);
	pthread_join(thread10, &res10);
	pthread_join(thread11, &res11);

	// Print Validation Responses
	if ((int)res1 == 1 && (int)res2 == 1 && (int)res3 == 1 && (int)res4 == 1 && 
		(int)res5 == 1 && (int)res6 == 1 && (int)res7 == 1 && (int)res8 == 1 &&
		(int)res9 == 1 && (int)res10 == 1 && (int)res11 == 1) {
		printf("\nThere are 27 valid sub-grids, and thus the solution is valid.\n\n");
	}
	else {
		printf("\nThere are 27 valid sub-grids. only %d of them are valid.\n\n", numValidSubgrids);
	}


	// Free allocated memory
	free(cells);
	free(row1);
	free(row2);
	free(row3);
	free(row4);
	free(row5);
	free(row6);
	free(row7);
	free(row8);
	free(row9);
	return 0;
}

void *validateRow(void *param) {
	++currentRow;
	FILE *pFile;
	pFile = fopen("log", "a+");

	if(pFile==NULL) {
		perror("Error opening log file.\n");
	}

	threadParams *data = (threadParams *)param;
	randomDelay = (rand() % maxDelay) + 1;
	int row[10] = { 0 };
	int j;
	for (j = 0; j < 9; ++j) {
		int val = data->values[j];
		if (row[val] != 0) {
			sleep(randomDelay);
			printf("Validation result from thread - %d: row %d is invalid\n", data->rowId, data->rowId);		

			char str[256];
			char rid[4];
			sprintf(rid, "%d", data->rowId);

			strcpy(str, "thread ID-");
			strcat(str, rid);
			strcat(str, ": row ");
			strcat(str, rid);
			strcat(str, " is invalid ");

			fprintf(pFile, "%s", str);
			return (void *)0;
		}
		else {
			row[val] = 1;
		}
	}

	numValidSubgrids++;
	sleep(randomDelay);
	printf("Validation result from thread - %d: row %d is valid\n", data->rowId, data->rowId);
	fclose(pFile);
	return (void *)1;
}

void *validateColumns(void **param) {
	FILE *pFile;
	pFile = fopen("log", "a+");

	if(pFile==NULL) {
		perror("Error opening log file.\n");
	}

	sudokuCells *data = (sudokuCells *)(*param);
	randomDelay = (rand() % maxDelay) + 1;
	int i, j, res = 1;
	for (i = 0; i < 9; i++) {
		int col[10] = { 0 };
		for (j = 0; j < 9; j++) {
			int val = data[j][i]->value;
			if (col[val] != 0) {
				numValidColumns--;
				numValidSubgrids--;
				res = 0;
				break;
			}
			else {
				col[val] = 1;
			}
		}
		numValidColumns++;
		numValidSubgrids++;
	}
	sleep(maxDelay);
	printf("Validation result from thread - 10: %d - 9 columns are valid\n", numValidColumns);

	char str[256];
	char rid[4];
	sprintf(rid, "%d", numValidColumns );

	strcpy(str, "thread ID-10");
	strcat(str, ": columns ");
	strcat(str, rid);
	strcat(str, ", 9 are invalid ");

	fprintf(pFile, "%s", str);
	fclose(pFile);
	return (void *)res;
}

void *validateSquares(void **param) {
	FILE *pFile;
	pFile = fopen("log", "a+");

	if(pFile==NULL) {
		perror("Error opening log file.\n");
	}

	sudokuCells *data = (sudokuCells *)(*param);
	randomDelay = (rand() % maxDelay) + 1;
	int i, j, s, t, row = 0, col = 0, res = 1;
	int nums[10];
	memset(nums, 0, sizeof nums);

	for (s = 0; s < 3; s++) {
		for (t = 0; t < 3; t++) {
			memset(nums, 0, sizeof nums);
			for (i = s * 3; i < s * 3 + 3; i++) {
				for (j = t * 3; j < t * 3 + 3; j++) {
					int val = data[i][j]->value;
					if (nums[val] != 0) {
						numValidSubgrids--;
						numValidSquares--;
						res = 0;
						break;
					}
					else {
						nums[val] = 1;
					}
				}
			}
			numValidSubgrids++;
			numValidSquares++;
		}
	}
	sleep(maxDelay);
	printf("Validation result from thread - 11: %d - 9 subgrids are valid\n", numValidSquares);

	char str[256];
	char rid[4];
	sprintf(rid, "%d", numValidSquares );

	strcpy(str, "thread ID-10");
	strcat(str, ": subgrids ");
	strcat(str, rid);
	strcat(str, ", 9 are invalid ");

	fprintf(pFile, "%s", str);
	fclose(pFile);

	return (void *)res;
}

void setRowValues(threadParams **param, int rowId) {
	int j;
	for (j = 0; j < 9; j++) {
		int val = cells[rowId - 1][j]->value;
		(*param)->values[j] = val;
	}
	(*param)->rowId = rowId;
}

void loadData(char *argv[]) {
	// int sudoku[9][9] = {
	//     {6, 2, 4, 5, 3, 9, 1, 8, 7},
	//     {5, 1, 9, 7, 2, 8, 6, 3, 4},
	//     {8, 3, 7, 6, 1, 4, 2, 9, 5},
	//     {1, 4, 3, 8, 6, 5, 7, 2, 9},
	//     {9, 5, 8, 2, 4, 7, 3, 6, 1},
	//     {7, 6, 2, 3, 9, 1, 4, 5, 8},
	//     {3, 7, 1, 9, 5, 6, 8, 4, 2},
	//     {4, 9, 6, 1, 8, 2, 5, 7, 3},
	//     {2, 8, 5, 4, 7, 3, 9, 1, 6}};

	// int i, j;
	// for (i = 0; i < 9; i++)
	// {
	//     for (j = 0; j < 9; j++)
	//     {
	//         cells[i][j]->row = i + 1;
	//         cells[i][j]->col = j + 1;
	//         cells[i][j]->value = sudoku[i][j];
	//     }
	// }

	char const *const filename = argv[1];
	FILE *file;
	if ((file = fopen(filename, "rt")) == NULL) {
		printf("File unable to load");
	}
	else {
		char vals[11];
		int i, j, num;
		j = 0;
		while (fgets(vals, sizeof(vals), file) && j < 9) {
			for (i = 0; i < 9; i++) {
				num = vals[i] - '0';
				cells[j][i]->row = j + 1;
				cells[j][i]->col = i + 1;
				cells[j][i]->value = num;
			}
			j++;
		}
	}
	fclose(file);
}

void printData() {
	printf("\n******* SUDOKU DATA *******\n");
	printf("***************************\n");
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			printf(" %d ", cells[i][j]->value);
		}
		printf("\n");
	}
	printf("***************************\n");
	printf("\n\n");

}

