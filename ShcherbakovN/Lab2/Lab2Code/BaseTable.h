// Copyright 2026 Nikita
#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

const double EPSILON = 1e-9;

template <typename TKey, typename TValue> class BaseTable {
 public:
  struct FindResult {
    TValue *value;
    size_t operationsCount;

    FindResult() : value(nullptr), operationsCount(0) {}
  };

  virtual size_t insert(const TKey &key, const TValue &value) = 0;
  virtual FindResult find(const TKey &key) = 0;
  virtual size_t remove(const TKey &key) = 0;

  virtual std::string getTableName() const = 0;
  virtual void clear() = 0;
  virtual size_t size() const = 0;
};
