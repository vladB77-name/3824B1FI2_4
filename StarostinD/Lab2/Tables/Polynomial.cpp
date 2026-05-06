#include "Polynomial.h"

static const double EPS = 0.000'001;

Polynomial::Monomial::Monomial() : Monomial(0.) {}

Polynomial::Monomial::Monomial(const double& src) : coef(src), degs(0) {}

Polynomial::Monomial::Monomial(std::string src) : coef(1.), degs(0) {
	if (src.size() < 3) {
		throw std::length_error("The length of the monomial must be >= 3");
	}
	const int SZ = src.length();
	int i = 0;
	switch (SZ) {
		default:
			for (; i < SZ - 3; ++i) {
				if (!isdigit(src[i]) && src[i] != '-' && src[i] != '+' && src[i] != '.') {
					throw std::invalid_argument("The coefficient must be a real number");
				}
			}
		case 3:
			for (; i < SZ; ++i) {
				if (!isdigit(src[i])) {
					throw std::invalid_argument("Degrees must be numbers from 0 to 9");
				}
			}
	}
	i = SZ - 1;
	if (SZ > 3) {
		size_t shift = isdigit(src[0]) ? 0 : 1;
		if (SZ - 3 - shift != 0) {
			coef = std::stod(src.substr(shift, SZ - 3 - shift));
		}
		if (shift && src[0] == '-') {
			coef *= -1;
		}
	}
	for (int t = 1; i >= SZ - 3; --i) {
		degs += (src[i] - '0') * t;
		t *= 10;
	}
}

Polynomial::Monomial::Monomial(const Monomial& other) : coef(other.coef), degs(other.degs) {}

Polynomial::Monomial Polynomial::Monomial::operator-() const {
	Monomial res(*this);
	res.coef = -res.coef;
	return res;
}

Polynomial::Monomial Polynomial::Monomial::operator+(const Monomial& other) const {
	Monomial res(*this);
	res += other;
	return res;
}

Polynomial::Monomial Polynomial::Monomial::operator-(const Monomial& other) const {
	Monomial res(*this);
	res -= other;
	return res;
}


Polynomial::Monomial Polynomial::Monomial::operator*(const double& scalar) const {
	Monomial res(*this);
	res.coef *= scalar;
	return res;
}

Polynomial::Monomial Polynomial::Monomial::operator*(const Monomial& other) const {
	Monomial res(*this);
	res *= other;
	return res;
}

Polynomial::Monomial& Polynomial::Monomial::operator*=(const Monomial& other) {
	coef *= other.coef;
	for (int i = 0, other_degs = other.degs, curr_var = 1; i < 3; ++i) {
		int other_pow = other_degs % 10;
		if ((degs / curr_var) % 10 + other_pow > 9) {
			throw std::overflow_error("when multiplying, the power of one of the variables turned out to be greater than 9");
		}
		degs += other_pow * curr_var;
		curr_var *= 10;
		other_degs /= 10;

	}
	return *this;
}

Polynomial::Monomial& Polynomial::Monomial::operator+=(const Monomial& other) {
	coef += other.coef;
	return *this;
}

Polynomial::Monomial& Polynomial::Monomial::operator-=(const Monomial& other) {
	coef -= other.coef;
	return *this;
}


bool Polynomial::Monomial::operator<(const Monomial& other) const {
	if (degs == other.degs) {
		return false;
	}
	return degs < other.degs;
}

bool Polynomial::Monomial::operator>(const Monomial& other) const {
	if (degs == other.degs) {
		return false;
	}
	return degs > other.degs;
}

bool Polynomial::Monomial::operator==(const Monomial& other) const {
	return degs == other.degs;
}

bool Polynomial::Monomial::operator!=(const Monomial& other) const {
	return !operator==(other);
}

Polynomial::Node::Node(const Monomial& src, Node* next) : data(src), next(next) {}

//Polynomial

Polynomial::iterator Polynomial::find(const Monomial& monomial) {
	auto prev = before_begin();
	auto curr = begin();
	for (; curr != end() && *curr < monomial; ++prev, ++curr);
	return prev;
}

void Polynomial::insert(const Monomial& src) {
	Node* before_src = find(src).ptr;
	insert_after(before_src, src);
}

