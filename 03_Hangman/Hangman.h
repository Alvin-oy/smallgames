#ifndef HANGMAN_H
#define HANGMAN_H

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

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

//brilliant outgoing parameter
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

#endif
