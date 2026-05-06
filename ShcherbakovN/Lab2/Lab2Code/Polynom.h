// Copyright 2026 Nikita
#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "BaseTable.h"

struct Monom {
  double coeff;
  size_t deg;

  explicit Monom(double coefficient = 0.0, size_t degree = 0)
      : coeff{coefficient}, deg{degree} {}
};

struct Node {
  Monom data;
  Node *next;

  explicit Node(const Monom &data_, Node *next_ = nullptr)
      : data{data_}, next{next_} {}
};

class Forward_list_with_f_head {
  size_t size;
  Node *head;

  Node *find_middle_elem(Node *node_);

  Node *merge(Node *left, Node *right);

  Node *merge_sort(Node *node_);

  void push_front(double coeff_, size_t deg_);

  friend class Polynomial;
 public:
  Forward_list_with_f_head();

  Forward_list_with_f_head(const Forward_list_with_f_head &other_list);

  void clear();

  ~Forward_list_with_f_head();

  Forward_list_with_f_head &
  operator=(const Forward_list_with_f_head &other_list);

  bool is_empty() const;

  void sort();

  void push_quick(double coeff_, size_t deg_);

  std::vector<std::pair<double, size_t>> get_list() const;
};

class Polynomial {
  Forward_list_with_f_head polynom;

 public:
  Polynomial();

  explicit Polynomial(const std::vector<std::pair<double, size_t>> &source);

  Polynomial(const Polynomial &other_polynom);

  ~Polynomial();

  Polynomial &operator=(const Polynomial &other_polynom);

  Polynomial operator+(const Polynomial &other_polynom) const;

  Polynomial operator*(const double &val) const;

  Polynomial operator-(const Polynomial &other_polynom) const;

  Polynomial operator*(const Polynomial &other_polynom) const;

  void print_polynom() const;

  bool is_empty();

  std::vector<std::pair<double, size_t>> get_polynom() const;
};

// Realization Forward_list_with_f_head:

Node *Forward_list_with_f_head::find_middle_elem(Node *node_) {
  if (node_ == nullptr || node_->next == nullptr) {
    return nullptr;
  }

  Node *slow = node_;
  Node *fast = node_->next;

  while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;
  }

  Node *middle = slow->next;
  slow->next = nullptr;

  return middle;
}

Node *Forward_list_with_f_head::merge(Node *left, Node *right) {
  if (!left) {
    return right;
  }
  if (!right) {
    return left;
  }

  if (left->data.deg > right->data.deg) {
    left->next = merge(left->next, right);

    return left;
  } else if (left->data.deg < right->data.deg) {
    right->next = merge(left, right->next);
    return right;
  } else {
    left->data.coeff = left->data.coeff + right->data.coeff;
    Node *next = merge(left->next, right->next);
    delete right;

    if (std::fabs(left->data.coeff) < EPSILON) {
      delete left;
      return next;
    } else {
      left->next = next;
      return left;
    }
  }
}

Node *Forward_list_with_f_head::merge_sort(Node *node_) {
  if (node_ == nullptr || node_->next == nullptr) {
    return node_;
  }

  Node *middle = find_middle_elem(node_);

  Node *left = merge_sort(node_);
  Node *right = merge_sort(middle);

  return merge(left, right);
}

Forward_list_with_f_head::Forward_list_with_f_head() {
  size = 0;
  head = new Node(Monom());
}

Forward_list_with_f_head::Forward_list_with_f_head(
    const Forward_list_with_f_head &other_list) {
  size = other_list.size;
  head = new Node(Monom());

  Node *current_other = other_list.head->next;
  Node *current_this = head;

  while (current_other) {
    current_this->next = new Node(current_other->data);

    current_this = current_this->next;
    current_other = current_other->next;
  }
}

void Forward_list_with_f_head::clear() {
  while (head->next) {
    Node *temp = head->next;
    head->next = head->next->next;
    delete temp;
  }
  size = 0;
}

Forward_list_with_f_head::~Forward_list_with_f_head() {
  clear();
  delete head;
}

