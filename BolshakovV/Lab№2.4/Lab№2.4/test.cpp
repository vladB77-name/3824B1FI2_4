#include "pch.h"
#include "../Project1/Tables.h"
#include "../Project1/Experiments.h"

TEST(test_unordered_table, test_insert_find_delete) {

	unordered_table<Polinom> table;

	///////
	Polinom pol1;
	pol1.add_monom(2, 123);
	pol1.add_monom(3, 234);

	Polinom pol2;
	pol2.add_monom(1, 1);
	pol2.add_monom(4, 345);
	//////

	table.Insert("101", pol1);
	table.Insert("2", pol2);


	EXPECT_EQ(*table.Find("101"), pol1);
	EXPECT_EQ(table.Find("104"), nullptr);


	EXPECT_EQ(table.GetSize(), 2);

	//////
	Polinom pol3;
	pol3.add_monom(12, 001);

	table.Insert("101", pol3);
	EXPECT_EQ(*table.Find("101"), pol1);
	EXPECT_EQ(table.GetSize(), 2);

	/////
	table.Delete("101");
	EXPECT_EQ(table.GetSize(), 1);
	EXPECT_EQ(table.Find("101"), nullptr);


	table.Insert("101", pol1);
	Polinom Pol = table.Extract("101");
	EXPECT_EQ(Pol, pol1);
	EXPECT_EQ(table.GetSize(), 1);
	EXPECT_EQ(table.Find("101"), nullptr);

	EXPECT_THROW({ Polinom pol = table.Extract("103"); }, std::runtime_error);

}


TEST(AVLtable, test_insert_find) {
	AVLtable<Polinom> table;

	Polinom pol1;
	pol1.add_monom(3, 111);

	Polinom pol2;
	pol2.add_monom(4, 778);

	Polinom pol3;
	pol3.add_monom(5, 456);

	table.Insert("1", pol1);
	table.Insert("2", pol2);
	table.Insert("3", pol3);

	EXPECT_EQ(*table.Find("2"), pol2);
	EXPECT_EQ(*table.Find("3"), pol3);

	pol3.add_monom(1, 101);
	EXPECT_TRUE(*table.Find("3") != pol3);

	EXPECT_EQ(table.Find("4"), nullptr);

	//////
	Polinom pol4;
	pol4.add_monom(7, 988);
	table.Insert("1", pol4);

	EXPECT_EQ(*table.Find("1"), pol1);
	/////

	Polinom* change_polinom1 = table.Find("1");
	*change_polinom1 = pol4;

	EXPECT_EQ(*table.Find("1"), pol4);

}

TEST(AVLtable, test_cout) {
	AVLtable<Polinom> table;
	Polinom pol1;
	pol1.add_monom(1, 111);

	Polinom pol2;
	pol2.add_monom(2, 222);

	Polinom pol3;
	pol3.add_monom(3, 333);

	Polinom pol4;
	pol4.add_monom(4, 444);

	Polinom pol5;
	pol5.add_monom(5, 555);


	table.Insert("ab", pol1);
	table.Insert("ac", pol2);
	table.Insert("aa", pol3);
	table.Insert("ad", pol4);
	table.Insert("af", pol5);


	std::ostringstream out;
	out << table;
	std::string print = "(aa:3x3y3z3),(ab:x1y1z1),(ac:2x2y2z2),(ad:4x4y4z4),(af:5x5y5z5)";
	EXPECT_EQ(out.str(), print);

	table.Delete("ad");
	std::ostringstream out1;
	out1 << table;

	std::string print1 = "(aa:3x3y3z3),(ab:x1y1z1),(ac:2x2y2z2),(af:5x5y5z5)";
	EXPECT_EQ(out1.str(), print1);
}

