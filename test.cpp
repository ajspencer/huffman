#include <iostream>     // std::cin, std::cout
#include <fstream>      // std::ifstream
#include<bitset>
#include<string>

int main () {

  std::ifstream is("test.txt");     // open file

  char ch;
  while ( is.get(ch) ) {          // loop getting single characters
    unsigned char c = (unsigned char) ch;
    std::bitset<8> test(c);
    std::cout << "char: " << (int) c << "has bits: ";
    std::cout << test << "\n";
  }
  

  is.close();                // close file

  return 0;
}
