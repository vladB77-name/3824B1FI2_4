// Copyright 2026 Nikita

#include <chrono>
#include <string>
#include <vector>

#include "../Lab2/pch.h"
#include "../Lab2Code/MainTable.h"
#include "../Lab2Code/Polynom.h"

// Tests Forward_list_with_f_head:

TEST(Test_Forward_list_with_f_head, CreateDefaultEmptyListIsCorrectNoThrow) {
  EXPECT_NO_THROW({
    Forward_list_with_f_head list;
    EXPECT_TRUE(list.is_empty());
  });
}

TEST(Test_Forward_list_with_f_head, PushQuickIsCorrectNoThrow) {
  Forward_list_with_f_head list;
  std::vector<std::pair<double, size_t>> data;

  EXPECT_NO_THROW({
    for (size_t i = 1; i < 999; ++i) {
      list.push_quick(10.0, i);
      data = list.get_list();
      EXPECT_EQ(data[0].second, i);
      EXPECT_DOUBLE_EQ(data[0].first, 10.0);
    }
    EXPECT_FALSE(list.is_empty());

    list.push_quick(76.89, 0);
    data = list.get_list();
    EXPECT_EQ(data[998].second, 0);
    EXPECT_DOUBLE_EQ(data[998].first, 76.89);
    EXPECT_FALSE(list.is_empty());

    list.push_quick(-45.8, 999);
    data = list.get_list();
    EXPECT_EQ(data[0].second, 999);
    EXPECT_DOUBLE_EQ(data[0].first, -45.8);
    EXPECT_FALSE(list.is_empty());

    EXPECT_EQ(data.size(), 1000);
  });
}

TEST(Test_Forward_list_with_f_head, CreateListByCopyingIsCorrectNoThrow) {
  std::vector<std::pair<double, size_t>> data1, data2;
  EXPECT_NO_THROW({
    Forward_list_with_f_head list1;
    EXPECT_TRUE(list1.is_empty());
    list1.push_quick(34.0, 111);
    list1.push_quick(-8.0, 654);
    EXPECT_FALSE(list1.is_empty());

    Forward_list_with_f_head list2(list1);
    EXPECT_FALSE(list2.is_empty());

    data1 = list1.get_list();
    data2 = list2.get_list();
    EXPECT_EQ(data1[0].second, data2[0].second);
    EXPECT_EQ(data1[1].second, data2[1].second);
    EXPECT_EQ(data1[0].first, data2[0].first);
    EXPECT_EQ(data1[1].first, data2[1].first);

    list2.clear();
    EXPECT_TRUE(list2.is_empty());
  });
}

TEST(Test_Forward_list_with_f_head, PushFrontAnyThrow) {
  Forward_list_with_f_head list;

  EXPECT_THROW(list.push_quick(0.0, 333), std::invalid_argument);

  EXPECT_THROW(list.push_quick(76.89, -1), std::range_error);

  EXPECT_THROW(list.push_quick(-45.8, 1000), std::range_error);
}

TEST(Test_Forward_list_with_f_head, AssignmentIsCorrectNoThrow) {
  Forward_list_with_f_head list1, list2;
  std::vector<std::pair<double, size_t>> data1, data2;

  EXPECT_NO_THROW({
    list1.push_quick(23.1, 123);
    EXPECT_FALSE(list1.is_empty());
    EXPECT_TRUE(list2.is_empty());

    list2 = list1;
    EXPECT_FALSE(list1.is_empty());
    EXPECT_FALSE(list2.is_empty());

    data1 = list1.get_list();
    data2 = list2.get_list();
    EXPECT_EQ(data1[0].second, data2[0].second);
    EXPECT_EQ(data1[0].first, data2[0].first);
  });
}

// Tests Polynomial:

TEST(Test_Polynomial, CreateDefaultEmptyPolynomIsCorrectNoThrow) {
  EXPECT_NO_THROW({
    Polynomial p;
    EXPECT_TRUE(p.is_empty());
  });
}

