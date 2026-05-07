#pragma once

#include <iostream>
#include <stack>
#include <fstream> //файловые потоки
#include <vector>
#include "Polinom.h"

struct Record { //для экспериментов
    std::string key;
    Polinom polinom;
    Record() {};
    Record(const std::string& k, const Polinom& v) {
        this->key = k;
        this->polinom = v;
    }
};

template <class Tvalue> //иначе объявление дружественных функций для эксперимента не сработает 
class AVLtable;


template <class Tvalue>
class HashTable;

template <class Tvalue>
class unordered_table {

    template<class Tvalue>
    friend class Logging_operations;

    friend std::vector< std::pair<size_t, double>> run_insert_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash); //для эксперимента   
    friend std::vector< std::pair<size_t, double>> run_delete_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash);
    friend std::vector< std::pair<size_t, double>> run_find_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash);

private:
    struct record {
        std::string key;
        Tvalue value;
    };
    std::vector<record> table{};
    size_t count_operations = 0;



protected:
    size_t get_count_operations() const {
        return count_operations;
    }

public:
    unordered_table() = default;


    size_t GetSize() const { 
        return table.size();
    }

    void Delete(const std::string& key) {

        count_operations = 0;

        count_operations += 1; //инициализация 
        for (size_t i = 0; i < table.size(); ++i) {
            count_operations += 2; // i<table.size() 

            if (table[i].key == key) {
                table[i] = table.back(); // заменяем на последнюю запись
                table.pop_back();        // удаляем последнюю запись
                count_operations += 7;  //if + две строчки выше (if т.к. после идет return)
                return;
            }
            count_operations += 3; //2 - if, 1 (++i)

        }

    }

    Tvalue* Find(const std::string& key) {

        count_operations = 0;

        count_operations += 1; //инициализация 
        for (size_t i = 0; i < table.size(); ++i) {

            count_operations += 2; // i<table.size()

            count_operations += 3; //[],.,==
            if (table[i].key == key) {

                count_operations += 3; //[],.,&

                return &table[i].value;
            }

            count_operations += 3;

        }


        return nullptr;
    }



    void Insert(const std::string& key, const Tvalue& value) {

        count_operations = 0;

        count_operations += 1;
        if (key == "") {
            throw std::runtime_error("The key cannot be an empty string");
        }

        count_operations += 1; //сравнение с nullptr 
        size_t temp_count = count_operations;

        if (this->Find(key)) { //Find + сравнение с nullptr
            return;
        }

        table.push_back({ key, value });
        count_operations += (1 + temp_count);


    }

    Tvalue Extract(const std::string& key) { //извлечь
        count_operations = 0;

        //Find + =
        Tvalue* ptr = this->Find(key);

        count_operations += 2; // =, ==
        if (ptr == nullptr) {

            //throw - не считаю

            throw std::runtime_error("Key not found in table");
        }

        Tvalue result = *ptr;

        count_operations += 2; // = , *

        size_t temp_count = count_operations;

        this->Delete(key);

        count_operations += temp_count; //Delete, предудыщий расчет 


        return result;

    }


};

template<class Tvalue>
class AVLtable {

    template<class Tvalue>
    friend class Logging_operations;

    friend std::vector< std::pair<size_t, double>> run_insert_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash); //для эксперимента   
    friend std::vector< std::pair<size_t, double>> run_delete_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash);
    friend std::vector< std::pair<size_t, double>> run_find_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash);
