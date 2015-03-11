/*
 * File: Lexicon.cpp
 * -----------------
 * A Lexicon is a word list. This Lexicon is backed by a data
 * structure called a prefix tree or trie ("try").
 *
 * This is a re-implementation of Lexicon.  Its previous implementation used
 * a pair of structures: a directed acyclic word graph (DAWG) and an STL set.
 * This implementation was discarded because of several reasons:
 *
 * - It relied on binary file formats that were not readable by students.
 * - It did not provide for expected class members like remove.
 * - It had a clunky pair of data structures that had to be searched separately.
 * - It was optimized for space usage over ease of use and maintenance.
 *
 * The original DAWG implementation is retained as dawglexicon.h/cpp.
 * 
 * @version 2014/10/10
 * - added comparison operators ==, !=
 * - removed 'using namespace' statement
 */

#include "lexicon.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "dawglexicon.h"
#include "error.h"
#include "strlib.h"

static bool isDAWGFile(std::string filename);
static void scrub(std::string& str);

Lexicon::Lexicon() {
    m_root = NULL;
    m_size = 0;
}

Lexicon::Lexicon(std::string filename) {
    m_root = NULL;
    m_size = 0;
    addWordsFromFile(filename);
}

Lexicon::Lexicon(const Lexicon& src) {
    m_root = NULL;
    m_size = 0;
    deepCopy(src);
}

Lexicon::~Lexicon() {
    clear();
}

bool Lexicon::add(std::string word) {
    scrub(word);
    if (word.empty()) {
        return false;
    }
    return addHelper(m_root, word, /* originalWord */ word);
}

void Lexicon::addWordsFromFile(std::string filename) {
    if (isDAWGFile(filename)) {
        readBinaryFile(filename);
    } else {
        std::ifstream istr(filename.c_str());
        if (istr.fail()) {
            error("Lexicon::addWordsFromFile: Couldn't open lexicon file " + filename);
        }
        std::string line;
        while (getline(istr, line)) {
            add(line);
        }
        istr.close();
    }
}

void Lexicon::clear() {
    m_size = 0;
    m_allWords.clear();
    deleteTree(m_root);
    m_root = NULL;
}

bool Lexicon::contains(std::string word) const {
    scrub(word);
    if (word.empty()) {
        return false;
    } else {
        return containsHelper(m_root, word, /* isPrefix */ false);
    }
}

bool Lexicon::containsPrefix(std::string prefix) const {
    scrub(prefix);
    if (prefix.empty()) {
        return true;
    } else {
        return containsHelper(m_root, prefix, /* isPrefix */ true);
    }
}

bool Lexicon::equals(const Lexicon& lex2) const {
    // optimization: if literally same lexicon, stop
    if (this == &lex2) {
        return true;
    }
    if (size() != lex2.size()) {
        return false;
    }
    return m_allWords == lex2.m_allWords;
}

bool Lexicon::isEmpty() const {
    return size() == 0;
}

void Lexicon::mapAll(void (*fn)(std::string)) const {
    for (std::string word : m_allWords) {
        fn(word);
    }
}

void Lexicon::mapAll(void (*fn)(const std::string &)) const {
    for (std::string word : m_allWords) {
        fn(word);
    }
}

bool Lexicon::remove(std::string word) {
    scrub(word);
    if (word.empty()) {
        return false;
    } else {
        return removeHelper(m_root, word, /* originalWord */ word, /* isPrefix */ false);
    }
}

bool Lexicon::removePrefix(std::string prefix) {
    scrub(prefix);
    if (prefix.empty()) {
        bool result = !isEmpty();
        clear();
        return result;
    } else {
        return removeHelper(m_root, prefix, /* originalWord */ prefix, /* isPrefix */ true);
    }
}

int Lexicon::size() const {
    return m_size;
}

std::string Lexicon::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::set<std::string> Lexicon::toStlSet() const {
    std::set<std::string> result;
    for (std::string word : m_allWords) {
        result.insert(word);
    }
    return result;
}

/*
 * Operators
 */
bool Lexicon::operator ==(const Lexicon& lex2) const {
    return equals(lex2);
}

bool Lexicon::operator !=(const Lexicon& lex2) const {
    return !equals(lex2);
}