TEST(Test_Polynomial, CreateNotEmptyPolynomIsCorrectNoThrow) {
  std::vector<std::pair<double, size_t>> source1 = {
      {1.0, 111}, {0.0, 245}, {15.0, 645}};
  std::vector<std::pair<double, size_t>> data1, data2;
  EXPECT_NO_THROW({
    Polynomial p1(source1);
    EXPECT_FALSE(p1.is_empty());
    data1 = p1.get_polynom();
    EXPECT_EQ(data1[0].second, 645);
    EXPECT_EQ(data1[1].second, 111);
    EXPECT_EQ(data1[0].first, 15.0);
    EXPECT_EQ(data1[1].first, 1.0);
    // p1.print_polynom();

    Polynomial p2(p1);
    EXPECT_FALSE(p2.is_empty());
    data2 = p2.get_polynom();
    EXPECT_EQ(data2[0].second, data1[0].second);
    EXPECT_EQ(data2[1].second, data1[1].second);
    EXPECT_EQ(data2[0].first, data1[0].first);
    EXPECT_EQ(data2[1].first, data1[1].first);
    // p2.print_polynom();
  });
}

TEST(Test_Polynomial, CreateNotEmptyPolynomIsCorrectAnyThrow) {
  std::vector<std::pair<double, size_t>> source1 = {{1.0, -1}};
  EXPECT_THROW(Polynomial p1(source1), std::range_error);

  std::vector<std::pair<double, size_t>> source2 = {{1.0, 1000}};
  EXPECT_THROW(Polynomial p2(source2), std::range_error);
}

TEST(Test_Polynomial, AssignmentIsCorrectNoThrow) {
  std::vector<std::pair<double, size_t>> source1 = {
      {1.0, 100}, {2.0, 10}, {3.0, 1}};
  std::vector<std::pair<double, size_t>> data1, data2;
  EXPECT_NO_THROW({
    Polynomial p1(source1);
    EXPECT_FALSE(p1.is_empty());
    data1 = p1.get_polynom();
    // p1.print_polynom();

    Polynomial p2;
    p2 = p1;
    EXPECT_FALSE(p2.is_empty());
    data2 = p2.get_polynom();
    EXPECT_EQ(data2[0].second, data1[0].second);
    EXPECT_EQ(data2[1].second, data1[1].second);
    EXPECT_EQ(data2[2].second, data1[2].second);
    EXPECT_EQ(data2[0].first, data1[0].first);
    EXPECT_EQ(data2[1].first, data1[1].first);
    EXPECT_EQ(data2[2].first, data1[2].first);
    // p2.print_polynom();
  });
}

