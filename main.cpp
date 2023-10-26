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
        }
    }

    return wordTree;
}
int main()
{


    std::shared_ptr<WordTree> dictTree = readDictionary("dictionary.txt");

    // Clear the screen.
    rlutil::cls();
    int xPos = 1;

    std::string word;
    bool done = false;

    // This vector 'phrase' will contain what we have types so far. If there is a space, it is added to the phrase.
    // In my implementation, I have spaces in the phrase, so if we are currently on a space, there is no given prediction.
    std::vector<std::string> phrase;
    phrase.push_back("");
    while (!done)
    {
        rlutil::locate(xPos, 1);

        int key = rlutil::getkey();

        xPos = 1;
        switch (key)
        {
            case rlutil::KEY_ESCAPE:
                done = true;
                break;
            case rlutil::KEY_BACKSPACE:
                if (phrase.size() > 1)
                {
                    if (word == "")
                    {
                        // pop back of vector and set word to previous word in phrase
                        phrase.pop_back();
                        word = phrase.at(phrase.size() - 1);
                    }
                    else
                    {
                        // if current word is not blank, pop back end of word
                        word = phrase.at(phrase.size() - 1);
                        word.pop_back();
                        phrase.pop_back();
                        phrase.push_back(word);
                    }
                }
                else
                {
                    if (word != "")
                    {
                        word = phrase.at(phrase.size() - 1);
                        word.pop_back();
                        phrase.pop_back();
                        phrase.push_back(word);
                    }
                }

                rlutil::locate(xPos, 1);
                std::cout << "";

                break;
            case rlutil::KEY_SPACE:
                // add blank word to vector
                word = "";
                phrase.push_back(word);
                break;
            default:
                // add key to current word
                phrase.pop_back();
                word.push_back(static_cast<char>(key));
                phrase.push_back(word);
                break;
        }
        rlutil::locate(xPos, 1);

        // print out the current phrase
        for (std::string tempWord : phrase)
        {
            rlutil::locate(xPos, 1);

            for (char letter : tempWord)
            {
                rlutil::setChar(letter);
                xPos += 1;
                rlutil::locate(xPos, 1);
            }
            rlutil::setChar(' ');
            xPos += 1;
            rlutil::locate(xPos, 1);
        }
        xPos -= 1;
        // Initialize new vector for giving blank words
        std::vector<std::string> tempWords;
        unsigned int yPos = 3;
        unsigned int tempX = 1;
        int rows = rlutil::trows();

        // Longest word I could find was 20 ish in length, so I have 25 spaces
        for (int i = 0; i < rows - 4; i++)
        {
            tempWords.push_back("                         ");
        }

        std::vector<std::string> words = dictTree->predict(word, static_cast<std::uint8_t>(rows - 4));

        // Add predicted words to tempWords vector, and add spaces at end to delete 'extra' characters that would be left over
        for (unsigned int j = 0; j < words.size(); j++)
        {
            tempWords.at(j) = words.at(j);
            tempWords.at(j) += "                    ";
        }
        rlutil::locate(tempX, yPos);

        std::cout << "--- prediction ---";
        yPos += 1;
        rlutil::locate(tempX, yPos);

        // Print out the predicted words
        for (std::string tempWord : tempWords)
        {
            rlutil::locate(tempX, yPos);

            std::cout << tempWord;
            tempX = 1;
            rlutil::locate(tempX, yPos);
            yPos += 1;
        }

        rlutil::locate(xPos, yPos);
        std::cout.flush();
    }

    return 0;
}