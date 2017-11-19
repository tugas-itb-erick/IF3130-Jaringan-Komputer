import java.util.Scanner;

public class Main {

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int N, S;

		// Read nodes and construct topological network
		N = in.nextInt();
		in.nextLine(); // dummy
		DVP network = new DVP(N);
		for(int i=1; i<=N; i++) {
			String line = in.nextLine();
			
			if (!line.equals("")) {
				// Parse string
				String[] numstr = line.split(",");
				int[] nums = new int[numstr.length];
				for(int num=0; num<nums.length; num++){
					nums[num] = Integer.parseInt(numstr[num]);
				}

				network.initialize(i, nums);
			}
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