/*
** Assignment: Laboratory Assignment 05 "Statistic Calculator"
**
** Filename: StatisticsCalculator.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 9/26/2023, 9PM
**
** Description: A program that takes in a user input of the number of inputs (integers)
** and a set of integers and subsequently calculates and displays the minimum, maximum,
** arithmetic mean, median, and mode of that set
**
**
** Input: A natural number and a set of integers of the same amount
**
** Output: the minimum, maximum, arithmetic mean, median, and mode of
** the set of integers
**
** Algorithm: takes a natural number (integers > 1) for the size of the array that
** stores the set of integers.
** Calculates the minimum and maximum by setting both to the first entry of
** the array and updating their values once an entry has either a smaller value or greater value respectively.
** Calculates the mean by summing up all entries in the array, and dividing by the number of entries.
** Calculates the median by checking the number of entries in the array and if even, taking the middle
** two entries and displaying their average or, if odd, displaying the middle entry.
** Calculates the mode by starting at the first entry and comparing it to every other entry in the array, then
** incrementing a counter if the values match and updating a maximum count if the count value exceeds that
** maximum, repeating this process for every entry in the data array to ultimately obtain the value that appears
** the most in the array.
**
** Caveats: assumes that the set of integers is sorted from least to greatest,
** for the purpose of finding the correct median, can only display one mode,
** the most frequent number that appears first in the list. Couldn't really
** figure out how to make use of ++frequency[data[i]] as for extreme cases,
** i.e. minimum = 1, maximum = 10000, the program wouldn't output the correct mode.
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>

int main(void) {

	unsigned int numValues = 0; // index of list, cannot be negative
	scanf("%d", &numValues);
	int data[numValues]; // Array for data, integers only

	// initializing data
	for (size_t i = 0; i < numValues; ++i)
		scanf("%d", &data[i]);

	/*
	// checking inputs of array from scanf
	for (size_t i = 0; i < numValues; ++i)
		printf("%d ", data[i]);
	*/

	// STEP 1: MINIMUM AND MAXIMUM
	int min = data[0], max = data[0]; // declaring and initializing to first index of data array
	for (size_t i = 0; i < numValues; ++i) {
		if (min > data[i]) // if minimum is bigger, update to new smallest
			min = data[i];
		if (max < data[i]) // if maximum is smaller, update to new largest
			max = data[i];
	}
	printf("Minimum: %d\nMaximum: %d\n", min, max);


	// STEP 2: MEAN
	double sum = 0.0; // declaring and initializing sum to zero
	for (size_t i = 0; i < numValues; ++i)
		sum += data[i]; // adding current index to a total sum
	double mean = sum/numValues; // arithmetic mean
	printf("Mean: %0.1lf\n", mean);


	// STEP 3: MEDIAN
	double median;
	if (numValues % 2 == 0) // if the amount of integers is even, take the middle two and average
		median = (data[(numValues - 1)/2] + data[(numValues)/2]) / 2.0;
	else // if odd, just take middle value
		median = data[numValues/2];
	printf("Median: %0.1lf\n", median);


	// STEP 4: MODE
	int mostFrequent = data[0]; // assuming that the first index is most frequent
	int maxCount = 0; // initialize maxCount (frequency) to 0
	for (size_t i = 0; i < numValues; ++i) {
		int count = 0; // initialize count to 0 (haven't counted yet)
		for (size_t j = 0; j < numValues; ++j) {
			if (data[j] == data[i]) // if data is the same value
			++count; // increment count, frequency of that value
		}
		// immediately comparing the count to highest count
		if (maxCount < count) {
				maxCount = count; // updating the maxCount to new highest count
				mostFrequent = data[i]; // the data value that appears the most
		}
	}
	printf("Mode: %d\n", mostFrequent);

	return 0;
}
