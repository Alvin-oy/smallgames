

#include "Hangman.h"

int main(){
	LengthGuess Init = Welcome ();
	int CurrentGuessCount {0};
	char Letter {};
	int BiggestPatternIndex =0;
	//source wordlist, isograms only, constant
	const vector<string> WordList  {"box","boy","and","bad","bed","dog","bid","dry","end","due","day","die","mad","old", "red","sad","kid","led"};
	//create a working wordlist from the source, which can be modefied.
	vector<string> WorkingWordList = WordList;
	
	double WordLength = Init.Length;
	double MaxGuessNum = Init.Guesses;
	
	bool YouWon = false, Hang = false;
	
	int MatchCount {0}; //increment the num of matches to determine win/loss
	 
	//display gameplay by percentage
	double Progress {0.0};
	double Danger {0.0};

	unordered_map<char,int> MatchingLetters {};
	unordered_map<int, char> MatchingLettersAdapter {};
	for(int i = WordLength-1; i>=0; --i){ //finish touch 17/04/2020
		MatchingLettersAdapter[i] = 'X';
	}
	
	unordered_map<char,string> GuessRecord {};
	
	do {
		Letter = Guess(CurrentGuessCount);
		if(IsGuessRepetition (GuessRecord, Letter)){
			Letter = Guess();
		}//filter repetition
		
		Result GuessResult = Pattern (WorkingWordList,Letter,BiggestPatternIndex,MatchCount);
		
		if(GuessResult == Result::Match) {
			MatchingLetters[Letter] = BiggestPatternIndex;
			GuessRecord[Letter] = "Match";
		}
		else {GuessRecord[Letter] = "Miss";}
		
		Progress = (MatchCount/WordLength) * 100;
		Danger = (CurrentGuessCount/MaxGuessNum) * 100;
		cout <<"Progress: "<< Progress <<"%"<<endl;
		cout <<"Danger: "<< Danger <<"%"<<endl;
		
		for(auto l:MatchingLetters){
			MatchingLettersAdapter[l.second] = l.first;
		}						
		
		cout<<"The word is: ";
		for(auto l:MatchingLettersAdapter){
			cout<<l.second <<" ";
		}				
		cout<<endl;
		
		int i = GuessRecord.size();
		for(auto r:GuessRecord){
			cout<<"Guess " <<i<<": " <<r.first <<", "
				<< r.second<<endl;
			--i;
		}
		
		YouWon = GameIsWon (MatchCount, WordLength);
		Hang = DangerIsReal (CurrentGuessCount, MaxGuessNum,MatchCount,WordLength);
		if (YouWon) cout<<"Congrats, you won the game!"<<endl;
		if(Hang) cout<<"Ah, you're hanged!"<<endl;

	}while(CurrentGuessCount < MaxGuessNum && YouWon == false && Hang == false);
	
	cout<<"Thanks for playing, bye!"<<endl;
	return 0;
}


