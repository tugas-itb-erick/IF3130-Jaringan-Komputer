#include <stdio.h>

int distance[1010][1010];
int nextHop[1010][1010];

int main() {
	int n, e, s;
	scanf("%d %d", &n, &e);
	for(int i=1; i<=e; i++) {
		int temp1, temp2;
		scanf("%d %d", &temp1, &temp2);

		distance[temp1][temp2] = 1;
		distance[temp2][temp1] = 1;
		// Inisialisasi nexthop
		nextHop[temp1][temp2] = temp2;
		nextHop[temp2][temp1] = temp1;	
	}

	scanf("%d", &s);
	for(int i=1; i<=s; i++) {
		int src, dest;
		scanf("%d %d", &src, &dest);

		for (int j = 1; j <= n; j++) {
			if (distance[src][j] != -1 && i != dest) {
				if (distance[dest][j] == -1) {
					distance[dest][j] += 1;
					distance[dest][j] += distance[src][j] + 1;
					nextHop[dest][j] = src;
				}
				
			}
		}
	}

	for(int i=1; i <= n; i++){
		for (int j=1; j <=n; j++) {
			printf("%d %d\n", distance[i][j], nextHop[i][j]);
		}
	}

	return 0;
}