Forward_list_with_f_head &Forward_list_with_f_head::operator=(
    const Forward_list_with_f_head &other_list) {
  if (this != &other_list) {
    Forward_list_with_f_head temp(other_list);

    std::swap(size, temp.size);
    std::swap(head, temp.head);
  }

  return *this;
}

bool Forward_list_with_f_head::is_empty() const { return size == 0; }

void Forward_list_with_f_head::sort() {
  head->next = merge_sort(head->next);

  size = 0;
  for (Node *current = head->next; current != nullptr;
       current = current->next) {
    ++size;
  }
}

void Forward_list_with_f_head::push_front(double coeff_, size_t deg_) {
  if (deg_ > 999) {
    throw std::range_error("The degrees of the variables are greater than 9. "
                           "The maximum degree of the polynomial is 999.\n");
  }

  if (std::fabs(coeff_) < EPSILON) {
    throw std::invalid_argument(
        "The coefficient of the monomial must be different from 0.\n");
  }

  Node *new_node = new Node(Monom(coeff_, deg_), head->next);
  head->next = new_node;
  ++size;
}

void Forward_list_with_f_head::push_quick(double coeff_, size_t deg_) {
  if (deg_ > 999) {
    throw std::range_error("The degrees of the variables are greater than 9. "
                           "The maximum degree of the polynomial is 999.\n");
  }

  if (std::fabs(coeff_) < EPSILON) {
    throw std::invalid_argument(
        "The coefficient of the monomial must be different from 0.\n");
  }

  Node *prev = head;
  Node *current = head->next;

  while (current != nullptr && current->data.deg > deg_) {
    prev = current;
    current = current->next;
  }

  if (current != nullptr && current->data.deg == deg_) {
    current->data.coeff = current->data.coeff + coeff_;

    if (std::fabs(current->data.coeff) < EPSILON) {
      prev->next = current->next;
      delete current;
      --size;
    }

    return;
  }

  Node *new_node = new Node(Monom(coeff_, deg_), current);
  prev->next = new_node;
  ++size;
}

std::vector<std::pair<double, size_t>>
Forward_list_with_f_head::get_list() const {
  std::vector<std::pair<double, size_t>> res_list;

  Node *current = head->next;

  while (current != nullptr) {
    res_list.push_back({current->data.coeff, current->data.deg});
    current = current->next;
  }

  return res_list;
}

// Realization Polynomial:

Polynomial::Polynomial() {}

Polynomial::Polynomial(const std::vector<std::pair<double, size_t>> &source) {
  size_t size_source = source.size();

  for (size_t i = 0; i < size_source; ++i) {
    if (std::fabs(source[i].first) < EPSILON) {
      continue;
    }

    polynom.push_front(source[i].first, source[i].second);
  }

  polynom.sort();
}

Polynomial::Polynomial(const Polynomial &other_polynom)
    : polynom{other_polynom.polynom} {}

Polynomial::~Polynomial() {}

Polynomial &Polynomial::operator=(const Polynomial &other_polynom) {
  if (this != &other_polynom) {
    this->polynom = other_polynom.polynom;
  }

  return *this;
}

Polynomial Polynomial::operator+(const Polynomial &other_polynom) const {
  Polynomial res_polynom;

  Node *this_current = this->polynom.head->next;
  Node *other_current = other_polynom.polynom.head->next;

  while (this_current != nullptr && other_current != nullptr) {
    if (this_current->data.deg > other_current->data.deg) {
      res_polynom.polynom.push_front(this_current->data.coeff,
                                     this_current->data.deg);
      this_current = this_current->next;
    } else if (this_current->data.deg < other_current->data.deg) {
      res_polynom.polynom.push_front(other_current->data.coeff,
                                     other_current->data.deg);
      other_current = other_current->next;
    } else {
      double sum_coeff = this_current->data.coeff + other_current->data.coeff;

      if (!(std::fabs(sum_coeff) < EPSILON)) {
        res_polynom.polynom.push_front(sum_coeff, this_current->data.deg);
      }

      this_current = this_current->next;
      other_current = other_current->next;
    }
  }

  while (this_current != nullptr) {
    res_polynom.polynom.push_front(this_current->data.coeff,
                                   this_current->data.deg);
    this_current = this_current->next;
  }

  while (other_current != nullptr) {
    res_polynom.polynom.push_front(other_current->data.coeff,
                                   other_current->data.deg);
    other_current = other_current->next;
  }

  res_polynom.polynom.sort();

  return res_polynom;
}

