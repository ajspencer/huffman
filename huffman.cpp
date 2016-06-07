/**
 * Author: Andrew Spencer
 */
#include<string>
#include<cmath>
#include<string.h>
#include<vector>
#include<queue>
#include<fstream>
#include<iostream>
#include<stack>

#include "huffman.h"

/*
 * Constructor for the node class
 * @param letter | the letter being stored in the node
 * @param freq | the frequency of the letter being stored in the node
 * @param left | the node to the left in the tree, null if does not exist
 * @param right | the node to the right in the tree, null if does not exist
 */
Node::Node(unsigned char letter, int freq, Node *left, Node *right) {
    this->letter = letter; 
    this->freq = freq;
    this->right = right;
    this->left = left;
}

/*
 * Prints the usage of the program
 * Called when the user provides incorrect input
 */
void printUsage() {
    cout << "Input: inputfile.* | no additional arguments\n";
    cout << "Output: inputfile.huff [outputFile.*] | output defaults to inputfile.txt\n";
}

/**
 * Comparator for comparing queues.
 * @param *a | a pointer to the first Node
 * @param *b | a pointer to the second Node
 */
bool compareQueues(Node *a, Node *b) {
        return a->freq > b->freq;
}

/**
 * Function to get the encoding of the file from the array of encodings
 */
string generateEncoding(vector<string> encodings, string filename) {
    ifstream inputStream(filename.c_str());
    //Store a vector to hold the encoding for the file
    string fileCode;
    //Get the name of the file from the filepath
    int indexOfSlash = filename.rfind("\\");
    string fileName = indexOfSlash == -1 ? filename : filename.substr(indexOfSlash+1);
    int ch;
    //Loop through the file getting a character at a time
    while( (ch = inputStream.get()) != EOF) { 
        //(int)c gives the ascii value of the character being read into the file
        unsigned char c = (unsigned char) ch;
        int asciiValue = (int) c;
        //Get the encoding for the current character, and insert into into the list of encodings
        string encoding = encodings.at(asciiValue);
        fileCode += encoding;
    }
    inputStream.close();
    return fileCode;
}

/**
 * Function that finishes the encoding process by writing the encodings to a file
 * @param encodings | the array that holds the encoding for each ascii value
 * @param filename | the path to the output file TODO: make this true
 */
void writeToFile(string fileCode, vector<int> frequencies, string filename) {
        //Open the output stream, ensure we start at the begin of the file 
        ofstream outputStream(filename, ios::trunc);
        //We prepend the number of frequencies we are encoding in the file
        //Leave a space for the number we inserted into the stream
        outputStream << ' ';
        //To ensure that the frequency tree can be reconstructed for decoding, we will write the frequency of every value in the file with ' ' to seperate them.
        int numberWritten = 0;
        for(int i = 0; i < frequencies.size(); i++) {
            //Only write the value to the file of the frequency is nonzero
            if(frequencies.at(i) != 0) {
                numberWritten++;
                outputStream << (unsigned char)i;
                outputStream << frequencies.at(i);
                outputStream << ' ';
            }
        }
        //Change the write mode to binary now that we are done encoding those.
        outputStream.flags(ios::binary);
        //Get the current location of the file pointer, set it back to 0 to write the number put
        int currentLocation = outputStream.tellp();
        //Set the current location of the file pointer to zero, write the number we wrote
        outputStream.seekp(0);
        outputStream.put(numberWritten);
        outputStream.seekp(currentLocation);
        //Define the byte buffer for writing to the file
        char byteBuffer = 0;
        //Now, write the ascii code for our character that signifies that the transmission is over the ASCII value 4, end of transmission
        //We prepend the solution with how many bits at the end we should throw out
        char byte = 8 - fileCode.size() % 8;
        outputStream.put(byte);
        //Now, write the file codes to the file
        for(int i = 0; i < fileCode.size(); i++) {
            unsigned char bit = fileCode.at(i);
            //If the current bit is one, change the byte buffer at that index to one
            if(bit == '1') {
                //Set the relevent bit to one
                byteBuffer |= (1 << (7 - (i % 8)));
            }
            //if the current bit is zero we don't need to do anything since the buffer is initally zero
            //every 8th iteration, write the byte to the file
            if ((i+1) % 8 == 0 && i > 0) {
                outputStream.write(&byteBuffer, sizeof(byteBuffer));
                //Zero out the byte buffer 
                byteBuffer = 0;
            }
    }
    //Write the last of the bytes to the file. We zero it out after each iteration, so we don't need to worry about zeroing it out to pack the leftover bits. 
    outputStream.write(&byteBuffer, sizeof(byteBuffer));
    outputStream.close();
}

