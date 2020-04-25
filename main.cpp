#include <iostream>
#include <ctime>
using std::cin; using std::cout;
//過五關，猜謎

void PrintIntroduction(int Difficulty){
    std::cout << "\nYou are a secret agent breaking into a level " << Difficulty;
    std::cout << " secure server room...\n" <<std::endl;
}

const int MaxDifficulty = 5;    

bool PlayGame(int Difficulty){
    PrintIntroduction(Difficulty);

    // Generate Code //後台創建謎題
    const int CodeA = rand() % Difficulty + Difficulty;
    const int CodeB = rand() % Difficulty + Difficulty;
    const int CodeC = rand() % Difficulty + Difficulty;
    
    const int CodeSum = CodeA + CodeB + CodeC;
    const int CodeProduct = CodeA * CodeB * CodeC;


    // Print CodeSum and CodeProduct to the terminal//公佈謎面開始互動
    std::cout << "+ There are 3 numbers in the code";
    std::cout << "\n+ The codes add up to: " << CodeSum;
    std::cout << "\n+ The codes multiply to give: " << CodeProduct << std::endl;

	std::cout << "\nEnter the correct code to continue...\n\n"; //better sequence of dialogue with player
	
    // Store player guess
    int GuessA, GuessB, GuessC;
    std::cin >> GuessA >> GuessB >> GuessC;

    int GuessSum = GuessA + GuessB + GuessC;
    int GuessProduct = GuessA * GuessB * GuessC;

    // Check if the players guess is correct//判題
    if (GuessSum == CodeSum && GuessProduct == CodeProduct){
    		if (Difficulty == MaxDifficulty){    	
    			std::cout << "\n*** Great work agent! You got all the files! Now get out of there! ***\n";
    		}
	    	else {
	        	std::cout << "\n*** Well done agent! You have extracted a file! Keep going! ***";   		    
    		}
    		return true;
    }
    else{
        std::cout << "\n*** You entered the wrong code! Careful agent! Try again! ***";
        return false;
    }
}

int main(){
	int LevelDifficulty = 1;
    srand(time(NULL)); // create new random sequence based on time of day
    
    while (LevelDifficulty <= MaxDifficulty) {	// Loop game until all levels completed
        bool bLevelComplete = PlayGame(LevelDifficulty);
        
        //if user enters 2 3 4 x and hit enter, the program runs endlessly
		//clear and ignore expressions prevent endless loop from happening.
        cin.clear(); // Clears any errors
        cin.ignore(); // Discards the buffer
        if (bLevelComplete) {
            ++LevelDifficulty;
        }
    }
    
    return 0;
}
