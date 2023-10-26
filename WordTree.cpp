
#include "WordTree.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <locale>
#include <queue>
#include <string>
#include <vector>
TreeNode::TreeNode()
{
}
WordTree::WordTree()
{
}
void WordTree::add(std::string word)
{
    if (word.size() == 0)
    {
        return;
    }

    // If all of the characters in given word are not punctuation...
    if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                    {
                        return std::isalpha(c);
                    }))
    {
        std::transform(word.begin(), word.end(), word.begin(), [](char c)
                       {
                           return static_cast<char>(std::tolower(c));
                       });
    }
    else
    {
        return;
    }

    // Loop through each node, one character at a time.
    std::shared_ptr<TreeNode> root = m_root;
    for (unsigned int i = 0; i < word.size(); i++)
    {
        // Converstion to find placement in vector.
        unsigned int value = int(word[i]) - 97;
        if (root->children.at(value) == nullptr)
        {
            std::shared_ptr<TreeNode> node = std::make_shared<TreeNode>();
            root->children.at(value) = node;
        }
        root = root->children.at(value);

        if (i == word.size() - 1)
        {
            if (!root->endOfWord)
            {

                root->endOfWord = true;
                m_sizeOfTree += 1;
            }
        }
    }
}
bool WordTree::find(std::string word)
{
    if (word.size() == 0)
    {
        return false;
    }
    // If all of the characters in given word are not punctuation...

    if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                    {
                        return std::isalpha(c);
                    }))
    {
        std::transform(word.begin(), word.end(), word.begin(), [](char c)
                       {
                           return static_cast<char>(std::tolower(c));
                       });
    }
    else
    {
        return false;
    }
    std::shared_ptr<TreeNode> root = m_root;

    for (unsigned int i = 0; i < word.size(); i++)
    {
        // Converstion to find placement in vector.

        unsigned int value = int(word[i]) - 97;
        if (root->children.at(value) == nullptr)
        {

            return false;
        }
        root = root->children.at(value);
        if (i == word.size() - 1)
        {
            if (root->endOfWord)
            {
                return true;
            }
            return false;
        }
    }

    return false;
}
std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{

    if (partial.size() == 0)
    {
        return std::vector<std::string>();
    }
    // If all of the characters in given word are not punctuation...

    if (std::all_of(partial.begin(), partial.end(), [](unsigned char c)
                    {
                        return std::isalpha(c);
                    }))
    {
        std::transform(partial.begin(), partial.end(), partial.begin(), [](char c)
                       {
                           return static_cast<char>(std::tolower(c));
                       });
    }
    else
    {
        return std::vector<std::string>();
    }
    std::vector<std::string> prediction;

    std::shared_ptr<TreeNode> root = m_root;
    for (unsigned int i = 0; i < partial.size(); i++)
    {
        // Converstion to find placement in vector.

        unsigned int value = int(partial[i]) - 97;
        if (root->children.at(value) == nullptr)
        {

            return std::vector<std::string>();
        }
        root = root->children.at(value);
    }

    // Queues for TreeNodes and word.
    std::queue<std::shared_ptr<TreeNode>> queue;
    std::queue<std::string> wordQueue;
    std::string tempWord;

    queue.push(root);
    wordQueue.push(partial);
    while (queue.size() != 0)
    {
        root = queue.front();
        tempWord = wordQueue.front();
        wordQueue.pop();
        queue.pop();

        for (unsigned int j = 0; j < root->children.size(); j++)
        {
            if (root->children.at(j) != nullptr)
            {
                // Current 'letter' that we are at.
                char ascii = '0' + (static_cast<char>(j) + static_cast<char>(49));

                if (root->children.at(j)->endOfWord)
                {

                    prediction.push_back(tempWord + ascii);
                    howMany -= 1;
                    if (howMany == 0)
                    {
                        return prediction;
                    }
                }

                wordQueue.push(tempWord + ascii);
                queue.push(root->children.at(j));
            }
        }
    }

    return prediction;
}
std::size_t WordTree::size()
{
    return m_sizeOfTree;
}