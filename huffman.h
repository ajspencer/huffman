/**
 * Author: Andrew Spencer
 */

#ifndef huffman
#define huffman
 

using namespace std;

/**
 * A node in the frequency tree. The node either represents a letter, 
 * or a combination of letters in the tree. 
 * Letter is not null iff the node is a leaf in the frequency tree.
 */
class Node {
    public: 
        unsigned char letter; 
        int freq;
        Node *left;
        Node *right;
        Node(unsigned char letter, int freq, Node* left, Node* right);
};

/*
 * Prints the usage of the program
 * Called when the user provides incorrect input
 */
void printUsage();

/*
 * Comparator for comparing queues.
 * @param *a | a pointer to the first Node
 * @param *b | a pointer to the seconde Node
 */
bool compareQueues(Node *a, Node *b);

/**
 * Function to get the encoding of a file from the array of encodings
 */
string generateEncoding(vector<string> encodings, string filename);

/**
 * Function that finishes the encoding process by writing the encodings to ta file
 * @param encodings | the array that hold the encoding for each ascii value
 * @param filename  | the path to the output file
 */
void writeToFile(string fileCode, vector<int> frequencies, string filename);

/**
 * Retrieves the frequency array and encoding given the path to a .huff file.
 * Returns the frequency array and the encoding in the last index. 
 * @param filepath | the path to a .huff file to be decoded
 * @return frequencyList | a list of the frequency of each ascii value in the encoded file, and the encoding stored in the last index
 */
vector<string> readFromEncodedFile(string filename);

/**
 * Travereses the tree of frequencies adn stores the values of all of the encodings stored in the tree.
 * @param head | the head of the tree
 * @return encodings | an array that contains the encoding of each character stored in the file
 */
vector<string> generateFrequencies(Node *head);

/**
 * Function to read in the file and construct the array of the frequency of each ascii value
 * @param filepath | the path to the file that will be read in 
 * @return frequencyArray | an array of the frequency of each ascii value
 */
vector<int> constructFrequencyArray(string filename);

/*
 * Function to fill in the frequency tree given the frequency array
 * @param frequencyArray | an array of the frequency of each file
 */
Node* constructFrequencyTree(vector<int> frequencyArray);

/*
 * Decodes the file and writes the result to the given name for the output file
 * @param *top | the node that points to the top of the frequency tree for the file
 * @param fileEncoding | a string that holds the encoding for the file
 * @param filename | the path to the output file
 */
void decode(Node *top, string fileEncoding, string filename);

#endif