private:

    struct Node {
        std::string key;
        Tvalue data;
        Node* left;
        Node* right;
        int height; 

        Node(const std::string& k, const Tvalue& d) : key(k), data(d), left(nullptr), right(nullptr), height(0) {}
    };

    Node* root; //корень
    size_t size;
    size_t count_operations = 0;


    int getHeight(Node* node) {

        count_operations += 1;
        if (node == nullptr) {
            return -1;
        }
        else {
            count_operations += 1;
            return node->height;
        }
    }

    void NewHeight(Node* node) {

        count_operations += 1;

        if (node != nullptr) {


            int left_height = getHeight(node->left);

            int right_height = getHeight(node->right);

            count_operations += 4;

            node->height = 1 + std::max(left_height, right_height);
            count_operations += 4; // max, +, =, ->

        }
    }

    Node* rightRotate(Node* node) { //малое правое вращение
        Node* l1 = node->left;
        //правый потомок не используется

        
        Node* r2 = l1->right;  //правый потомок (левый не используется)

        node->left = r2;

        count_operations += 6; //=,->

        NewHeight(node);

        l1->right = node;
        count_operations += 2;

        NewHeight(l1);


        return l1;
    }

    Node* leftRotate(Node* node) { //малое левое вращение

        Node* r1 = node->right; //правый потомок (левый не используется)

        
        Node* l2 = r1->left; //левый потомок

        node->right = l2;

        count_operations += 6; //=,->

        NewHeight(node);

        r1->left = node;

        count_operations += 2;

        NewHeight(r1); 



        return r1;
    }

    void Balancing(std::stack<Node*>& path) { 

        while (!path.empty()) {
            Node* current = path.top();
            path.pop(); //когда будем обрабатывать корень, то стек будет уже пуст

            count_operations += 6; //empty(),!,top,=,pop,if

            int left_height = getHeight(current->left);
            int right_height = getHeight(current->right);
            current->height = 1 + std::max(left_height, right_height);

            count_operations += 7;

            int current_balance = right_height - left_height;

            Node* new_current_root = nullptr; 

            count_operations += 2;//первое сравнение, ->
            if (current_balance >= -1 && current_balance <= 1) { //если баланс [-1,1], то всё впорядке
                //второе сравнение, ->
                count_operations += 2;
                continue;
            }

            if (current_balance >= -1 && current_balance > 1) {
                count_operations += 1;
            }

            //иначе выполняем балансировку
            count_operations += 1; //if
            if (current_balance == -2) {

                Node* l = current->left;

                int b = getHeight(l->right) - getHeight(l->left);

                count_operations += 7; 
                if (b <= 0) {
                    new_current_root = rightRotate(current);
                    count_operations += 1;
                }
                else {
                    Node* leftR = leftRotate(l); 
                    current->left = leftR;
                    new_current_root = rightRotate(current);

                    count_operations += 4;
                }
            }
            else if (current_balance == 2) {
                Node* r = current->right;

                count_operations += 6;

                int b = getHeight(r->right) - getHeight(r->left); 

                if (b >= 0) {
                    new_current_root = leftRotate(current); 

                    count_operations += 1;
                }
                else {
                    Node* rightR = rightRotate(r);
                    current->right = rightR;
                    new_current_root = leftRotate(current);

                    count_operations += 4;
                }
            }


            count_operations += 2;

            if (!path.empty()) { //после балансировки подкючаем к родителю
                Node* parent = path.top();

                count_operations += 5;
                if (parent->key > new_current_root->key) {
                    parent->left = new_current_root;

                    count_operations += 2;
                }
                else {
                    parent->right = new_current_root;

                    count_operations += 2;
                }
            }
            else {
                root = new_current_root;

                count_operations += 1;
            }

        }
    }

    void print_node(std::ostream& out, Node* node, size_t& count) const {

        node;
        node->right;
        node->left;

        if (node != nullptr) {

            print_node(out, node->left, count);

            out << "(" << node->key << ":" << node->data << ")";
            if (count > 0) {
                out << ",";
                count--;
            }


            print_node(out, node->right, count);
        }
    }


protected:
    size_t get_count_operations() const {
        return this->count_operations;
    }

