#include <iostream>		//	for std::cout
#include "Constants.h"
#include "Scoring.h"

//	Calculate the score of a given roll for the given scoring category.
unsigned int GetScore(Category category, const Roll& roll)
{
	if (!IsRollValid(roll))
	{
		std::cout << "Attempt to get score of an invalid roll.\n";
		return 0;
	}

	//	Count each value in the roll.
	RollMap rollMap = { 0, 0, 0, 0, 0, 0 };
	FillRollMap(rollMap, roll);

	switch (category)
	{
		case Ones:				return ScoreSingles(rollMap, Ones);
		case Twos:				return ScoreSingles(rollMap, Twos);
		case Threes:			return ScoreSingles(rollMap, Threes);
		case Fours:				return ScoreSingles(rollMap, Fours);
		case Fives:				return ScoreSingles(rollMap, Fives);
		case Sixes:				return ScoreSingles(rollMap, Sixes);
		case FourOfAKind:		return ScoreFourOfAKind(rollMap);
		case FullHouse:			return ScoreFullHouse(rollMap);
		case LittleStraight:	return ScoreLittleStraight(rollMap);
		case BigStraight:		return ScoreBigStraight(rollMap);
		case Choice:			return ScoreChoice(rollMap);
		case Yacht:				return ScoreYacht(rollMap);
		default:				return 0;
	}
}

//	Determine the optimal scoring category of a given roll.
Category GetSuggestion(const Roll& roll)
{
	//	Initialize maxes.
	unsigned int maxScore = GetScore(Category::Ones, roll);
	Category maxScoreCategory = Category::Ones;
	
	//	Loop through the category list, checking for higher score values.
	unsigned int scoreForThisCategory;
	for (unsigned int curCategory = Category::Twos; curCategory < Category::MAXVALUE; ++curCategory)
	{
		scoreForThisCategory = GetScore(static_cast<Category>(curCategory), roll);
		if (scoreForThisCategory >= maxScore)
		{
			maxScore = scoreForThisCategory;
			maxScoreCategory = static_cast<Category>(curCategory);
		}
	}

	return maxScoreCategory;
}

//	Calculate score of singles categories (Ones, Twos, ..., Sixes).
unsigned int ScoreSingles(const RollMap& rollMap, Category category)
{
	//	Category is a 0-base enumeration, so it's representative value is off by one.
	return (category + 1) * rollMap[category];
}

//	Calculate score of four of a kind.
unsigned int ScoreFourOfAKind(const RollMap& rollMap)
{
	unsigned int matchingValue = HasXOfAKind(rollMap, 4);

	switch (matchingValue)
	{
		case 1:		return ScoreSingles(rollMap, Ones);
		case 2:		return ScoreSingles(rollMap, Twos);
		case 3:		return ScoreSingles(rollMap, Threes);
		case 4:		return ScoreSingles(rollMap, Fours);
		case 5:		return ScoreSingles(rollMap, Fives);
		case 6:		return ScoreSingles(rollMap, Sixes);
		default:	return 0;
	}
}

//	Calculate score of a full house (three of one value, two of another).
unsigned int ScoreFullHouse(const RollMap& rollMap)
{
	//	Iterate over the rollMap, looking for a value with three of a kind and a value with two of a kind.
	bool foundPair = false;
	bool foundTrio = false;
	RollMap::const_iterator rollMapIterator;
	for (rollMapIterator = rollMap.begin(); rollMapIterator != rollMap.end(); ++rollMapIterator)
	{
		if (*rollMapIterator == 3)
		{
			foundTrio = true;
		}
		else if (*rollMapIterator == 2)
		{
			foundPair = true;
		}
	}

	if (foundTrio && foundPair)
	{
		return CalculateSum(rollMap);
	}
	else
	{
		return 0;
	}
}

//	Calculate score of a little straight (lowest value straight possible, i.e., 1-2-3-4-5).
unsigned int ScoreLittleStraight(const RollMap& rollMap)
{
	//	If our roll map has a non-zero number of our minimum die value and the number of consecutive non-repeating values matches our number of dice...
	if (*(rollMap.begin()) != 0 && GetMaxStraightLength(rollMap) == NUM_DICE)
	{
		return SCORE_LITTLE_STRAIGHT;
	}
	else
	{
		return 0;
	}
}

