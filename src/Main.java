import java.util.Scanner;
import java.lang.StringBuilder;

class DVP {
	private int size;
	private int[][] dist; // 1-indexing
	private int[][] nexthop; // 1-indexing
	private static final int DEFAULT_SIZE = 10;
	private static final int UNDEFINED = -1;

	public DVP() {
		size = DEFAULT_SIZE;
		dist = new int[size+1][size+1];
		nexthop = new int[size+1][size+1];
		for(int i=1; i<=size; i++){
			for(int j=1; j<=size; j++){
				dist[i][j] = UNDEFINED;
				if (i == j)
					dist[i][j] = 0;
				nexthop[i][j] = i; // TODO: nexthop masi blm diinisialisasi dgn benar
			}
		}
	}

	public DVP(int size) {
		this.size = size;
		dist = new int[size+1][size+1];
		nexthop = new int[size+1][size+1];
		for(int i=1; i<=size; i++){
			for(int j=1; j<=size; j++) {
				dist[i][j] = UNDEFINED;
				if (i == j) {
					dist[i][j] = 0;
					// Inisialisasi nexthop
					nexthop[i][j] = j;
				} else {
					nexthop[i][j] = UNDEFINED;
				}
				//nexthop[i][j] = i; // TODO: nexthop masi blm diinisialisasi dgn benar
			}
		}
	}

	public void initialize(int[] neighbours) {
		// for(int i=0; i<neighbours.length; i++) {
		// 	dist[row][neighbours[i]] = 1;
		// 	dist[neighbours[i]][row] = 1;
		// 	// TODO: inisialisasi nexthop
		// }
		int node_x = neighbours[0];
		int node_y = neighbours[1];
		dist[node_x][node_y] = 1;
		dist[node_y][node_x] = 1;
		// Inisialisasi nexthop
		nexthop[node_x][node_y] = node_y;
		nexthop[node_y][node_x] = node_x;		
	}

	public void update(int src, int dest) {
		// klo ini kelar, tubes kelar
		for (int i = 1; i <= size; i++) {
			if (dist[src][i] != UNDEFINED && i != dest) {
				if (dist[dest][i] == UNDEFINED) {
					dist[dest][i] += 1;
					dist[dest][i] += dist[src][i] + dist[dest][src];
					nexthop[dest][i] = src;
				}
				
			}
		}
	}

	public String toString() {
		StringBuilder str = new StringBuilder();
		for(int i=1; i<=size; i++){
			for(int j=1; j<=size; j++) {
				str.append(dist[i][j]);
				str.append(" ");
				str.append(nexthop[i][j]);
				if (i != size || j != size)
					str.append("\n");
			}
		}

		return str.toString();
	}

	public String printDistance() {
		StringBuilder str = new StringBuilder();
		for (int i = 1; i <= size; i++) {
			for (int j = 1; j <= size; j++) {
				str.append(dist[i][j]);
				str.append(" ");
			}
			str.append("\n");
		}
		return str.toString();
	}
}

public class Main {

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int N, E, S;

		// Read nodes and construct topological network
		N = in.nextInt();
		E = in.nextInt();
		DVP network = new DVP(N);
		for(int i=1; i<=E; i++) {
			int[] nums = new int[2];
			nums[0] = in.nextInt();
			nums[1] = in.nextInt();
			network.initialize(nums);
		}

		// Read scenarios
		S = in.nextInt();
		for(int i=1; i<=S; i++) {
			int src, dest;
			src = in.nextInt();
			dest = in.nextInt();

			network.update(src, dest);
		}
		in.close();

		// Print routing table
		System.out.println(network.toString());
	}

}