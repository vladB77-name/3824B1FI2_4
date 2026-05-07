#pragma once
#include <iostream>
#include <vector>
#include <string> //для преобразования в строку
#include <sstream>  //для оператора ввода
#include <cmath> //модуль в операторе вывода
#include <utility> //std::pair, std::swap
#define SIZE_DEGREE 3

template <class T>
class F_List {

private:

	template <class T>
	struct Node {
		Node<T>* pNext;
		T data;
		Node(T data = T(), Node<T>* next = nullptr) :data(data), pNext(next) {}
	};

	Node<T>* head; //указывает на первый элемент списка 
	size_t size;
public:

	class Iterator {
	private:
		Node<T>* current;
	public:

		Iterator(Node<T>* ptr = nullptr) : current(ptr) {}

		T& operator*() {
			if (current == nullptr) {
				throw std::runtime_error("The iterator is equal to nullptr. Cannot access data");
			}

			return current->data;
		}

		Iterator& operator++() {
			if (current != nullptr) {
				current = current->pNext;
			}
			return *this;
		}

		Iterator operator++(int) {
			Iterator it(this->current);
			++(*this);
			return it;
		}

		bool operator !=(const Iterator& other) const {
			return this->current != other.current;
		}

		bool operator ==(const Iterator& other) const { //не используется при реализации класса Polinom
			//return this->current == other.current;
			return this->current == other.current;
		}

		Iterator& operator+=(size_t n) { //не используется при реализации класса Polinom 
			while (n > 0 && current != nullptr) {
				current = current->pNext;
				--n;
			}
			return *this;
		}
	};

	Iterator begin() const {
		return Iterator(head);
	}

	Iterator end() const {
		return Iterator(nullptr);
	}


	F_List() : size(0), head(nullptr) {};

	F_List(const F_List& other) {
		if (other.size != 0) {
			this->size = other.size;
			head = new Node<T>(other.head->data);
			Node<T>* current = head; //указывает на первый элемент
			Node<T>* other_current = other.head->pNext; //указывает на второй элемент other
			while (other_current != nullptr) {
				current->pNext = new Node<T>(other_current->data);
				current = current->pNext;
				other_current = other_current->pNext;
			}
		}
		else {
			this->head = nullptr;
			this->size = 0;
		}
	};

	F_List& operator = (const F_List& other) {
		if (this != &other) {
			F_List temp(other);
			std::swap(size, temp.size);
			std::swap(head, temp.head);
		}
		return *this;
	}

	size_t get_size() const {
		return this->size;
	}

	void push_back(const T& value) {
		if (head == nullptr) {
			head = new Node<T>(value);
		}
		else {
			Node<T>* current = this->head;
			while (current->pNext != nullptr) {
				current = current->pNext;
			}
			current->pNext = new Node<T>(value);
		}
		++size;
	}

	T& operator [] (const size_t& index) {
		if (size == 0) {
			throw std::out_of_range("Error.The size is zero");
		}
		else if (index >= this->size || index < 0) {
			throw std::out_of_range("Error. Incorrect index");
		}

		int count = 0;

		Node<T>* current = this->head;
		while (current != nullptr) {
			if (count == index) {
				return current->data;
			}
			++count;
			current = current->pNext;
		}
	}

	const T& operator [] (const size_t& index) const {
		if (size == 0) {
			throw std::out_of_range("Error.The size is zero");
		}
		else if (index >= this->size || index < 0) {
			throw std::out_of_range("Error. Incorrect index");
		}

		int count = 0;

		Node<T>* current = this->head;
		while (current != nullptr) {
			if (count == index) {
				return current->data;
			}
			++count;
			current = current->pNext;
		}
	}

	void pop_front() {
		if (head == nullptr) {
			return;
		}
		Node<T>* temp = head;
		head = head->pNext;
		delete temp;
		--size;
	}

	void clear() {
		while (size != 0) {
			this->pop_front();
		}
	}

	~F_List() {
		this->clear();
	}

	void push_front(const T& value) {
		head = new Node<T>(value, head);
		++size;
	}

	bool empty() {
		return this->size == 0;
	}

