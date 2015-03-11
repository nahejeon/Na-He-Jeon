// Name: Na He Jeon
// This program is used to compress the data of a file into smaller size for more feasible storage
// and sharing. It can also be used to decompress the compressed data back to the original size.
// I received help from the lecture slides and Section Leaders Michael Weingert and Mitch Dumovic.

#include "encoding.h"
#include "pqueue.h"
#include "strlib.h"

void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string code);

/*
 * buildFrequencyTable: builds a map of the frequencies of each character of a given input.
 * Input: istream& input - the string or file input of which the frequencies of each character will
 *                         be counted and stored
 * Output: Map<int, int> - a map with the integer values of characters as keys and the occurance of
 *                         those characters in the input as values.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    int i = input.get();
    while (i != -1){
        if (!freqTable.containsKey(i)){
            freqTable.put(i, 1);
        } else{
            freqTable[i]++;
        }
        i = input.get();
    }
    freqTable.put(PSEUDO_EOF, 1);
    return freqTable;
}

/*
 * buildEncodingTree: builds a tree of Huffmannodes based on a frequency table. The more frequent
 *                    characters get stored near the top, whereas the less frequent ones get stored
 *                    near the bottom.
 * Input: Map<int, int>& freqTable - the map of freqencies on which the tree will be based
 * Output: Huffmannode* - the pointer to the root of the tree
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> queue;
    for(int i : freqTable){
        HuffmanNode* node = new HuffmanNode(NOT_A_CHAR, 0, NULL, NULL);
        node->character = i;
        double count = freqTable[i];
        node->count = count;
        queue.enqueue(node, count);
    }
    while(queue.size() >= 2){
        HuffmanNode* node = new HuffmanNode(NOT_A_CHAR, 0, NULL, NULL);
        HuffmanNode* child1 = queue.dequeue();
        HuffmanNode* child2 = queue.dequeue();
        node->character = NOT_A_CHAR;
        node->count = child1->count + child2->count;
        double count = node->count;
        node->zero = child1;
        node->one = child2;
        queue.enqueue(node, count);
    }
    HuffmanNode* root = queue.peek();
    return root;
}

/*
 * buildEncodingMap: builds a map of the codes of the characters based on the pointer to the root of
 *                   a Huffman tree.
 * Input: HuffmanNode* encodingTree - the root of a Huffman tree
 * Output: Map<int, string> - the map in which the keys are the integer values of the characters
 *                            and the values are the codes that represent those characters
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    if(encodingTree == NULL){
        return encodingMap;
    } else{
        buildEncodingMapHelper(encodingTree,encodingMap, "");
        return encodingMap;
    }
}

/*
 * buildEncodingMapHelper: helps the buildEncodingMap function through recursion.
 * Input: HuffmanNode* encodingTree - the pointer to the root of an encoding tree
 *        Map<int, string>& encodingMap - the map of integer values of characters and string codes of
 *                                        those characters that will be modified throughout this helper fn
 *        string code - the string that stores the code of an integer value of a character as the
 *                      function recursively works on
 * Output: nothing
 */
void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string code){
    if(encodingTree->character != NOT_A_CHAR){
        int key = encodingTree->character;
        encodingMap.put(key, code);
    } else {
        if(encodingTree->zero != NULL){
            buildEncodingMapHelper(encodingTree->zero, encodingMap, code + "0");
        }
        if(encodingTree->one != NULL){
            buildEncodingMapHelper(encodingTree->one, encodingMap, code + "1");
        }
    }
}

/*
 * encodeData: encodes the given input string/file with the given encoding map and puts it in
 *             an obitstream
 * Input: istream& input - the input stream from which data is taken to be encoded
 *        Map<int, string>& encodingMap - the map that stores the codes for each integerized character
 *                                        of the data in the istream
 *        obitstream& output - the obistream to which the encoded data is printed
 * Output: nothing
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int i = input.get();
    while(i != -1){
        string code = encodingMap[i];
        while(code.size() > 0){
            int n = code[0] - '0';
            output.writeBit(n);
            code = code.substr(1);
        }
        i = input.get();
    }
    string code = encodingMap[PSEUDO_EOF];
    while(code.size() > 0){
        int n = code[0] - '0';
        output.writeBit(n);
        code = code.substr(1);
    }
}

/*
 * decodeData: decodes the given input stream of bits into readable data using the given encoding tree.
 * Input: ibitstream& input - the input stream from which the bits of encoded data is taken in
 *        HuffmanNode* encodingTree - the pointer to the root of the encoding tree which is going to
 *                                    be used to decode the data
 *        ostream& output - the output stream on which the data will be decoded in readable form
 * Output: nothing important honestly, but anything that comes out of this is very unlikely to have any function in the totality of normal active popular society because really in the grand scheme of things it's not necessary to activate or actualize any sort of mechanism as it pertains to normalcy in relevant society, and is actually totally a joke pulled over on everyone who says "Oh yeah I'm a techie LOL CS is SO MUCH FUN OHMAIGAWDDDDD <3 lalalalalalaal I'm flying around in the clouds because I'm a special Silicon Valley fairy or something lelelellelelelelelelee <3 tfti to actual knowledge like history or literature or something"
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int i = input.readBit();
    HuffmanNode* node = encodingTree;
    while(i != -1){
        if(i == 0){
            node = node->zero;
            if(node->isLeaf()){
                if(node->character == PSEUDO_EOF){
                    return;
                } else{
                    output.put(node->character);
                    node = encodingTree;
                    i = input.readBit();
                }
            } else{
                i = input.readBit();
            }
        } else if (i == 1){
            node = node->one;
            if(node->isLeaf()){
                if(node->character == PSEUDO_EOF){
                    return;
                } else{
                    output.put(node->character);
                    node = encodingTree;
                    i = input.readBit();
                }
            } else{
                i = input.readBit();
            }
        }
    }
}

/*
 * compress: compresses a given string/file and outputs it with the header that specifies the
 *           encoding information
 * Input: istream& input - the input stream of string/file that is going to be compressed
 *        obitstream& output - of the compressed file
 * Output: nothing
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    input.clear();
    input.seekg(0, ios::beg);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/*
 * decompress: decompresses a given encoded file and outputs the decompressed version
 * Input: ibitstream& input - of the compressed version of the file
 *        ostream& output - of the decompressed version of the file
 * Output: nothing
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

/*
 * freeTree: frees the memory of all the nodes of a given tree
 * Input: HuffmanNode* node - points to the root of the tree
 * Output: nothing
 */
void freeTree(HuffmanNode* node) {
    if(node == NULL){
        return;
    } else{
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
