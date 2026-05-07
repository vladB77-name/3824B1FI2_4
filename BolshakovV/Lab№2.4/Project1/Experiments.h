#pragma once
#include "Polinom.h"
#include "Tables.h"

#include <chrono>
#include <random>
#include <iomanip>

std::string generate_key(int id) {
	std::stringstream ss;
	ss << "key_" << std::setw(4) << std::setfill('0') << id; // key_0001, key_0002...
	return ss.str();
}

unsigned int generate_degr() {


	std::mt19937 gen(std::random_device{}());

	std::uniform_int_distribution<unsigned int> dis(0, 999);
	unsigned int deg = dis(gen);
	return deg;
}

Polinom generate_pol(int id) {

	Polinom p;
	unsigned int deg = generate_degr();
	p.add_monom(id * 0.1, deg);
	return p;
}

std::vector< std::pair<size_t, double>> run_insert_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash) {

	std::pair<size_t, double> res_exp;
	std::vector< std::pair<size_t, double>> Result; //unordered_table - 0, AVLtable - 1, HashTable - 2;

	// unordered_table
	auto start_time = std::chrono::high_resolution_clock::now();

	size_t op_count = 0;

	for (size_t i = 0; i < data.size(); ++i) {
		un.Insert(data[i].key, data[i].polinom);
		op_count += un.get_count_operations();
	}

	auto end_time = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res_time = end_time - start_time;
	// Доступ через friend


	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);


	//////// AVLtable
	op_count = 0;

	start_time = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < data.size(); ++i) {
		avl.Insert(data[i].key, data[i].polinom);
		op_count += avl.get_count_operations();
	}

	end_time = std::chrono::high_resolution_clock::now();

	res_time = end_time - start_time;


	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);

	///////// HashTable
	op_count = 0;

	start_time = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < data.size(); ++i) {
		hash.Insert(data[i].key, data[i].polinom);
		op_count += hash.get_count_operations();
	}

	end_time = std::chrono::high_resolution_clock::now();

	res_time = end_time - start_time;

	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);

	return Result;
}
/// Операция Delete
std::vector< std::pair<size_t, double>> run_delete_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash) {

	std::pair<size_t, double> res_exp;
	std::vector< std::pair<size_t, double>> Result; //unordered_table - 0, AVLtable - 1, HashTable - 2;

	/////// unordered_table

	size_t op_count = 0;

	auto start_time = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < data.size(); ++i) {
		un.Delete(data[i].key);
		op_count += un.get_count_operations();
	}

	auto end_time = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res_time = end_time - start_time;


	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);


	//////// AVLtable
	op_count = 0;

	start_time = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < data.size(); ++i) {
		avl.Delete(data[i].key);
		op_count += avl.get_count_operations();
	}

	end_time = std::chrono::high_resolution_clock::now();

	res_time = end_time - start_time;


	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);

	///////// HashTable
	op_count = 0;

	start_time = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < data.size(); ++i) {
		hash.Delete(data[i].key);
		op_count += hash.get_count_operations();
	}

	end_time = std::chrono::high_resolution_clock::now();

	res_time = end_time - start_time;

	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);

	return Result;
}


/// Операция Find
std::vector< std::pair<size_t, double>> run_find_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash) {

	std::pair<size_t, double> res_exp;
	std::vector< std::pair<size_t, double>> Result; //unordered_table - 0, AVLtable - 1, HashTable - 2;

	///////// unordered_table

	size_t op_count = 0; // Доступ через friend

	auto start_time = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < data.size(); ++i) {
		un.Find(data[i].key);
		op_count += un.get_count_operations();
	}

	auto end_time = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res_time = end_time - start_time;

	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);


	//////// AVLtable
	op_count = 0;

	start_time = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < data.size(); ++i) {
		avl.Find(data[i].key);
		op_count += avl.get_count_operations();
	}

	end_time = std::chrono::high_resolution_clock::now();

	res_time = end_time - start_time;

	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);

	///////// HashTable
	op_count = 0;

	start_time = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < data.size(); ++i) {
		hash.Find(data[i].key);
		op_count += hash.get_count_operations();
	}

	end_time = std::chrono::high_resolution_clock::now();

	res_time = end_time - start_time;


	res_exp = { op_count, res_time.count() };
	Result.push_back(res_exp);

	return Result;
}