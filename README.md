# huffman
##Compilation
To compile the program, run `make all`
##Usage
To compress a file, run the executable and give it the name as a file to compress as a parameter. For example, `./huff big.txt` will save a compressed file as big.huff.

To decompress, run the executable and pass in the compressed file and optionally a name for the output file. By default, the decompressed file will be named as "inputFileName.txt". For example, `./huff big.huff` will decompress big.huff into big.txt.