void Polynomial::insert_after(const iterator& pos, const Monomial& src) {
	Node* ptr = pos.ptr;
	if (ptr->next == nullptr) {
		push_back(src);
	}
	else if (ptr->next->data == src) {
		ptr->next->data = ptr->next->data + src;
		if (std::abs(ptr->next->data.coef) < EPS) {
			Node* to_delete = ptr->next;
			ptr->next = to_delete->next;
			if (tail == to_delete) {
				tail = ptr;
			}
			delete to_delete;
		}

	}
	else {
		Node* newNode = new Node(src, ptr->next);
		ptr->next = newNode;
	}
}

Polynomial::Polynomial() : head(new Node()), tail(head) {}

Polynomial::Polynomial(const Polynomial& other) : head(new Node()), tail(head) {
	Node** curr = &head->next;
	Node* other_curr = other.head->next;
	
	while (other_curr) {
		*curr = new Node(other_curr->data);
		tail = *curr;
		curr = &((*curr)->next);
		other_curr = other_curr->next;
	}
}

Polynomial::Polynomial(const std::string& src) : head(new Node()), tail(head) {
	std::istringstream in(src);
	std::string monomial;
	while (in >> monomial) {
		if (monomial.length() == 1 && monomial[0] == '-') {
			in >> monomial;
			Monomial tmp(monomial);
			tmp.coef = -tmp.coef;
			if (std::abs(tmp.coef) >= EPS) {
				insert(tmp);
			}
		}
		else if (monomial.length() >= 3) {
			Monomial tmp(monomial);
			if (std::abs(tmp.coef) >= EPS) {
				insert(tmp);
			}

		}
		else if (monomial.length() < 3 && monomial[0] != '+') {
			throw std::length_error("The length of the monomial must be >= 3");
		}
	}
}

Polynomial::Polynomial(const double& src) : head(new Node()), tail(head) {
	if (std::abs(src) >= EPS)
		push_back(Monomial(src));
}

void Polynomial::push_back(const Monomial& src) {
	Node* newNode = new Node(src);
	tail->next = newNode;
	tail = newNode;
}

void Polynomial::swap(Polynomial& other) {
	std::swap(head, other.head);
	std::swap(tail, other.tail);
}

void Polynomial::clear() {
	Node* curr = head->next;
	while (curr) {
		Node* tmp = curr;
		curr = curr->next;
		delete tmp;
	}
	head->next = nullptr;
	tail = head;
}

std::string Polynomial::get(bool format) const {
	std::string P = "";
	if(format){
		for (const auto& e : *this) {
			int deg = e.degs;
			if (!P.empty() && e.coef < 0) {
				P.pop_back();
				P.pop_back();
				P += "- ";
			}
			P += std::to_string(std::abs(e.coef));
			if (deg / 100) 
				P += "*x^" + std::to_string(deg / 100);

			if ((deg / 10) % 10)
				P += "y^" + std::to_string((deg / 10) % 10);

			if (deg % 10)
				P += "z^" + std::to_string(deg % 10);

			P += " + ";
		}
		if (head != tail && begin()->coef < 0) {
			P = "-" + P;
		}
	}
	else {
		for (const auto& e : *this) {
			if (!P.empty() && e.coef < 0) {
				P.pop_back();
				P.pop_back();
				P += "- ";
			}
			P += std::to_string(std::abs(e.coef));
			if (e.degs != 0) {
				P += "*" + std::to_string(e.degs);
			}
			P += " + ";
		}
		if (head != tail && begin()->coef < 0) {
			P = "-" + P;
		}
	}
	if (!P.empty()) {
		P.pop_back();
		P.pop_back();
		P.pop_back();
	}
	else {
		P = "0";
	}
	return P;
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
	if (this != &other) {
		Polynomial tmp(other);
		swap(tmp);
	}
	return *this;
}

Polynomial Polynomial::operator-() const {
	Polynomial res(*this);
	for (auto& elem : res) {
		elem = -elem;
	}
	return res;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
	Polynomial res(*this);
	res += other;
	return res;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
	Polynomial res(*this);
	res -= other;
	return res;
}