	void insert(const T& value, const size_t& index) { // утсанавливает значение по индексу (не после и не перед индексом)

		if (index >= this->size || index < 0) {
			throw std::out_of_range("Incorrect index");
		}

		if (index == 0) {
			this->push_front(value);
		}
		else {

			Node<T>* current = head;
			for (size_t i = 0; i < index - 1; ++i) {
				current = current->pNext; //в итоге сам указатель направлен на индекс перед нужным нам
			}
			Node<T>* new_node = new Node<T>(value, current->pNext); //новый элемент по адресу индекса (он должен указывать на элемент по индексу)

			current->pNext = new_node;
			++size;
		}
	}

	void remove(const size_t& index) {

		if (size == 0) {
			throw std::out_of_range("Error.The size is zero");
		}
		else if (index >= this->size) {
			throw std::out_of_range("Error. Incorrect index (\"remove element\")");
		}

		Node<T>* toDelete = nullptr;
		if (index == 0) {
			toDelete = head;
			head = head->pNext;
		}
		else {
			Node<T>* previous = head;
			for (size_t i = 0; i < index - 1; ++i) {
				previous = previous->pNext;
			}

			toDelete = previous->pNext;
			previous->pNext = toDelete->pNext;
		}

		delete toDelete;
		--size;
	}

};

class Polinom {

private:

	struct monom {
		unsigned int degree;
		double coeff;
	};

	F_List<monom> list;

	void QuicSort(std::vector<double>& coeff, std::vector<unsigned int>& degree, int left, int right) {
		int i = left;
		int j = right;

		int mid = degree[(i + j) / 2];

		do {
			while (degree[i] < mid) {
				++i;
			}
			while (degree[j] > mid) {
				--j;
			}
			if (i <= j) {
				std::swap(degree[i], degree[j]);
				std::swap(coeff[i], coeff[j]);
				++i;
				--j;
			}
		} while (i < j);

		if (i < right) {
			QuicSort(coeff, degree, i, right);
		}
		if (j > left) {
			QuicSort(coeff, degree, left, j);
		}
	}

	void push_front_monom(const double& coeff, const unsigned int& degree) {
		monom mon;
		mon.coeff = coeff;
		mon.degree = degree;
		this->list.push_front(mon);
	}


	int count_digits(const unsigned int& value) {
		if (value == 0) {
			return 1;
		}
		int count = 0;
		int val = value;
		while (val != 0) {
			val /= 10;
			++count;
		}
		return count;

	}

public:
	Polinom(std::vector<double>& coeff, std::vector<unsigned int>& degree) {

		if (coeff.size() != degree.size()) {
			throw std::invalid_argument("The size of the coefficient vector and the size of the degree vector must match");
		}


		for (size_t i = 0; i < degree.size(); ++i) {
			int count = count_digits(degree[i]);
			if (count > SIZE_DEGREE) {
				throw std::invalid_argument("The number of digits in the exponent must be 3. Monomom number: " + std::to_string(i + 1));
			}
		}


		std::vector<double> co;
		std::vector<unsigned int> deg;

		for (size_t i = 0; i < degree.size(); ++i) {
			if (coeff[i] != 0.0) {
				co.push_back(coeff[i]);
				deg.push_back(degree[i]);
			}
		}

		if (deg.size() > 1) {
			this->QuicSort(co, deg, 0, deg.size() - 1);
		}

		for (size_t i = 0; i < deg.size(); ++i) { //приведение подобных слагаемых
			double new_coeff = co[i];
			while ((i + 1) < deg.size() && deg[i] == deg[i + 1]) {
				new_coeff += co[i + 1];
				++i;
			}
			this->push_front_monom(new_coeff, deg[i]);
		}

	};

	Polinom() : list() {};

	void add_monom(const double& coeff, const unsigned int& degree) {

		if (count_digits(degree) > SIZE_DEGREE) {
			throw std::invalid_argument("The number of digits in the exponent must be 3");
		}

		if (coeff != 0) {
			if (this->list.get_size() == 0) {
				monom mon;
				mon.coeff = coeff;
				mon.degree = degree;
				this->list.push_front(mon);
			}
			else {


				monom mon;
				mon.coeff = coeff;
				mon.degree = degree;

				bool flag_add = 0;

				F_List<monom>::Iterator it = this->list.begin();

				size_t index = 0;

				for (F_List<monom>::Iterator it = this->list.begin(); it != this->list.end(); ++it, ++index) {
					if (degree > (*it).degree) {
						list.insert(mon, index);
						flag_add = 1;
						break;
					}
					else if (degree == (*it).degree) {
						double new_coeff = (*it).coeff + coeff;
						if (new_coeff == 0) {
							list.remove(index);
						}
						else {
							(*it).coeff = new_coeff;
						}
						flag_add = 1;
						break;
					}
				}

				if (flag_add == 0) {
					this->list.push_back(mon);
				}
			}
		}
	}