TEST(Test_Polynomial, PolynomAdditionAndSubtractionIsCorrectNoThrow) {
  std::vector<std::pair<double, size_t>> source1 = {
      {2.0, 200}, {3.0, 110}, {1.0, 1}};

  std::vector<std::pair<double, size_t>> source2 = {
      {1.0, 200}, {-3.0, 110}, {4.0, 0}};

  Polynomial p1(source1);
  // p1.print_polynom();
  Polynomial p2(source2);
  // p2.print_polynom();

  std::vector<std::pair<double, size_t>> data1, data2;
  data1 = p1.get_polynom();
  data2 = p2.get_polynom();

  EXPECT_NO_THROW({
    Polynomial add_p_1 = p1 + p2;
    // add_p_1.print_polynom();
    Polynomial sub_p_1 = p1 - p2;
    // sub_p_1.print_polynom();

    EXPECT_FALSE(add_p_1.is_empty());
    EXPECT_FALSE(sub_p_1.is_empty());

    data1 = add_p_1.get_polynom();
    EXPECT_EQ(data1[0].first, 3.0);
    EXPECT_EQ(data1[1].first, 1.0);
    EXPECT_EQ(data1[2].first, 4.0);
    EXPECT_EQ(data1[0].second, 200);
    EXPECT_EQ(data1[1].second, 1);
    EXPECT_EQ(data1[2].second, 0);

    data2 = sub_p_1.get_polynom();
    EXPECT_EQ(data2[0].first, 1.0);
    EXPECT_EQ(data2[1].first, 6.0);
    EXPECT_EQ(data2[2].first, 1.0);
    EXPECT_EQ(data2[3].first, -4.0);
    EXPECT_EQ(data2[0].second, 200);
    EXPECT_EQ(data2[1].second, 110);
    EXPECT_EQ(data2[2].second, 1);
    EXPECT_EQ(data2[3].second, 0);

    Polynomial add_p_2 = p2 + p1;
    // add_p_2.print_polynom();
    Polynomial sub_p_2 = p2 - p1;
    // sub_p_2.print_polynom();

    EXPECT_FALSE(add_p_2.is_empty());
    EXPECT_FALSE(sub_p_2.is_empty());

    data1 = add_p_2.get_polynom();
    EXPECT_EQ(data1[0].first, 3.0);
    EXPECT_EQ(data1[1].first, 1.0);
    EXPECT_EQ(data1[2].first, 4.0);
    EXPECT_EQ(data1[0].second, 200);
    EXPECT_EQ(data1[1].second, 1);
    EXPECT_EQ(data1[2].second, 0);

    data2 = sub_p_2.get_polynom();
    EXPECT_EQ(data2[0].first, -1.0);
    EXPECT_EQ(data2[1].first, -6.0);
    EXPECT_EQ(data2[2].first, -1.0);
    EXPECT_EQ(data2[3].first, 4.0);
    EXPECT_EQ(data2[0].second, 200);
    EXPECT_EQ(data2[1].second, 110);
    EXPECT_EQ(data2[2].second, 1);
    EXPECT_EQ(data2[3].second, 0);
  });
}

TEST(Test_Polynomial, PolynomMultiplicationConstantIsCorrectNoThrow) {
  std::vector<std::pair<double, size_t>> source = {{2.0, 200}, {-3.0, 10}};

  Polynomial p(source);
  // p.print_polynom();
  double const_p = 2.0;

  std::vector<std::pair<double, size_t>> data1, data2;
  data1 = p.get_polynom();

  EXPECT_NO_THROW({
    Polynomial res_p = p * const_p;
    // res_p.print_polynom();
    EXPECT_FALSE(res_p.is_empty());

    data2 = res_p.get_polynom();
    EXPECT_EQ(data2[0].first, data1[0].first * const_p);
    EXPECT_EQ(data2[1].first, data1[1].first * const_p);
    EXPECT_EQ(data2[0].second, data1[0].second);
    EXPECT_EQ(data2[1].second, data1[1].second);
  });
}

TEST(Test_Polynomial, PolynomMultiplicationZeroNoThrow) {
  std::vector<std::pair<double, size_t>> source = {{2.0, 200}};

  Polynomial p(source);
  // p.print_polynom();

  EXPECT_NO_THROW({
    Polynomial res_p = p * 0.0;
    // res_p.print_polynom();
    EXPECT_TRUE(res_p.is_empty());
  });
}

TEST(Test_Polynomial, MultiplicationPolynomIsCorrectNoThrow) {
  std::vector<std::pair<double, size_t>> source1 = {{2.0, 100}, {1.0, 1}};

  std::vector<std::pair<double, size_t>> source2 = {{3.0, 100}, {4.0, 0}};

  Polynomial p1(source1);
  // p1.print_polynom();
  Polynomial p2(source2);
  // p2.print_polynom();

  std::vector<std::pair<double, size_t>> data;

  EXPECT_NO_THROW({
    Polynomial res_p = p1 * p2;
    // res_p.print_polynom();
    EXPECT_FALSE(res_p.is_empty());

    data = res_p.get_polynom();
    EXPECT_EQ(data[0].first, 6.0);
    EXPECT_EQ(data[1].first, 3.0);
    EXPECT_EQ(data[2].first, 8.0);
    EXPECT_EQ(data[3].first, 4.0);
    EXPECT_EQ(data[0].second, 200);
    EXPECT_EQ(data[1].second, 101);
    EXPECT_EQ(data[2].second, 100);
    EXPECT_EQ(data[3].second, 1);
  });
}

