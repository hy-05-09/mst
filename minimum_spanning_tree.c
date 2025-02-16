#define MAX 50000000
#define full(n) (n==MAX-1)
#define empty(n) (!n)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int sp;
	int ep;
	int w;
}element;

element* minheap;
int q = 0;
int* n = &q;
int vn;
int en;
int j = 1;
int* parent;
FILE* fout;
FILE* fp;

void insert(element, int*);
element delete(int*);
void ascend(element*, int*);
void weightedunion(int, int);
int collapsingfind(int);

int main(int argc, char* argv[]) {
	int p1, p2, weight, sum = 0;
	int root1, root2;
	char* ptr;
	element k;
	element* array;

	char command[255];
	element r;
	float count, start = 0, end = 0;
	minheap = (element*)malloc(MAX*sizeof(element));
	if (minheap == NULL) {
		fprintf(stderr, "메모리 할당 실패");
		return 1;
	}

	start = clock();
	if (argc != 2) {
		fprintf(stderr, "usage: %s input_filename\n", argv[0]);
		return 0;
	}
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "The input file does not exist.\n");
		return 1;
	}

	fout = fopen("hw3_result.txt", "w");
	if (fout == NULL) {
		fprintf(stderr, "파일을 열 수 없습니다");
		return 1;
	}

	if (fgets(command, sizeof(command), fp) != NULL)
		vn = (int)strtol(command, &ptr, 10);
	if (fgets(command, sizeof(command), fp) != NULL)
		en = (int)strtol(command, &ptr, 10);

	parent = (int*)malloc(sizeof(int) * 10000);
	for (int i = 0; i < vn; i++)
		parent[i] = -1;
	array = (element*)malloc(sizeof(element) * MAX);
	
	while (fgets(command, sizeof(command), fp) != NULL) {
		sscanf(command, "%d %d %d", &p1, &p2, &weight);
		r.sp = p1;
		r.ep = p2;
		r.w = weight;
		insert(r, n);
	}
	ascend(minheap, n);

	k = delete(n);
	p1 = k.sp;
	p2 = k.ep;
	weightedunion(p1, p2);
	sum += k.w;
	array[j-1] = k;
	for (;;) {
		k = delete(n);
		p1 = k.sp;
		p2 = k.ep;
		root1 = collapsingfind(p1);
		root2 = collapsingfind(p2);
		if (root1 != root2) {
			weightedunion(root1,root2);
			sum += k.w;
			j++;
			array[j-1] = k;
		}
		if (j == (vn - 1)) break;
		if (empty(minheap)) break;
	}
	for (int q = 0; q < vn - 1; q++) {
		fprintf(fout, "%d %d %d\n", array[q].sp,array[q].ep,array[q].w);
	}
	fprintf(fout, "%d\n", sum);
	if (j==vn-1) fprintf(fout, "CONNECTED\n");
	else fprintf(fout, "DISCONNECTED\n");
	

	end = clock();

	count = (end - start) / CLOCKS_PER_SEC;
	printf("output written to hw3_result.txt.\n");
	printf("running time: %.6f seconds\n", count);

	free(minheap);
	free(array);


	fclose(fp);
	fclose(fout);
	return 1;
}

void insert(element e, int* k) {
	int m;
	if (full(*k)) {
		fprintf(stderr, "The heap is full.\n");
		exit(1);
	}

	m = ++(*k);
	while ((m != 1) && (minheap[m / 2].w > e.w)) {
		minheap[m] = minheap[m / 2];
		m = m / 2;
	}
	minheap[m] = e;
}

element delete(int* n) {
	int parent, child;
	element item, temp;
	if (empty(*n)) {
		fprintf(stderr, "The heap is empty.\n");
		exit(1);
	}
	item = minheap[1];
	temp = minheap[(*n)--];
	parent = 1, child = 2;
	while (child <= *n) {
		if ((child < *n) && (minheap[child].w > minheap[child+1].w)) child++;
		if (temp.w <= minheap[child].w) break;
		minheap[parent] = minheap[child];
		parent = child;
		child = child * 2;
	}
	minheap[parent] = temp;
	return item;
}

void ascend(element* heap, int* p) {
	int pat, ch;
	element temp;
	element* copy;
	int k;
	k = *p;
	copy = (element*)malloc(sizeof(element) * MAX);
	if (copy == NULL) {
		fprintf(stderr, "메모리 할당 실패");
		return;
	}
	for (int i = 0; i <= k; i++) {
		copy[i] = heap[i];
	}
	if (empty(k)) {
		fprintf(stderr, "The heap is empty.");
		exit(1);
	}
	while (k > 0) {
		temp = copy[(k)--];
		pat = 1, ch = 2;
		while (ch <= k) {
			if ((ch < k) && (copy[ch].w > copy[ch + 1].w))
				ch++;
			if (temp.w <= copy[ch].w)
				break;
			copy[pat] = copy[ch];
			pat = ch;
			ch = pat * 2;
		}
		copy[pat] = temp;
	}
	free(copy);
}
 
void weightedunion(int i, int j) {
	int temp = parent[i] + parent[j];
	if (parent[i] > parent[j]) {
		parent[i] = j;
		parent[j] = temp;
	}
	else {
		parent[j] = i;
		parent[i] = temp;
	}
}

int collapsingfind(int i) {
	int root, trail, lead;
	for (root = i; parent[root] >= 0; root = parent[root])
		;
	for (trail = i; trail != root; trail = lead) {
		lead = parent[trail];
		parent[trail] = root;
	}
	return root;
}