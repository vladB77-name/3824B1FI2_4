#pragma once
#include "Tables.h"

template<typename T>
class unordered_table : public Table<T> {
private:
	struct Record {
		std::string key;
		T data;
		Record(const std::string key, const T &data): key(key), data(data) {}
	};

	std::vector<Record> table;
public:
	unordered_table(const size_t capacity, std::ofstream &file): Table<T>(file) {
		this->tableType = "Unordered Table";
		table.reserve(capacity);
	}
	unordered_table(std::ofstream &file) : unordered_table(100, file) {}

	T *find(const std::string &key) override {
		size_t t = this->findOperationsCnt;
		const size_t SZ = table.size();
		this->findOperationsCnt += 5;
		for (size_t i = 0; i < SZ; ++i) {
			this->findOperationsCnt += 3;
			if (table[i].key == key) {
				this->findOperationsCnt += 3;

				this->file << this->tableType << " find: " << this->findOperationsCnt - t << '\n';
				return &table[i].data;
			}
			this->findOperationsCnt += 2;
		}
		this->file << this->tableType << " find: " << this->findOperationsCnt - t << '\n';
		return nullptr;
	}

	void insert(const std::string &key, const T &data) override {
		size_t t = this->findOperationsCnt, k = this->insertOperationsCnt;
		T* elem = find(key);
		this->file << this->tableType << " find: " << this->findOperationsCnt - t << '\n';
		this->insertOperationsCnt += 3 + this->findOperationsCnt - t;
		
		if (elem) {
			*elem = data;
			this->insertOperationsCnt += 2;

			this->file << this->tableType << " insert: " << this->insertOperationsCnt - k << '\n';
			return;
		}
		table.emplace_back(key, data);
		this->insertOperationsCnt += 3;
		this->file << this->tableType << " insert: " << this->insertOperationsCnt - k << '\n';
		return;
	}

	void erase(const std::string &key) override {
		size_t t = this->eraseOperationsCnt;
		const size_t SZ = table.size();
		this->eraseOperationsCnt += 5;
		for (size_t i = 0; i < SZ; ++i) {
			this->eraseOperationsCnt += 3;
			if (table[i].key == key) {
				std::swap(table[i], table.back());
				table.pop_back();
				this->eraseOperationsCnt += 9;

				this->file << this->tableType << " erase: " << this->eraseOperationsCnt - t << '\n';
				return;
			}
			this->eraseOperationsCnt += 2;
		}
		this->file << this->tableType << " erase: " << this->eraseOperationsCnt - t << '\n';
	}

	bool extract(const std::string &key, T &out) override {
		size_t t = this->extractOperationsCnt;
		const size_t SZ = table.size();
		this->extractOperationsCnt += 5;

		for (size_t i = 0; i < SZ; ++i) {
			this->extractOperationsCnt += 3;
			if (table[i].key == key) {
				out = table[i].data;
				std::swap(table[i], table.back());
				table.pop_back();
				this->extractOperationsCnt += 12;

				this->file << this->tableType << " extract: " << this->extractOperationsCnt - t << '\n';
				return true;
			}
			this->extractOperationsCnt += 2;
		}

		this->file << this->tableType << " extract: " << this->extractOperationsCnt - t << '\n';
		return false;
	}
};