TEST(Test_Polynomial, MultiplicationPolynomAnyThrow) {
  std::vector<std::pair<double, size_t>> source1 = {{2.0, 999}};

  std::vector<std::pair<double, size_t>> source2 = {{4.0, 998}};

  Polynomial p1(source1);
  // p1.print_polynom();
  Polynomial p2(source2);
  // p2.print_polynom();

  EXPECT_THROW(Polynomial res_p = p1 * p2, std::range_error);
}

// Tests UnorderedTable:

using FindResultInt = typename BaseTable<std::string, int>::FindResult;

TEST(Test_UnorderedTable, CreateIsCorrect) {
  UnorderedTable<std::string, int> table;
  EXPECT_EQ(table.size(), 0);
  EXPECT_EQ(table.getTableName(), "UnorderedTable");
}

TEST(Test_UnorderedTable, InsertAndFind) {
  UnorderedTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.insert("b", 20);

    FindResultInt res1 = table.find("a");
    FindResultInt res2 = table.find("b");

    EXPECT_NE(res1.value, nullptr);
    EXPECT_NE(res2.value, nullptr);

    EXPECT_EQ(*res1.value, 10);
    EXPECT_EQ(*res2.value, 20);
  });
}

TEST(Test_UnorderedTable, OverwriteKey) {
  UnorderedTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.insert("a", 50);

    FindResultInt res = table.find("a");
    EXPECT_EQ(*res.value, 50);
    EXPECT_EQ(table.size(), 1);
  });
}

TEST(Test_UnorderedTable, FindNonExistent) {
  UnorderedTable<std::string, int> table;

  EXPECT_NO_THROW({
    FindResultInt res = table.find("x");
    EXPECT_EQ(res.value, nullptr);
  });
}

TEST(Test_UnorderedTable, Remove) {
  UnorderedTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.insert("b", 20);

    table.remove("a");

    FindResultInt res1 = table.find("a");
    EXPECT_EQ(res1.value, nullptr);
    EXPECT_EQ(table.size(), 1);

    FindResultInt res2 = table.find("b");
    EXPECT_NE(res2.value, nullptr);
    EXPECT_EQ(*res2.value, 20);
  });
}

TEST(Test_UnorderedTable, RemoveNonExistent) {
  UnorderedTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.remove("x");

    EXPECT_EQ(table.size(), 1);
  });
}

TEST(Test_UnorderedTable, Clear_GetTableName_Size) {
  UnorderedTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 1);
    EXPECT_EQ(table.size(), 1);
    table.insert("b", 2);
    EXPECT_EQ(table.size(), 2);

    table.clear();

    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.getTableName(), "UnorderedTable");
  });
}

// Tests AVLTable:

TEST(Test_AVLTable, CreateIsCorrect) {
  AVLTable<std::string, int> table;
  EXPECT_EQ(table.size(), 0);
  EXPECT_EQ(table.getTableName(), "AVLTable");
}

TEST(Test_AVLTable, InsertAndFind) {
  AVLTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 1);
    table.insert("b", 2);
    table.insert("c", 3);

    FindResultInt res1 = table.find("a");
    FindResultInt res2 = table.find("b");
    FindResultInt res3 = table.find("c");

    EXPECT_EQ(*res1.value, 1);
    EXPECT_EQ(*res2.value, 2);
    EXPECT_EQ(*res3.value, 3);
  });
}

TEST(Test_AVLTable, OverwriteKey) {
  AVLTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 1);
    table.insert("a", 99);

    FindResultInt res = table.find("a");

    EXPECT_EQ(*res.value, 99);
    EXPECT_EQ(table.size(), 1);
  });
}

