/* The game logic (no view code or direct user interaction)
The game is a simple guess the word game based on Mastermind
*/

#pragma once 
#include "FBullCowGame.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

// to make syntax Unreal friendly
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame() {
	FString DefaultWord = "Planet";
	Reset(DefaultWord); 
} // default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const 
{ 
	TMap<int32, int32> WordLengthToMaxTries { {3,4}, {4,7}, {5,10}, {6,16}, {7,20}, {8,23} };
	return WordLengthToMaxTries[MyHiddenWord.length()]; 
}

void FBullCowGame::Reset(FString HiddenWord)
{
	const FString HIDDEN_WORD = HiddenWord; // this MUST be an isogram
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!bIsIsogram(Guess)) // if the guess isn't an isogram 
	{
		return EGuessStatus::Not_Isogram;
	} 
	else if (!bIsLowercase(Guess))	// if the guess isn't all lowercase 
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength())	// if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

// receives a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;

	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	// loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) {
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) {
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar]) {
				if (MHWChar == GChar) { // if they're in the same place
					BullCowCount.Bulls++; // increment bulls
				}
				else {
					BullCowCount.Cows++; // must be a cow
				}	
			}
		}
	}
	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::bIsIsogram(FString Word) const
{
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen;// setup our map
	for (auto Letter : Word)	// for all letters of the word
	{
		Letter = tolower(Letter); // handle mixed case
		if (LetterSeen[Letter]) // if the letter is in the map
			return false; // we do NOT have an isogram
		else
			LetterSeen[Letter] = true; // add the letter to the map as seen		
	}
	return true; // for example in cases where \0 is entered
}

bool FBullCowGame::bIsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter)) // if not a lowercase letter
			return false;
	}
	return true;
}

FString FBullCowGame::HiddenWordDictionary(int32 Difficulty)
{
	bool bRightNumber = false;
	srand(time(NULL));
	TMap<int32, FString> ThreeLetterHiddenWords { { 0, "aim" }, { 1, "air" }, { 2, "bam" },
	{ 3, "big" }, { 4, "sag" }, { 5, "orb" },
	{ 6, "mob" }, { 7, "rib" }, { 8, "sob" } };

	TMap<int32, FString> FourLetterHiddenWords { { 0, "aims" }, { 1, "boar" }, { 2, "coma" },
	{ 3, "drag" }, { 4, "ergo" }, { 5, "farm" },
	{ 6, "gags" }, { 7, "hair" }, { 8, "iris" } };

	TMap<int32, FString> FiveLetterHiddenWords { { 0, "gizmo" }, { 1, "swarm" }, { 2, "grasp" },
	{ 3, "amigo" }, { 4, "micro" }, { 5, "germs" },
	{ 6, "frogs" }, { 7, "marsh" }, { 8, "major" } };

	TMap<int32, FString> SixLetterHiddenWords { { 0, "orgasm" }, { 1, "mosaic" }, { 2, "disarm" },
	{ 3, "ransom" }, { 4, "gators" }, { 5, "savior" },
	{ 6, "grails" }, { 7, "sailor" }, { 8, "morgan" } };

	TMap<int32, FString> SevenLetterHiddenWords { { 0, "isogram" }, { 1, "mirages" }, { 2, "viragos" },
	{ 3, "isogamy" } };

	TMap<int32, FString> EigthLetterHiddenWords { { 0, "organism" }, { 1, "isograms" } };

	if (Difficulty == 3)
	{
		int32 RandomWord = rand() % 9;
		return ThreeLetterHiddenWords[RandomWord];
	}
	else if (Difficulty == 4)
	{
		int32 RandomWord = rand() % 9;
		return FourLetterHiddenWords[RandomWord];
	}
	else if (Difficulty == 5)
	{
		int32 RandomWord = rand() % 9;
		return FiveLetterHiddenWords[RandomWord];
	}
	else if (Difficulty == 6)
	{
		int32 RandomWord = rand() % 9;
		return SixLetterHiddenWords[RandomWord];
	}
	else if (Difficulty == 7)
	{
		int32 RandomWord = rand() % 3;
		return SevenLetterHiddenWords[RandomWord];
	}
	else if (Difficulty == 8)
	{
		int32 RandomWord = rand() % 2;
		return EigthLetterHiddenWords[RandomWord];
	}
	return FString();
}