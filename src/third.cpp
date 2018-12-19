#include "third.hpp"

#include <algorithm>

/**
 * Since task was to specifically implement only Serialize/Deserialize methods
 * here follows minimal testable implementation of Serialize/Deserialize
 * without any considerations of portability, memory layout, performance of List class
 */

namespace sirius {
namespace {

constexpr auto UINT_64_SIZE = sizeof(uint64_t);
constexpr auto CHAR_SIZE = sizeof(char);
constexpr uint64_t RAND_NULLPTR = 0xFFFFFFFFFFFFFFFF;

void writeUint64(uint64_t value, FILE *file) {
  fwrite(&value, UINT_64_SIZE, 1, file);
}

void writeString(const std::string &value, FILE *file) {
  writeUint64(value.size(), file);
  fwrite(value.c_str(), CHAR_SIZE, value.size(), file);
}

uint64_t readUint64(FILE *file) {
  uint64_t data;
  fread(&data, UINT_64_SIZE, 1, file);
  return data;
}

std::string readString(FILE *file) {
  const uint64_t length = readUint64(file);
  std::string ret(length, 0); // pre-allocate string for writing
  fread(&ret[0], CHAR_SIZE, length, file);
  return ret;
}

}

List::~List() {
  cleanup();
}

void List::cleanup() {
  ListNode *nextNode = head;
  while (nextNode) {
    ListNode *currentNode = nextNode;
    nextNode = currentNode->next;
    delete currentNode;
  }
  head = nullptr;
  tail = nullptr;
  count = 0;
}

void List::PushBack(std::string &&value) {
  auto newNode = new ListNode();
  newNode->data = std::move(value);
  newNode->prev = tail;
  if (count > 0) {
    tail->next = newNode;
    tail = newNode;
  } else {
    head = newNode;
    tail = newNode;
  }
  ++count;
}

void List::Serialize(FILE *file) {
  fseek(file, 0, SEEK_SET);
  // file format:
  // [Header]
  // sizeof(uint64_t): size of list
  // [Node#1]
  // sizeof(uint64_t): 1-based index of rand or 0 for nullptr
  // sizeof(uint64_t): size of string (including trailing 0) in bytes
  // sizeof(string): 0-terminated string data
  // [Node#2]
  // ...

  writeUint64(count, file);

  // On first pass prepare pointers to all nodes:
  std::vector<ListNode *> allNodes;
  allNodes.reserve(count);
  ListNode *currentNode = head;
  while (currentNode) {
    allNodes.emplace_back(currentNode);
    currentNode = currentNode->next;
  }

  // On second pass do actual serialization:
  currentNode = head;
  while (currentNode) {
    uint64_t randIndex = RAND_NULLPTR;
    if (currentNode->rand != nullptr) {
      auto it = std::find(allNodes.begin(), allNodes.end(), currentNode->rand);
      if (it != allNodes.end()) {
        randIndex = (it - allNodes.begin());
      }
    }
    writeUint64(randIndex, file);
    writeString(currentNode->data, file);
    currentNode = currentNode->next;
  }
}

void List::Deserialize(FILE *file) {
  cleanup();
  fseek(file, 0, SEEK_SET);
  uint64_t fileSize = readUint64(file);
  std::vector<ListNode *> allNodes;
  allNodes.reserve(fileSize);
  std::vector<uint64_t> randomNodeIndices;
  randomNodeIndices.reserve(fileSize);
  while (count < fileSize) {
    randomNodeIndices.emplace_back(readUint64(file));
    PushBack(readString(file));
    allNodes.emplace_back(tail);
  }
  // Patch rand to actual ListNode*:
  ListNode *currentNode = head;
  for (uint64_t rand : randomNodeIndices) {
    if (rand != RAND_NULLPTR)
      currentNode->rand = allNodes[rand];
    currentNode = currentNode->next;
  }
}

ListNode *List::accessNode(uint64_t index) {
  ListNode *currentNode = head;
  while ((index > 0) && (currentNode != nullptr)) {
    --index;
    currentNode = currentNode->next;
  }
  return currentNode;
}

}