public:

    AVLtable() {
        this->size = 0;
        root = nullptr;
    }



    Tvalue* Find(const std::string& key) {

        count_operations = 0;

        if (root == nullptr) {
            count_operations += 1; // if +
            return nullptr;
        }

        Node* current = root;

        count_operations += 3; //if, = , current != nullptr;

        while (current != nullptr) {

            if (current->key == key) {
                count_operations += 3; //if, ->  
                return &current->data; //если нашли, то возвращаем данные
            }
            else if (current->key < key) {
                count_operations += (2 + 2 + 2);
                current = current->right;
            }
            else {
                count_operations += (2 + 2 + 2);
                current = current->left;
            }
            count_operations += 1; //current!=nullptr
        }

        
        return nullptr; //если не нашли
    }

    void Insert(const std::string& key, const Tvalue& data) {

        count_operations = 0;

        count_operations += 1;
        if (key.size() == 0) {
            throw std::runtime_error("The key cannot be an empty string");
        }

        count_operations += 1;
        if (root == nullptr) {
            root = new Node(key, data);
            ++size;

            count_operations += (6 + 1); //=, конструктор Node (если не учитывать количество операций для копирования полинома), 
            return;
        }

        std::stack<Node*> path;
        Node* current = root;

        count_operations += 2; //=, current!=nullptr
        while (current != nullptr) {

            path.push(current);

            count_operations += 3; //push,->,==
            if (current->key == key) {
                return;
            }

            current = current->key < key ? current->right : current->left;
            count_operations += (4 + 1); //строка выше, current != nullptr
        }

        /// вставка нового узла
        Node* parent = path.top();
        Node* newNode = new Node(key, data);

        count_operations += (1 + 1 + 5 + 2);
        if (parent->key <= key) {
            parent->right = newNode;
            count_operations += 2;
        }
        else {
            parent->left = newNode;
            count_operations += 2;
        }

        ++size;
        count_operations += 1;
        ///
        Balancing(path);

    }

    void Delete(const std::string& key) {

        count_operations = 0;

        count_operations += 1;
        if (root == nullptr) {
            return; //таблица пуста - выходим
        }

        std::stack<Node*> path;
        Node* current = root;

        count_operations += 2;
        while (current != nullptr) {

            path.push(current);

            count_operations += 3;
            if (current->key == key) {
                break;
            }

            current = current->key < key ? current->right : current->left;
            count_operations += (4 + 1);
        }

        count_operations += 1;
        if (current != nullptr) {

            Node* parent = nullptr; // у корня нет родителя

            count_operations += 2; //size, !=
            if (path.size() != 1) {
                path.pop();
                parent = path.top();
                path.push(current);

                count_operations += 4;
            }

            count_operations += 2;
            if (current->left == nullptr && current->right == nullptr) { //если вершина - лист

                count_operations += (2 + 1);
                if (parent == nullptr) {
                    delete root;
                    root = nullptr;

                    count_operations += 1;
                }
                else if (parent->left == current) {
                    delete parent->left;
                    parent->left = nullptr;

                    count_operations += (2 + 3);
                }
                else {
                    delete parent->right;
                    parent->right = nullptr;

                    count_operations += (2 + 1 + 2);
                }

                path.pop(); //удалили current (восстанавливаем баланс с родителя)

                count_operations += 1;
            }
            else if (current->left == nullptr || current->right == nullptr) { //есть один потомок у удаляемой вершины (данный потомок заменит саму вершину)

                count_operations += 2;
                if (current->left != nullptr) {
                    count_operations += 2; //проверяется второе условие
                }

                Node* child = (current->left == nullptr) ? current->right : current->left;

                count_operations += (4 + 1);
                if (parent == nullptr) {
                    root = child;
                    count_operations += 1;
                }
                else if (parent->left == current) {
                    parent->left = child;
                    count_operations += 4;
                    
                }
                else {
                    parent->right = child;
                    count_operations += (2 + 2); //else if, ->, =
                }

                
                path.pop();
                count_operations += 1;

                delete current;
            }
            else {
                Node* maxLeftParent = current;
                Node* maxLeft = current->left;

                count_operations += (3 + 2);
                while (maxLeft->right != nullptr) {
                    maxLeftParent = maxLeft;
                    maxLeft = maxLeft->right;

                    count_operations += (3 + 2);
                }

                count_operations += 1;
                if (maxLeftParent == current) { //максимальная вершина в левом поддереве - потомок удаляемой вершины
                    maxLeft->right = current->right;

                    count_operations += 4; //->, =, ->, ==
                    if (parent == nullptr) {
                        root = maxLeft;

                        count_operations += 1;
                    }
                    else if (parent->left == current) {
                        count_operations += 4;
                        parent->left = maxLeft;
                    }
                    else {
                        count_operations += (2 + 2);
                        parent->right = maxLeft;
                    }

                    //нужно удалить current, считать баланс с maxLeft
                    path.pop();
                    path.push(maxLeft);

                    count_operations += 2;
                }
                else {
                    maxLeftParent->right = maxLeft->left; //если у максимальной вершины левого поддерева есть левая ветвь
                   

                    maxLeft->right = current->right;
                    maxLeft->left = current->left;

                    count_operations += 10;

                    if (parent == nullptr) {

                        count_operations += 1;
                        root = maxLeft;
                    }
                    else if (parent->left == current) {
                        count_operations += 4;
                        parent->left = maxLeft;
                    }
                    else {
                        count_operations += 4;
                        parent->right = maxLeft;
                    }

                    //в стек добавим maxLeftParent и maxLeft;
                    path.pop();
                    path.push(maxLeft);
                    path.push(maxLeftParent); //баланс нужно пересчитать сначала для данной вершины
                    count_operations += 3;
                }

                delete current;

            }

            current = nullptr;
            count_operations += 1;
        }
        --size;

        count_operations += 1;
        Balancing(path);

    }

    Tvalue Extract(const std::string& key) {

        count_operations = 0;

        Tvalue* ptr = this->Find(key);

        count_operations += 2; //Find, =
        if (ptr == nullptr) {
            throw std::runtime_error("Key not found in table");
        }
        Tvalue result = *ptr;
        count_operations += 2; //*, =

        size_t temp_count = count_operations;


        this->Delete(key);

        count_operations += temp_count;

        return result;
    }


    size_t GetSize() const {
        return this->size;
    }

    ~AVLtable() {

        if (this->root != nullptr) { //если пользователь захочет вызвать дуструктор самостоятельно
            std::stack<Node*> stack1;
            std::stack<Node*> stack2;

            stack1.push(root);
            while (!stack1.empty()) {
                Node* current = stack1.top();
                stack1.pop();
                stack2.push(current);
                if (current->right != nullptr) {
                    stack1.push(current->right);
                }
                if (current->left != nullptr) {
                    stack1.push(current->left);
                }
            }

            while (!stack2.empty()) {
                Node* current = stack2.top();
                delete current;
                current = nullptr;
                this->size--;
                stack2.pop();
            }
            this->root = nullptr;
        }
    }


    friend std::ostream& operator<<(std::ostream& out, const AVLtable<Tvalue>& table) {

        size_t count = table.GetSize() - 1;
        table.print_node(out, table.root, count);

        return out;
    }

};


