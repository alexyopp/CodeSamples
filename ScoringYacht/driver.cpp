/*
	SETUP: A game of Yacht.  A player has five 6-sided dice.  A roll is scored differently depending on the chosen category, as indicated below.
				ONES - The sum of all ones showing (e.g., 1-1-1-4-5 would score 3 points)
				TWOS - The sum of all twos showing (e.g., 1-1-2-2-2 would score 6 points)
				THREES - The sum of all threes showing (e.g., 1-2-3-3-3 would score 9 points)
				FOURS - The sum of all fours showing (e.g., 2-2-4-4-4 would score 12 points)
				FIVES - The sum of all fives showing (e.g., 2-5-5-5-7 would score 15 points)
				SIXES - The sum of all sixes showing (e.g., 2-4-6-6-6 would score 18 points)
				4 OF A KIND - The sum of the shown values of the four matching dice (e.g., 2-2-2-2-7 would score 8 points)
				FULL HOUSE - The sum of all values showing (e.g., 1-1-2-2-2 would score 8 points)
				LITTLE STRAIGHT - Score 30 points if the roll is the low straight (e.g., 1-2-3-4-5 would score 30 points)
				LARGE STRAIGHT - Score 30 points if the roll is the high straight (e.g., 2-3-4-5-6 would score 30 points)
				CHOICE - The sum of all values showing (e.g., 1-3-3-6-7 would score 20 points)
				YACHT - Score 50 points if all values showing are the same (e.g., 1-1-1-1-1 would score 50 points)

	PROBLEM: Create a function that returns the score for a given roll of five dice and a specified
				category.
			Create a function that returns the optimal category for a given roll.
*/

#include <iostream>
#include <array>
#include "Constants.h"
#include "Scoring.h"

void RunTest(const Roll& roll);

int main()
{
	Roll roll = { 0, 0, 0, 0, 0 };

	roll = { 1, 1, 1, 2, 3 };	//	min three of a kind
	RunTest(roll);
	roll = { 3, 3, 3, 4, 5 };	//	mid three of a kind
	RunTest(roll);
	roll = { 6, 6, 6, 4, 5 };	//	max three of a kind
	RunTest(roll);
	
	roll = { 1, 1, 1, 1, 2 };	//	min four of a kind
	RunTest(roll);
	roll = { 3, 3, 3, 3, 4 };	//	mid four of a kind
	RunTest(roll);
	roll = { 6, 6, 6, 6, 5 };	//	max four of a kind
	RunTest(roll);
	
	roll = { 1, 1, 1, 1, 1 };	//	min all of a kind
	RunTest(roll);
	roll = { 4, 4, 4, 4, 4 };	//	mid all of a kind
	RunTest(roll);
	roll = { 6, 6, 6, 6, 6 };	//	max all of a kind
	RunTest(roll);

	roll = { 1, 1, 1, 2, 2 };	//	min full house
	RunTest(roll);
	roll = { 3, 3, 3, 4, 4 };	//	mid full house
	RunTest(roll);
	roll = { 6, 6, 6, 5, 5 };	//	max full house
	RunTest(roll);

	roll = { 1, 2, 3, 4, 5 };	//	little straight
	RunTest(roll);

	roll = { 2, 3, 4, 5, 6 };	//	big straight
	RunTest(roll);

	return 0;
}

void RunTest(const Roll& roll)
{
	std::cout << "Roll = [";
	for (unsigned int i = 0; i < NUM_DICE; ++i)
	{
		std::cout << roll[i];
		if (i != NUM_DICE - 1)
		{
			std::cout << ", ";
		}
	}
	std::cout << "]\n";

	std::cout << "Ones: " << GetScore(Ones, roll) << std::endl;
	std::cout << "Twos: " << GetScore(Twos, roll) << std::endl;
	std::cout << "Threes: " << GetScore(Threes, roll) << std::endl;
	std::cout << "Fours: " << GetScore(Fours, roll) << std::endl;
	std::cout << "Fives: " << GetScore(Fives, roll) << std::endl;
	std::cout << "Sixes: " << GetScore(Sixes, roll) << std::endl;
	std::cout << "Four Of A Kind: " << GetScore(FourOfAKind, roll) << std::endl;
	std::cout << "Full House: " << GetScore(FullHouse, roll) << std::endl;
	std::cout << "Little Straight: " << GetScore(LittleStraight, roll) << std::endl;
	std::cout << "Big Straight: " << GetScore(BigStraight, roll) << std::endl;
	std::cout << "Choice: " << GetScore(Choice, roll) << std::endl;
	std::cout << "Yacht: " << GetScore(Yacht, roll) << std::endl;

	std::cout << "Suggested Category is " << GetSuggestion(roll) << std::endl;
	std::cout << "----------------------------------------------------------------\n";
}