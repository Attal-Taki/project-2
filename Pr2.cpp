#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

// Define a node structure for the planar tree
struct Node {
    std::unordered_map<char, Node*> children;
};

// Function to load words from dictionary file into planar tree
void loadDictionary(const std::string& filename, int wordLength, Node* root) {
    std::ifstream file(filename);
    std::string word;
    while (file >> word) {
        if (word.length() == wordLength) {
            Node* current = root;
            for (int i = 0; i < wordLength - 1; ++i) {
                if (!current->children[word[i]]) {
                    current->children[word[i]] = new Node();
                }
                current = current->children[word[i]];
            }
            current->children[word[wordLength - 1]] = nullptr; // Mark end of word
        }
    }
    file.close();
}

// Function to search for words in planar tree
void searchWords(const std::string& prefix, int wordLength, Node* root, std::vector<std::string>& result) {
    Node* current = root;
    for (char ch : prefix) {
        if (!current->children[ch]) {
            return; // Prefix not found
        }
        current = current->children[ch];
    }
    // Traverse subtree to find words of length wordLength
    std::string word = prefix;
    searchWordsHelper(current, wordLength - 1, word, result);
}

void searchWordsHelper(Node* node, int remainingLength, std::string& word, std::vector<std::string>& result) {
    if (remainingLength == 0) {
        result.push_back(word);
        return;
    }
    for (auto& pair : node->children) {
        word.push_back(pair.first);
        searchWordsHelper(pair.second, remainingLength - 1, word, result);
        word.pop_back();
    }
}

// Function to delete planar tree
void deleteTree(Node* root) {
    if (!root) {
        return;
    }
    for (auto& pair : root->children) {
        deleteTree(pair.second);
    }
    delete root;
}

int main() {
    int wordLength = 3; // Change to desired word length
    Node* root = new Node();

    // Load dictionary into planar tree
    loadDictionary("dictionary.txt", wordLength, root);

    // Example search
    std::string prefix = "PAR";
    std::vector<std::string> words;
    searchWords(prefix, wordLength, root, words);

    // Display search results
    std::cout << "Words with prefix '" << prefix << "':\n";
    for (const std::string& word : words) {
        std::cout << word << "\n";
    }

    // Cleanup
    deleteTree(root);

    return 0;
}