template <class Tvalue>
class HashTable {

    template<class Tvalue>
    friend class Logging_operations;

    friend std::vector< std::pair<size_t, double>> run_insert_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash); //для эксперимента  
    friend std::vector< std::pair<size_t, double>> run_delete_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash);
    friend std::vector< std::pair<size_t, double>> run_find_experiment(std::vector<Record>& data, unordered_table<Polinom>& un, AVLtable<Polinom>& avl, HashTable<Polinom>& hash);

private:
    struct record {
        Tvalue data;
        std::string key;
        int flag_deleted; //-1 запись не использовалась, 0 - запись удалена, 1 - запись не удалена

        record() :flag_deleted(-1) {};
    };

    std::vector<record> table;
    static const size_t default_capacity = 500;
    size_t capacity; //при resize будет обновляться
    size_t size;

    size_t count_operations = 0;

    size_t hash(const std::string& key) { //функция хэширования

        int res = 0;

        count_operations += 2;

        for (size_t i = 0; i < key.size(); ++i) {
            count_operations += (1 + 4); //приведение к int не считаю
            res = res * 17 + (int)key[i];
            count_operations += 1; // ++i
        }

        count_operations += 1;
        return res % capacity;
    }

    size_t repeat_hash(const size_t result_hash) {
        count_operations += 2; //+, %
        return (result_hash + 1) % capacity;
    }

    int find_ind(const std::string& key) {
        int h = hash(key);
        size_t i = 0;

        count_operations += 3; //this-> не считаю
        while (i < this->capacity) {

            count_operations += 3; //[], . , ==
            if (table[h].flag_deleted == -1) { //если позиция не использовалась, значит данных нет
                return -1;
            }

            count_operations += 3; //[], . , ==
            if (table[h].key == key && table[h].flag_deleted == 1) { //нужная ячейка используется
                count_operations += 3; // после &&
                return h;
            }

            //что делать, если ячейка была удалена, а пользователь захотел вставить данные с тем же ключом.
            h = repeat_hash(h);
            ++i;
            count_operations += 3; // =, ++i, <
        }

        return -1;
    }

