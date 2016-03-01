#ifndef SCORING_H
#define SCORING_H
#pragma once

#include "Constants.h"

//	Interface functions
unsigned int GetScore(Category category, const Roll& roll);
Category GetSuggestion(const Roll& roll);

//	Helper functions
unsigned int ScoreSingles(const RollMap& rollMap, Category category);
unsigned int ScoreFourOfAKind(const RollMap& rollMap);
unsigned int ScoreFullHouse(const RollMap& rollMap);
unsigned int ScoreLittleStraight(const RollMap& rollMap);
unsigned int ScoreBigStraight(const RollMap& rollMap);
unsigned int ScoreChoice(const RollMap& rollMap);
unsigned int ScoreYacht(const RollMap& rollMap);

unsigned int CalculateSum(const RollMap& rollMap);
unsigned int HasXOfAKind(const RollMap& rollMap, const unsigned int x);
unsigned int GetMaxStraightLength(const RollMap& rollMap);

void FillRollMap(RollMap& rollMap, const Roll& roll);
bool IsRollValid(const Roll& thisRoll);
bool IsValueValid(unsigned int value);

#endif	//	SCORING_H