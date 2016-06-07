/**
 * Author: Andrew Spencer
 */
#include<string>
#include<vector>
#include<iostream>
#include <unistd.h>
#include "huffman.h"
int main(int argc, char **argv) {
    //If they only provide one argument, print the usage and return
    if(argc < 2 || argc > 3) {
        printUsage();
        return 1;
    }
    //Define strings for the input and output files
    string inputFile(argv[1]);
    //Ensure that the input file exists
    if(( access( inputFile.c_str(), F_OK ) == -1 )) {
        cout << "Input file does not exist\n";
        return 1;
    }
    //if they provide an output file but we are encoding, show the usage
    if(inputFile.find(".huff") == string::npos && argc == 3) {
        printUsage();
        return 1;
    }
    //Generate output string
    string output;
    if(argc == 3) {
        output = argv[2];
    }
    //If both files are .huff, show the usage
    if(argc == 3 && inputFile.find(".huff") != -1 && output.find(".huff") != -1) {
        printUsage();
        return 1;
    }
    //The case where the first file is not a .huff file we encode
    if(inputFile.find(".huff") == -1) { 
        vector<int> frequencyArray = constructFrequencyArray(inputFile);
        Node *z = constructFrequencyTree(frequencyArray);
        vector<string> frequencies = generateFrequencies(z);
        string encoding = generateEncoding(frequencies, inputFile);
        string outputFile(inputFile.substr(0, inputFile.find(".")));
        outputFile += ".huff";
        writeToFile(encoding, frequencyArray, outputFile);
        cout << "Output written to " << outputFile << "\n";
        return 0;
    }
    //In the case where the first file
    if(inputFile.find(".huff") != -1) {
        vector<string> oldFrequencies = readFromEncodedFile(inputFile);
        string fileEncoding = oldFrequencies.at(256);
        oldFrequencies.pop_back();
        vector<int> newFrequencies(256);
        for(int i = 0; i < newFrequencies.size(); i++) {
            if(oldFrequencies.at(i) != "") 
            newFrequencies.at(i) = stoi(oldFrequencies.at(i));
        }
        Node *top = constructFrequencyTree(newFrequencies);
        //If an output file is specified, create a string for it
        string outputFile = argc == 3 ? argv[2] : output;
        decode(top, fileEncoding, outputFile);
        cout << "Output written to " << outputFile << "\n";
        return 0;
    }
}

