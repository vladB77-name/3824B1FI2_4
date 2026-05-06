// Copyright 2026 Nikita
#pragma once

#include <string>

#include "BaseTable.h"

template <typename TKey, typename TValue>
class AVLTable : public BaseTable<TKey, TValue> {
  using FindResult = typename BaseTable<TKey, TValue>::FindResult;

  struct Node {
    TKey key;
    TValue value;

    Node *left;
    Node *right;

    int height;

    Node(const TKey &key_, const TValue &value_)
        : key(key_), value(value_), left(nullptr), right(nullptr), height(1) {}
  };

  Node *root;
  size_t NodeCount;

  int getHeight(Node *node_, size_t &opCount) const {
    opCount += 2;
    return (node_ != nullptr) ? (node_->height) : 0;
  }

  int getBalanceIndicator(Node *node_, size_t &opCount) const {
    opCount++;
    if (node_ == nullptr) {
      return 0;
    }

    opCount++;
    return (getHeight(node_->right, opCount) - getHeight(node_->left, opCount));
  }

  void updatedHeight(Node *node_, size_t &opCount) {
    opCount++;
    if (node_ != nullptr) {
      opCount++;
      int max = 0;

      opCount++;
      if (getHeight(node_->left, opCount) >= getHeight(node_->right, opCount)) {
        opCount++;
        max = getHeight(node_->left, opCount);
      } else {
        opCount++;
        max = getHeight(node_->right, opCount);
      }

      opCount += 2;
      node_->height = 1 + max;
    }
  }

  Node *smallLeftRotation(Node *node_, size_t &opCount) {
    opCount += 3;
    if (node_ == nullptr || node_->right == nullptr) {
      return node_;
    }

    opCount += 2;
    Node *rightNode = node_->right;
    Node *leftSubTreeOfRightNode = rightNode->left;

    opCount += 2;
    rightNode->left = node_;
    node_->right = leftSubTreeOfRightNode;

    updatedHeight(node_, opCount);
    updatedHeight(rightNode, opCount);

    return rightNode;
  }

  Node *smallRightRotation(Node *node_, size_t &opCount) {
    opCount += 3;
    if (node_ == nullptr || node_->left == nullptr) {
      return node_;
    }

    opCount += 2;
    Node *leftNode = node_->left;
    Node *rightSubTreeOfLeftNode = leftNode->right;

    opCount += 2;
    leftNode->right = node_;
    node_->left = rightSubTreeOfLeftNode;

    updatedHeight(node_, opCount);
    updatedHeight(leftNode, opCount);

    return leftNode;
  }

  Node *bigLeftRotation(Node *node_, size_t &opCount) {
    opCount++;
    node_->right = smallRightRotation(node_->right, opCount);
    return smallLeftRotation(node_, opCount);
  }

  Node *bigRightRotation(Node *node_, size_t &opCount) {
    opCount++;
    node_->left = smallLeftRotation(node_->left, opCount);
    return smallRightRotation(node_, opCount);
  }

  Node *balancingTree(Node *node_, size_t &opCount) {
    opCount++;
    if (node_ == nullptr) {
      return nullptr;
    }

    updatedHeight(node_, opCount);

    opCount += 3;
    int balanceIndicatorNode = getBalanceIndicator(node_, opCount);
    int balanceIndicatorLeftNode = getBalanceIndicator(node_->left, opCount);
    int balanceIndicatorRightNode = getBalanceIndicator(node_->right, opCount);

    opCount++;
    if (balanceIndicatorNode >= 2) {
      opCount++;
      if (balanceIndicatorRightNode >= 0) {
        return smallLeftRotation(node_, opCount);
      } else if (balanceIndicatorRightNode <= -1) {
        opCount++;
        return bigLeftRotation(node_, opCount);
      }
    }

    opCount++;
    if (balanceIndicatorNode <= -2) {
      opCount++;
      if (balanceIndicatorLeftNode <= 0) {
        return smallRightRotation(node_, opCount);
      } else if (balanceIndicatorLeftNode >= 1) {
        opCount++;
        return bigRightRotation(node_, opCount);
      }
    }

    return node_;
  }