//	Calculate score of a big straight (highest value straight possible, i.e., 2-3-4-5-6).
unsigned int ScoreBigStraight(const RollMap& rollMap)
{
	//	If our roll map has a non-zero number of our maximum die value and the number of consecutive non-repeating values matches our number of dice...
	if (*(rollMap.end()-1) != 0  && GetMaxStraightLength(rollMap) == NUM_DICE)
	{
		return SCORE_BIG_STRAIGHT;
	}
	else
	{
		return 0;
	}
}

//	Calculate score of Choice.
unsigned int ScoreChoice(const RollMap& rollMap)
{
	return CalculateSum(rollMap);
}

//	Calculate score for all values being the same.
unsigned int ScoreYacht(const RollMap& rollMap)
{
	if (HasXOfAKind(rollMap, NUM_DICE) != 0)
	{
		return SCORE_YACHT;
	}
	else
	{
		return 0;
	}
}

//	Calculate the sum of all values in the roll map.
unsigned int CalculateSum(const RollMap& rollMap)
{
	//	Iterate over the roll map.
	unsigned int sum = 0;
	RollMap::const_iterator rollMapIterator;
	for (rollMapIterator = rollMap.begin(); rollMapIterator != rollMap.end(); ++rollMapIterator)
	{
		//	Add to the sum the product of the value at this index and it's category (which is its index + 1).
		sum += (*rollMapIterator) * (rollMapIterator._Idx + 1);
	}

	return sum;
}

//	Determine if any value is duplicated x times in the roll.
//		Returns the die value that is repeated x times in the roll, or 0 if no value is repeated x times.
unsigned int HasXOfAKind(const RollMap& rollMap, const unsigned int x)
{
	unsigned int dieValue = 1;
	
	//	Iterate over the rollMap, looking for a value with x or more of a kind.
	RollMap::const_iterator rollMapIterator;
	for (rollMapIterator = rollMap.begin(); rollMapIterator != rollMap.end(); ++rollMapIterator)
	{
		if (*rollMapIterator >= x)
		{
			return dieValue;
		}
		else
		{
			dieValue++;
		}
	}

	return 0;
}

//	Return the highest number of consecutive values in the roll map.
unsigned int GetMaxStraightLength(const RollMap& rollMap)
{
	//	Initialize max.
	unsigned int maxSequenceLength = 0;

	//	Iterate over the rollMap, looking for consecutive values.
	unsigned int curSequenceLength = 0;
	RollMap::const_iterator rollMapIterator;
	for (rollMapIterator = rollMap.begin(); rollMapIterator != rollMap.end(); ++rollMapIterator)
	{
		//	If value is 0...
		if (*rollMapIterator == 0)
		{
			//	If current sequence is longer than longest sequence so far, save it as the longest sequence.
			if (curSequenceLength > maxSequenceLength)
			{
				maxSequenceLength = curSequenceLength;
			}

			//	...sequence is over.
			curSequenceLength = 0;
		}
		//	Otherwise...
		else
		{
			//	...sequence continues.
			curSequenceLength += 1;
		}
	}

	//	If the final value was part of a sequence, check if it was the longest sequence.
	if (curSequenceLength > maxSequenceLength)
	{
		maxSequenceLength = curSequenceLength;
	}

	return maxSequenceLength;
}

//	Creates a map of the roll for quick reference.
void FillRollMap(RollMap& rollMap, const Roll& roll)
{
	//	Iterate over the roll, incrementing the count of that value in the map.
	Roll::const_iterator rollIterator;
	for (rollIterator = roll.begin(); rollIterator != roll.end(); ++rollIterator)
	{
		//	NOTE: Array is 0-based, so 1's are stored at index 0, 2's at index 1, etc.
		rollMap[(*rollIterator) - 1]++;
	}
}

//	Checks roll for valid dice values.
bool IsRollValid(const Roll& roll)
{
	//	Iterate over the roll, testing the values for validity.
	Roll::const_iterator rollIterator;
	for (rollIterator = roll.begin(); rollIterator != roll.end(); ++rollIterator)
	{
		if (!IsValueValid(*rollIterator))
		{
			return false;
		}
	}

	return true;
}

//	Checks if value is a legal die result, i.e., 1 - 8.
bool IsValueValid(unsigned int value)
{
	if (value < MIN_DIE_VALUE || value > MAX_DIE_VALUE)
	{
		return false;
	}
	else
	{
		return true;
	}
}