protected:

    size_t get_count_operations() const {
        return this->count_operations;
    }

public:

    HashTable() {
        table.resize(default_capacity);
        size = 0;
        capacity = default_capacity;

    }

    void Resize() {

        std::vector<record> old_table = std::move(table);
        size_t old_capacity = capacity;


        capacity *= 2;
        table.resize(capacity);
        size = 0;


        for (size_t i = 0; i < old_capacity; ++i) {
            if (old_table[i].flag_deleted == 1) {

                size_t new_index = hash(old_table[i].key);

                while (table[new_index].flag_deleted == 1) {
                    new_index = repeat_hash(new_index);
                }

                table[new_index] = old_table[i];
                ++size;
            }
        }
    }

    Tvalue* Find(const std::string& key) {

        count_operations = 0;

        if (this->size == 0) {
            return nullptr;
        }

        int h = find_ind(key);

        count_operations += 3;
        if (h != -1) {
            return &table[h].data;

            count_operations += 3;
        }

        return nullptr;
    }

    void Insert(const std::string& key, const Tvalue& data) {

        count_operations = 0;

        if (this->size == capacity) { //все ячейки заполнены 
            return;
        }

        if (key.size() == 0) {
            throw std::runtime_error("Error. The key cannot be an empty string");
        }

        size_t h = hash(key);
        size_t i = 0; //счётчик шагов

        int first_find_del = -1;

        count_operations += (5 + 2);

        while (i < this->capacity) {

            count_operations += 3; //if до &&

            if (table[h].key == key && table[h].flag_deleted == 1) {

                count_operations += 3; //после && 

                return;
            }

            count_operations += 3;
            if (table[h].flag_deleted == 0 && first_find_del == -1) {
                first_find_del = h;

                count_operations += 2;

            }
            else if (table[h].flag_deleted == -1 && first_find_del == -1) { //если свободна и удаленная запись не была найдена (дубликатов быть не может) 

                count_operations += 3;

                if (table[h].flag_deleted == -1) {
                    count_operations += 1;
                }

                table[h].key = key;
                table[h].data = data;
                table[h].flag_deleted = 1;//ячейка занята


                ++size;

                count_operations += 10;

                break;
            }
            else if (table[h].flag_deleted == -1 && first_find_del > -1) {

                count_operations += 3; //предыдущий else if

                if (table[h].flag_deleted == -1) {
                    count_operations += (1 + 1); // == -1, > -1
                }

                count_operations += 3;

                table[first_find_del].key = key;
                table[first_find_del].data = data;
                table[first_find_del].flag_deleted = 1;//ячейка занята 

                ++size;
                count_operations += 10;
                break;
            }

            h = this->repeat_hash(h);
            ++i;
            count_operations += 3; // =, ++, <
        }

    }

    void Delete(const std::string& key) {
        count_operations = 0;
        if (this->size == 0) {
            return;
        }

        int h = find_ind(key);

        count_operations += 3;
        if (h == -1) {
            return;
        }
        else {
            table[h].flag_deleted = 0;
            --size;

            count_operations += 4;
        }
    }

    Tvalue Extract(const std::string& key) {

        count_operations = 0;
        if (this->size == 0) {
            throw std::runtime_error("Error. The table is empty");
        }

        int h = find_ind(key);

        if (h == -1) {
            throw std::runtime_error("Error. Key not found in table");
        }

        count_operations += 3;

        Tvalue res = table[h].data;
        table[h].flag_deleted = 0;
        --size;

        count_operations += 7;
        return res;

    }

    size_t GetSize() const {
        return size;
    }

    size_t GetCapacity() {
        return this->capacity;
    }

};


