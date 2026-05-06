#include "pch.h"
#include <random>
#include <map>
#include "../Tables/unordered_table.h"
#include "../Tables/rb_tree.h"
#include "../Tables/hash_table.h"
#include "../Tables/Polynomial.h"
#include "../Tables/Polynomial.cpp"


using namespace std;
using P = Polynomial;

ofstream f("tests.txt");
using UT = unordered_table<Polynomial>;
using RB = rb_tree<Polynomial>;
using HT = hash_table<Polynomial>;

Polynomial randP() {
	default_random_engine re;
	uniform_int_distribution<> un(0, 9), sz(1, 20);
	bernoulli_distribution sign(0.5);
	size_t size = sz(re);

	string p{};
	for (int i = 0; i < size; ++i) {
		p += sign(re) == 0 ? '-' : '+';
		for (int j = 0; j < 3; ++j) {
			p += to_string(un(re));
		}
		p += " + ";
	}
	return Polynomial(p);
}

//unordered table 
TEST(UnorderedTable, BasicInsertFind) {
	UT t(f);

	t.insert("a", 1);
	t.insert("b", 2);
	t.insert("c", 3);

	ASSERT_NE(t.find("a"), nullptr);
	EXPECT_EQ(*t.find("a"), 1);

	ASSERT_NE(t.find("b"), nullptr);
	EXPECT_EQ(*t.find("b"), 2);

	ASSERT_NE(t.find("c"), nullptr);
	EXPECT_EQ(*t.find("c"), 3);

	EXPECT_EQ(t.find("x"), nullptr);
}

TEST(UnorderedTable, UpdateValue) {
	UT t(f);

	t.insert("a", 1);
	t.insert("a", 999);

	ASSERT_NE(t.find("a"), nullptr);
	EXPECT_EQ(*t.find("a"), 999);
}

TEST(UnorderedTable, Erase) {
    UT t(f);

	t.insert("a", 1);
	t.insert("b", 2);
	t.insert("c", 3);

	t.erase("b");

	EXPECT_EQ(t.find("b"), nullptr);
	EXPECT_EQ(*t.find("a"), 1);
	EXPECT_EQ(*t.find("c"), 3);
}

TEST(UnorderedTable, Extract) {
	UT t(f);

	t.insert("a", 10);
	t.insert("b", 2);
	t.insert("c", 3);

	P out = 0;
	EXPECT_TRUE(t.extract("a", out));
	EXPECT_EQ(out, 10);
	EXPECT_EQ(t.find("a"), nullptr);
}

// Hash table
TEST(HashTable, BasicInsertFind) {
	HT t(f);

	t.insert("a", 1);
	t.insert("b", 2);
	t.insert("c", 3);

	ASSERT_NE(t.find("a"), nullptr);
	EXPECT_EQ(*t.find("a"), 1);

	ASSERT_NE(t.find("b"), nullptr);
	EXPECT_EQ(*t.find("b"), 2);

	ASSERT_NE(t.find("c"), nullptr);
	EXPECT_EQ(*t.find("c"), 3);

	EXPECT_EQ(t.find("x"), nullptr);
}

TEST(HashTable, BasicCollisionInsertFind) {
	HT t(f);

	t.insert("11", 1);
	t.insert("b", 2);
	t.insert("c", 3);

	ASSERT_NE(t.find("11"), nullptr);
	EXPECT_EQ(*t.find("11"), 1);

	ASSERT_NE(t.find("b"), nullptr);
	EXPECT_EQ(*t.find("b"), 2);

	ASSERT_NE(t.find("c"), nullptr);
	EXPECT_EQ(*t.find("c"), 3);

	EXPECT_EQ(t.find("x"), nullptr);
}

TEST(HashTable, Erase) {
	HT t(f);

	t.insert("a", 1);
	t.insert("b", 2);
	t.insert("c", 3);

	t.erase("b");

	EXPECT_EQ(t.find("b"), nullptr);
	EXPECT_EQ(*t.find("a"), 1);
	EXPECT_EQ(*t.find("c"), 3);
}

TEST(HashTable, SimpleCollisionEraseInsideChain) {
	HT t(f);

	t.insert("11", 1);
	t.insert("b", 2);
	t.insert("c", 3);
	t.insert("02", 4);

	t.erase("b");

	EXPECT_EQ(t.find("b"), nullptr);
	EXPECT_EQ(*t.find("11"), 1);
	EXPECT_EQ(*t.find("c"), 3);
	EXPECT_EQ(*t.find("02"), 4);
}

TEST(HashTable, SimpleCollisionEraseHead) {
	HT t(f);

	t.insert("11", 1);
	t.insert("b", 2);
	t.insert("c", 3);
	t.insert("02", 4);

	t.erase("11");

	EXPECT_EQ(t.find("11"), nullptr);
	EXPECT_EQ(*t.find("b"), 2);
	EXPECT_EQ(*t.find("c"), 3);
	EXPECT_EQ(*t.find("02"), 4);
}

TEST(HashTable, SimpleCollisionEraseAtEndOfChain) {
	HT t(f);

	t.insert("11", 1);
	t.insert("b", 2);
	t.insert("c", 3);
	t.insert("02", 4);

	t.erase("02");

	EXPECT_EQ(t.find("02"), nullptr);
	EXPECT_EQ(*t.find("b"), 2);
	EXPECT_EQ(*t.find("c"), 3);
	EXPECT_EQ(*t.find("11"), 1);
}

