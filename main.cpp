#include "WordTree.hpp"
#include "rlutil.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);
    int yeah = 0;
    while (!inFile.eof())
    {
        std::string word;

        std::getline(inFile, word);
        // Need to consume the carriage return character for some systems, if it exists
        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        // Keep only if everything is an alphabetic character -- Have to send isalpha an unsigned char or
        // it will throw exception on negative values; e.g., characters with accent marks.
        if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                        {
                            return std::isalpha(c);
                        }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c)
                           {
                               return static_cast<char>(std::tolower(c));
                           });
            wordTree->add(word);
            yeah += 1;
        }
    }
    std::cout << yeah << std::endl;

    return wordTree;
}
int main()
{

    std::shared_ptr<WordTree> tree = std::make_shared<WordTree>();
    // std::cout << tree->size() << std::endl;
    // std::cout << tree->m_root->children.size() << std::endl;

    // testing my knowledge of this program...
    // inserting the word 'fared' into the tree..

    tree->add("aeah");
    if (tree->find("aeah"))
    {
        std::cout << "Found aeah" << std::endl;
    }

    // NOTE: the size of dictTree should be 10,681
    std::shared_ptr<WordTree> dictTree = readDictionary("dictionary.txt");

    // TODO: Fix tree size method.
    // std::cout << tree->size() << std::endl;

    // std::cout << dictTree->size() << std::endl;
    std::vector<std::string> words = dictTree->predict("pred", 10);
    std::cout << words.size() << std::endl;
    for (unsigned int i = 0; i < words.size(); i++)
    {
        std::cout << words.at(i) << std::endl;
    }

    rlutil::cls();
    int xPos = 1;
    /*rlutil::locate(1, 0);
    std::cout << "(1, 0)";
    rlutil::locate(5, 0);
    std::cout << "(5, 0)";
    rlutil::locate(10, 0);
    std::cout << "(10, 0)";
    rlutil::locate(20, 0);
    std::cout << "(20, 0)";
    rlutil::locate(10, 20);
    std::cout << "(10, 20)";*/

    std::string word;

    while (true)
    {
        rlutil::locate(xPos, 1);


        char key = rlutil::getkey();
        if (key == rlutil::KEY_BACKSPACE)
        {
            xPos -= 1;
            rlutil::locate(xPos, 1);
            std::cout << " ";
            key = '\0';
            xPos -= 1;
        }
        word += key;
        rlutil::setChar(key);
        unsigned int prev = xPos;
        int rows = rlutil::trows();
        std::vector<std::string> words = dictTree->predict(word, rows - 1);
        int yPos = 10;
        for (int i = 0; i < words.size(); i++)
        {
            rlutil::locate(0, yPos);
            std::cout << words.at(i) << std::endl;

            yPos += 10;
        }

        xPos += 1;

        // std::cout << xPos;
    }

    return 0;
}