template<class Tvalue>
class Logging_operations {
private:

    //0 - unordered_table, 1 - AVLtable, 2 - HeshTable

    size_t del[3];
    size_t ins[3];
    size_t find[3];
    size_t extr[3];

    size_t un_table_all_operations[4];
    size_t avl_table_all_operations[4];
    size_t hash_table_all_operations[4];

    std::string name_file_without_dot_txt;


public:

    Logging_operations() {

        this->name_file_without_dot_txt = "Logging operation.txt";

        for (size_t i = 0; i < 4; ++i) {
            un_table_all_operations[i] = 0;
            avl_table_all_operations[i] = 0;
            hash_table_all_operations[i] = 0;
        }
    }

    Logging_operations(const std::string& name_file_without_dot_txt) {

        int size = name_file_without_dot_txt.size();
        if (size >= 4 && name_file_without_dot_txt.substr(size - 4) == ".txt") {
            throw std::runtime_error("Error. The file name cannot end with .txt");
        }

        this->name_file_without_dot_txt = name_file_without_dot_txt + ".txt";
        for (size_t i = 0; i < 4; ++i) {
            un_table_all_operations[i] = 0;
            avl_table_all_operations[i] = 0;
            hash_table_all_operations[i] = 0;
        }

    }

    void Delete_operations(const unordered_table<Tvalue>& t1, const AVLtable<Tvalue>& t2, const HashTable<Tvalue>& t3) {
        del[0] = t1.get_count_operations();
        del[1] = t2.get_count_operations();
        del[2] = t3.get_count_operations();

        this->un_table_all_operations[0] = (this->un_table_all_operations[0] + del[0]);
        this->avl_table_all_operations[0] = (this->avl_table_all_operations[0] + del[1]);
        this->hash_table_all_operations[0] = (this->hash_table_all_operations[0] + del[2]);
    }

    void Insert_operations(const unordered_table<Tvalue>& t1, const AVLtable<Tvalue>& t2, const HashTable<Tvalue>& t3) {
        ins[0] = t1.get_count_operations();
        ins[1] = t2.get_count_operations();
        ins[2] = t3.get_count_operations();

        this->un_table_all_operations[1] = (this->un_table_all_operations[1] + ins[0]);
        this->avl_table_all_operations[1] = (this->avl_table_all_operations[1] + ins[1]);
        this->hash_table_all_operations[1] = (this->hash_table_all_operations[1] + ins[2]);
    }

    void Find_operations(const unordered_table<Tvalue>& t1, const AVLtable<Tvalue>& t2, const HashTable<Tvalue>& t3) {
        find[0] = t1.get_count_operations();
        find[1] = t2.get_count_operations();
        find[2] = t3.get_count_operations();

        this->un_table_all_operations[2] = (this->un_table_all_operations[2] + find[0]);
        this->avl_table_all_operations[2] = (this->avl_table_all_operations[2] + find[1]);
        this->hash_table_all_operations[2] = (this->hash_table_all_operations[2] + find[2]);
    }