TEST(AVLtable, test_delete) {
	AVLtable<Polinom> table;
	Polinom pol1;
	Polinom pol2;
	Polinom pol3;
	Polinom pol4;
	Polinom pol5;
	Polinom pol6;
	Polinom pol7;
	Polinom pol8;

	table.Insert("1", pol1);
	table.Insert("2", pol2);
	table.Insert("3", pol3);
	table.Insert("4", pol4);
	table.Insert("5", pol5);
	table.Insert("6", pol6);
	table.Insert("7", pol7);
	table.Insert("8", pol8);


	EXPECT_EQ(table.GetSize(), 8);

	table.Delete("3");
	EXPECT_EQ(table.Find("3"), nullptr);
	EXPECT_EQ(table.GetSize(), 7);

	table.Delete("1");
	EXPECT_EQ(table.Find("1"), nullptr);
	EXPECT_EQ(table.GetSize(), 6);


}

TEST(AVLtable, test_extract) {
	AVLtable<Polinom> table;
	Polinom pol1;
	pol1.add_monom(3, 111);
	pol1.add_monom(4, 121);
	table.Insert("a", pol1);

	Polinom pol2;
	pol2.add_monom(2, 101);
	pol2.add_monom(6, 234);
	table.Insert("b", pol2);

	Polinom pol3;
	pol3.add_monom(34, 345);

	table.Insert("c", pol3);

	Polinom res = table.Extract("a");
	EXPECT_EQ(res, pol1);
	EXPECT_EQ(table.Find("a"), nullptr);
	EXPECT_EQ(table.GetSize(), 2);

	EXPECT_THROW({ Polinom res = table.Extract("a"); }, std::runtime_error);

}

TEST(AVLtable, test_destruct) {
	AVLtable<Polinom> table;

	Polinom pol1;
	pol1.add_monom(3, 111);

	Polinom pol2;
	pol2.add_monom(2, 101);

	Polinom pol3;
	pol3.add_monom(34, 345);

	Polinom pol4;
	Polinom pol5;
	Polinom pol6;
	Polinom pol7;
	Polinom pol8;

	table.Insert("1", pol1);
	table.Insert("2", pol2);
	table.Insert("3", pol3);
	table.Insert("4", pol4);
	table.Insert("5", pol5);
	table.Insert("6", pol6);
	table.Insert("7", pol7);
	table.Insert("8", pol8);

	table.~AVLtable();
	EXPECT_EQ(table.GetSize(), 0);

}
TEST(test_hash_table, test_operations) {
	HashTable<Polinom> table;

	Polinom pol1;
	pol1.add_monom(1, 111);
	Polinom pol2;
	pol2.add_monom(2, 222);
	Polinom pol3;
	pol3.add_monom(3, 333);

	table.Insert("1", pol1);
	table.Insert("23", pol2);
	table.Insert("12", pol3);

	EXPECT_EQ(table.GetSize(), 3);
	EXPECT_EQ(*table.Find("23"), pol2);
	EXPECT_EQ(table.Find("103"), nullptr);

	//// изменение данных с помощью указателя
	Polinom pol4;
	pol4.add_monom(4, 444);

	Polinom* pol = table.Find("23");
	*pol = pol4;

	EXPECT_EQ(*table.Find("23"), pol4);
	///// применение Delete

	table.Delete("23");
	EXPECT_EQ(table.GetSize(), 2);
	EXPECT_EQ(table.Find("23"), nullptr);
	///// применение Extract

	Polinom pol5;
	pol5.add_monom(5, 555);

	table.Insert("23", pol5);
	EXPECT_EQ(table.GetSize(), 3);
	EXPECT_EQ(table.Extract("23"), pol5);
	EXPECT_EQ(table.GetSize(), 2);
	EXPECT_EQ(table.Find("23"), nullptr);
	///// исключения при использовании Extract

	EXPECT_THROW({ table.Extract("104"); }, std::runtime_error); //ключ не найден

	table.Delete("23"); // 23 уже  удален
	table.Delete("1");
	table.Delete("12");

	EXPECT_EQ(table.GetSize(), 0);

	EXPECT_THROW({ table.Extract("1"); }, std::runtime_error); // size = 0
	/////

	table.Insert("23", pol5);

	EXPECT_EQ(table.GetCapacity(), 500);
	//table.Resize(); 

}