TEST(Test_AVLTable, FindNonExistent) {
  AVLTable<std::string, int> table;

  EXPECT_NO_THROW({
    FindResultInt res = table.find("x");
    EXPECT_EQ(res.value, nullptr);
  });
}

TEST(Test_AVLTable, RemoveLeaf) {
  AVLTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("b", 1);
    table.insert("a", 2);

    table.remove("a");

    FindResultInt res = table.find("a");

    EXPECT_EQ(res.value, nullptr);
    EXPECT_EQ(table.size(), 1);
  });
}

TEST(Test_AVLTable, RemoveNodeWithOneChild) {
  AVLTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("c", 1);
    table.insert("b", 2);
    table.insert("a", 3);

    table.remove("b");

    FindResultInt res1 = table.find("b");
    FindResultInt res2 = table.find("a");

    EXPECT_EQ(res1.value, nullptr);

    EXPECT_NE(res2.value, nullptr);
    EXPECT_EQ(*res2.value, 3);
  });
}

TEST(Test_AVLTable, RemoveNodeWithTwoChild) {
  AVLTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("b", 1);
    table.insert("a", 2);
    table.insert("c", 3);

    table.remove("b");

    FindResultInt res1 = table.find("b");
    FindResultInt res2 = table.find("a");
    FindResultInt res3 = table.find("c");

    EXPECT_EQ(res1.value, nullptr);

    EXPECT_NE(res2.value, nullptr);
    EXPECT_EQ(*res2.value, 2);
    EXPECT_NE(res3.value, nullptr);
    EXPECT_EQ(*res3.value, 3);
  });
}

TEST(Test_AVLTable, RemoveNonExistent) {
  AVLTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.remove("x");

    EXPECT_EQ(table.size(), 1);
  });
}

TEST(Test_AVLTable, Clear_GetTableName_Size) {
  AVLTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 1);
    EXPECT_EQ(table.size(), 1);
    table.insert("b", 2);
    EXPECT_EQ(table.size(), 2);

    table.clear();

    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.getTableName(), "AVLTable");
  });
}

// Tests HashTable:

TEST(Test_HashTable, CreateIsCorrect) {
  HashTable<std::string, int> table;
  EXPECT_EQ(table.size(), 0);
  EXPECT_EQ(table.getTableName(), "HashTable");
}

TEST(Test_HashTable, InsertAndFind) {
  HashTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.insert("b", 20);

    FindResultInt res1 = table.find("a");
    FindResultInt res2 = table.find("b");

    EXPECT_EQ(*res1.value, 10);
    EXPECT_EQ(*res2.value, 20);
  });
}

TEST(Test_HashTable, OverwriteKey) {
  HashTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.insert("a", 50);

    FindResultInt res = table.find("a");

    EXPECT_EQ(*res.value, 50);
    EXPECT_EQ(table.size(), 1);
  });
}

TEST(Test_HashTable, FindNonExistent) {
  HashTable<std::string, int> table;

  EXPECT_NO_THROW({
    FindResultInt res = table.find("x");
    EXPECT_EQ(res.value, nullptr);
  });
}

TEST(Test_HashTable, Remove) {
  HashTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.insert("b", 20);

    table.remove("a");

    FindResultInt res1 = table.find("a");
    EXPECT_EQ(res1.value, nullptr);
    EXPECT_EQ(table.size(), 1);

    FindResultInt res2 = table.find("b");
    EXPECT_NE(res2.value, nullptr);
    EXPECT_EQ(*res2.value, 20);
  });
}

TEST(Test_HashTable, RemoveNonExistent) {
  HashTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 10);
    table.remove("x");

    EXPECT_EQ(table.size(), 1);
  });
}