    void Extract_operations(const unordered_table<Tvalue>& t1, const AVLtable<Tvalue>& t2, const HashTable<Tvalue>& t3) {
        extr[0] = t1.get_count_operations();
        extr[1] = t2.get_count_operations();
        extr[2] = t3.get_count_operations();

        this->un_table_all_operations[3] = (this->un_table_all_operations[3] + extr[0]);
        this->avl_table_all_operations[3] = (this->avl_table_all_operations[3] + extr[1]);
        this->hash_table_all_operations[3] = (this->hash_table_all_operations[3] + extr[2]);
    }

    void Writing_to_file(size_t mode) { //0 - Delete, 1 - Insert, 2 - Find, 3 - Extract

        std::ofstream file;
        file.open(this->name_file_without_dot_txt, std::ofstream::app); // std:ofstream::app - новые записи не затирают старые
        if (!file.is_open()) {
            throw std::runtime_error("File opening error");
        }
        else {

            if (mode == 0) {
                file << "Name of the operation: Delete\n";
                file << "Unordered_table: " << del[0] << '\n';
                file << "AVLtable: " << del[1] << '\n';
                file << "HashTable: " << del[2] << '\n' << '\n';
            }
            else if (mode == 1) {
                file << "Name of the operation: Insert\n";
                file << "Unordered_table: " << ins[0] << '\n';
                file << "AVLtable: " << ins[1] << '\n';
                file << "HashTable: " << ins[2] << '\n' << '\n';
            }
            else if (mode == 2) {
                file << "Name of the operation: Find\n";
                file << "Unordered_table: " << find[0] << '\n';
                file << "AVLtable: " << find[1] << '\n';
                file << "HashTable: " << find[2] << '\n' << '\n';
            }
            else if (mode == 3) {
                file << "Name of the operation: Extract\n";
                file << "Unordered_table: " << extr[0] << '\n';
                file << "AVLtable: " << extr[1] << '\n';
                file << "HashTable: " << extr[2] << '\n' << '\n';
            }
        }
        file.close();
    }

    void Writing_end_work() {

        std::ofstream file;
        file.open(this->name_file_without_dot_txt, std::ofstream::app); // std:ofstream::app - новые записи не затирают старые 
        if (!file.is_open()) {
            throw std::runtime_error("File opening error");
        }
        else {
            file << "Total number of operations\n";
            file << "Unordered_table:\n";
            file << "Delete: " << this->un_table_all_operations[0] << '\n';
            file << "Insert: " << this->un_table_all_operations[1] << '\n';
            file << "Find: " << this->un_table_all_operations[2] << '\n';
            file << "Extract: " << this->un_table_all_operations[3] << '\n';
            size_t res = this->un_table_all_operations[0] + this->un_table_all_operations[1] + this->un_table_all_operations[2] + this->un_table_all_operations[3];
            file << "Total operations for this table: " << res << '\n' << '\n';

            file << "AVLtable:\n";
            file << "Delete: " << this->avl_table_all_operations[0] << '\n';
            file << "Insert: " << this->avl_table_all_operations[1] << '\n';
            file << "Find: " << this->avl_table_all_operations[2] << '\n';
            file << "Extract: " << this->avl_table_all_operations[3] << '\n';
            size_t res1 = this->avl_table_all_operations[0] + this->avl_table_all_operations[1] + this->avl_table_all_operations[2] + this->avl_table_all_operations[3];
            file << "Total operations for this table: " << res1 << '\n' << '\n';

            file << "HashTable:\n";
            file << "Delete: " << this->hash_table_all_operations[0] << '\n';
            file << "Insert: " << this->hash_table_all_operations[1] << '\n';
            file << "Find: " << this->hash_table_all_operations[2] << '\n';
            file << "Extract: " << this->hash_table_all_operations[3] << '\n';
            size_t res2 = this->hash_table_all_operations[0] + this->hash_table_all_operations[1] + this->hash_table_all_operations[2] + this->hash_table_all_operations[3];
            file << "Total operations for this table: " << res2 << '\n' << '\n';

        }
        file.close();
    }

    void Clear() {

        std::ofstream file;
        file.open(this->name_file_without_dot_txt);
        if (!file.is_open()) {
            throw std::runtime_error("File opening error");
        }
        file.close();
    }

};


