/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_set>
#define ALPHA_SIZE 26

using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"


static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}


static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;
    using WordType = string;
    using LadderType = vector<WordType>;
    using QueueType = queue<LadderType>;
    using WordSetType = unordered_set<WordType>;
    QueueType word_queue;
    WordSetType wordset;

    LadderType init_ladder({start});
    word_queue.push(init_ladder);
    while(!word_queue.empty()) {
        LadderType current_ladder = word_queue.front();
        word_queue.pop();

        if (current_ladder.back() == end) {
            cout << "Found Ladder: ";
            for (auto i = 0; i < current_ladder.size(); i++) {
                cout << current_ladder[i] << " ";
            }
            cout << endl;
            return ;
        }

        size_t current_ladder_word_size = current_ladder.back().size();
        WordType current_word = current_ladder.back();
        for (size_t i = 0; i < current_ladder_word_size; i++) {
            char ori_char = current_word[i];
            for (size_t j = 1; j < 26; j++) {
                current_word[i] = 'a' + ((ori_char + j - 'a') % 26);
                if ((english.contains(current_word) || end == current_word) && wordset.find(current_word) == wordset.end()) {
                    LadderType copy_ladder = current_ladder;
                    copy_ladder.push_back(current_word);
                    word_queue.push(copy_ladder);
                    wordset.insert(current_word);
                }
            }
            current_word[i] = ori_char;
        }
    }
    cout << "cannot find the ladder" << endl;
    return ;

}

static const string kEnglishLanguageDatafile = "dictionary.txt";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
