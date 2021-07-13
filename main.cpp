/*
HW2WordSearch

Toni Avila (TXA180025), CS 1337.010
2/17/2020

When executed, this program reads in a word puzzle from a file (alongside its dimensions)
and a list of movie titles. The program then searches for the movie titles one by one and,
if found, will display to the user where the movie title begins and the direction in which
it flows. If not found, it is sent to a vector which, at the end of the program, displays
which movies were not found. User can loop program as many times as wanted.


Change/Improvement Log:
-SPECIAL FEATURE* -> added option to enter another word puzzle and search it as well. (2/16/2020)
-Created 8 different specific search functions, each with similar use/code but each checks a different direction. (2/14/2020)
-Moved majority of variables used repeatedly to top to use as globals for easier use of modularity. (2/14/2020)
-Created global bool variable which is used to check if a word was found or not. (2/15/2020)
-Functions which check directions also utilize length of the movie string to also verify word is there. (2/14/2020)
-When searching, search doesn't not wrap around matrix, instead ends and returns word is not there if it was still
in the process of searching. (2/14/2020)
-Set a global vector which will be used to store movies not found and subsequently displayed at the end. (2/16/2020)
-Modularized program along the way, to which main is now more prompt and allocates the work to other functions. (Ongoing)
-Declared the 2D array global, set components to a max, then used a function to read in actual num of rows and columns. (2/13/2020)


Notes:
--Words may be written to the:
East (E), West (W), North (N), South (S), Northeast (NE),
Northwest (NW), Southeast (SE), Southwest (SW)
--Format of displaying words found is "word" was found "direction" at [row][column]
*first row and column are denoted as 1 on display rather than 0 i.e. row 0 is row 1
so user is easier able to locate the word -> more logical
-Row-- denotes up a row, Row++ denotes down a row (similar for columns)
*/



#include <iostream>
#include <fstream> // for file input
#include <string> // for string class
#include <vector> // for vector

using namespace std;

const int ROWS = 100; // NOT hardwiring dimensions, just setting a global const int for easier use in functions
const int COLUMNS = 100;
int actualRows, actualColumns; // Will be filled later, used to check ACTUAL array size
char wordSearchArray[ROWS][COLUMNS];

bool wordWasFound; // used to check if word is found or not

vector<string> notFound; // will store words not found

bool openForReading(ifstream &infile,string fileName)
{
    infile.open(fileName);
    if (infile.good()) //then the file opened just fine
        return true;
    else
    {
        perror("Can't open the specified input file");
        cout << "The file name used was: " << fileName << endl;
        cout << "Enter another file name to use (or type 'quit'): ";
        getline(cin,fileName);
        if (fileName != "quit")   //maybe check for QUIT too
        {
            cout << "--- The new file name is: " << fileName << endl;
            //note the use of recursion below
            return openForReading(infile,fileName);
        }
        else
        {
            cout << "Quitting at user's request." << endl;
            return false;
        }
    }
} //openForReading

void skipAnyCommentLines(ifstream &infile)
{
    string textLine;
    char ch;
    ch = infile.peek();
    while (ch == '#' || ch == '\n')
    {
        getline(infile,textLine);
        ch = infile.peek();
    }
    //when we leave, we're at a non-comment, non-blank line

} //skipAnyCommentLines

void readArrayAndDisplay(ifstream& infile,int &rows, int &columns) // by reference so we can store actual rows and columns
{
    skipAnyCommentLines(infile);

    infile >> rows >> columns;

    cout << endl << rows << " Rows and " << columns << " Columns" << endl;

    skipAnyCommentLines(infile);

    // Filling up word puzzle and displaying it
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
        {
            infile >> wordSearchArray[i][j];
        }

    for (int i = 0; i < rows; i++)
    {

        for (int j = 0; j < columns; j++)
            cout << wordSearchArray[i][j];

        cout << endl;
    }

    cout << endl;
} // readArrayAndDisplay

