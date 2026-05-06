#pragma once
#include <cctype>
#include <stdexcept>
#include <string>
#include <algorithm> //std::swap
#include <sstream>

class Polynomial {
public:
	class const_iterator;

private:
	class iterator;
	struct Monomial;
	struct Node;

	struct Monomial {
		double coef;
		int degs;
		
		Monomial();

		Monomial(const double& src);
		
		Monomial(std::string src);
		
		Monomial(const Monomial& other);
				
		Monomial operator-() const;
		
		Monomial operator+(const Monomial& other) const;

		Monomial operator-(const Monomial& other) const;

		Monomial operator*(const double& scalar) const;

		Monomial operator*(const Monomial& other) const;
		
		Monomial& operator*=(const Monomial& other);

		Monomial& operator+=(const Monomial& other);

		Monomial& operator-=(const Monomial& other);

		bool operator<(const Monomial& other) const;

		bool operator>(const Monomial& other) const;

		bool operator==(const Monomial& other) const;
		
		bool operator!=(const Monomial& other) const;
	};

	struct Node {
		Polynomial::Monomial data;
		Node* next;
		Node(const Monomial& src=Monomial(), Node* next = nullptr);
	};

	Node* head; //фиктивный head
	Node* tail;

	void insert(const Monomial& src);

	void insert_after(const iterator& pos, const Monomial& src);

	void push_back(const Monomial& src);

	class iterator {
	private:
		friend Polynomial;
		Node* ptr;
	public:
		iterator(Node* src=nullptr);
		Monomial& operator*() const;
		Monomial* operator->() const;
		iterator& operator++();
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
	};

	iterator find(const Monomial& monomial);

	iterator before_begin();

	iterator begin();

	iterator end();

public:
	Polynomial();
	Polynomial(const Polynomial& other);
	Polynomial(const std::string& src);
	Polynomial(const double& src);

	class const_iterator {
	private:
		friend Polynomial;
		const Node* ptr;
	public:
		const_iterator(const Node* src=nullptr);
		const Monomial& operator*() const;
		const Monomial* operator->() const;
		const_iterator& operator++();
		bool operator==(const const_iterator& other) const;
		bool operator!=(const const_iterator& other) const;
	};
	
	const_iterator before_begin() const;

	const_iterator begin() const;

	const_iterator end() const;

	void swap(Polynomial& other);

	void clear();

	//format: 0 - set of degrees, 1 - mathematical form 
	std::string get(bool format=0) const;

	Polynomial& operator=(const Polynomial& other);

	Polynomial operator-() const;

	Polynomial operator+(const Polynomial& other) const;

	Polynomial operator-(const Polynomial& other) const;

	Polynomial operator*(const double& scalar) const;

	Polynomial operator*(const Polynomial& other) const;

	Polynomial& operator*=(const Polynomial& other);

	Polynomial& operator+=(const Polynomial& other);

	Polynomial& operator-=(const Polynomial& other);

	bool operator==(const Polynomial& other) const;

	friend std::ostream& operator<<(std::ostream& out, const Polynomial& obj);
	friend std::istream& operator>>(std::istream& in, Polynomial& obj);

	~Polynomial();
};

