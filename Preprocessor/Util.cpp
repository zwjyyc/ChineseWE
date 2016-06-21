#include "Util.h"

int main(int argc, char** argv){
    Util util;

    string srcFile = argv[1];
    string outFile = argv[2];
    
    string dictFile = argv[3];

    //util.words2Chars(srcFile, outFile);
    //util.get_words(srcFile, dictFile);
    util.words2Chars(srcFile, dictFile, outFile);
    return 0;
}
