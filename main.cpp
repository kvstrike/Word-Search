#include<iostream>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include "colormod.h"
using namespace std;

void check(vector< vector<char> >& matrix, char* word, vector<vector<bool>>& highlight);

void print(vector< vector<char> >& matrix, vector<vector<bool>>& highlight);

void directionSearch(vector< vector<char> >& matrix, char* word, vector<vector<bool>>& highlight, int i, int j);

int main(int argc, char* argv[]) {

    //open the file to read from
    fstream input;
    //the last argument in the command lineis the file name, so we open it here
    input.open(argv[argc-1]);
    vector<string> inputWords;

    string word;

    /*add all words from the command line into the vector starting from the first argument after the program call 
    and stopping before the last argument which is the file name*/
    for(int i = 1; i < argc-1; i++){
        inputWords.push_back(argv[i]);
    }

    //make sure the file opened correctly, if not print an error message and exit
    if(!input.is_open()){
        cout << "Error opening file!" << endl;
        return 1;
    }

    int rows;
    int columns;

    input >> rows; //gets the number of rows and columns from the file to create the matrix
    input >> columns;

    vector< vector<char> > matrix;
    matrix.resize(rows, vector<char>(columns)); //sets the matrix size to the number of rows and columns

    for (int i = 0; i < rows; i++) { //inputs the characters from the file into the matrix
        for (int j = 0; j < columns; j++) {
            input >> matrix[i][j];
        }
    }

    //close the file after reading the matrix
    input.close();

    //create a 2D vector of booleans to keep track of which characters should be highlighted, initialized to false
    vector<vector<bool>> highlight(rows, vector<bool>(columns, false));

    //iterate through the inputted words and check if they are in the matrix
    for(unsigned int i = 0; i < inputWords.size(); i++){
        word = inputWords[i];
        check(matrix, (char*)word.c_str(), highlight);
    }

    print(matrix, highlight);

    return 0;
}

void print(vector< vector<char> >& matrix, vector<vector<bool>>& highlight){
    Color::Setting red(Color::FG_RED);
    Color::Setting def(Color::FG_DEFAULT);

    //iterates through the matrix
    for(unsigned int i = 0; i < matrix.size(); i++){
        for(unsigned int j = 0; j < matrix[i].size(); j++){
            //if the character is in position with other character that form the word we were searching for highlight it
            if(highlight[i][j]){
                cout << red << matrix[i][j] << def << " ";
            } 
            //otherwise just print the character without highlighting
            else {
                cout << matrix[i][j] << " ";
            }
        }
        //after each column we print a new line
        cout << endl;
    }

    cout << endl;
}

void check(vector< vector<char> >& matrix, char* word, vector<vector<bool>>& highlight) {
    unsigned int i, j;

    //iterates through the matrix
    for (i = 0; i < matrix.size(); i++) {
        for (j = 0; j < matrix[i].size(); j++) {
            //check if the current character matches the first character of the word we are searching for
            if(matrix[i][j] == word[0]){
                directionSearch(matrix, word, highlight, i, j);
            }
        }
    }
}

void directionSearch(vector< vector<char> >& matrix, char* word, vector<vector<bool>>& highlight, int i, int j){
    int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
    int dy[8] = {1, -1, 0, 0, 1, -1, 1, -1};

    int len = strlen(word); //sets the length of the word to be searched for

    for (int dir = 0; dir < 8; dir++) {
        bool match = true;
        vector<int> positionX; //to store the positions of the matched characters
        vector<int> positionY;            
        //iterates through the 8 different directions to check if the characters in the matrix match the characters in the word we are searching for             
        for (int k = 0; k < len; k++) {
            unsigned int newX = i + dx[dir] * k; 
            unsigned int newY = j + dy[dir] * k;

            //boundary check to make sure we do not go out of bounds of the matrix, if we do, break out of the loop
            if (newX < 0 || newX >= matrix.size() || newY < 0 || newY >= matrix[0].size()) {
                match = false;
                break;
            }
            //if the character does not match, break out of the loop
            if(matrix[newX][newY] != word[k]) {
                match = false;
                break;
            }

            positionX.push_back(newX); // store the position of the matched character in X vector
            positionY.push_back(newY); // store the position of the matched character in Y vector
        }

        if(match){
            for(int p = 0; p < len; p++){
                highlight[positionX[p]][positionY[p]] = true; // mark the positions to be highlighted
            }   
        }
    }
}