TEST(Test_HashTable, RehashTrigger) {
  HashTable<std::string, int> table(2, 0.5);

  EXPECT_NO_THROW({
    table.insert("a", 1);
    EXPECT_EQ(table.size_capacity(), 2);
    table.insert("b", 2);

    EXPECT_EQ(table.size(), 2);
    EXPECT_EQ(table.size_capacity(), 4);

    FindResultInt res1 = table.find("a");
    FindResultInt res2 = table.find("b");

    EXPECT_NE(res1.value, nullptr);
    EXPECT_EQ(*res1.value, 1);
    EXPECT_NE(res2.value, nullptr);
    EXPECT_EQ(*res2.value, 2);
  });
}

TEST(Test_HashTable, Clear_GetTableName_Size) {
  HashTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insert("a", 1);
    EXPECT_EQ(table.size(), 1);
    table.insert("b", 2);
    EXPECT_EQ(table.size(), 2);

    table.clear();

    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.getTableName(), "HashTable");
  });
}

// Tests MainTable:

TEST(Test_MainTable, InsertALLAndFindALL) {
  MainTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insertALL("a", 10);
    table.insertALL("b", 20);

    FindResultInt res1 = table.findALL("a");
    FindResultInt res2 = table.findALL("b");

    EXPECT_NE(res1.value, nullptr);
    EXPECT_EQ(*res1.value, 10);
    EXPECT_NE(res2.value, nullptr);
    EXPECT_EQ(*res2.value, 20);
  });
}

TEST(Test_MainTable, RemoveALL) {
  MainTable<std::string, int> table;

  EXPECT_NO_THROW({
    table.insertALL("a", 10);
    table.insertALL("b", 20);

    table.removeALL("a");

    FindResultInt res1 = table.findALL("a");
    EXPECT_EQ(res1.value, nullptr);

    FindResultInt res2 = table.findALL("b");
    EXPECT_NE(res2.value, nullptr);
    EXPECT_EQ(*res2.value, 20);
  });
}

TEST(Test_MainTable, is_emptyALL) {
  MainTable<std::string, int> table;

  EXPECT_NO_THROW({
    EXPECT_TRUE(table.is_emptyALL());

    table.insertALL("a", 1);

    EXPECT_FALSE(table.is_emptyALL());
  });
}

// Polynom Table Tests:

using FindResultPolynomial =
    typename BaseTable<std::string, Polynomial>::FindResult;

TEST(Test_TableWithPolynomial, UnorderedTableWithPolynomial) {
  UnorderedTable<std::string, Polynomial> table;

  std::vector<std::pair<double, size_t>> source = {{1.0, 111}};
  std::vector<std::pair<double, size_t>> data;
  Polynomial p(source);

  EXPECT_NO_THROW({
    table.insert("a", p);

    FindResultPolynomial res = table.find("a");

    EXPECT_FALSE((*res.value).is_empty());
    data = (*res.value).get_polynom();

    EXPECT_EQ(data[0].first, 1.0);
    EXPECT_EQ(data[0].second, 111);
  });
}

TEST(Test_TableWithPolynomial, AVLTableWithPolynomial) {
  AVLTable<std::string, Polynomial> table;

  std::vector<std::pair<double, size_t>> source = {{2.0, 222}};
  std::vector<std::pair<double, size_t>> data;
  Polynomial p(source);

  EXPECT_NO_THROW({
    table.insert("a", p);

    FindResultPolynomial res = table.find("a");

    EXPECT_FALSE((*res.value).is_empty());
    data = (*res.value).get_polynom();

    EXPECT_EQ(data[0].first, 2.0);
    EXPECT_EQ(data[0].second, 222);
  });
}

TEST(Test_TableWithPolynomial, HashTableWithPolynomial) {
  HashTable<std::string, Polynomial> table;

  std::vector<std::pair<double, size_t>> source = {{3.0, 333}};
  std::vector<std::pair<double, size_t>> data;
  Polynomial p(source);

  EXPECT_NO_THROW({
    table.insert("a", p);

    FindResultPolynomial res = table.find("a");

    EXPECT_FALSE((*res.value).is_empty());
    data = (*res.value).get_polynom();

    EXPECT_EQ(data[0].first, 3.0);
    EXPECT_EQ(data[0].second, 333);
  });
}