void checkIfWordNorth(int posRowGiven, int posColGiven, string movieName, int lengthOfMovie)
{
    int amountIncremented = 0;
    int startRow = posRowGiven;
    int startColumn = posColGiven;

    // We have already checked first letter/char of word, no need to check again.
    posRowGiven--;

    while (amountIncremented < lengthOfMovie) // while times through is less than the length
    {
        amountIncremented++;

        if (movieName[amountIncremented] == ' ') // go over space
            amountIncremented++;

        // Check if chars match and search is still within bounds of the array
        if ((wordSearchArray[posRowGiven][posColGiven] == movieName[amountIncremented]) && (posRowGiven >= 0))
            posRowGiven--; // up 1

        else
            break; // Does not match -> word not there so stop checking
    }
    // Loops is done, now final check
    if (lengthOfMovie == amountIncremented)
    {
        cout << movieName << " was found N at [" << startRow+1 << "] [" << startColumn+1 << "]" << endl;
        wordWasFound = true; // setting true so not stored in not found vector
    }

} // checkIfWordNorth

void checkIfWordSouth(int posRowGiven, int posColGiven, string movieName, int lengthOfMovie)
{
    int amountIncremented = 0;
    int startRow = posRowGiven;
    int startColumn = posColGiven;

    // We have already checked first letter/char of word, no need to check again.
    posRowGiven++;

    while (amountIncremented < lengthOfMovie)
    {
        amountIncremented++;

        if (movieName[amountIncremented] == ' ')
            amountIncremented++;

        // Check if chars match and search is still within bounds of matrix
        if ((wordSearchArray[posRowGiven][posColGiven] == movieName[amountIncremented]) && (posRowGiven < actualRows))
            posRowGiven++; // move down 1
        else
            break; // Does not match -> word not there, leave loop
    }
    // Loops is done, now final check
    if (lengthOfMovie == amountIncremented)
    {
        cout << movieName << " was found S at [" << startRow+1 << "] [" << startColumn+1 << "]" << endl;
        wordWasFound = true;
    }

} // checkIfWordSouth

void checkIfWordLeft(int posRowGiven, int posColGiven, string movieName, int lengthOfMovie)
{
    int amountIncremented = 0;
    int startRow = posRowGiven;
    int startColumn = posColGiven;

    posColGiven--; // We have already checked first letter/char of word, no need to check again.

    while (amountIncremented < lengthOfMovie)
    {
        amountIncremented++;

        if (movieName[amountIncremented] == ' ')
            amountIncremented++;

        // Check if char matches and search is still in bounds of the matrix
        if ((wordSearchArray[posRowGiven][posColGiven] == movieName[amountIncremented]) && (posColGiven >= 0))
            posColGiven--; // move left 1
        else
            break; // Does not match -> word not there so stop checking
    }
    // Loops is done, now final check
    if (lengthOfMovie == amountIncremented)
    {
        cout << movieName << " was found W at [" << startRow+1 << "] [" << startColumn+1 << "]" << endl;
        wordWasFound = true;
    }

} // checkIfWordLeft

void checkIfWordRight(int posRowGiven, int posColGiven, string movieName, int lengthOfMovie)
{
    int amountIncremented = 0;
    int startRow = posRowGiven;
    int startColumn = posColGiven;

    posColGiven++; // We have already checked first letter/char of word, no need to check again.

    while (amountIncremented < lengthOfMovie) // while value is less than the length
    {
        amountIncremented++;

        if (movieName[amountIncremented] == ' ')
            amountIncremented++;

        // Check if chars match and also make sure search is still in the bounds of the matrix
        if ((wordSearchArray[posRowGiven][posColGiven] == movieName[amountIncremented]) && (posColGiven < actualColumns))
            posColGiven++; // Right 1
        else
            break; // Does not match -> word not there so stop checking
    }
    // Loops is done, now final check
    if (lengthOfMovie == amountIncremented)
    {
        cout << movieName << " was found E at [" << startRow+1 << "] [" << startColumn+1 << "]" << endl;
        wordWasFound = true;
    }

} // checkIfWordRight

