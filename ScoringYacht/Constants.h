#ifndef CONSTANTS_H
#define CONSTANTS_H
#pragma once

#include <array>

enum Category {
	Ones,
	Twos,
	Threes,
	Fours,
	Fives,
	Sixes,
	FourOfAKind, 
	FullHouse,
	LittleStraight,
	BigStraight,
	Choice,
	Yacht,
	MAXVALUE		//	Added for iterating over enum
};

const unsigned int NUM_CATEGORIES = 12;

const unsigned int NUM_DICE = 5;
const unsigned int MIN_DIE_VALUE = 1;
const unsigned int MAX_DIE_VALUE = 6;

const unsigned int SCORE_LITTLE_STRAIGHT = 30;
const unsigned int SCORE_BIG_STRAIGHT = 30;
const unsigned int SCORE_YACHT = 50;

//	Using STL arrays so that size is inherent and uninitialized values will be 0 instead of undefined.
typedef std::array<unsigned int, NUM_DICE> Roll;
typedef std::array<unsigned int, MAX_DIE_VALUE> RollMap;

#endif	//	CONSTANTS_H