// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
// TODO: include any other headers you need
#include <bits/stdc++.h>
#include "pqueue.h"
#include <string>
#include "strlib.h"
#include "filelib.h"

Map<int, int> buildFrequencyTable(istream& input) {
    // TODO: implement this function
    Map<int, int> freqTable;   // this is just a placeholder so it will compile
    string tempstring;
    getline(input, tempstring);
    cout << tempstring << endl;
    for (char tempchar : tempstring) {
        freqTable[tempchar]++;
    }
    freqTable[256] = 1;
//    cout << freqTable << ' ';
    return freqTable;          // this is just a placeholder so it will compile
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    // TODO: implement this function
    PriorityQueue<HuffmanNode*> pq;

    for(int charac : freqTable) {
        HuffmanNode* node = new HuffmanNode;
        node->character = charac;
        node->count = freqTable[charac];
        node->zero = NULL;
        node->one = NULL;
        pq.enqueue(node, node->count);
    }
//    makeTree(pq);
    while (pq.size() > 1) {
        HuffmanNode* left = pq.dequeue();
        HuffmanNode* right = NULL;
        if (!pq.isEmpty()) {
            right = pq.dequeue();
        }
        HuffmanNode* parent = new HuffmanNode;
        parent->character = NOT_A_CHAR;
        parent->one = right;
        parent->zero = left;
        parent->count = right->count + left->count;
        pq.enqueue(parent, parent->count);
    }
    return pq.dequeue();   // this is just a placeholder so it will compile
}

void traverseTree(Map<int,string>& encodingMap, string prefix, HuffmanNode* encodingTree) {
    if (encodingTree->character != NOT_A_CHAR) {
        encodingMap[encodingTree->character] = prefix;
    }
    if (encodingTree->zero != NULL) { // if character is NOT_A_CHAR
        traverseTree(encodingMap, prefix+"0", encodingTree->zero);
    }
    if (encodingTree->one != NULL) { // if character is NOT_A_CHAR
        traverseTree(encodingMap, prefix+"1", encodingTree->one);
    }
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    Map<int, string> encodingMap;   // this is just a placeholder so it will compile
    string prefix = "";
    traverseTree(encodingMap, prefix, encodingTree);
    return encodingMap;             // this is just a placeholder so it will compile
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    // TODO: implement this function
    string eachCode;
    char character;
    while (input.get(character)) {
        eachCode = encodingMap[character];
        for (int i = 0; eachCode[i] != '\0'; i++)
            output.writeBit(eachCode[i]-'0');
    }
    //write EOF
    eachCode = encodingMap[PSEUDO_EOF];
    for (int i = 0; eachCode[i] != '\0'; i++) {
        output.writeBit(eachCode[i]-'0');
    }
}

void untraverseTree(Map<string,int>& decodingMap, string prefix, HuffmanNode* encodingTree) {
    if (encodingTree->character != NOT_A_CHAR) { // LEAF
        decodingMap[prefix] = encodingTree->character;
    }
    if (encodingTree->zero != NULL) { // if character is NOT_A_CHAR
        untraverseTree(decodingMap, prefix+"0", encodingTree->zero);
    }
    if (encodingTree->one != NULL) { // if character is NOT_A_CHAR
        untraverseTree(decodingMap, prefix+"1", encodingTree->one);
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    // TODO: implement this function
    Map<string,int> decodingMap;
    string prefix = "";
    untraverseTree(decodingMap, prefix, encodingTree);
    string bitreadStr = "";
    while (true) {
        int bitreadInt = input.readBit();
        bitreadStr += integerToString(bitreadInt);

        if (decodingMap.containsKey(bitreadStr)) {
            int character = decodingMap[bitreadStr];
            if (character == PSEUDO_EOF) {
                break;
            }
            else {
                output.put(character);
                bitreadStr = "";
            }
        }
    }
}
void compress(istream& input, obitstream& output) {
    // TODO: implement this function
    // - read the input file one char at a time
    // - build an encoding of its contents
    // - write a compressed version of that input file including a header, to the output
    Map<int, int> freqTable = buildFrequencyTable(input);

    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    output << freqTable;
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

void decompress(ibitstream& input, ostream& output) {
    // TODO: implement this function
    Map<int, int> freqTable;
    input >> freqTable;
    cout << freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
    if (node == NULL) {
        return;
    }
    freeTree(node->zero);
    freeTree(node->one);
    delete node;
}


