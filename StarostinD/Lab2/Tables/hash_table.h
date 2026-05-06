#pragma once
#include "Tables.h"

template<typename T>
class hash_table : public Table<T> {
private:
	struct Node {
		std::string key;
		T data;
		int next;
		Node(const std::string& key, const T& data, int next = -1): key(key), data(data), next(next) {}
	};
	
	const size_t CAPACITY;
	size_t size;
	std::vector<int> buckets;
	std::vector<Node> nodes;
	std::vector<int> freeIndices;
	
	size_t Hash(const std::string& s, size_t& cnt) {
		size_t sum = 0, sz = s.size();
		cnt += 6;
		for (int i = 0; i < sz; ++i) {
			sum += s[i];
			cnt += 4;
		}
		cnt += 1;
		return sum % CAPACITY;
	}

	bool isInTable(const std::string& key, size_t& _hash, int& out, int& prev, size_t& cnt) {
		_hash = Hash(key, cnt);
		out = buckets[_hash];
		int cur = out;

		cnt += 7;
		while (cur != -1) {
			cnt += 3;
			if (nodes[cur].key == key) {
				out = cur;
				cnt += 1;
				return true;
			}
			prev = cur;
			cur = nodes[cur].next;
			cnt += 4;
		}
		out = prev;
		cnt += 1;
		return false;
	}

public:
	hash_table(size_t capacity, std::ofstream& file) : CAPACITY(capacity), size(0), buckets(capacity, -1), Table<T>(file) {
		this->tableType = "Hash Table";
		nodes.reserve(capacity);
	}

	hash_table(std::ofstream& file) : hash_table(100, file) {}

	T* find(const std::string& key) override {
		size_t k = 0, t = this->findOperationsCnt;
		int pos = buckets[Hash(key, k)];

		this->findOperationsCnt += k + 5;
		while (pos != -1) {
			this->findOperationsCnt += 3;
			if (nodes[pos].key == key) {
				this->findOperationsCnt += 3;

				this->file << this->tableType << " find: " << this->findOperationsCnt - t << '\n';
				return &nodes[pos].data;
			}
			pos = nodes[pos].next;
			this->findOperationsCnt += 4;
		}
		this->file << this->tableType << " find: " << this->findOperationsCnt - t << '\n';
		return nullptr;
	}

	void insert(const std::string& key, const T& data) override {
		size_t t = this->insertOperationsCnt;
		size_t _hash;
		int pos, prev = -1;

		this->insertOperationsCnt += 6;
		if (isInTable(key, _hash, pos, prev, this->insertOperationsCnt)) {
			nodes[pos].data = data;
			this->insertOperationsCnt += 3;

			this->file << this->tableType << " insert: " << this->insertOperationsCnt - t << '\n';
			return;
		}

		this->insertOperationsCnt += 3;
		if (!freeIndices.empty()) {
			int id = freeIndices.back();
			freeIndices.pop_back();

			this->insertOperationsCnt += 7;
			if (buckets[_hash] == -1) {
				buckets[_hash] = id;
				this->insertOperationsCnt += 2;
			}
			else {
				nodes[pos].next = id;
				this->insertOperationsCnt += 3;
			}
			nodes[id].key = key;
			nodes[id].data = data;
			nodes[id].next = -1;
			this->insertOperationsCnt += 9;
		}
		else {
			this->insertOperationsCnt += 2;
			if (buckets[_hash] == -1) {
				buckets[_hash] = nodes.size();
				this->insertOperationsCnt += 4;
			}
			else {
				nodes[pos].next = nodes.size();
				this->insertOperationsCnt += 5;
			}
			nodes.emplace_back(key, data);
			this->insertOperationsCnt += 4;
		}

		++size;
		this->insertOperationsCnt += 1;
		this->file << this->tableType << " insert: " << this->insertOperationsCnt - t << '\n';
	}

	void erase(const std::string& key) override {
		size_t t = this->eraseOperationsCnt;

		size_t _hash;
		int pos, prev = -1;

		this->eraseOperationsCnt += 7;
		if (!isInTable(key, _hash, pos, prev, this->eraseOperationsCnt)) {
			this->file << this->tableType << " erase: " << this->eraseOperationsCnt - t << '\n';
			return;
		}

		freeIndices.push_back(pos);
		this->eraseOperationsCnt += 3;
		if (prev == -1) {
			buckets[_hash] = nodes[pos].next;
			this->eraseOperationsCnt += 4;
		}
		else {
			nodes[prev].next = nodes[pos].next;
			this->eraseOperationsCnt += 5;
		}
		--size;
		this->eraseOperationsCnt += 1;
		this->file << this->tableType << " erase: " << this->eraseOperationsCnt - t << '\n';
	}

};