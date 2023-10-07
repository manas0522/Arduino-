// JavaScript Code for above approach
// Function to sort the array
function bingoSort(vec, n) {
	let bingo = vec[0];
	let nextBingo = vec[0];
	
	// For finding the maximum and minimum element of
	// the Array
	for (let i = 1; i < n; bingo = Math.min(bingo, vec[i]), nextBingo = Math.max(nextBingo, vec[i]), i++)
		;
	let largestEle = nextBingo;
	let nextElePos = 0;
	while (bingo < nextBingo)
	{
	
		// Will keep the track of the element position to
		// shifted to their correct position
		let startPos = nextElePos;
		for (let i = startPos; i < n; i++) {
			if (vec[i] == bingo) {
				[vec[i], vec[nextElePos]] = [vec[nextElePos], vec[i]];
				nextElePos = nextElePos + 1;
			}
			
			// Here we are finding the next Bingo Element
			// for the next pass
			else if (vec[i] < nextBingo)
				nextBingo = vec[i];
		}
		bingo = nextBingo;
		nextBingo = largestEle;
	}
	for (let i = 0; i < vec.length; i++) {
		// console.log("arr: ",vec[i]);
	}
}

// Function to print the array
function printArray(arr, n) {
	console.log("Sorted Array: ");
	for (let i = 0; i < n; i++) {
		console.log(arr[i] + " ");
	}
	console.log("\n");
}

// Driver Code
let arr = [5, 4, 8, 5, 4, 8, 5, 4, 4, 4];
bingoSort(arr, arr.length);
printArray(arr, arr.length);
let arr2 = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1];
bingoSort(arr2, arr2.length);
printArray(arr2, arr2.length);
let arr3 = [0, 1, 0, 1, 0, 1];
bingoSort(arr3, arr3.length);
printArray(arr3, arr3.length);

// This code is contributed by ishankhandelwals.
