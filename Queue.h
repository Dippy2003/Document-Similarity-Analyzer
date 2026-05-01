#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <vector>

class Queue {
public:
    Queue();
    ~Queue();

    void enqueue(const std::string& word);
    bool dequeue(std::string& word);
    bool front(std::string& word) const;
    bool isEmpty() const;
    std::size_t size() const;
    std::vector<std::string> toVector() const;

private:
    struct Node {
        std::string word;
        Node* next;

        Node(const std::string& w)
            : word(w), next(nullptr) {}
    };

    Node* frontNode;
    Node* backNode;
    std::size_t count;

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
};

inline Queue::Queue()
    : frontNode(nullptr), backNode(nullptr), count(0) {}

inline Queue::~Queue() {
    std::string tmp;
    while (dequeue(tmp)) {
    }
}

inline void Queue::enqueue(const std::string& word) {
    Node* node = new Node(word);
    if (backNode) {
        backNode->next = node;
    } else {
        frontNode = node;
    }
    backNode = node;
    ++count;
}

inline bool Queue::dequeue(std::string& word) {
    if (!frontNode) {
        return false;
    }
    word = frontNode->word;
    Node* oldFront = frontNode;
    frontNode = frontNode->next;
    if (!frontNode) {
        backNode = nullptr;
    }
    delete oldFront;
    --count;
    return true;
}

inline bool Queue::front(std::string& word) const {
    if (!frontNode) {
        return false;
    }
    word = frontNode->word;
    return true;
}

inline bool Queue::isEmpty() const {
    return frontNode == nullptr;
}

inline std::size_t Queue::size() const {
    return count;
}

inline std::vector<std::string> Queue::toVector() const {
    std::vector<std::string> words;
    words.reserve(count);
    Node* current = frontNode;
    while (current) {
        words.push_back(current->word);
        current = current->next;
    }
    return words;
}

#endif

