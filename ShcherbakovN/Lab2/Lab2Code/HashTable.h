// Copyright 2026 Nikita
#pragma once

#include <string>
#include <vector>

#include "BaseTable.h"

template <typename TKey, typename TValue>
class HashTable : public BaseTable<TKey, TValue> {
  using FindResult = typename BaseTable<TKey, TValue>::FindResult;

  struct NodeHash {
    TKey key;
    TValue value;
    NodeHash *next;

    NodeHash(const TKey &key_, const TValue &value_)
        : key(key_), value(value_), next(nullptr) {}
  };

  std::vector<NodeHash *> table_lists;
  size_t sizeTable;
  size_t elementsCount;
  double loadFactor;

  size_t hashFunction(const TKey &key_, size_t sizeTable_,
                      size_t &opCount) const {
    opCount++;
    size_t hashValue = 0;

    opCount++;
    size_t sizeStr = key_.length();

    opCount++;
    for (size_t i = 0; i < sizeStr; ++i) {
      opCount++;

      opCount += 4;
      hashValue = hashValue * 31 + (unsigned char)key_[i];

      opCount++;
    }
    opCount++;

    opCount++;
    return hashValue % sizeTable_;
  }

  NodeHash *findNode(const TKey &key, size_t &opCount) const {
    opCount++;
    size_t index = hashFunction(key, sizeTable, opCount);

    opCount += 2;
    NodeHash *currentNode = table_lists[index];

    opCount++;
    while (currentNode != nullptr) {
      opCount++;
      if (currentNode->key == key) {
        return currentNode;
      }

      opCount++;
      currentNode = currentNode->next;

      opCount++;
    }

    return nullptr;
  }

  void rehash(size_t &opCount) {
    opCount += 2;
    size_t newSizeTable = sizeTable * 2;

    std::vector<NodeHash *> new_table_lists(newSizeTable, nullptr);

    opCount++;
    for (size_t i = 0; i < sizeTable; ++i) {
      opCount++;

      opCount += 2;
      NodeHash *currentNode = table_lists[i];

      opCount++;
      while (currentNode != nullptr) {
        opCount++;
        NodeHash *currentNextNode = currentNode->next;

        opCount++;
        size_t index = hashFunction(currentNode->key, newSizeTable, opCount);

        opCount += 4;
        currentNode->next = new_table_lists[index];
        new_table_lists[index] = currentNode;

        opCount++;
        currentNode = currentNextNode;

        opCount++;
      }

      opCount++;
    }
    opCount++;

    opCount += 2;
    table_lists = new_table_lists;
    sizeTable = newSizeTable;
  }

 public:
  explicit HashTable(size_t sizeTable_ = 10, double loadFactor_ = 0.75) {
    if (loadFactor_ < 0 || std::fabs(loadFactor_) < EPSILON) {
      loadFactor_ = 0.75;
    }
    loadFactor = loadFactor_;

    if (sizeTable_ == 0) {
      sizeTable_ = 10;
    }
    sizeTable = sizeTable_;
    table_lists.resize(sizeTable, nullptr);

    elementsCount = 0;
  }

  void clear() override {
    for (size_t i = 0; i < sizeTable; ++i) {
      NodeHash *currentNode = table_lists[i];

      while (currentNode != nullptr) {
        NodeHash *currentNextNode = currentNode->next;
        delete currentNode;
        currentNode = currentNextNode;
      }

      table_lists[i] = nullptr;
    }

    elementsCount = 0;
  }

  ~HashTable() { clear(); }

  std::string getTableName() const override { return "HashTable"; }

  size_t size() const override { return elementsCount; }

  size_t size_capacity() const { return sizeTable; }

  size_t insert(const TKey &key, const TValue &value) override {
    size_t opCount = 0;

    opCount++;
    size_t index = hashFunction(key, sizeTable, opCount);
    opCount += 2;
    NodeHash *currentNode = table_lists[index];

    opCount++;
    while (currentNode != nullptr) {
      opCount++;
      if (currentNode->key == key) {
        opCount++;
        currentNode->value = value;
        return opCount;
      }

      opCount++;
      currentNode = currentNode->next;

      opCount++;
    }

    opCount += 2;
    NodeHash *resultNode = new NodeHash(key, value);

    opCount += 4;
    resultNode->next = table_lists[index];
    table_lists[index] = resultNode;

    opCount++;
    elementsCount++;

    double count = static_cast<double>(elementsCount);
    double sizeT = static_cast<double>(sizeTable);

    opCount += 2;
    if (count > loadFactor * sizeT) {
      rehash(opCount);
    }

    return opCount;
  }

  FindResult find(const TKey &key) override {
    size_t opCount = 0;

    FindResult result;

    opCount++;
    NodeHash *resultNode = findNode(key, opCount);

    opCount++;
    if (resultNode != nullptr) {
      opCount += 2;
      result.value = &(resultNode->value);

      opCount++;
      result.operationsCount = opCount;

      return result;
    }

    opCount++;
    result.operationsCount = opCount;

    return result;
  }

  size_t remove(const TKey &key) override {
    size_t opCount = 0;

    opCount++;
    size_t index = hashFunction(key, sizeTable, opCount);

    opCount += 3;
    NodeHash *currentNode = table_lists[index];
    NodeHash *previousNode = nullptr;

    opCount++;
    while (currentNode != nullptr) {
      opCount++;
      if (currentNode->key == key) {
        opCount++;
        if (previousNode == nullptr) {
          opCount += 2;
          table_lists[index] = currentNode->next;
        } else {
          opCount++;
          previousNode->next = currentNode->next;
        }

        opCount++;
        delete currentNode;
        opCount++;
        elementsCount--;

        return opCount;
      }

      opCount += 2;
      previousNode = currentNode;
      currentNode = currentNode->next;

      opCount++;
    }

    return opCount;
  }
};
