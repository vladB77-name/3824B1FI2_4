#pragma once
#include "Tables.h"

template<typename T>
class rb_tree : public Table<T> {
private:
	struct Node {
		std::string key;
		bool color; // 0 - black, 1 - red;
		T data;
		Node* left, * right, * parent;

		Node(const std::string& key, const T& value) : key(key), color(1), data(value), left(NIL), right(NIL), parent(NIL) {}
	};

	static Node* createNIL() {
		Node* newNIL = new Node(std::string{}, T{});
		newNIL->color = false;
		newNIL->left = newNIL->right = newNIL->parent = newNIL;
		return newNIL;
	}

	static Node* NIL;

	Node* root;

	Node* find(Node* x, const std::string& key) {
		this->findOperationsCnt += 1;
		while (x != NIL) {
			this->findOperationsCnt += 2;

			if (x->key == key) {
				return x;
			}

			if (x->key > key) {
				x = x->left;
			}
			else {
				x = x->right;
			}
			this->findOperationsCnt += 4;
		}

		return NIL;
	}

	Node* findParent(Node* x, const std::string& key) {
		Node* parent = NIL;
		Node* current = x;

		this->findOperationsCnt += 3;

		while (current != NIL) {
			parent = current;
			this->findOperationsCnt += 3;
			if (key == current->key) {
				return current;
			}
			else if (key < current->key) {
				current = current->left;
			}
			else {
				current = current->right;
			}
			this->findOperationsCnt += 5;
		}

		return parent;
	}

	// x->right != NIL
	void leftRotate(Node* x, int& cnt) {
		Node* y = x->right;

		x->right = y->left;
		cnt += 7;
		if (y->left != NIL) {
			y->left->parent = x;
			cnt += 3;
		}

		y->parent = x->parent;

		cnt += 5;
		if (x->parent == NIL) {
			root = y;
			cnt += 1;
		}
		else if (x->parent->left == x) {
			x->parent->left = y;
			cnt += 6;
		}
		else {
			x->parent->right = y;
			cnt += 6;
		}
		y->left = x;

		x->parent = y;

		cnt += 4;
	}

	void rightRotate(Node* x, int& cnt) {
		Node* y = x->left;

		x->left = y->right;
		cnt += 7;
		if (y->right != NIL) {
			y->right->parent = x;
			cnt += 3;
		}

		y->parent = x->parent;

		cnt += 5;
		if (x->parent == NIL) {
			root = y;
			cnt += 1;
		}
		else if (x->parent->right == x) {
			x->parent->right = y;
			cnt += 6;
		}
		else {
			x->parent->left = y;
			cnt += 6;
		}
		y->right = x;

		x->parent = y;

		cnt += 2;
	}

	void insertFixup(Node* x) {
		int t = 0;
		this->insertOperationsCnt += 3;
		while (x->parent->color == 1) {
			this->insertOperationsCnt += 5;
			if (x->parent == x->parent->parent->left) {
				Node* uncle = x->parent->parent->right;

				this->insertOperationsCnt += 6;

				if (uncle->color == 1) { //case 1
					x->parent->color = 0;
					uncle->color = 0;
					x->parent->parent->color = 1;
					x = x->parent->parent;

					this->insertOperationsCnt += 12;
				}
				else {
					this->insertOperationsCnt += 3;
					if (x == x->parent->right) { // case 2
						x = x->parent;
						leftRotate(x, t);
						this->insertOperationsCnt += 3;
					}
					x->parent->color = 0;		// case 3
					x->parent->parent->color = 1;
					rightRotate(x->parent->parent, t);
					
					this->insertOperationsCnt += 10;
				}
			}
			else {
				Node* uncle = x->parent->parent->left;

				this->insertOperationsCnt += 6;
				if (uncle->color == 1) { //case 1
					x->parent->color = 0;
					uncle->color = 0;
					x->parent->parent->color = 1;
					x = x->parent->parent;

					this->insertOperationsCnt += 12;
				}
				else {
					this->insertOperationsCnt += 3;
					if (x == x->parent->left) { // case 2
						x = x->parent;
						rightRotate(x, t);
						this->insertOperationsCnt += 3;
					}

					x->parent->color = 0;
					x->parent->parent->color = 1;
					leftRotate(x->parent->parent, t);

					this->insertOperationsCnt += 10;
				}
			}
		}
		root->color = 0;
		this->insertOperationsCnt += 2 + t;
	}

