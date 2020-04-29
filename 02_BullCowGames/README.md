## This is the second project I touched. 28/04/2020
## Code in peace
Experimenting on branches is safe.
Keeping another copy on origin/remote is safe
Different copies are synchronized easily.
All files are lastest version, up-to-date.

## Note on CheckGuessValidity

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const;
calling chain is how a function is joined in main function, the program data flow avoid implicit dependency

避免隱藏的邏輯依賴關係。實測極端情形，如輸入ab,這是isogram，但不是五個字母（預設HiddenWord有5個字母），過濾條件形成多重攔截，即用戶輸入的單詞必須同時符合所有條件才算有效輸入。因此不存在哪個排除條件在前於是後面的排除條件依賴它作為前提條件的情況。20/07/2019

## Note on FText GetValidGuess(FString Guess);
架構確定之後，helper functions一句一句按部就班地寫出來就行了，沒有意外。
真的很像寫作文，記敘文，議論文，表述過程從架構借力.
19/07/2019	
	