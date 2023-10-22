#pragma once
#include <array>
#include <memory>
#include <string>
#include <vector>

class TreeNode
{
  public:
    TreeNode();
    bool endOfWord = false;
    std::array<std::shared_ptr<TreeNode>, 26> children;
};
class WordTree
{
  public:
    WordTree();
    std::shared_ptr<TreeNode> m_root = std::make_shared<TreeNode>();
    void add(std::string word);
    bool find(std::string word);
    std::vector<std::string> predict(std::string partial, std::uint8_t howMany);
    std::size_t size();
    int m_sizeOfTree;
};