	void deleteFixup(Node* x) {
		int t = 0;
		this->eraseOperationsCnt += 4;
		while (x != root && x->color == 0) {
			this->eraseOperationsCnt += 3;
			if (x == x->parent->left) {
				Node* w = x->parent->right;

				this->eraseOperationsCnt += 5;
				if (w->color == 1) {  // case 1
					w->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent, t);
					w = x->parent->right;
					this->eraseOperationsCnt += 11;
				}

				this->eraseOperationsCnt += 7;
				if (w->left->color == 0 && w->right->color == 0) {  // case 2
					w->color = 1;
					x = x->parent;
					this->eraseOperationsCnt += 4;
				}
				else {
					this->eraseOperationsCnt += 3;
					if (w->right->color == 0) {  // case 3
						w->left->color = 0;
						w->color = 1;
						rightRotate(w, t);
						w = x->parent->right;
						this->eraseOperationsCnt += 10;
					}
					w->color = x->parent->color; // case 4
					x->parent->color = 0;
					w->right->color = 0;
					leftRotate(x->parent, t);
					x = root;
					this->eraseOperationsCnt += 14;
				}
			}
			else {
				Node* w = x->parent->left;

				this->eraseOperationsCnt += 5;
				if (w->color == 1) {
					w->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent, t);
					w = x->parent->left;
					this->eraseOperationsCnt += 11;
				}

				this->eraseOperationsCnt += 7;
				if (w->right->color == 0 && w->left->color == 0) {
					w->color = 1;
					x = x->parent;
					this->eraseOperationsCnt += 4;
				}
				else {
					this->eraseOperationsCnt += 3;
					if (w->left->color == 0) {
						w->right->color = 0;
						w->color = 1;
						leftRotate(w, t);
						w = x->parent->left;
						this->eraseOperationsCnt += 10;
					}
					w->color = x->parent->color;
					x->parent->color = 0;
					w->left->color = 0;
					rightRotate(x->parent, t);
					x = root;
					this->eraseOperationsCnt += 14;
				}
			}
		}
		x->color = 0;
		this->eraseOperationsCnt += 1 + t;
	}

	void transplant(Node* u, Node* v, size_t& cnt) {
		cnt += 2;
		if (u->parent == NIL) {
			root = v;
			cnt += 1;
		}
		else if (u->parent->left == u) {
			u->parent->left = v;
			cnt += 6;
		}
		else {
			cnt += 6;
			u->parent->right = v;
		}
		v->parent = u->parent;
		cnt += 3;
	}

	Node* successor(Node* x, size_t& cnt) {
		// deg(x) = 2;
		x = x->right;
		cnt += 4;
		while (x->left != NIL) {
			x = x->left;
			cnt += 4;
		}
		return x;
	}

	bool checkNode(Node* node, int blackCount, int& expectedBlackHeight) {
		if (node == NIL) {
			if (expectedBlackHeight == -1) {
				expectedBlackHeight = blackCount;
				return true;
			}
			return blackCount == expectedBlackHeight;
		}

		if (node->color == 0) {
			++blackCount;
		}

		if (node->color == 1) {
			if (node->left->color == 1 || node->right->color == 1) {
				return false;
			}
		}

		return checkNode(node->left, blackCount, expectedBlackHeight) && 
			   checkNode(node->right, blackCount, expectedBlackHeight);
	}

	void clear(Node* x) {
		if (x == NIL) return;
		clear(x->left);
		clear(x->right);
		delete x;
	}