void checkIfWordNW(int posRowGiven, int posColGiven, string movie, int movieTitleLength)
{
    int amountIncremented = 0;
    int startRow = posRowGiven;
    int startColumn = posColGiven;

    posRowGiven--; // We have already checked first letter/char of word, no need to check again.
    posColGiven--; // going up and left

    while (amountIncremented < movieTitleLength)
    {
        amountIncremented++;

        if (movie[amountIncremented] == ' ') // continue through space
            amountIncremented++;

        // Check if char matches, also check that search is still within bounds of matrix
        if ((wordSearchArray[posRowGiven][posColGiven] == movie[amountIncremented]) && (posRowGiven >= 0) && (posColGiven >= 0))
        {
            posRowGiven--; // Up and left 1
            posColGiven--;
        }
        else
            break; // Does not match -> word not there so stop checking
    }
    // Loop is done, now final check
    if (movieTitleLength == amountIncremented)
    {
        cout << movie << " was found NW at [" << startRow+1 << "] [" << startColumn+1 << "]" << endl;
        wordWasFound = true;
    }

} // checkIfWordNW

void checkIfWordNE(int posRowGiven, int posColGiven, string movieName, int lengthOfMovie)
{

    int amountIncremented = 0;
    int startRow = posRowGiven;
    int startColumn = posColGiven;

    posRowGiven--; // We have already checked first letter/char of word, no need to check again.
    posColGiven++; // up and right

    while (amountIncremented < lengthOfMovie)
    {
        amountIncremented++;

        if (movieName[amountIncremented] == ' ')
            amountIncremented++;

        // Check if chars match and search is still within the bounds of the matrix
        if ((wordSearchArray[posRowGiven][posColGiven] == movieName[amountIncremented]) && (posRowGiven >= 0) && (posColGiven < actualColumns))
        {
            posRowGiven--; // Up and right 1
            posColGiven++;
        }
        else
            break; // Does not match -> word not there so stop checking
    }
    // Loop is done, now final check
    if (lengthOfMovie == amountIncremented)
    {
        cout << movieName << " was found NE at [" << startRow+1 << "] [" << startColumn+1 << "]" << endl;
        wordWasFound = true;
    }
} // checkIfWordNE

void checkIfWordSW(int posRowGiven, int posColGiven, string movieName, int lengthOfMovie)
{
    int amountIncremented = 0;
    int startRow = posRowGiven;
    int startColumn = posColGiven;

    // We have already checked first letter/char of word, no need to check again.
    posRowGiven++; // going down and left
    posColGiven--;

    while (amountIncremented < lengthOfMovie)
    {
        amountIncremented++;

        if (movieName[amountIncremented] == ' ')
            amountIncremented++;

        // Check if chars match and search is still within the bounds of the matrix
        if ((wordSearchArray[posRowGiven][posColGiven] == movieName[amountIncremented]) && (posRowGiven < actualRows) && (posColGiven >= 0))
        {
            posRowGiven++; // Down and left 1
            posColGiven--;
        }
        else
            break; // Does not match -> word not there so stop checking
    }
    // Loop is done, now final check to make sure, checks length with number of times incremented in loop
    if (lengthOfMovie == amountIncremented)
    {
        cout << movieName << " was found SW at [" << startRow+1 << "] [" << startColumn+1 << "]" << endl; // +1 added so counting rows/columns starts at 1
        wordWasFound = true;
    }

} // checkIfWordSW

void checkIfWordSE(int posRowGiven, int posColGiven, string movieName, int lengthOfMovie)
{
    int amountIncremented = 0;
    int startRow = posRowGiven;
    int startColumn = posColGiven;

    // We have already checked first letter/char of word, no need to check again.
    posRowGiven++; // going down and right
    posColGiven++;

    while (amountIncremented < lengthOfMovie)
    {
        amountIncremented++;

        if (movieName[amountIncremented] == ' ')
            amountIncremented++;

        // Checks that char matches and that we are still within the boundaries of the matrix
        if ((wordSearchArray[posRowGiven][posColGiven] == movieName[amountIncremented]) && (posRowGiven < actualRows) && (posColGiven < actualColumns))
        {
            posRowGiven++; // down and right 1
            posColGiven++;
        }
        else
            break; // Does not match -> word not there obviously so stop checking
    }
    // Loop is done, now final check to make sure, checks length with number of times incremented in loop
    if (lengthOfMovie == amountIncremented)
    {
        cout << movieName << " was found SE at [" << startRow+1 << "] [" << startColumn+1 << "]" << endl;
        wordWasFound = true;
    }

} // CheckIfWordSE

