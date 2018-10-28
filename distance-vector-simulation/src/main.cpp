#include <stdio.h>

int distance[1010][1010];
int nextHop[1010][1010];

int main() {
	int n, e, s;
	scanf("%d %d", &n, &e);

	// Inisialisasi seluruh nilai matriks distance dan nextHop.
	for (int i = 1; i <= 1000; i++) {
		for (int j = 1; j <= 1000; j++) {
			if (i == j) {
				distance[i][j] = 0;
				nextHop[i][j] = j;
			} else {
				distance[i][j] = -1;
				nextHop[i][j] = -1;
			}
		}
	}

	// Membaca node-node yang berhubungan (bertetangga).
	// Node yang berhubungan diberi nilai distance 1 dan nextHop diisi dengan nilai node tetangganya.
	int temp1, temp2;
	for (int i=1; i<=e; i++) {
		scanf("%d %d", &temp1, &temp2);
		// Inisialisasi distance
		distance[temp1][temp2] = 1;
		distance[temp2][temp1] = 1;
		// Inisialisasi nexthop
		nextHop[temp1][temp2] = temp2;
		nextHop[temp2][temp1] = temp1;	
	}

	// Membaca jumlah skenario
	scanf("%d", &s);

	// Melakukan update nilai distance dan nextHop sebanyak jumlah skenario.
	// src adalah node sumber informasi, dest adalah node yang menerima informasi.
	int src, dest;
	for(int i=1; i<=s; i++) {
		
		scanf("%d %d", &src, &dest);

		for (int j = 1; j <= n; j++) {
			if (distance[src][j] != -1 && j != dest) {
				
				if (distance[dest][j] == -1) {
					distance[dest][j] = distance[src][j] + 1;
					nextHop[dest][j] = src;
				} else {
					if (distance[dest][j] > distance[src][j] + 1 || (distance[dest][j] == distance[src][j] + 1 && nextHop[dest][j] > src)) {
						distance[dest][j] = distance[src][j] + 1;
						nextHop[dest][j] = src;
					}
				}
				
			}
		}
	}

	// Print distance dan nexthop setelah update
	for(int i=1; i <= n; i++){
		for (int j=1; j <=n; j++) {
			printf("%d %d\n", distance[i][j], nextHop[i][j]);
		}
	}

	return 0;
}
