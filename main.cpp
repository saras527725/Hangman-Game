#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <vector>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;

bool run;

// Function declarations
int countLines(vector<string> words);
string getWord();
string runAI();
void endGame();
string updateWord(string word, vector<int> characterLocations, char to_update);
bool has_only_alpha(string word);
vector<int> findLocation(string sample, char findIt);
void toLower(string& word);

int countLines(vector<string> words) {
    return words.size();
}

string getWord() {
    ifstream words_file("words.txt");
    if (!words_file) {
        cout << "Error opening file" << endl;
        run = false;
        return "";
    }
    
    vector<string> words;
    string line;
    while (getline(words_file, line)) {
        words.push_back(line);
    }
    words_file.close();

    srand(time(nullptr));
    return words.at(rand() % countLines(words));
}

string runAI() {
    return getWord();
}

void endGame() {
    cout << "Thank you for playing. Goodbye!" << endl;
    run = false;
}

string updateWord(string word, vector<int> characterLocations, char to_update) {
    vector<char> word_vec(word.length(), '_');
    for (int i = 0; i < word.length(); i++) {
        if (find(characterLocations.begin(), characterLocations.end(), i) != characterLocations.end()) {
            word_vec[i] = to_update;
        } else {
            word_vec[i] = word[i];
        }
    }
    return string(word_vec.begin(), word_vec.end());
}

bool has_only_alpha(string word) {
    for (char c : word) {
        if (!isalpha(c)) return false;
    }
    return true;
}

vector<int> findLocation(string sample, char findIt) {
    vector<int> characterLocations;
    for (int i = 0; i < sample.size(); i++) {
        if (sample[i] == findIt) characterLocations.push_back(i);
    }
    return characterLocations;
}

void toLower(string& word) {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
}

int main() {
    cout << "Welcome to Hangman!" << endl;
    run = true;

    while (run) {
        string gameMode;
        cout << "Would you like to play against:\n 1) A friend\n 2) The computer?\nType 'quit' to end the game." << endl;
        cin >> gameMode;

        if (gameMode == "1" || gameMode == "2") {
            string input = (gameMode == "1") ? "" : runAI();
            string hidden_word(input.size(), '_');
            bool game_over = false;
            int num_tries = input.length() + 1;

            if (gameMode == "1") {
                bool isValidWord = false;
                cout << "=========PLAYER 1'S TURN=========" << endl;
                while (!isValidWord) {
                    cout << "Give me a word: ";
                    cin >> input;
                    isValidWord = has_only_alpha(input);
                    if (isValidWord) {
                        for (int i = 0; i < 100; i++) cout << "\n";
                        toLower(input);
                    } else {
                        cout << "Only letters are allowed in words." << endl;
                    }
                }
            }

            cout << "=========PLAYER 2'S TURN=========" << endl;
            cout << "The word has " << input.length() << " letters." << endl;
            cout << "Updated Word: " << hidden_word << endl;

            while (num_tries > 0 && !game_over) {
                char letter;
                cout << "Number of tries left: " << num_tries << endl;
                cout << "Guess a letter: ";
                cin >> letter;
                toLower(letter);

                vector<int> characterLocations = findLocation(input, letter);
                if (!characterLocations.empty()) {
                    hidden_word = updateWord(hidden_word, characterLocations, letter);
                    cout << "Updated Word: " << hidden_word << endl;
                    game_over = (hidden_word == input);
                } else {
                    cout << "That letter is not in the word." << endl;
                }
                num_tries--;
            }

            cout << (game_over ? "You win!" : "The other player wins!") << endl;
            cout << "The word was: " << input << endl;
            run = false;
        } 
        else if (gameMode == "quit") {
            run = false;
        } 
        else {
            cout << "Please input only 1, 2, or 'quit'." << endl;
        }
    }
    endGame();
    return 0;
}
