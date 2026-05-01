#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>
#include <vector>

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void insert(const std::string& word);
    bool remove(const std::string& word);
    bool search(const std::string& word) const;
    std::vector<std::string> toVector() const;
    bool empty() const;

private:
    struct Node {
        std::string word;
        Node* next;

        Node(const std::string& w, Node* n = nullptr)
            : word(w), next(n) {}
    };

    Node* head;

    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;
};

inline LinkedList::LinkedList()
    : head(nullptr) {}

inline LinkedList::~LinkedList() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

inline void LinkedList::insert(const std::string& word) {
    // Insert at head; allow duplicates to store all words.
    head = new Node(word, head);
}

inline bool LinkedList::remove(const std::string& word) {
    Node* current = head;
    Node* prev = nullptr;
    while (current) {
        if (current->word == word) {
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

inline bool LinkedList::search(const std::string& word) const {
    Node* current = head;
    while (current) {
        if (current->word == word) {
            return true;
        }
        current = current->next;
    }
    return false;
}

inline std::vector<std::string> LinkedList::toVector() const {
    std::vector<std::string> words;
    Node* current = head;
    while (current) {
        words.push_back(current->word);
        current = current->next;
    }
    return words;
}

inline bool LinkedList::empty() const {
    return head == nullptr;
}

#endif