TEST(logging_op, test_operations_file) {
	unordered_table<Polinom> t1;
	AVLtable<Polinom> t2;
	HashTable<Polinom> t3;

	Logging_operations<Polinom> logg("Test");

	Polinom pol1;
	pol1.add_monom(3, 444);

	t1.Insert("a",pol1);
	t2.Insert("a", pol1);
	t3.Insert("a", pol1);

	logg.Insert_operations(t1, t2, t3);
	logg.Writing_to_file(1);


	t1.Find("a");
	t2.Find("a");
	t3.Find("a");

	logg.Find_operations(t1, t2, t3);
	logg.Writing_to_file(2);

	t1.Delete("a");
	t2.Delete("a");
	t3.Delete("a");

	logg.Delete_operations(t1, t2, t3);
	logg.Writing_to_file(0);

	t1.Insert("a", pol1);
	t2.Insert("a", pol1);
	t3.Insert("a", pol1);

	t1.Extract("a");
	t2.Extract("a");
	t3.Extract("a");

	logg.Extract_operations(t1, t2, t3);
	logg.Writing_to_file(3);

	logg.Writing_end_work();

	std::ifstream file("Test.txt");
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}

	EXPECT_EQ(lines[0], "Name of the operation: Insert");
	EXPECT_EQ(lines[5], "Name of the operation: Find");
	EXPECT_EQ(lines[10], "Name of the operation: Delete");
	EXPECT_EQ(lines[15], "Name of the operation: Extract");

	EXPECT_EQ(lines[20], "Total number of operations");
	EXPECT_EQ(lines[21], "Unordered_table:");
	EXPECT_EQ(lines[28], "AVLtable:");
	EXPECT_EQ(lines[35], "HashTable:");

	logg.Clear(); 

	EXPECT_THROW({ Logging_operations<Polinom> logg("abc.txt"); }, std::runtime_error);
}

/// Эксперименты

/*
int main() {

	unordered_table<Polinom> un;
	AVLtable<Polinom> avl;
	HashTable<Polinom> hash;

	hash.Resize();
	hash.Resize(); 

	//генерация данных

	std::vector<Record> data;
	size_t data_size = 1900; //после проведения эксперимента изменить (тогда нужно будет использовать Resize() для HashTable)
	data.reserve(data_size);

	for (int i = 0; i < data_size; ++i) {

		data.push_back(Record(generate_key(i), generate_pol(i)));
	}

	std::vector< std::pair<size_t, double>> res_ins;
	std::vector< std::pair<size_t, double>> res_find;
	std::vector< std::pair<size_t, double>> res_del;

	res_ins = run_insert_experiment(data, un, avl, hash);
	res_find = run_find_experiment(data, un, avl, hash);
	res_del = run_delete_experiment(data, un, avl, hash);

	std::cout << "Unordered table:\n";
	std::cout << "Count operations \"Insert\": " << res_ins[0].first << "\n";
	std::cout << "Time: " << res_ins[0].second << "\n";
	std::cout << "Count operations \"Find\": " << res_find[0].first << "\n";
	std::cout << "Time: " << res_find[0].second << "\n";
	std::cout << "Count operations \"Delete\": " << res_del[0].first << "\n";
	std::cout << "Time: " << res_del[0].second << "\n";

	std::cout << "\nAVLtable:\n";
	std::cout << "Count operations \"Insert\": " << res_ins[1].first << "\n";
	std::cout << "Time: " << res_ins[1].second << "\n";
	std::cout << "Count operations \"Find\": " << res_find[1].first << "\n";
	std::cout << "Time: " << res_find[1].second << "\n";
	std::cout << "Count operations \"Delete\": " << res_del[1].first << "\n";
	std::cout << "Time: " << res_del[1].second << "\n";

	std::cout << "\nHashTable:\n";
	std::cout << "Count operations \"Insert\": " << res_ins[2].first << "\n";
	std::cout << "Time: " << res_ins[2].second << "\n";
	std::cout << "Count operations \"Find\": " << res_find[2].first << "\n";
	std::cout << "Time: " << res_find[2].second << "\n";
	std::cout << "Count operations \"Delete\": " << res_del[2].first << "\n";
	std::cout << "Time: " << res_del[2].second << "\n";
}
*/
