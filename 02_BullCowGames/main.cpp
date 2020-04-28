//#pragma once
#include <iostream>
#include <string>
#include <map>

// to make syntax Unreal friendly
using FString = std::string;
using FText = std::string; //cf Line 71
using int32 = int;

struct FBullCowCount
{
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

class FBullCowGame
{
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
	const FString HIDDEN_WORD = ”play“; 	//Isogram
	MyHiddenWord = HIDDEN_WORD;
	
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

//defining labels for: FText GetValidGuess();
//FString is used as parameter while FText is the return type of a function
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess)) // if the guess isn’t an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) // if the guess isn‘t all lowercase
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
// 避免字裡行間隱藏的邏輯上的依賴關係。實測極端情形，如輸入ab,這是isogram，但不是五個字母（預先定下一個單詞有5個字母），過濾條件形成多重攔截，即必須同時符合所有條件用戶輸入的單詞才算有效輸入。因此不存在哪個排除條件在前於是後面的排除條件依賴它作為前提條件的情況。20/07/2019

// receives a VALID guess, incriments turn, and returns count

FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	for (int32 MHWCharIndex = 0; MHWCharIndex < WordLength; MHWCharIndex++) {
		for (int32 GCharIndex = 0; GCharIndex < WordLength; GCharIndex++) {
			if (Guess[GCharIndex] == MyHiddenWord[MHWCharIndex]) {
				if (MHWCharIndex == GCharIndex) { // if they’re in the same place
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

// function prototypes as outside a class
void PrintIntro();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame NewGame; // instantiate a new game, which we re-use across plays

// the entry point for our application
int main()
{
	bool bPlayAgain = false;
	do {
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	}
	while (bPlayAgain);
	
	std::cout<<”Bye!“<<std::endl;//friendly response
	
	return 0; // exit the application
}

void PrintIntro()
{
	std::cout << ”Welcome to Bulls and Cows, a fun word game.\n“;
	std::cout << std::endl;
	std::cout << ”          }   {         ___ “ << std::endl;
	std::cout << ”          (o o)        (o o) “ << std::endl;
	std::cout << ”   /-------\\ /          \\ /-------\\ “ << std::endl;
	std::cout << ”  / | BULL |O            O| COW  | \\ “ << std::endl;
	std::cout << ” *  |-,--- |              |------|  * “ << std::endl;
	std::cout << ”    ^      ^              ^      ^ “ << std::endl;
	std::cout << ”Can you guess the “ << NewGame.GetHiddenWordLength();
	std::cout << ” letter isogram I‘m thinking of?\n“;
	std::cout << std::endl;
	return;
}

// plays a single game to completion
void PlayGame()
{
	NewGame.Reset();	//歸零，新的一局
	int32 MaxTries = NewGame.GetMaxTries();
	FBullCowCount BullCowCountPerGuess {}; //每有效猜一次，核對報數一次
	// loop asking for guesses while the game
	// is NOT won and there are still tries remaining
	while (!NewGame.IsGameWon() && NewGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess();
				
		// submit a valid guess and check it out
		BullCowCountPerGuess = NewGame.SubmitValidGuess(Guess);

		std::cout << ”Bulls = “ << BullCowCountPerGuess.Bulls;
		std::cout << ”. Cows = “ << BullCowCountPerGuess.Cows << ”\n\n“;
	}

	PrintGameSummary();
	return;
	//架構確定之後，helper functions一句一句按部就班地寫出來就行了，沒有意外。
	//真的很像寫作文，記敘文，議論文，不同文體有基本的結構，表述過程從這架構借力，
	//19/07/2019	
	
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = ”“;
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// prompt the player to key in a guess
		int32 CurrentTry = NewGame.GetCurrentTry();
		std::cout << ”Try “ << CurrentTry << ” of “ << NewGame.GetMaxTries();
		std::cout << ”. Enter your guess: “;
		std::getline(std::cin, Guess);

		// check status and give feedback
		Status = NewGame.CheckGuessValidity(Guess);
		switch (Status) {
		case EGuessStatus::Wrong_Length:
			std::cout << ”Please enter a “ << NewGame.GetHiddenWordLength() << ” letter word.\n\n“;
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << ”Please enter a word witout repeating letters.\n\n“;
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << ”Please enter all lowercase letters.\n\n“;
			break;
		default:	// all the invalid guesses are filtered 
			break;	//Status = EGuessStatus::OK; logically assume the guess is valid
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << ”Do you want to play again with the same hidden word (y/n)? “;
	FText Response = ”“;
	std::getline(std::cin, Response);

	return (Response[0] == ’y‘) || (Response[0] == ’Y‘);
}

void PrintGameSummary()
{
	if (NewGame.IsGameWon())
	{
		std::cout << ”WELL DONE - YOU WIN!\n“;
	}
	else
	{
		std::cout << ”Better luck next time!\n“;
	}
}
