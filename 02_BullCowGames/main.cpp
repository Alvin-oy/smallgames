#include "FBullCowGame.h"

// function prototypes
void PrintIntro();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

// instantiate a new game, which we re-use across plays
FBullCowGame NewGame; 

// the entry point for our application
int main()
{
	bool bPlayAgain = false;
	do {
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);
	
	std::cout<<"Bye!"<<std::endl;//friendly response
	
	return 0; // exit the application
}

void PrintIntro()
{
	std::cout << "Welcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	std::cout << "Can you guess the " << NewGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

// plays a single game to completion
void PlayGame()
{
	NewGame.Reset();	// reset to start a new game
	int32 MaxTries = NewGame.GetMaxTries();
	FBullCowCount BullCowCountPerGuess {}; // Count per valid guess
	// loop asking for guesses while the game
	// is NOT won and there are still tries remaining
	while (!NewGame.IsGameWon() && NewGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess();
				
		// submit a valid guess and check it out
		BullCowCountPerGuess = NewGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCountPerGuess.Bulls;
		std::cout << ". Cows = " << BullCowCountPerGuess.Cows << "\n\n";
	}

	PrintGameSummary();
	return;
		
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// prompt the player to key in a guess
		int32 CurrentTry = NewGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << NewGame.GetMaxTries();
		std::cout << ". Enter your guess: ";
		std::getline(std::cin, Guess);
		
		//lower all the letters in Guess
		for(int i = 0; i<Guess.size(); ++i){
			Guess[i] = tolower(Guess[i]);
		}

		// check status and give feedback
		Status = NewGame.CheckGuessValidity(Guess);
		switch (Status) {
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " 
					<< NewGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word witout repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Alphabet:
			std::cout << "Please enter word in letters.\n\n";
			break;
		default:		// all the invalid guesses are filtered 
			break;	// logically assume the guess is valid,
		}			// ie, Status == EGuessStatus::OK;
	} while (Status != EGuessStatus::OK); 
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again with the same hidden word (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);

	return (Response[0] == 'y') || (Response[0] == 'Y');
}

void PrintGameSummary()
{
	if (NewGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n";
	}
	else
	{
		std::cout << "Better luck next time!\n";
	}
}