  Node *insertNode(Node *node_, const TKey &key, const TValue &value,
                   size_t &opCount) {
    opCount++;
    if (node_ == nullptr) {
      opCount += 2;
      NodeCount++;
      return new Node(key, value);
    }

    opCount++;
    if (key < node_->key) {
      opCount++;
      node_->left = insertNode(node_->left, key, value, opCount);
    } else if (key > node_->key) {
      opCount++;
      node_->right = insertNode(node_->right, key, value, opCount);
    } else {
      opCount++;

      opCount++;
      node_->value = value;
      return node_;
    }
    opCount++;

    return balancingTree(node_, opCount);
  }

  Node *findNode(Node *node_, const TKey &key, size_t &opCount) {
    opCount++;
    if (node_ == nullptr) {
      return nullptr;
    }

    opCount++;
    if (node_->key == key) {
      return node_;
    }

    opCount++;
    if (key < node_->key) {
      return findNode(node_->left, key, opCount);
    } else {
      return findNode(node_->right, key, opCount);
    }
  }

  Node *maxNode(Node *node_, size_t &opCount) {
    opCount += 3;
    while (node_ != nullptr && node_->right != nullptr) {
      opCount++;
      node_ = node_->right;

      opCount += 3;
    }
    return node_;
  }

  Node *removeNode(Node *node_, const TKey &key, size_t &opCount) {
    opCount++;
    if (node_ == nullptr) {
      return nullptr;
    }

    opCount++;
    if (key < node_->key) {
      opCount++;
      node_->left = removeNode(node_->left, key, opCount);
    } else if (key > node_->key) {
      opCount++;
      node_->right = removeNode(node_->right, key, opCount);
    } else {
      opCount++;

      opCount += 3;
      if (node_->left == nullptr && node_->right == nullptr) {
        opCount++;
        delete node_;

        opCount++;
        NodeCount--;

        return nullptr;
      } else if ((node_->left != nullptr && node_->right == nullptr) ||
                 (node_->left == nullptr && node_->right != nullptr)) {
        opCount += 7;

        opCount += 3;
        Node *childNode = (node_->left == nullptr) ? node_->right : node_->left;

        opCount++;
        delete node_;

        opCount++;
        NodeCount--;

        return childNode;
      } else if (node_->left != nullptr && node_->right != nullptr) {
        opCount += 7;
        opCount += 3;

        opCount++;
        Node *maxNodeLeftSubTree = maxNode(node_->left, opCount);

        opCount++;
        node_->key = maxNodeLeftSubTree->key;
        opCount++;
        node_->value = maxNodeLeftSubTree->value;

        opCount++;
        node_->left = removeNode(node_->left, maxNodeLeftSubTree->key, opCount);
      }
      opCount += 7;
      opCount += 3;
    }
    opCount++;

    return balancingTree(node_, opCount);
  }

  void clearTree(Node *node_) {
    if (node_ == nullptr) {
      return;
    }

    clearTree(node_->left);
    clearTree(node_->right);

    delete node_;
  }

 public:
  AVLTable() : root(nullptr), NodeCount(0) {}

  void clear() override {
    clearTree(root);
    root = nullptr;
    NodeCount = 0;
  }

  ~AVLTable() { clear(); }

  std::string getTableName() const override { return "AVLTable"; }

  size_t size() const override { return NodeCount; }

  size_t insert(const TKey &key, const TValue &value) override {
    size_t opCount = 0;

    root = insertNode(root, key, value, opCount);
    return opCount;
  }

  FindResult find(const TKey &key) override {
    size_t opCount = 0;
    FindResult result;

    opCount++;
    Node *current_node = findNode(root, key, opCount);

    opCount++;
    if (current_node != nullptr) {
      opCount += 2;
      result.value = &(current_node->value);

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

    root = removeNode(root, key, opCount);
    return opCount;
  }
};