// Experiments Tests:

// Generating unique string keys
std::vector<std::string> generateKeys(size_t n) {
  std::vector<std::string> keys;
  keys.resize(n);
  for (size_t i = 0; i < n; ++i) {
    keys[i] = "key" + std::to_string(i);
  }
  return keys;
}

// Generation of simple polynomials (each contains one monomial)
std::vector<Polynomial> generatePolynomials(size_t n) {
  std::vector<Polynomial> polynoms;
  polynoms.resize(n);
  for (size_t i = 0; i < n; ++i) {
    std::vector<std::pair<double, size_t>> source;
    source.push_back({static_cast<double>(i + 1), i % 1000});
    polynoms[i] = Polynomial(source);
  }
  return polynoms;
}

TEST(ExperimentInsert, Comparison_of_tables) {
  std::vector<size_t> volume;
  volume.push_back(1000);
  volume.push_back(5000);
  volume.push_back(10000);
  volume.push_back(20000);

  std::cout << "+------------------------+INSERT_EXPERIMENT+-------------------"
               "-----+\n";

  for (size_t option = 0; option < volume.size(); ++option) {
    size_t N = volume[option];
    std::vector<std::string> keys = generateKeys(N);
    std::vector<Polynomial> values = generatePolynomials(N);

    size_t Unordered_opCount = 0;
    size_t AVL_opCount = 0;
    size_t Hash_opCount = 0;

    UnorderedTable<std::string, Polynomial> Unordered_t;
    AVLTable<std::string, Polynomial> AVL_t;
    HashTable<std::string, Polynomial> Hash_t;

    std::cout << "N = " << N << ":\n";

    // UnorderedTable:
    std::chrono::high_resolution_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      Unordered_opCount += Unordered_t.insert(keys[i], values[i]);
    }
    std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    double Unordered_time = std::chrono::duration<double>(end - start).count();
    std::cout << "Unordered: opCount = " << Unordered_opCount
              << ", Time = " << Unordered_time << " seconds\n";

    // AVLTable:
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      AVL_opCount += AVL_t.insert(keys[i], values[i]);
    }
    end = std::chrono::high_resolution_clock::now();

    double AVL_time = std::chrono::duration<double>(end - start).count();
    std::cout << "AVL: opCount = " << AVL_opCount << ", Time = " << AVL_time
              << " seconds\n";

    // HashTable:
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      Hash_opCount += Hash_t.insert(keys[i], values[i]);
    }
    end = std::chrono::high_resolution_clock::now();

    double Hash_time = std::chrono::duration<double>(end - start).count();
    std::cout << "Hash: opCount = " << Hash_opCount << ", Time = " << Hash_time
              << " seconds\n\n";
  }

  std::cout << "+--------------------------------------------------------------"
               "-----+\n";
}