/**
 * Retrieves the frequency array and encoding given the path to a .huff file. Returns the frequency array, and the encoding in the last index.
 * @param filepath | a path to a .huff file to be decoded
 * @return frequencyList | a list of the frequency of each ascii value in the encoded file, and the encoding stored in the last index
 */
vector<string> readFromEncodedFile(string filepath) {
    char byteBuffer;
    vector<string> frequencyList(257);
    ifstream inputStream(filepath, ios::binary);
    inputStream.get(byteBuffer);
    int numEncoded = (int) byteBuffer;
    for(int i = 0; i < numEncoded; i++) {
        inputStream.get(byteBuffer); 
        int charRead = (unsigned char) byteBuffer;
        //cout << "char read: " << charRead << "\n";
        //We know that we are done reading when our byte buffer reads our deliminating character
        string frequency = "";
        //Start reading in the number
        inputStream.get(byteBuffer);
        while(byteBuffer != ' ') {
            frequency += byteBuffer;
            inputStream.get(byteBuffer);
        }
        frequencyList.at(charRead) = frequency;
    }
    //Now that we have the frequency tree, generate the encoding for the file
    string encoding = "";
    while( inputStream.get(byteBuffer) ) {
        for(int i = 0; i < 8; i++) {
            unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
            if(byteBuffer & mask[i]) encoding += '1';
            else encoding += '0';
        }
    }
    //The offset is encoded in the first 8 characters of the encoding after the frequency tree, so read the offest in and delete that many at the end
    int padding = 0;
    //define a string to hold the encoding of how much padding is at the end
    string paddingEncoding = encoding.substr(0, 8);
    //Erase that from the encoding
    encoding.erase(0, 8);
    for(int i = 0; i < paddingEncoding.size(); i++) {
        if(paddingEncoding.at(7 - i) == '1') padding += pow(2, i); 
    }
    //Now, erase the extra padding from the end of the string
    if(padding > 0) {
        encoding.erase(encoding.size() - padding, encoding.size());
    }
    frequencyList.at(256) = encoding;
    return frequencyList;
}

/**
 * Traverses the tree of frequencies and stores the values of all of the encoding stored in the tree
 * @param head | the head of the tree 
 * @return encodings | an array that contains the encoding of each character stored in the file
 */
