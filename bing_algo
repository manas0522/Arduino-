// Java Code for the above approach

import java.io.*;

class GFG {

	static int bingo;
	static int nextBingo;

	// Function for finding the maximum and minimum element
	// of
	// the Array
	static void maxMin(int[] vec, int n)
	{
		for (int i = 1; i < n; i++) {
			bingo = Math.min(bingo, vec[i]);
			nextBingo = Math.max(nextBingo, vec[i]);
		}
	}

	// Function to sort the array
	static int[] bingoSort(int[] vec, int n)
	{
		bingo = vec[0];
		nextBingo = vec[0];
		maxMin(vec, n);
		int largestEle = nextBingo;
		int nextElePos = 0;
		while (bingo < nextBingo) {
			// Will keep the track of the element position
			// to
			// shifted to their correct position
			int startPos = nextElePos;
			for (int i = startPos; i < n; i++) {
				if (vec[i] == bingo) {
					int temp = vec[i];
					vec[i] = vec[nextElePos];
					vec[nextElePos] = temp;
					nextElePos = nextElePos + 1;
				}
				// Here we are finding the next Bingo
				// Element for the next pass
				else if (vec[i] < nextBingo)
					nextBingo = vec[i];
			}
			bingo = nextBingo;
			nextBingo = largestEle;
		}
		return vec;
	}

	// Function to print the array
	static void printArray(int[] arr, int n)
	{
		System.out.print("Sorted Array: ");
		for (int i = 0; i < n; i++) {
			System.out.print(arr[i] + " ");
		}
		System.out.println();
	}

	public static void main(String[] args)
	{
		int[] arr = { 5, 4, 8, 5, 4, 8, 5, 4, 4, 4 };
		arr = bingoSort(arr, arr.length);
		printArray(arr, arr.length);

		int[] arr2 = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		arr2 = bingoSort(arr2, arr2.length);
		printArray(arr2, arr2.length);

		int[] arr3 = { 0, 1, 0, 1, 0, 1 };
		arr3 = bingoSort(arr3, arr3.length);
		printArray(arr3, arr3.length);
	}
}

// This code is contributed by lokeshmvs21.
