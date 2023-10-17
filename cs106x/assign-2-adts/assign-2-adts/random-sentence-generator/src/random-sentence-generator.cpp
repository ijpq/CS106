/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
using namespace std;

#include "console.h"
#include "map.h"
#include "simpio.h"  // for getLine
#include "strlib.h"  // for toLowerCase, trim
using std::shared_ptr;

static const string kGrammarsDirectory = "res/grammars/";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

enum class READ_STATUS { START = 0, BRACKET_WORD = 1, NUM_INFO = 2, TEXT = 3, END = 4 };
static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]:\n"));
        if (filename.empty() || isValidGrammarFilename(filename))
            return filename;
        cout << "Failed to open the grammar file named \"" << filename
             << "\". Please try again...." << endl;
    }
}

struct RuntimeType {
    using ValueType = string;
    ValueType text;
    RuntimeType(ValueType v) : text(v) {}
};

struct BracketWord : RuntimeType {
    BracketWord(ValueType v) : RuntimeType(v) {}
};

struct Word : RuntimeType {
    Word(ValueType v) : RuntimeType(v) {}
};

ostream& operator<<(ostream& os, RuntimeType* word) {
    os << word->text;
    return os;
}

/*
 * a Sentence is one word
 * */
struct Sentence {
    using ChildType = shared_ptr<Sentence>;
    vector<ChildType> childs;

    using ValueType = shared_ptr<RuntimeType>;
    vector<ValueType> container;

    size_t len() const {  // how many words in this sentence
        return container.size();
    }

    size_t nr_childs() const { return childs.size(); }

    void save_word(ValueType&& value) { container.push_back(std::move(value)); }

    void save_word(const ValueType& value) { container.push_back(value); }

    void push_sentence_to_childs(ChildType&& value) {
        childs.push_back(std::move(value));
    }

    void push_sentence_to_childs(const ChildType& value) { childs.push_back(value); }

    Sentence(const Sentence& value) {
        container = value.container;
        childs = value.childs;
    }

    Sentence() = default;

    template <typename T1, typename... T>
    Sentence(T1&& arg, T&&... args) {
        // static_assert(
        //         is_base_of<T1, ValueType>::value,
        //         "runtime tuple accepted derived class of Sentence");
        container.push_back(forward<T1>(arg));
        Sentence(forward<T>(args)...);
    }

    template <typename T>
    Sentence(T&& arg) {
        // static_assert(is_base_of<T, ValueType>::value, "");
        container.push_back(forward<T>(arg));
    }
};

/*
template <typename... T>
struct Node;
template <>
struct Node<> : std::integral_constant<bool, true> {};

template <typename T1, typename... T>
struct Node<T1, T...> {
    // a sentence describe sets of non-terminal, terminal, empty
    using value_type = RuntimeTuple;
    value_type text;

    Node() = default;

    Node(const value_type& value) { text = value; }

    Node(const Node& node) { text = node.text; }

    static constexpr bool isTerminal = std::integral_constant < bool,
                          std::is_same<T1, Word>::value &&Node<T...>::value > ::value;
    static constexpr bool isBracketExpansion = std::integral_constant < bool,
                          (sizeof...(T) == 0) && is_same<T1, BracketWord>::value >
                                                         ::value;

    std::vector<Node*> childs;
};
*/

ostream& operator<<(ostream& os, const Sentence& sentence) {
    for (const Sentence::ValueType& i : sentence.container) {
        os << i.get() << " ";
    }
    os << endl;
    if (sentence.childs.size() == 0) {
        return os;
    }
    for (const Sentence::ChildType& i : sentence.childs) {
        os << *i;
    }
    return os;
}

bool isSingleBracketWord(const string& text) {
    if (text[0] == '<' && text.back() == '>' && text.find('>', 0) == text.size() - 1) {
        return true;
    }
    return false;
}

bool isPureNumber(const string& text) {
    for (auto ch : text) {
        if (!isdigit(ch))
            return false;
    }
    return true;
}

void parser_line_to_childs(
        string text, shared_ptr<Sentence> node,
        Map<string, shared_ptr<Sentence>>& map) {
    size_t pos = 0;
    shared_ptr<Sentence> sentence = make_shared<Sentence>();
    while (true) {
        if (pos >= text.size())
            break;
        auto find_pos = text.find(' ', pos);
        find_pos = find_pos > text.size() ? text.size() : find_pos;
        string oneword = text.substr(pos, find_pos - pos);
        if (isSingleBracketWord(oneword)) {
            shared_ptr<Sentence> one_bracket_word = map[oneword];
            sentence->save_word(one_bracket_word->container[0]);
        } else if (oneword.size() > 0 && !isSingleBracketWord(oneword)) {  // word
            sentence->save_word(make_shared<RuntimeType>(oneword));
        } else {
            cout << __LINE__ << "unexpected, " << oneword << endl;
            exit(-1);
        }
        pos = find_pos + 1;
    }
    node->push_sentence_to_childs(sentence);
}

void init_bracket_word(
        std::fstream& file, Map<string, std::shared_ptr<Sentence>>& bracket_node_map) {
    while (true) {
        string line;
        if (!std::getline(file, line))
            break;
        if (isSingleBracketWord(line)) {
            auto sentence_ptr = make_shared<Sentence>();
            sentence_ptr->save_word(make_shared<BracketWord>(line));
            bracket_node_map.put(line, sentence_ptr);
        }
    }
}

