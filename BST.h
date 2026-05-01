#ifndef BST_H
#define BST_H

#include <string>
#include <vector>

class BST {
public:
    struct Entry {
        std::string key;
        int count;
    };

    BST();
    ~BST();

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    void clear();
    bool empty() const;

    // Return all keys in-order (sorted).
    std::vector<std::string> inorderTraversal() const;

    // Return key/count entries in-order.
    std::vector<Entry> inorderEntries() const;

private:
    struct Node {
        std::string key;
        int count;
        Node* left;
        Node* right;

        Node(const std::string& k)
            : key(k), count(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void insert(Node*& node, const std::string& word);
    Node* searchNode(Node* node, const std::string& word) const;
    void clear(Node* node);
    void inorderKeys(Node* node, std::vector<std::string>& result) const;
    void inorderEntries(Node* node, std::vector<Entry>& result) const;

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;
};

inline BST::BST()
    : root(nullptr) {}

inline BST::~BST() {
    clear();
}

inline void BST::insert(const std::string& word) {
    insert(root, word);
}

inline bool BST::search(const std::string& word) const {
    return searchNode(root, word) != nullptr;
}

inline void BST::clear() {
    clear(root);
    root = nullptr;
}

inline bool BST::empty() const {
    return root == nullptr;
}

inline std::vector<std::string> BST::inorderTraversal() const {
    std::vector<std::string> result;
    inorderKeys(root, result);
    return result;
}

inline std::vector<BST::Entry> BST::inorderEntries() const {
    std::vector<Entry> result;
    inorderEntries(root, result);
    return result;
}

inline void BST::insert(Node*& node, const std::string& word) {
    if (!node) {
        node = new Node(word);
        return;
    }
    if (word < node->key) {
        insert(node->left, word);
    } else if (word > node->key) {
        insert(node->right, word);
    } else {
        ++node->count;
    }
}

inline BST::Node* BST::searchNode(Node* node, const std::string& word) const {
    if (!node) {
        return nullptr;
    }
    if (word == node->key) {
        return node;
    }
    if (word < node->key) {
        return searchNode(node->left, word);
    }
    return searchNode(node->right, word);
}

inline void BST::clear(Node* node) {
    if (!node) {
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

inline void BST::inorderKeys(Node* node, std::vector<std::string>& result) const {
    if (!node) {
        return;
    }
    inorderKeys(node->left, result);
    result.push_back(node->key);
    inorderKeys(node->right, result);
}

inline void BST::inorderEntries(Node* node, std::vector<Entry>& result) const {
    if (!node) {
        return;
    }
    inorderEntries(node->left, result);
    result.push_back(Entry{node->key, node->count});
    inorderEntries(node->right, result);
}

#endif

