#pragma once

#include <string>
#include <vector>

namespace sirius {

// Problem 3:
// Implement Serialize and Deserialize methods of List class. Serialize into binary
// file. Note: all relationships between elements of the list must be preserved.
// Definitions of struct ListNode and class List are provided:

//struct ListNode {
//  ListNode *prev;
//  ListNode *next;
//  ListNode *rand; // points to a random element of the list or is NULL
//  std::string data;
//};
//
//class List {
//
// public:
//  void Serialize(FILE *file);
//  void Deserialize(FILE *file);
//
// private:
//  ListNode *head;
//  ListNode *tail;
//  int count;
//};

struct ListNode {
  ListNode *prev{nullptr};
  ListNode *next{nullptr};
  ListNode *rand{nullptr}; // points to a random element of the list or is NULL
  std::string data{};
};

class List {
 public:
  ~List();

  void PushBack(std::string &&value);
  ListNode *accessNode(uint64_t index);
  uint32_t Size() { return count; }
  /**
   * @param file must be opened with fopen(path, "wb"))
   */
  void Serialize(FILE *file);
  /**
   * @param file must be opened with fopen(path, "rb"))
   */
  void Deserialize(FILE *file);

 private:
  void cleanup();
  ListNode *head{nullptr};
  ListNode *tail{nullptr};
  uint64_t count{0};
};

}