	Polinom(const Polinom& other) {
		this->list = other.list;
	}

	Polinom& operator = (const Polinom& other) {
		this->list = other.list;
		return *this;
	}

	Polinom operator + (const Polinom& other) {
		Polinom result;

		Polinom temp;

		F_List<monom>::Iterator it1 = this->list.begin();
		F_List<monom>::Iterator it2 = other.list.begin();

		while (it1 != this->list.end() && it2 != other.list.end()) {
			unsigned int this_degr = (*it1).degree;
			unsigned int other_degr = (*it2).degree;

			double this_coeff = (*it1).coeff;
			double other_coeff = (*it2).coeff;
			if (this_degr == other_degr) {
				double sum_coeff = this_coeff + other_coeff;
				if (sum_coeff != 0) {

					temp.push_front_monom(sum_coeff, this_degr);
				}
				++it1;
				++it2;
			}
			else if (this_degr > other_degr) {

				temp.push_front_monom(this_coeff, this_degr);
				++it1;

			}
			else {
				temp.push_front_monom(other_coeff, other_degr);
				++it2;

			}

		}

		while (it1 != list.end()) {
			temp.push_front_monom((*it1).coeff, (*it1).degree);
			++it1;
		}

		while (it2 != other.list.end()) {
			temp.push_front_monom((*it2).coeff, (*it2).degree);
			++it2;
		}

		for (F_List<monom>::Iterator it = temp.list.begin(); it != temp.list.end(); ++it) {
			result.push_front_monom((*it).coeff, (*it).degree);
		}

		return result;
	}

	Polinom operator - (const Polinom& other) {
		Polinom polinom_other = other * (-1);
		return (*this) + polinom_other;
	}

	Polinom operator* (const double& value) const {

		if (this->list.get_size() == 0) {
			return *this;
		}

		Polinom pol;

		if (value != 0) {
			Polinom pol_temp;


			for (F_List<monom>::Iterator it = this->list.begin(); it != list.end(); ++it) {
				double new_coeff = (*it).coeff * value;
				unsigned int deg = (*it).degree;
				pol_temp.push_front_monom(new_coeff, deg);
			}

			for (F_List<monom>::Iterator it = pol_temp.list.begin(); it != pol_temp.list.end(); ++it) {
				double new_coeff = (*it).coeff;
				unsigned int deg = (*it).degree;
				pol.push_front_monom((*it).coeff, (*it).degree);
			}
		}
		return pol;
	}

