#ifndef FBULLCOWGAME_H
#define FBULLCOWGAME_H

//#pragma once
#include <iostream>
#include <string>
#include <map>

// to make syntax Unreal friendly
using FString = std::string;
using FText = std::string; 
using int32 = int;

struct FBullCowCount {
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EGuessStatus {
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
	
};

class FBullCowGame {
private:
	int32 MyCurrentTry;
	FString MyHiddenWord;
	bool bGameIsWon;
	
	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;
public:
	FBullCowGame ();
	int32 GetMaxTries() const;
	int32 GetCurrentTry () const;
	int32 GetHiddenWordLength () const;
	bool IsGameWon() const;
	EGuessStatus CheckGuessValidity(FString) const;
	void Reset();
	FBullCowCount SubmitValidGuess(FString);
	
};


# define TMap std::map

FBullCowGame::FBullCowGame() {Reset();}
int32 FBullCowGame::GetCurrentTry()const {return MyCurrentTry;}
int32 FBullCowGame::GetHiddenWordLength ()const {return MyHiddenWord.length();}
bool FBullCowGame::IsGameWon () const {return bGameIsWon;}

int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32,int32> WordLengthToMaxTries {{3,4},{4,7},{5,10},{6,16},{7,20}};
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "play"; 	//Isogram
	MyHiddenWord = HIDDEN_WORD;
	
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

//defining labels for: FText GetValidGuess();
//FString is used as parameter while FText is the return type of a function
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess)) // if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) // if the guess isn't all lowercase
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength()) // if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

//calling chain is how a function is joined in main function, the program data flow
//avoid implicit dependency

// receives a VALID guess, incriments turn, and returns count

FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	for (int32 MHWCharIndex = 0; MHWCharIndex < WordLength; MHWCharIndex++) {
		for (int32 GCharIndex = 0; GCharIndex < WordLength; GCharIndex++) {
			if (Guess[GCharIndex] == MyHiddenWord[MHWCharIndex]) {
				if (MHWCharIndex == GCharIndex) { // if they're in the same place
					BullCowCount.Bulls++; // incriment bulls
				}
				else {
					BullCowCount.Cows++; // must be a cow
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength) {
		bGameIsWon = true;
	}
	else //else block is redundant because bGameIsWon is initialized to be false
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}


bool FBullCowGame::IsIsogram(FString Word) const
{
	if (Word.length() <= 1) { return true; }		
	TMap<char, bool> LetterSeen; 
	for (auto Letter : Word) 
	{
		Letter = tolower(Letter); 
		if (LetterSeen[Letter]) {// if the letter is in the map
		//	std::cout <<Letter <<std::endl;
			return false; // we do NOT have an isogram
		} else { 
		//	std::cout <<Letter <<std::endl;
			LetterSeen[Letter] = true;// add the letter to the map
		}	
	} 
	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter)) // if not a lowercase letter
		{
			return false;
		}
	}
	return true;
}


#endif