vector<string > generateFrequencies(Node* head) {
    //make an empty array for the initial function call
    vector<string> encodings(256);
    //Define a struct to hold the current node, along with the encoding up to that point
    struct nodeAndEncoding {
        Node *n; 
        //The encoding of this node
        string encoding;
    };
    //Stack for doing a DFT of the tree
    stack<nodeAndEncoding> s;
    //Make a struct for the top node and add it to the stack
    nodeAndEncoding topNode = {};
    topNode.n = head;
    topNode.encoding = string();
    s.push(topNode);
    //Do the DFT of the tree and update the value of the encoding as we go along, save in the array when we reach a leaf
    //The new encoding value is the old value + 1 when we go right, and + 0 when we go left
    while(! s.empty()) {
        nodeAndEncoding currentNodeAndEncoding = s.top();
        s.pop();
        Node *currentNode = currentNodeAndEncoding.n;
        //If there is not a left or a right child, we know that this is a leaf and it represents a letter
        if(currentNode->left == NULL && currentNode->right == NULL) {
            string encoding = currentNodeAndEncoding.encoding;
            int index = (int) currentNode->letter;
            //cout << "here is the index " << index;
            encodings.at(index) = encoding;
            continue;
        }
        //If the current node has a left child, add the left child to the search
        if(currentNode->left != NULL && currentNode->right != NULL) { 
            //Construct the struct for the left node
            string leftEncoding(currentNodeAndEncoding.encoding);
            //Since we are going to the left, we add zero
            leftEncoding += '0';
            nodeAndEncoding leftNodeAndEncoding = {};
            leftNodeAndEncoding.n = currentNode->left;
            leftNodeAndEncoding.encoding = leftEncoding;
            s.push(leftNodeAndEncoding);
            //Now do the same thing for the right
            string rightEncoding(currentNodeAndEncoding.encoding);
            //Since we are going to the right, we add one
            rightEncoding += '1';
            nodeAndEncoding rightNodeAndEncoding = {};
            rightNodeAndEncoding.n = currentNode->right;
            rightNodeAndEncoding.encoding = rightEncoding;
            s.push(rightNodeAndEncoding);
        }
    }
    return encodings;
}

/**
 * Function to read in the file and construct the array of the frequency of each ascii value
 * @param filepath | the path to the file that will be read in
 * @return frequencyArray | an array of the frequency of each ascii value
 */
vector<int> constructFrequencyArray(string filepath) {
    ifstream stream(filepath);
    //Array to hold the frequency of each number
    vector<int> frequencyArray(256);
    char c;
    //Loop through the file, getting a character at a time
    while( stream.get(c) ) {
        unsigned char ch = c;
        //(int)c gives the ascii value of the character being read into the file
        frequencyArray[(int)ch]++;
    }
    stream.close();
    return frequencyArray;
}

/**
 * Function to fill in the frequency tree given the frequency array
 * @param frequencyArray | an array of the frequency of each file
 */
Node* constructFrequencyTree(vector<int> frequencyArray) {
    //define the comparision function for the queue as a lambda
    //We use a priority queue to keep track of the lowest two frequencies easily
    priority_queue<Node*, vector<Node*>, decltype(&compareQueues)> nodes(&compareQueues);
    for(int i = 0; i < 256; i++) {
        if(frequencyArray.at(i) > 0) {
            Node *n = new Node((char) i, frequencyArray.at(i), NULL, NULL);
            nodes.push(n);
        }
    }
    //construct tree
    int itrs = nodes.size();
    Node *z;
    for(int i = 0; i < itrs - 1; i++) {
        Node *x = nodes.top();
        nodes.pop();
        Node *y = nodes.top();
        nodes.pop();
        //The lesser of the two goes on the left, and the greater goes on the right
        z = new Node('\0', x->freq + y->freq, x, y);
        nodes.push(z);
    }
    return z;
}

/**
 * Decodes the file and writes the result to the given name for the ouput file
 * @param *top | the node that points to the top of the frequency tree for the file
 * @param fileEncoding | a string that holds the encoding for the file
 * @param filename | the path to the output file
 */
void decode(Node *top, string fileEncoding, string filename) {
    ofstream outputStream(filename, ios::trunc);
    Node *currentNode = top;
    //Iterate down the tree based on the encoding and whenever we hit a leaf write the character to the file
    for(int i = 0; i < fileEncoding.length(); i++) {
        //if the current node has a letter, we are done reading that code and we should print the letter and return to the top
        if(fileEncoding.at(i) == '1') currentNode = currentNode->right;
        else                          currentNode = currentNode->left;
        if(currentNode->letter) {
            outputStream.put(currentNode->letter);
            currentNode = top;
        }
    }
    outputStream.close();
}


