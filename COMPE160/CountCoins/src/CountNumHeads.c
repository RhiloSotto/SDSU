/*
** Assignment: Laboratory Assignment 07 "CountNumHeads"
**
** Filename: CountNumHeads.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 10/10/2023, 9PM
**
** Description: Given a seed value, calculates the number of coin flips necessary to
** reach a desired number of heads with a created coin object.
**
** Input: None.
**
** Output: String "Total number of flips for (number of heads) heads: (number of coin flips)"
**
** Algorithm: creates a coin object, given a head goal and calls function CountHeads to return a value of coin
** flips, this is displayed alongside the head goal.
**
** Caveats: No variation in seed value, seed value of 15 doesn't produce expected numFlips value 40 for 20 heads.
** Version: 1.0
**
*/

#include <stdio.h>
#include "Coin.h"

#define SEEDNUMBER 10

int CountHeads(Coin coin, int goal) {
/* Type your code here */
	while(NumHeads(coin) < goal) { // stop flipping as soon as it hits goal numHeads
		coin = Flip(coin); // update coin to new flipped coin stats
	}
	return NumFlips(coin); // return number of flips
}

// This function creates a coin object and calls the method for testing
int main() {
	Coin coin = InitCoin(SEEDNUMBER); // Create a coin object with seed value 15
	int numHeads = 20; // Desire 20 heads
	int numFlips = CountHeads(coin, numHeads); // Should return 40 using coin object with seed value 15
	printf("Total number of flips for %d heads: %d\n", numHeads, numFlips);

	coin = InitCoin(SEEDNUMBER);
	numHeads = 100; // Desire 100 heads
	numFlips = CountHeads(coin, numHeads); // Should return 194 using coin object with seed value 15
	printf("Total number of flips for %d heads: %d\n", numHeads, numFlips);

return 0;
}
