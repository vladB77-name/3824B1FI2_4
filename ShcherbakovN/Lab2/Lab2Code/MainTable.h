// Copyright 2026 Nikita
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "AVLTable.h"
#include "HashTable.h"
#include "UnorderedTable.h"

class LoggingOperations {
  struct InputLogs {
    std::string tableName;
    std::string operationName;
    size_t operationsCount;
  };

  std::vector<InputLogs> logs;

  size_t totalInsert;
  size_t totalFind;
  size_t totalRemove;

  size_t totalUnorderedOp;
  size_t totalAVLOp;
  size_t totalHashOp;

 public:
  LoggingOperations() {
    totalInsert = 0;
    totalFind = 0;
    totalRemove = 0;

    totalUnorderedOp = 0;
    totalAVLOp = 0;
    totalHashOp = 0;
  }

  ~LoggingOperations() {}

  void add(const std::string &tableName_, const std::string &operationName_,
           size_t operationsCount) {
    logs.push_back({tableName_, operationName_, operationsCount});

    if (operationName_ == "insert") {
      totalInsert += operationsCount;
    }
    if (operationName_ == "find") {
      totalFind += operationsCount;
    }
    if (operationName_ == "remove") {
      totalRemove += operationsCount;
    }

    if (tableName_ == "UnorderedTable") {
      totalUnorderedOp += operationsCount;
    }
    if (tableName_ == "AVLTable") {
      totalAVLOp += operationsCount;
    }
    if (tableName_ == "HashTable") {
      totalHashOp += operationsCount;
    }
  }

  void writeToFile(const std::string &fileName) const {
    std::ofstream fout;
    fout.open(fileName);

    if (fout.is_open()) {
      size_t logs_size = logs.size();
      for (size_t i = 0; i < logs_size; ++i) {
        fout << logs[i].tableName << " " << logs[i].operationName << " "
             << logs[i].operationsCount << std::endl;
      }

      fout << std::endl
           << "+------------------+TOTAL ACTIONS+------------------+"
           << std::endl;
      fout << "Insert: " << totalInsert << std::endl;
      fout << "Find: " << totalFind << std::endl;
      fout << "Remove: " << totalRemove << std::endl;

      fout << std::endl
           << "+---------------+TOTAL FOR EACH TABLE+---------------+"
           << std::endl;
      fout << "Unordered: " << totalUnorderedOp << std::endl;
      fout << "AVL: " << totalAVLOp << std::endl;
      fout << "Hash: " << totalHashOp << std::endl;

      fout.close();
    } else {
      std::cerr << "The file did not open!" << std::endl;
    }
  }

  void print() const {
    size_t logs_size = logs.size();
    for (size_t i = 0; i < logs_size; ++i) {
      std::cout << logs[i].tableName << " " << logs[i].operationName << " "
                << logs[i].operationsCount << "\n";
    }
  }

  void clear() {
    logs.clear();

    totalInsert = 0;
    totalFind = 0;
    totalRemove = 0;

    totalUnorderedOp = 0;
    totalAVLOp = 0;
    totalHashOp = 0;
  }
};

template <typename TKey, typename TValue> class MainTable {
  using FindResult = typename BaseTable<TKey, TValue>::FindResult;

  UnorderedTable<TKey, TValue> unorderedTable;
  AVLTable<TKey, TValue> avlTable;
  HashTable<TKey, TValue> hashTable;
  LoggingOperations logger;

 public:
  MainTable() {}

  ~MainTable() {}

  size_t insertALL(const TKey &key, const TValue &value) {
    size_t opCountUnordered = unorderedTable.insert(key, value);
    logger.add(unorderedTable.getTableName(), "insert", opCountUnordered);

    size_t opCountAVL = avlTable.insert(key, value);
    logger.add(avlTable.getTableName(), "insert", opCountAVL);

    size_t opCountHash = hashTable.insert(key, value);
    logger.add(hashTable.getTableName(), "insert", opCountHash);

    return opCountUnordered + opCountAVL + opCountHash;
  }

  FindResult findALL(const TKey &key) {
    FindResult resulttUnordered = unorderedTable.find(key);
    logger.add(unorderedTable.getTableName(), "find",
               resulttUnordered.operationsCount);

    FindResult resultAVL = avlTable.find(key);
    logger.add(avlTable.getTableName(), "find", resultAVL.operationsCount);

    FindResult resultHash = hashTable.find(key);
    logger.add(hashTable.getTableName(), "find", resultHash.operationsCount);

    FindResult result;

    result.operationsCount = resulttUnordered.operationsCount +
                             resultAVL.operationsCount +
                             resultHash.operationsCount;

    if (resulttUnordered.value != nullptr) {
      result.value = resulttUnordered.value;
    } else if (resultAVL.value != nullptr) {
      result.value = resultAVL.value;
    } else if (resultHash.value != nullptr) {
      result.value = resultHash.value;
    } else {
      result.value = nullptr;
    }

    return result;
  }

  size_t removeALL(const TKey &key) {
    size_t opCountUnordered = unorderedTable.remove(key);
    logger.add(unorderedTable.getTableName(), "remove", opCountUnordered);

    size_t opCountAVL = avlTable.remove(key);
    logger.add(avlTable.getTableName(), "remove", opCountAVL);

    size_t opCountHash = hashTable.remove(key);
    logger.add(hashTable.getTableName(), "remove", opCountHash);

    return opCountUnordered + opCountAVL + opCountHash;
  }

  bool is_emptyALL() const {
    return (unorderedTable.size() == 0 && avlTable.size() == 0 &&
            hashTable.size() == 0);
  }

  void saveLogs(const std::string &fileName) { logger.writeToFile(fileName); }
};