public:
	rb_tree(std::ofstream& file) : Table<T>(file) {
		if (NIL == nullptr) {
			NIL = createNIL();
		}
		root = NIL;
		this->tableType = "Red-Black Tree";
	}

	T* find(const std::string& key) override {
		size_t t = this->findOperationsCnt;
		Node* search_res = find(root, key);
		this->findOperationsCnt += 4;
		if (search_res == NIL) {
			this->file << this->tableType << " find: " << this->findOperationsCnt - t << '\n';
			return nullptr;
		}
		this->findOperationsCnt += 2;
		this->file << this->tableType<< " find: " << this->findOperationsCnt - t << '\n';
		return &search_res->data;
	}

	void insert(const std::string& key, const T& data) override {
		size_t t = this->insertOperationsCnt;
		
		Node* newNode;

		size_t f = this->findOperationsCnt;
		Node* parent = findParent(root, key);
		this->file << this->tableType << " find: " << this->findOperationsCnt - f << '\n';
		t -= this->findOperationsCnt - f;

		this->insertOperationsCnt += 4;
		if (parent == NIL) {
			newNode = new Node(key, data);
			root = newNode;
			newNode->parent = NIL;
			this->insertOperationsCnt += 6;
		}
		else {
			this->insertOperationsCnt += 2;
			if (key == parent->key) {
				parent->data = data;
				this->file << this->tableType << " insert: " << this->insertOperationsCnt - t << '\n';
				return;
			}

			newNode = new Node(key, data);
			newNode->parent = parent;
			this->insertOperationsCnt += 7;
			if (key < parent->key) {
				this->insertOperationsCnt += 2;
				parent->left = newNode;
			}
			else if (key > parent->key) {
				this->insertOperationsCnt += 4;
				parent->right = newNode;
			}
		}
		this->insertOperationsCnt += 1;
		insertFixup(newNode);

		this->file << this->tableType << " insert: " << this->insertOperationsCnt - t << '\n';
	}

	void erase(const std::string& key) override {
		size_t t = this->eraseOperationsCnt;

		size_t f = this->findOperationsCnt;
		Node* x = find(root, key); // удаляемый узел
		this->file << this->tableType << " find: " << this->findOperationsCnt - f << '\n';

		t -= this->findOperationsCnt - f;
		this->eraseOperationsCnt += 4;
		f = 0;

		if (x == NIL) {
			this->file << this->tableType << " erase: " << this->eraseOperationsCnt - t << '\n';
			return;
		}

		Node* y = x;			   // узел для отслеживания красно-чёрный свойств дерева
		Node* z;				   // узел-замена y
		bool ySrcColor = y->color;

		this->eraseOperationsCnt += 5;

		if (x->left == NIL) {
			z = x->right;
			transplant(x, z, f);
			this->eraseOperationsCnt += 5;
		}
		else if (x->right == NIL) {
			z = x->left;
			transplant(x, z, f);

			this->eraseOperationsCnt += 7;
		}
		else {
			y = successor(x, f);
			ySrcColor = y->color;
			z = y->right;

			this->eraseOperationsCnt += 10;

			if (y->parent == x) {
				z->parent = y;
				this->eraseOperationsCnt += 2;
			}
			else {
				transplant(y, y->right, f);
				y->right = x->right;
				y->right->parent = y;

				this->eraseOperationsCnt += 10;
			}
			transplant(x, y, f);
			y->left = x->left;
			y->left->parent = y;
			y->color = x->color;

			this->eraseOperationsCnt += 12;
		}


		this->eraseOperationsCnt += 1;
		if (ySrcColor == 0) {
			this->eraseOperationsCnt += 1;
			deleteFixup(z);
		}
		delete x;

		this->eraseOperationsCnt += f;
		this->file << this->tableType << " erase: " << this->eraseOperationsCnt - t  << '\n';
	}

	bool isValidRBTree() {
		if (root == NIL) return true;

		if (root->color != 0) return false;

		int blackHeight = -1;
		return checkNode(root, 0, blackHeight);
	}

	~rb_tree() {
		clear(root);
	}

};

template<typename T>
typename rb_tree<T>::Node* rb_tree<T>::NIL;