void recursive_expand(
        vector<vector<std::shared_ptr<Sentence>>>& inputs, int idx,
        vector<std::shared_ptr<Sentence>>& output,
        vector<std::shared_ptr<Sentence>>& outputs) {
    auto stack_size = inputs.size();
    if (output.size() == stack_size) {
        auto sentence = make_shared<Sentence>();
        for (auto one_sentence : output) {
            for (auto one_word : one_sentence->container) {
                sentence->save_word(one_word);
            }
        }
        if (sentence->container.size())
            outputs.push_back(sentence);
        return;
    }

    for (auto sentence : inputs[idx]) {
        output.push_back(sentence);
        recursive_expand(inputs, idx + 1, output, outputs);
        output.pop_back();
    }
}

// accepts a sentence as starting point, which is one of [bracket word, bracket word and
// word, word only]. the 2th param is map storing the ptr of word string return a vector
// of sentence, results of all possible expansion with the context of current sentence
vector<std::shared_ptr<Sentence>> expand_sentence(
        std::shared_ptr<Sentence> sentence,
        Map<string, std::shared_ptr<Sentence>>& node_map) {
    vector<vector<std::shared_ptr<Sentence>>> container;
    vector<std::shared_ptr<Sentence>> output;

    // is a single bracket word
    shared_ptr<Sentence> expanded_one_child_sentence = make_shared<Sentence>();
    if (sentence->len() == 1 && isSingleBracketWord(sentence->container[0]->text)) {
        // each child may generates more than one sentence
        // but all sentences generated by current bracket word, should be pushed back
        // into vector, which would be involved in product
        vector<shared_ptr<Sentence>> vector_of_childs;
        for (auto child : sentence->childs) {
            vector<shared_ptr<Sentence>> ret = expand_sentence(child, node_map);
            for (auto s : ret) {
                vector_of_childs.push_back(s);
            }
        }
        container.push_back(vector_of_childs);
    } else {
        // a mixup sentence
        shared_ptr<Sentence> sentence_ptr = make_shared<Sentence>();
        for (auto word : sentence->container) {
            if (!isSingleBracketWord(word->text)) {
                sentence_ptr->save_word(word);
            } else if (isSingleBracketWord(word->text)) {  // is one bracket
                if (sentence_ptr->len()) {
                    container.push_back(vector<shared_ptr<Sentence>>{sentence_ptr});
                    sentence_ptr = make_shared<Sentence>();
                }
                vector<shared_ptr<Sentence>> ret =
                        expand_sentence(node_map[word->text], node_map);
                container.push_back(ret);
            }
        }
        if (sentence_ptr->len()) {
            container.push_back(vector<shared_ptr<Sentence>>{sentence_ptr});
            sentence_ptr = make_shared<Sentence>();
        }
    }
    vector<shared_ptr<Sentence>> outputs;
    recursive_expand(
            container, 0, output,
            outputs);  // each sentence is a non-expandable sentence
    return outputs;
}

int main() {
    while (true) {
        // FS
        string filename = getFileName();
        if (filename.empty())
            break;
        cout << "Here's where you read in the \"" << filename << "\" grammar "
             << "and generate three random sentences." << endl;
        std::fstream grammar_file(filename, std::ios_base::in);
        std::string line;
        Map<std::string, std::shared_ptr<Sentence>> bracket_node_map;

        init_bracket_word(grammar_file, bracket_node_map);

        grammar_file.close();
        grammar_file.open(filename, std::ios_base::in);
        std::shared_ptr<Sentence> node;
        READ_STATUS status = READ_STATUS::END;
        int read_cnt = 0, cnt = 0;
        while (true) {
            string line;
            if (!std::getline(grammar_file, line))
                break;
            line = trim(line);
            [&]() {
                if (isSingleBracketWord(line) &&
                    status == READ_STATUS::END) {  // start of a group of expansion
                    status = READ_STATUS::BRACKET_WORD;
                    node = bracket_node_map[line];
                } else if (  // the number line
                        status == READ_STATUS::BRACKET_WORD && line[0] >= 48 &&
                        line[0] <= 57) {
                    read_cnt = stoi(line);
                    node->childs.reserve(read_cnt);
                    status = READ_STATUS::TEXT;
                } else if (status == READ_STATUS::TEXT && cnt < read_cnt) {
                    cnt++;
                    parser_line_to_childs(
                            line, node,
                            bracket_node_map);  // e.g., <object> / <object> word / word
                                                // <object> / word text
                } else if (line.empty()) {
                    if (read_cnt != cnt) {
                        cout << "unexpected "
                             << "read_cnt" << read_cnt << ","
                             << "cnt" << cnt << endl;
                    }
                    cnt = 0;
                    status = READ_STATUS::END;
                } else {
                    cout << __LINE__ << "unexpected, " << line << endl;
                    exit(-1);
                }
            }();
        }

        auto sentences = expand_sentence(bracket_node_map["<start>"], bracket_node_map);
        for (auto sentence : sentences) {
            for (auto text : sentence->container) {
                cout << text->text << " ";
            }
            cout << endl;
        }
    }

    cout << "Thanks for playing!" << endl;
    return 0;
}
