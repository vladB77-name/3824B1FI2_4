#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

template<typename T>
class Table {
protected:
	std::string tableType;
	std::ofstream& file;
	size_t findOperationsCnt = 0, insertOperationsCnt = 0, eraseOperationsCnt = 0, extractOperationsCnt = 0;
public:
	Table(std::ofstream& file): file(file) {}

	virtual T* find(const std::string& key) = 0;
	virtual void insert(const std::string& key, const T& data) = 0;
	virtual void erase(const std::string& key) = 0;
	virtual bool extract(const std::string& key, T& out) {
		size_t t = findOperationsCnt;
		T* val = find(key);
		file << tableType << " find: " << findOperationsCnt - t << '\n';
		
		extractOperationsCnt += 3 + findOperationsCnt - t;
		if (val != nullptr) {
			out = *val;
			t = eraseOperationsCnt;
			erase(key);
			extractOperationsCnt += 3 + eraseOperationsCnt - t;
			return true;
		}
		return false;
	}

	size_t getFindOperationsCount() {
		return findOperationsCnt;
	}

	size_t getInsertOperationsCnt() {
		return insertOperationsCnt;
	}

	size_t getEraseOperationsCnt() {
		return eraseOperationsCnt;
	}

	size_t getExtractOperationsCnt() {
		return extractOperationsCnt;
	}

	void resetCounters() {
		findOperationsCnt = insertOperationsCnt = eraseOperationsCnt = extractOperationsCnt = 0;
	}

	std::string getType() {
		return tableType;
	}

	virtual ~Table() {
		file << "\nTotal number of operations in " << tableType << ":\n";
		file << "1. Find: " << findOperationsCnt << '\n';
		file << "2. Insert: " << insertOperationsCnt << '\n';
		file << "3. Erase: " << eraseOperationsCnt << '\n';
		file << "4. Extract: " << extractOperationsCnt << '\n';
	}
};
