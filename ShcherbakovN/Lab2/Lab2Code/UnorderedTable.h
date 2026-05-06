// Copyright 2026 Nikita
#pragma once

#include <string>
#include <vector>

#include "BaseTable.h"

template <typename TKey, typename TValue>
class UnorderedTable : public BaseTable<TKey, TValue> {
  using FindResult = typename BaseTable<TKey, TValue>::FindResult;

  struct TTableRecord {
    TKey key;
    TValue value;
  };

  std::vector<TTableRecord> records;

 public:
  UnorderedTable() {}

  ~UnorderedTable() {}

  std::string getTableName() const override { return "UnorderedTable"; }

  void clear() override { records.clear(); }

  size_t size() const override { return records.size(); }

  TValue &operator[](const size_t index) { return records[index].value; }

  size_t insert(const TKey &key, const TValue &value) override {
    size_t opCount = 0;

    opCount += 2;
    size_t sizeTable = records.size();

    opCount++;
    for (size_t i = 0; i < sizeTable; ++i) {
      opCount++;

      opCount += 2;
      if (records[i].key == key) {
        opCount += 2;
        records[i].value = value;
        return opCount;
      }

      opCount++;
    }
    opCount++;

    opCount++;
    records.push_back({key, value});
    return opCount;
  }

  FindResult find(const TKey &key) override {
    size_t opCount = 0;

    FindResult result;

    opCount += 2;
    size_t sizeTable = records.size();

    opCount++;
    for (size_t i = 0; i < sizeTable; ++i) {
      opCount++;

      opCount += 2;
      if (records[i].key == key) {
        opCount += 3;
        result.value = &records[i].value;

        opCount++;
        result.operationsCount = opCount;

        return result;
      }

      opCount++;
    }
    opCount++;

    opCount++;
    result.operationsCount = opCount;
    return result;
  }

  size_t remove(const TKey &key) override {
    size_t opCount = 0;

    opCount += 2;
    size_t sizeTable = records.size();

    opCount++;
    for (size_t i = 0; i < sizeTable; ++i) {
      opCount++;

      opCount += 2;
      if (records[i].key == key) {
        opCount += 2;
        records[i] = records[sizeTable - 1];

        opCount++;
        records.pop_back();

        return opCount;
      }

      opCount++;
    }
    opCount++;

    return opCount;
  }
};