TEST(ExperimentFind, Comparison_of_tables) {
  std::vector<size_t> volume;
  volume.push_back(1000);
  volume.push_back(5000);
  volume.push_back(10000);
  volume.push_back(20000);

  std::cout << "+-------------------------+FIND_EXPERIMENT+--------------------"
               "-----+\n";

  for (size_t option = 0; option < volume.size(); ++option) {
    size_t N = volume[option];
    std::vector<std::string> keys = generateKeys(N);
    std::vector<Polynomial> values = generatePolynomials(N);

    size_t Unordered_opCount = 0;
    size_t AVL_opCount = 0;
    size_t Hash_opCount = 0;

    UnorderedTable<std::string, Polynomial> Unordered_t;
    AVLTable<std::string, Polynomial> AVL_t;
    HashTable<std::string, Polynomial> Hash_t;

    // Filling tables with data:
    for (size_t i = 0; i < N; ++i) {
      Unordered_t.insert(keys[i], values[i]);
      AVL_t.insert(keys[i], values[i]);
      Hash_t.insert(keys[i], values[i]);
    }

    std::cout << "N = " << N << ":\n";

    // UnorderedTable:
    std::chrono::high_resolution_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      typename BaseTable<std::string, Polynomial>::FindResult res =
          Unordered_t.find(keys[i]);
      Unordered_opCount += res.operationsCount;
    }
    std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    double Unordered_time = std::chrono::duration<double>(end - start).count();
    std::cout << "Unordered: opCount = " << Unordered_opCount
              << ", Time = " << Unordered_time << " seconds\n";

    // AVLTable:
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      typename BaseTable<std::string, Polynomial>::FindResult res =
          AVL_t.find(keys[i]);
      AVL_opCount += res.operationsCount;
    }
    end = std::chrono::high_resolution_clock::now();

    double AVL_time = std::chrono::duration<double>(end - start).count();
    std::cout << "AVL: opCount = " << AVL_opCount << ", Time = " << AVL_time
              << " seconds\n";

    // HashTable:
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      typename BaseTable<std::string, Polynomial>::FindResult res =
          Hash_t.find(keys[i]);
      Hash_opCount += res.operationsCount;
    }
    end = std::chrono::high_resolution_clock::now();

    double Hash_time = std::chrono::duration<double>(end - start).count();
    std::cout << "Hash: opCount = " << Hash_opCount << ", Time = " << Hash_time
              << " seconds\n\n";
  }

  std::cout << "+--------------------------------------------------------------"
               "-----+\n";
}

TEST(ExperimentRemove, Comparison_of_tables) {
  std::vector<size_t> volume;
  volume.push_back(1000);
  volume.push_back(5000);
  volume.push_back(10000);
  volume.push_back(20000);

  std::cout << "+------------------------+REMOVE_EXPERIMENT+-------------------"
               "-----+\n";

  for (size_t option = 0; option < volume.size(); ++option) {
    size_t N = volume[option];
    std::vector<std::string> keys = generateKeys(N);
    std::vector<Polynomial> values = generatePolynomials(N);

    size_t Unordered_opCount = 0;
    size_t AVL_opCount = 0;
    size_t Hash_opCount = 0;

    UnorderedTable<std::string, Polynomial> Unordered_t;
    AVLTable<std::string, Polynomial> AVL_t;
    HashTable<std::string, Polynomial> Hash_t;

    // Filling tables with data:
    for (size_t i = 0; i < N; ++i) {
      Unordered_t.insert(keys[i], values[i]);
      AVL_t.insert(keys[i], values[i]);
      Hash_t.insert(keys[i], values[i]);
    }

    std::cout << "N = " << N << ":\n";

    // UnorderedTable:
    std::chrono::high_resolution_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      Unordered_opCount += Unordered_t.remove(keys[i]);
    }
    std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    double Unordered_time = std::chrono::duration<double>(end - start).count();
    std::cout << "Unordered: opCount = " << Unordered_opCount
              << ", Time = " << Unordered_time << " seconds\n";

    // AVLTable:
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      AVL_opCount += AVL_t.remove(keys[i]);
    }
    end = std::chrono::high_resolution_clock::now();

    double AVL_time = std::chrono::duration<double>(end - start).count();
    std::cout << "AVL: opCount = " << AVL_opCount << ", Time = " << AVL_time
              << " seconds\n";

    // HashTable:
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
      Hash_opCount += Hash_t.remove(keys[i]);
    }
    end = std::chrono::high_resolution_clock::now();

    double Hash_time = std::chrono::duration<double>(end - start).count();
    std::cout << "Hash: opCount = " << Hash_opCount << ", Time = " << Hash_time
              << " seconds\n\n";
  }

  std::cout << "+--------------------------------------------------------------"
               "-----+\n";
}