void checkIfWordAndDirection(int positionRowGiven, int positionColGiven, string nameOfMovie, int lengthMovie)
{
    //wordWasFound = false; // flag to see if word is found (didnt work here)

    // Checking every direction for the current word sent to the function
    checkIfWordNorth(positionRowGiven, positionColGiven, nameOfMovie, lengthMovie);
    checkIfWordSouth(positionRowGiven, positionColGiven, nameOfMovie, lengthMovie);
    checkIfWordLeft(positionRowGiven, positionColGiven, nameOfMovie, lengthMovie);
    checkIfWordRight(positionRowGiven, positionColGiven, nameOfMovie, lengthMovie);
    checkIfWordSW(positionRowGiven, positionColGiven, nameOfMovie, lengthMovie);
    checkIfWordNE(positionRowGiven, positionColGiven, nameOfMovie, lengthMovie);
    checkIfWordSE(positionRowGiven, positionColGiven, nameOfMovie, lengthMovie);
    checkIfWordNW(positionRowGiven, positionColGiven, nameOfMovie, lengthMovie);

} // checkIfWordAndDirection

void checkWordIsThere(ifstream& infile)
{
    // store important information that will be passed to majority of functions
    int movieTitleLength;
    string movieName;

    while (infile && !infile.eof()) // While open and not at the end
    {
        wordWasFound = false; // flag to store not found words
        skipAnyCommentLines(infile);
        getline(infile, movieName);
        movieTitleLength = movieName.length(); // storing length of the string

        for (int i = 0; i < actualRows; i++)
            for (int j = 0; j < actualColumns; j++)
            {
                if (wordSearchArray[i][j] == movieName[0]) // if first char of word = char in matrix
                    checkIfWordAndDirection(i, j, movieName, movieTitleLength); // where i is the row and j is the column
            }

        if(!wordWasFound)
            notFound.push_back(movieName);

        skipAnyCommentLines(infile);
    }
    // skipAnyCommentLines(puzzle);
}
// checkWordIsThere

void displayWordsNotFound()
{
    cout << "\nThese words could not be located: " << endl;
    for (unsigned int i = 0; i < notFound.size(); i++)
        cout << notFound[i] << endl;

    // clears vector so on a second run-through (if vector decides to enter another file)
    // movies not found will correspond to that file only
    notFound.clear();

} // displayWordsNotFound

void startUp(ifstream &wordPuzzle)
{
    string fileName;
    //char wordSearchArray[ROWS][COLUMNS]; // changed to global for easier use

    cout << "Enter file name" << endl;
    getline(cin, fileName);

    bool fileOpened = openForReading(wordPuzzle, fileName);

    if(!fileOpened) // file doesnt open, program exits
        exit(0);
} // startUp

int main()
{
    string play; // for replay

    do // iterates one time for certain, user can select to play again
    {
        ifstream wordPuzzle;
        startUp(wordPuzzle);
        readArrayAndDisplay(wordPuzzle, actualRows, actualColumns);
        checkWordIsThere(wordPuzzle);
        displayWordsNotFound();


        cout << "\n\nEnter and solve another word puzzle?" << endl;
        cout << "Type y (if yes) and press enter, else type anything else." << endl;
        getline(cin, play);
    }
    while (play == "y" || play == "Y");

    return 0;
} // main

/* for (int i = 0; i < actualRows; i++) // failed attempt
 {
     for (int j = 0; j < actualColumns; j++)
     {
         for (int k = 0; k < amountMovies; k++)
         {
             checkIfWordAndDirection(i, j, k, wordSearchArray, movieNames);
         }
     }
 } First method i tried ---> did not work in the end.
 */













