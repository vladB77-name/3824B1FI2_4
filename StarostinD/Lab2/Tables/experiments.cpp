#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include "../Tables/Tables.h"
#include "../Tables/unordered_table.h"
#include "../Tables/rb_tree.h"
#include "../Tables/hash_table.h"
#include "../Tables/Polynomial.h"
#include "../Tables/Polynomial.cpp"

using namespace std;
using P = Polynomial;

ofstream f("experiments.txt");
using UT = unordered_table<Polynomial>;
using RB = rb_tree<Polynomial>;
using HT = hash_table<Polynomial>;

struct Results {
	std::chrono::steady_clock::time_point start_, end_;
	std::chrono::duration<double> dur_;
	size_t fOp = 0, iOp= 0, eOp= 0;

	void start() {
		start_ = std::chrono::steady_clock::now();
		dur_ = std::chrono::duration<double>{ 0 };
	}

	void end() {
		end_ = std::chrono::steady_clock::now();
	}

	void getOpCnt(Table<Polynomial>* t) {
		fOp = t->getFindOperationsCount();
		iOp = t->getInsertOperationsCnt();
		eOp = t->getEraseOperationsCnt();
	}

	auto res() {
		return dur_.count();
	}

	void clear() {
		fOp = 0, iOp = 0, eOp = 0;
	}

};

Polynomial randP(size_t max_sz = 10) {
	static default_random_engine re;
	uniform_int_distribution<> un(0, 9), sz(1, max_sz);
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

vector<Polynomial> t100(100), t1k(1000), t10k(10000), t100k(100'000);
Results utR, htR, rtR;
Table<Polynomial>* ut;
Table<Polynomial>* ht;
Table<Polynomial>* rt;

void test(vector<Polynomial>& t) {
	size_t sz = t.size();
	ut = new UT(sz, f);
	ht = new HT(sz * 2, f);
	rt = new RB(f);

	vector<string> keys(sz);
	for (int i = 0; i < sz; ++i) {
		keys[i] = t[i].get();
	}

	utR.start(), htR.start(), rtR.start();
	for (int i = 0; i < sz; ++i) {
		auto t1 = std::chrono::steady_clock::now();
		const string& s = keys[i];
		ut->insert(s, t[i]);
		auto t2 = std::chrono::steady_clock::now();
		utR.dur_ += t2 - t1;

		t1 = std::chrono::steady_clock::now();
		ht->insert(s, t[i]);
		t2 = std::chrono::steady_clock::now();
		htR.dur_ += t2 - t1;

		t1 = std::chrono::steady_clock::now();
		rt->insert(s, t[i]);
		t2 = std::chrono::steady_clock::now();
		rtR.dur_ += t2 - t1;
	}
	cout << "Size: " << t.size() << endl;
	cout << "Insert:\n";
	cout << "1. " << ut->getType() << ":\n";
	cout << "Insert total time: " << utR.res() << endl;
	cout << "2. " << ht->getType() << ":\n";
	cout << "Insert total time: " << htR.res() << endl;
	cout << "3. " << rt->getType() << ":\n";
	cout << "Insert total time: " << rtR.res() << endl;

	utR.start(), htR.start(), rtR.start();
	for (int i = 0; i < sz; ++i) {
		const string& s = keys[i];
		auto t1 = std::chrono::steady_clock::now();
		ut->find(s);
		auto t2 = std::chrono::steady_clock::now();
		utR.dur_ += t2 - t1;

		t1 = std::chrono::steady_clock::now();
		ht->find(s);
		t2 = std::chrono::steady_clock::now();
		htR.dur_ += t2 - t1;

		t1 = std::chrono::steady_clock::now();
		rt->find(s);
		t2 = std::chrono::steady_clock::now();
		rtR.dur_ += t2 - t1;
	}
	cout << "Find:\n";
	cout << "1. " << ut->getType() << ":\n";
	cout << "Find total time: " << utR.res() << endl;
	cout << "2. " << ht->getType() << ":\n";
	cout << "Find total time: " << htR.res() << endl;
	cout << "3. " << rt->getType() << ":\n";
	cout << "Find total time: " << rtR.res() << endl;

	utR.start(), htR.start(), rtR.start();
	for (int i = 0; i < sz; ++i) {
		const string& s = keys[i];
		auto t1 = std::chrono::steady_clock::now();
		ut->erase(s);
		auto t2 = std::chrono::steady_clock::now();
		utR.dur_ += t2 - t1;

		t1 = std::chrono::steady_clock::now();
		ht->erase(s);
		t2 = std::chrono::steady_clock::now();
		htR.dur_ += t2 - t1;

		t1 = std::chrono::steady_clock::now();
		rt->erase(s);
		t2 = std::chrono::steady_clock::now();
		rtR.dur_ += t2 - t1;
	}
	cout << "Erase:\n";
	cout << "1. " << ut->getType() << ":\n";
	cout << "Erase total time: " << utR.res() << endl;
	cout << "2. " << ht->getType() << ":\n";
	cout << "Erase total time: " << htR.res() << endl;
	cout << "3. " << rt->getType() << ":\n";
	cout << "Erase total time: " << rtR.res() << endl;


	utR.getOpCnt(ut), htR.getOpCnt(ht), rtR.getOpCnt(rt);
	cout << "\nTotal operations:\n";
	cout << "Insert:\n";
	cout << "1. " << ut->getType() << ": ";
	cout << utR.iOp << endl;
	cout << "2. " << ht->getType() << ": ";
	cout << htR.iOp << endl;
	cout << "3. " << rt->getType() << ": ";
	cout << rtR.iOp << endl;
	
	cout << "Find:\n";
	cout << "1. " << ut->getType() << ": ";
	cout << utR.fOp << endl;
	cout << "2. " << ht->getType() << ": ";
	cout << htR.fOp << endl;
	cout << "3. " << rt->getType() << ": ";
	cout << rtR.fOp << endl;
	
	cout << "Erase:\n";
	cout << "1. " << ut->getType() << ": ";
	cout << utR.eOp << endl;
	cout << "2. " << ht->getType() << ": ";
	cout << htR.eOp << endl;
	cout << "3. " << rt->getType() << ": ";
	cout << rtR.eOp << endl;
	cout << "\n\n\n";

	delete ut;
	delete ht;
	delete rt;
	utR.clear(), htR.clear(), rtR.clear();
}

int main() {
	for (auto& e : t100) {
		e = randP();
	}

	for (auto& e : t1k) {
		e = randP();
	}

	for (auto& e : t10k) {
		e = randP();
	}

	for (auto& e : t100k) {
		e = randP();
	}

	// ===========
	// 100
	// ===========


	test(t100);

	// ===========
	// 1000
	// ===========


	test(t1k);

	// ===========
	// 10000
	// ===========

	
	test(t10k);

	// ===========
	// 1 000 000
	// ===========


	test(t100k);
}