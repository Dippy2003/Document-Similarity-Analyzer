#ifndef STACK_H
#define STACK_H

#include <string>
#include <vector>

class Stack {
public:
    Stack();
    ~Stack();

    void push(const std::string& word);
    bool pop(std::string& word);
    bool top(std::string& word) const;
    bool isEmpty() const;
    std::size_t size() const;
    std::vector<std::string> toVector() const;

private:
    struct Node {
        std::string word;
        Node* next;

        Node(const std::string& w, Node* n = nullptr)
            : word(w), next(n) {}
    };

    Node* topNode;
    std::size_t count;

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
};

inline Stack::Stack()
    : topNode(nullptr), count(0) {}

inline Stack::~Stack() {
    std::string tmp;
    while (pop(tmp)) {
    }
}

inline void Stack::push(const std::string& word) {
    topNode = new Node(word, topNode);
    ++count;
}

inline bool Stack::pop(std::string& word) {
    if (!topNode) {
        return false;
    }
    word = topNode->word;
    Node* oldTop = topNode;
    topNode = topNode->next;
    delete oldTop;
    --count;
    return true;
}

inline bool Stack::top(std::string& word) const {
    if (!topNode) {
        return false;
    }
    word = topNode->word;
    return true;
}

inline bool Stack::isEmpty() const {
    return topNode == nullptr;
}

inline std::size_t Stack::size() const {
    return count;
}

inline std::vector<std::string> Stack::toVector() const {
    std::vector<std::string> words;
    words.reserve(count);
    Node* current = topNode;
    while (current) {
        words.push_back(current->word);
        current = current->next;
    }
    return words;
}

#endif