Polynomial Polynomial::operator*(const double& scalar) const {
	if (std::abs(scalar) < EPS) {
		return Polynomial();
	}
	Polynomial res(*this);
	for (auto& elem : res) {
		elem.coef *= scalar;
	}
	return res;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
	Polynomial res;

	for (const auto& first : *this) {
		Polynomial tmp;
		for (const auto& second: other) {
			tmp.push_back(first * second);
		}
		res += tmp;
	}
	return res;
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {
	Polynomial tmp = *this * other;
	swap(tmp);
	return *this;
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
	auto prev = before_begin();
	auto it_l = begin();
	auto it_r = other.begin();
	while (it_l != end() && it_r != other.end()) {
		if (*it_l < *it_r) {
			++it_l;
			++prev;
		}
		else if (*it_l > *it_r) {
			insert_after(prev, *it_r);
			++it_r;
			++prev;
		}
		else {
			if (std::abs(it_l->coef + it_r->coef) > EPS) {
				*it_l += *it_r;
				++it_l;
				++prev;
			}
			else {
				auto t = it_l;
				prev.ptr->next = it_l.ptr->next;
				it_l = it_l.ptr->next;
				delete t.ptr;
				if (it_l == end()) {
					tail = prev.ptr;
				}
			}
			++it_r;
		}
	}
	while (it_r != other.end()) {
		push_back(*it_r);
		++it_r;
	}
	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
	auto prev = before_begin();
	auto it_l = begin();
	auto it_r = other.begin();
	while (it_l != end() && it_r != other.end()) {
		if (*it_l < *it_r) {
			++it_l;
			++prev;
		}
		else if (*it_l > *it_r) {
			insert_after(prev, -*it_r);
			++it_r;
			++prev;
		}
		else {
			if (std::abs(it_l->coef - it_r->coef) > EPS) {
				*it_l -= *it_r;
				++it_l;
				++prev;
			}
			else {
				auto t = it_l;
				prev.ptr->next = it_l.ptr->next;
				it_l = it_l.ptr->next;
				delete t.ptr;
				if (it_l == end()) {
					tail = prev.ptr;
				}
			}
			++it_r;
		}
	}
	while (it_r != other.end()) {
		push_back(-*it_r);
		++it_r;
	}
	return *this;
}

bool Polynomial::operator==(const Polynomial& other) const {
	return get() == other.get();
}

Polynomial::~Polynomial() { 
	clear(); 
	delete head; //удаление фиктивного узла
}

//iterator

Polynomial::iterator::iterator(Node* src) : ptr(src) {}

Polynomial::Monomial& Polynomial::iterator::operator*() const { return ptr->data; }

Polynomial::Monomial* Polynomial::iterator::operator->() const { return &ptr->data; }

Polynomial::iterator& Polynomial::iterator::operator++() { 
	ptr = ptr->next; 
	return *this;
}

bool Polynomial::iterator::operator==(const iterator& other) const { return ptr == other.ptr; }

bool Polynomial::iterator::operator!=(const iterator& other) const { return ptr != other.ptr; }

Polynomial::iterator Polynomial::before_begin() { return iterator(head); }

Polynomial::iterator Polynomial::begin() { return iterator(head->next); }

Polynomial::iterator Polynomial::end() { return iterator(nullptr); }

//const_iterator

Polynomial::const_iterator::const_iterator(const Node* src) : ptr(src) {}

const Polynomial::Monomial& Polynomial::const_iterator::operator*() const { return ptr->data; }

const Polynomial::Monomial* Polynomial::const_iterator::operator->() const { return &ptr->data; }


Polynomial::const_iterator& Polynomial::const_iterator::operator++() {
	ptr = ptr->next;
	return *this;
}

bool Polynomial::const_iterator::operator==(const const_iterator& other) const { return ptr == other.ptr; }

bool Polynomial::const_iterator::operator!=(const const_iterator& other) const { return ptr != other.ptr; }

Polynomial::const_iterator Polynomial::before_begin() const { return const_iterator(head); }

Polynomial::const_iterator Polynomial::begin() const { return const_iterator(head->next); }

Polynomial::const_iterator Polynomial::end() const { return const_iterator(nullptr); }

std::ostream& operator<<(std::ostream& out, const Polynomial& obj) {
	out << obj.get(1);
	return out;
}

std::istream& operator>>(std::istream& in, Polynomial& obj) {
	std::string s;
	getline(in, s);
	Polynomial tmp(s);
	obj.swap(tmp);
	return in;
}