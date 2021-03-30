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

static string target_word;

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;
    std::queue<std::vector<std::string>> ladder_queue;
    std::vector<string> word_ladder(1, start), cur_ladder;
    std::unordered_set<string> word_visited;

    string word;
    string adj_word;
    char tmp_char;
    ladder_queue.emplace(word_ladder);
    while (!ladder_queue.empty()) {
        cur_ladder = ladder_queue.front(); // get queue front element
        ladder_queue.pop();
        word = cur_ladder.back();
        word_visited.emplace(word);

        // if has found the target.
        if (word == target_word) {
            cout << "Found ladder:";
            for (size_t i = 0; i < cur_ladder.size(); i++) {
                cout << " " << cur_ladder[i] ;
            }
            cout << endl;
            return ;
        }

        // change one char each time
        size_t word_len = word.size();
        for (size_t i = 0; i < word_len; i++) {
            for (int j = 0; j < ALPHA_SIZE; j++) {
                tmp_char = (char) ('a' + j);
                if (tmp_char != word[i]) {
                    adj_word = word;
                    adj_word[i] = tmp_char;
                    if (english.contains(adj_word) && word_visited.find(adj_word) == word_visited.end()) {
                        vector<string> tmp_ladder(cur_ladder);
                        tmp_ladder.push_back(adj_word);
                        ladder_queue.emplace(tmp_ladder);
                    }

            }
        }

//        for (string word_in_lex : english) {
//            int diff_cnt = 0;
//            if (word_in_lex.size() == word_len	&& word_visited.find(word_in_lex) == word_visited.end()) {
//                for (size_t i = 0 ; i < word_len; i++) {
//                    if (word[i] != word_in_lex[i])
//                        diff_cnt++;
//                }
//                if (diff_cnt == 1) {
//                    vector<string> tmp_ladder(cur_ladder);
//                    tmp_ladder.push_back(word_in_lex);
//                    ladder_queue.emplace(tmp_ladder);
//                }
//            }
//        }



    }

    cout << "cannot found ladder" << endl;
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
        target_word = end;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