	Polinom operator* (const Polinom& other) {


		if (this->list.get_size() != 0 && other.list.get_size() != 0) {
			std::vector<unsigned int> result_degr;
			std::vector<double> result_coeff;

			for (F_List<monom>::Iterator it1 = this->list.begin(); it1 != this->list.end(); ++it1) {
				unsigned int this_deg = (*it1).degree;
				double this_coeff = (*it1).coeff;

				for (F_List<monom>::Iterator it2 = other.list.begin(); it2 != other.list.end(); ++it2) {
					unsigned int other_deg = (*it2).degree;
					double other_coeff = (*it2).coeff;

					double new_coeff = this_coeff * other_coeff;

					unsigned int new_degree = this_deg + other_deg;
					if (count_digits(new_degree) > SIZE_DEGREE) {
						throw std::runtime_error("The degree of any variable in each monomial must not exceed 9");
					}

					bool found = 0;

					for (size_t n = 0; n < result_degr.size(); ++n) {
						if (result_degr[n] == new_degree) {
							new_coeff = new_coeff + result_coeff[n];
							result_coeff[n] = new_coeff;
							found = 1;
							break;
						}
					}

					if (!found) {
						result_coeff.push_back(new_coeff);
						result_degr.push_back(new_degree);
					}
				}
			}

			Polinom result(result_coeff, result_degr);
			return result;
		}
		else {
			Polinom pol;
			return pol;
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const Polinom& pol) {

		char variables[] = { 'x','y','z' };

		size_t i = 0;
		for (F_List<monom>::Iterator it = pol.list.begin(); it != pol.list.end(); ++it, ++i) {
			if (i != 0) {
				if ((*it).coeff > 0) {
					out << " + ";
				}
				else {
					out << " - ";
				}
			}
			else if (i == 0 && (*it).coeff < 0) {
				out << "-";
			}

			if ((*it).coeff != 1 && (*it).coeff != -1) {
				out << fabs((*it).coeff);
			}

			unsigned int deg = (*it).degree;

			if (deg != 0) {
				int count = 0;
				std::string degr = std::to_string(deg);
				while (degr.size() < SIZE_DEGREE) {
					degr = "0" + degr;
				}
				for (size_t j = 0; j < SIZE_DEGREE; ++j) {

					out << variables[j];
					out << degr[j];

				}
			}
		}

		return out;
	}

	friend std::istream& operator>>(std::istream& istr, Polinom& polinom) {

		std::vector<double>coeff;
		std::vector<unsigned int> deg;

		std::string str;
		std::getline(istr, str);

		std::stringstream inp(str);
		std::string token;
		int number_monom = 0;
		while (std::getline(inp, token, ',')) {
			size_t pos = token.find(':');
			if (pos == std::string::npos) {
				throw std::runtime_error("Incorrect input");
			}
			std::string co = token.substr(0, pos);
			std::string degr = token.substr(pos + 1);

			if (co.size() == 0) {
				throw std::runtime_error("Incorrect coefficient");
			}

			if (degr.size() == 0) {
				throw std::runtime_error("Incorrect degree");
			}

			int count_dot = 0;
			number_monom += 1;

			if (co.size() > 1 && co[0] == '0') { //первая цифра числа 0 (длина числа не ноль) 
				throw std::runtime_error("Incorrect coefficient. Monome number: " + std::to_string(number_monom));
			}

			if (co.size() == 1 && co[0] == '-') { //указан только знак - 
				throw std::runtime_error("Incorrect coefficient. Monome number: " + std::to_string(number_monom));
			}


			for (size_t i = 0; i < co.size(); ++i) {

				if (co[i] >= '0' && co[i] <= '9') {

				}
				else if (co[0] == '-') {

				}
				else if (co[i] == '.') {
					++count_dot;
					if (count_dot > 1) { //количество точек больше 1
						throw std::runtime_error("Incorrect coefficient. Monome number: " + std::to_string(number_monom));
					}
				}
				else if (co[i] == '-' && i != 0) { // - в некорректном месте
					throw std::runtime_error("Incorrect coefficient. Monome number: " + std::to_string(number_monom));
				}
				else { //некорректные символы (пробелы, латинский алфавит и т.д.)
					throw std::runtime_error("Incorrect coefficient. Monome number: " + std::to_string(number_monom));
				}
			}

			if (degr.size() > 1 && degr[0] == '0') {
				throw std::runtime_error("Incorrect degree. The degree cannot start from zero. Monome number: " + std::to_string(number_monom));
			}

			for (size_t i = 0; i < degr.size(); ++i) {
				if (!(degr[i] >= '0' && degr[i] <= '9')) {
					throw std::runtime_error("Incorrect degree. Monome number: " + std::to_string(number_monom));
				}
			}

			double co_res = std::stod(co);
			unsigned int degr_res = std::stoi(degr);

			coeff.push_back(co_res);
			deg.push_back(degr_res);

		}

		try {
			Polinom pol(coeff, deg);
			polinom = pol;
		}
		catch (...) {
			throw;
		}
		return istr;
	}

	size_t count_monom() const {
		return this->list.get_size();
	}

	bool operator == (const Polinom& other) const {

		if (this->count_monom() != other.count_monom()) {
			return false;
		}

		F_List<monom>::Iterator it1 = this->list.begin();
		F_List<monom>::Iterator it2 = other.list.begin();

		for (; it1 != this->list.end(); ++it1, ++it2) {
			if (((*it1).coeff != (*it2).coeff) || ((*it1).degree != (*it2).degree)) {
				return false;
			}
		}

		return true;
	}

	bool operator != (const Polinom& other) const {
		return !(*this == other);
	}

};

Polinom operator*(const double& value, const Polinom& polinom)
{
	Polinom pol = polinom * value;
	return pol;

}