TEST(HashTable, UpdateValue) {
	HT t(f);

	t.insert("a", 1);
	t.insert("a", 999);

	ASSERT_NE(t.find("a"), nullptr);
	EXPECT_EQ(*t.find("a"), 999);
}

TEST(HashTable, Extract) {
	HT t(f);

	t.insert("a", 10);
	t.insert("b", 2);
	t.insert("c", 3);

	P out = 0;
	EXPECT_TRUE(t.extract("a", out));
	EXPECT_EQ(out, 10);
	EXPECT_EQ(t.find("a"), nullptr);
}

TEST(HashTable, StressTest) {
	HT t(1000, f);

	const int NUM = 1000;
	vector<P> ps(NUM);

	for (int i = 0; i < NUM; i++) {
		P poly(i);
		ps[i] = poly;
		t.insert(poly.get(), poly);
	}

	for (int i = 0; i < NUM; i += 2) {
		t.erase(ps[i].get());
	}

	for (int i = 0; i < NUM; i++) {
		auto res = t.find(ps[i].get());
		if (i % 2 == 0) {
			EXPECT_EQ(res, nullptr);
		}
		else {
			ASSERT_NE(res, nullptr);
			EXPECT_EQ(*res, i);
		}
	}
}


// Red-Black Tree
TEST(RBTree, BasicInsertFind) {
	RB t(f);

	t.insert("a", 1);
	EXPECT_TRUE(t.isValidRBTree());
	t.insert("b", 2);
	EXPECT_TRUE(t.isValidRBTree());
	t.insert("c", 3);
	EXPECT_TRUE(t.isValidRBTree());

	ASSERT_NE(t.find("a"), nullptr);
	EXPECT_EQ(*t.find("a"), 1);

	ASSERT_NE(t.find("b"), nullptr);
	EXPECT_EQ(*t.find("b"), 2);

	ASSERT_NE(t.find("c"), nullptr);
	EXPECT_EQ(*t.find("c"), 3);

	EXPECT_EQ(t.find("x"), nullptr);
}

TEST(RBTree, UpdateValue) {
	RB t(f);
	t.insert("A", 1);
	t.insert("A", 2);
	t.insert("A", 3);

	auto* val = t.find("A");

	ASSERT_NE(val, nullptr);
	EXPECT_EQ(*val, 3);

	EXPECT_TRUE(t.isValidRBTree());
}

TEST(RBTree, Erase) {
	RB t(f);

	vector<string> keys = {"7", "2", "8", "9", "1", "5", "6", "4", "3"};

	for (int i = 0; i < keys.size(); ++i) {
		t.insert(keys[i], i);
	}

	for (auto& k : keys) {
		t.erase(k);
		EXPECT_EQ(nullptr, t.find(k));
		EXPECT_TRUE(t.isValidRBTree());
	}
}

TEST(RBTree, emptyTree) {
	RB t(f);

	EXPECT_EQ(t.find("a"), nullptr);

	EXPECT_NO_THROW(t.erase("a"));

	EXPECT_TRUE(t.isValidRBTree());
}

TEST(RBTree, SortedInsert) {
	RB t(f);

	vector<P> poly;
	for (int i = 0; i < 1000; ++i) {
		poly.push_back(i);
		t.insert(poly[i].get(), poly[i]);
		EXPECT_TRUE(t.isValidRBTree());
	}

	for (int i = 0; i < 1000; ++i) {
		EXPECT_EQ(*t.find(poly[i].get()), poly[i]);
	}

	for (int i = 0; i < 1000; i += 2) {
		t.erase(poly[i].get());
		EXPECT_TRUE(t.isValidRBTree());
	}

	EXPECT_TRUE(t.isValidRBTree());
}

TEST(RBTree, Extract) {
	RB t(f);

	vector<P> poly;
	for (int i = 0; i < 1000; ++i) {
		poly.push_back(i);
		t.insert(poly[i].get(), poly[i]);
		EXPECT_TRUE(t.isValidRBTree());
	}

	for (int i = 0; i < 1000; ++i) {
		EXPECT_EQ(*t.find(poly[i].get()), poly[i]);
	}

	for (int i = 0; i < 1000; i += 2) {
		P tmp;
		t.extract(poly[i].get(), tmp);
		EXPECT_EQ(tmp, i);
		EXPECT_TRUE(t.isValidRBTree());
	}

	EXPECT_TRUE(t.isValidRBTree());
}


TEST(RBTree, StressTest) {
	RB t(f);
	map<string, P> ref;

	default_random_engine re;
	uniform_int_distribution<> rOp(0, 2), poly(0, 999);
	for (int i = 0; i < 10000; ++i) {
		int op = rOp(re);
		P tmp = poly(re);
		string key = tmp.get();
		if (op == 0) { // insert
			t.insert(key, tmp);
			ref[key] = tmp;
			EXPECT_TRUE(t.isValidRBTree());
		}
		else if (op == 1) { // erase
			t.erase(key);
			ref.erase(key);
			EXPECT_TRUE(t.isValidRBTree());
		}
		else { // find
			auto* res = t.find(key);
			auto it = ref.find(key);

			if (it == ref.end()) {
				ASSERT_EQ(res, nullptr);
			}
			else {
				ASSERT_NE(res, nullptr);
				EXPECT_EQ(*res, it->second);
			}
		}
	}
	EXPECT_TRUE(t.isValidRBTree());
}