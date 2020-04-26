//graduation project for TLAP
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <iomanip>
using std::cout; using std::endl; using std::cin;
using std::vector; using std::string; using std::set; using std::unordered_map;

struct LengthGuess{
	int Length;
	int Guesses;
};

enum class Result {
	Match,
	Miss
};

//allow player 2 to specify length of word and number of guesses
LengthGuess Welcome(){
	int WordLength {0}, MaxGuessNum {0};
	
	cout<<"\nWelcome to Hangman!"<<endl;
	//divide by zero error prevented
	do{
		cout<<"*Please choose word length(3/4/5): ";
		cin >>WordLength;
 		cout<<"*Please choose maximum number of guesses(10/15/20): ";
		cin >> MaxGuessNum;
		if (WordLength == 0 || MaxGuessNum == 0) 
			cout <<"invalid, Please choose again!"<<endl;
	} while( WordLength == 0 || MaxGuessNum == 0 );
	return {WordLength,MaxGuessNum};
}

char Guess(int &CurrentGuessCount){	
	cout<<"Guess the character: "<<endl;
	char CurrentGuess;
	cin >> CurrentGuess;	
	++CurrentGuessCount;
	return CurrentGuess;
}

char Guess(){	
	cout<<"You've guessed that, try another character: "<<endl;
	char CurrentGuessess;
	cin >> CurrentGuessess;	
	return CurrentGuessess;
}

void DisplayVector(vector<string> v){
	for(string s:v){
		cout<<s<<" ";
	}
	cout<<endl;	
}

bool DangerIsReal (int CurrentGuessCount, int MaxGuessNum,int MatchCount, int WordLength){
	if(CurrentGuessCount == MaxGuessNum && MatchCount != WordLength) return true;
	else return false;
}

bool GameIsWon (int MatchCount, int WordLength){
	if(MatchCount == WordLength) return true;
	else return false;
}

Result Pattern(vector<string> &v, char Guess, int &BiggestPatternIndex, int &MatchCount);

bool IsGuessRepetition(unordered_map<char,string> GuessRecord, char Guess);

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


Result Pattern(vector<string> &v, char Guess,int &BiggestPatternIndex, int &MatchCount){
	unordered_map<int,int> Patterns;
	
	for(auto s:v){
		for(int i=0; i<s.size(); ++i){
			if(s[i] == Guess){
				++Patterns[i];
			}
		}
	}
	
	for(int i=0; i<Patterns.size();++i){
		if(Patterns[i]>Patterns[BiggestPatternIndex]){
			BiggestPatternIndex = i;
		}
	}
	
	vector<string> BiggestPatternList;
	for(auto s:v){
		int i = 0;
		while(i<s.size()){
			if(s[i] == Guess) {
				if(s[BiggestPatternIndex] == Guess) BiggestPatternList.push_back(s);
				break;
			}
			++i;
		}
	}
	
	vector<string> InvisibleList;
	for(auto s:v){
		for(int i = 0; i<s.size(); ++i){
			if(s[i] == Guess) break;
		 	if(i == s.size()-1) InvisibleList.push_back(s);
		}
	}
	cout<<"Biggest pattern: ";
	DisplayVector(BiggestPatternList);
	cout<<"Invisible: ";
	DisplayVector(InvisibleList);
	if (BiggestPatternList.size() > InvisibleList.size() ){
		++MatchCount,
		v = BiggestPatternList;
		return Result::Match;
	}
	else {
		v = InvisibleList;
		return Result::Miss;
	}	
}

bool IsGuessRepetition(unordered_map<char,string> GuessRecord, char Guess){
	if(GuessRecord.size() == 0) return false;
	for(auto r:GuessRecord){
		if(Guess == r.first) return true;
	}
	return false;
}