/* private helpers implementation */

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::addHelper(TrieNode*& node, std::string word, const std::string& originalWord) {
    if (node == NULL) {
        // create nodes all the way down, one for each letter of the word
        node = new TrieNode();
    }

    if (word.empty()) {
        // base case: we have added all of the letters of this word
        if (node->isWord()) {
            return false;   // duplicate word; already present
        } else {
            // new word; add it
            node->setWord(true);
            m_size++;
            m_allWords.add(originalWord);
            return true;
        }
    } else {
        // recursive case: chop off first letter, traverse the rest
        return addHelper(node->child(word[0]), word.substr(1), originalWord);
    }
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::containsHelper(TrieNode* node, std::string word, bool isPrefix) const {
    if (node == NULL) {
        // base case: no pointer down to here, so prefix must not exist
        return false;
    } else if (word.length() == 0) {
        // base case: Found nodes all the way down.
        // If we are looking for a prefix, this means this path IS a prefix,
        // so we should return true.
        // If we are looking for an exact word match rather than a prefix,
        // we must check the isWord flag to see that this word was added
        return (isPrefix ? true : node->isWord());
    } else {
        // recursive case: follow appropriate child pointer for one letter
        return containsHelper(node->child(word[0]), word.substr(1), isPrefix);
    }
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::removeHelper(TrieNode*& node, std::string word, const std::string& originalWord, bool isPrefix) {
    if (node == NULL) {
        // base case: dead end; this word/prefix must not be contained
        return false;
    } else if (word.empty()) {
        // base case: we have walked all of the letters of this word/prefix
        // and now we must do the removal
        if (isPrefix) {
            // remove this node and all of its descendents
            removeSubtreeHelper(node, originalWord);   // removes from m_allWords, sets m_size
            node = NULL;
        } else {
            // remove / de-word-ify this node only
            if (node->isLeaf()) {
                delete node;
                node = NULL;
            } else {
                if (node->isWord()) {
                    node->setWord(false);
                    m_allWords.remove(originalWord);
                    m_size--;
                }
            }
        }
        return true;
    } else {
        // recursive case: chop off first letter, traverse the rest
        return removeHelper(node->child(word[0]), word.substr(1), originalWord, isPrefix);
    }
}

// remove/free this node and all descendents
void Lexicon::removeSubtreeHelper(TrieNode*& node, const std::string& originalWord) {
    if (node != NULL) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            removeSubtreeHelper(node->child(letter), originalWord + letter);
        }
        if (node->isWord()) {
            m_allWords.remove(originalWord);
            m_size--;
        }
        delete node;
        node = NULL;
    }
}

void Lexicon::deepCopy(const Lexicon& src) {
    for (std::string word : src.m_allWords) {
        add(word);
    }
}

void Lexicon::deleteTree(TrieNode* node) {
    if (node != NULL) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            deleteTree(node->child(letter));
        }
        delete node;
    }
}

/*
 * We just delegate to DawgLexicon, the old implementation, to read a binary
 * lexicon data file, and then we extract its yummy data into our trie.
 */
void Lexicon::readBinaryFile(std::string filename) {
    DawgLexicon ldawg(filename);
    for (std::string word : ldawg) {
        add(word);
    }
}

Lexicon& Lexicon::operator=(const Lexicon& src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

std::ostream& operator <<(std::ostream& out, const Lexicon& lex) {
    out << lex.m_allWords;
    return out;
}

std::istream& operator >>(std::istream& is, Lexicon& lex) {
    char ch;
    is >> ch;
    if (ch != '{') {
        error("Lexicon::operator >>: Missing {");
    }
    lex.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            std::string value;
            readGenericValue(is, value);
            lex.add(value);
            is >> ch;
            if (ch == '}') break;
            if (ch != ',') {
                error(std::string("Lexicon::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}


// returns true if the given file (probably) represents a
// binary DAWG lexicon data file
static bool isDAWGFile(std::string filename) {
    char firstFour[4], expected[] = "DAWG";
    std::ifstream istr(filename.c_str());
    if (istr.fail()) {
        error(std::string("Lexicon::addWordsFromFile: Couldn't open lexicon file ") + filename);
    }
    istr.read(firstFour, 4);
    bool result = strncmp(firstFour, expected, 4) == 0;
    istr.close();
    return result;
}

// lowercases the string and also scrubs out non-alphabetic characters,
// such that it can be used as a Lexicon word
static void scrub(std::string & str) {
    size_t nChars = str.length();
    size_t outIndex = 0;
    for (size_t i = 0; i < nChars; i++) {
        std::string::value_type ch = tolower(str[i]);
        if (ch >= 'a' && ch <= 'z') {
            str[outIndex++] = ch;
        } // else throw away
    }
    if (outIndex != nChars) {
        str.erase(outIndex, nChars - outIndex);
    }
}