Polynomial Polynomial::operator*(const double &val) const {
  Polynomial res_polynom;

  Node *this_current = this->polynom.head->next;

  while (this_current != nullptr) {
    if (!(std::fabs(this_current->data.coeff * val) < EPSILON)) {
      res_polynom.polynom.push_front(this_current->data.coeff * val,
                                     this_current->data.deg);
    }

    this_current = this_current->next;
  }

  res_polynom.polynom.sort();

  return res_polynom;
}

Polynomial Polynomial::operator-(const Polynomial &other_polynom) const {
  Polynomial res_polynom;

  res_polynom = *this + (other_polynom * (-1));

  return res_polynom;
}

Polynomial Polynomial::operator*(const Polynomial &other_polynom) const {
  Polynomial res_polynom;

  Node *this_current = this->polynom.head->next;
  Node *other_current = other_polynom.polynom.head->next;
  Node *temp_current = other_current;

  size_t this_units = 0, this_tens = 0, this_hundreds = 0;
  size_t other_units = 0, other_tens = 0, other_hundreds = 0;
  size_t res_units = 0, res_tens = 0, res_hundreds = 0;
  size_t res_deg = 0;

  while (this_current != nullptr) {
    while (other_current != nullptr) {
      double multi_coeff = this_current->data.coeff * other_current->data.coeff;

      if (!(std::fabs(multi_coeff) < EPSILON)) {
        this_units = this_current->data.deg % 10;
        this_tens = (this_current->data.deg / 10) % 10;
        this_hundreds = this_current->data.deg / 100;

        other_units = other_current->data.deg % 10;
        other_tens = (other_current->data.deg / 10) % 10;
        other_hundreds = other_current->data.deg / 100;

        res_units = this_units + other_units;
        res_tens = this_tens + other_tens;
        res_hundreds = this_hundreds + other_hundreds;

        if (res_units >= 10 || res_tens >= 10 || res_hundreds >= 10) {
          throw std::range_error(
              "The degrees of the obtained variables are greater than 9. The "
              "maximum degree of a polynomial is 999.\n");
        }

        res_deg = res_units + res_tens * 10 + res_hundreds * 100;

        res_polynom.polynom.push_front(multi_coeff, res_deg);
      }

      other_current = other_current->next;
    }

    this_current = this_current->next;

    other_current = temp_current;
  }

  res_polynom.polynom.sort();

  return res_polynom;
}

void Polynomial::print_polynom() const {
  if (polynom.size != 0) {
    Node *this_current = this->polynom.head->next;

    if (this_current->data.coeff < 0) {
      std::cout << "-";
    }

    while (this_current != nullptr) {
      std::cout << std::fabs(this_current->data.coeff);

      if ((this_current->data.deg / 100) != 0) {
        std::cout << "x^" << (this_current->data.deg / 100);
      }

      if (((this_current->data.deg / 10) % 10) != 0) {
        std::cout << "y^" << ((this_current->data.deg / 10) % 10);
      }

      if ((this_current->data.deg % 10) != 0) {
        std::cout << "z^" << (this_current->data.deg % 10);
      }

      if (this_current->next != nullptr) {
        if (this_current->next->data.coeff > 0) {
          std::cout << " +";
        } else {
          std::cout << " -";
        }
      }

      this_current = this_current->next;
    }

    std::cout << std::endl;
  } else {
    std::cout << "The polynomial is empty.\n";
  }
}

bool Polynomial::is_empty() { return this->polynom.is_empty(); }

std::vector<std::pair<double, size_t>> Polynomial::get_polynom() const {
  std::vector<std::pair<double, size_t>> res_polynom;

  Node *current = polynom.head->next;

  while (current != nullptr) {
    res_polynom.push_back({current->data.coeff, current->data.deg});
    current = current->next;
  }

  return res_polynom;
}
