// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();                                       // setting up game
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    // else Checking for the input
    else
    {
        ClearScreen();
        PrintLine(TEXT("You entered : "+Input));
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome To BUll Cow Game"));       // Welcome message
    
    HiddenWord = GetValidWords()[FMath::RandRange(0, GetValidWords().Num() - 1)];
    //PrintLine(TEXT("The Word was : "+HiddenWord));      //debug line

    Lives = HiddenWord.Len();
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i Lives left"), Lives);
    PrintLine(TEXT("Press Tab to enter and guess the word..."));
    bGameOver = false;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)                            // if input is correct
    {
        PrintLine(TEXT("It is correct"));
        PrintLine(TEXT("You WIN"));
        EndGame();
    }
    else                                               // if input is incorrect
    {
        --Lives;
        PrintLine(TEXT("It is inorrect so you lost a life"));
        if(Lives > 0)
        {
            if(HiddenWord.Len() != Guess.Len())            // check if length is correct
            {
                PrintLine(TEXT("Are you serious?"));
                PrintLine(TEXT("%s is not even a %i letter word"),*Guess, HiddenWord.Len());
                PrintLine(TEXT("\nYou have now % i lives left"), Lives);
            }
            else if(!IsIsogram(Guess))                     // Checkfor isogram
            {
                PrintLine(TEXT("As %s is not an isogram"), *Guess);
                PrintLine(TEXT("\nYou have now % i lives left"), Lives);
            }
            else
            {
                int32 Bulls = 0, Cows = 0;
                PrintLine(TEXT("\nYou have now % i lives left"), Lives);
                GetBullCows(Guess, Bulls, Cows);
            }   
        }
        else
        {
            PrintLine(TEXT("\nYou have no lives left"));
            PrintLine(TEXT("You lose"));
            PrintLine(TEXT("The Word was : "+HiddenWord));
            EndGame();
        }
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Guess) const
{
    int32 length = Guess.Len();
    for(int i = 0; i<length; i++)
    {
        for(int j = i+1; j<length; j++)
        {
            if(Guess[i] == Guess[j])
                return false;
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords() const
{
    TArray<FString> ValidWords;
    for(int32 i=0; i<1000; i++)
    {
        if(Words[i].Len() >=4 && Words[i].Len() <= 8 && IsIsogram(Words[i]))
        {
            ValidWords.Emplace(Words[i]);
        }
    }
    return ValidWords;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32 BullCount, int32 CowCount) const
{
    BullCount = 0;
    CowCount = 0;
    for(int32 i=0; i<Guess.Len(); i++)
    {
        if(Guess[i] == HiddenWord[i])
        {
           BullCount++;
           continue;
        }
        for(int32 j=0; j<HiddenWord.Len(); j++)
        {
            if(HiddenWord[i] == Guess[j])
            {
                CowCount++;
                break;
            }
        }
    }
    PrintLine(TEXT("You got %i Bulls and %i Cows"), BullCount, CowCount);
}