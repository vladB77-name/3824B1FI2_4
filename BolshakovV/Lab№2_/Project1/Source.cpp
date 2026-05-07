#include "Polinom.h"
#include "Tables.h"
#include <sstream> //getline

double correct_coeff() {

	bool flag_error = 1;
	std::string coeff;
	int count_dot;
	while (true) {

		std::cout << "Your coeff: ";
		std::getline(std::cin, coeff);
		count_dot = 0;
		size_t i;
		for (i = 0; i < coeff.size(); ++i) {

			if (coeff.size() > 1 && coeff[0] == '0') {
				std::cout << "Incorrect data. Please try again" << '\n';
				break;
			}

			if (coeff.size() == 1 && coeff[0] == '-') {
				std::cout << "Incorrect data. Please try again" << '\n';
				break;
			}

			if (coeff[i] >= '0' && coeff[i] <= '9') {

			}
			else if (coeff[i] == '-' && i != 0) {
				std::cout << "Incorrect data. Please try again" << '\n';
				break;
			}
			else if (coeff[i] == '.') {
				++count_dot;
				if (count_dot > 1) {
					std::cout << "Incorrect data. Please try again" << '\n';
					break;
				}
			}
			else {
				std::cout << "Incorrect data. Please try again" << '\n';
				break;
			}
		}

		if (i == coeff.size()) {
			break;
		}
	}

	double val = std::stod(coeff);
	return val;
};

unsigned int correct_deg() {
	std::string deg;

	bool flag_error = 0;
	while (true) {

		std::cout << "Your degree: ";
		std::getline(std::cin, deg);

		for (size_t i = 0; i < deg.size(); ++i) {
			if (deg[i] < '0' || deg[i]>'9') {
				std::cout << "Incorrect data. Please try again" << '\n';
				flag_error = 1;
				break;
			}
		}
		if (!flag_error) {


			if (deg.size() > 1 && deg[0] == '0') {
				std::cout << "Error. The degree cannot start from zero" << '\n';
			}
			else if (deg.size() > 3) {
				std::cout << "The monom degree must contain no more than 3 digits. Please try again" << '\n';
			}
			else {
				break;
			}
		}
		flag_error = 0;
	}

	unsigned int val = std::stoi(deg);
	return val;
}

void input_polinom(Polinom& pol) {
	bool flag_not_correct = 1;
	while (flag_not_correct) {
		try {
			std::cout << "Input: ";
			std::cin >> pol;
			flag_not_correct = 0;
		}
		catch (const std::exception& e) {
			std::cout << e.what() << ". Please try again.\n";
		}
	}
}

void add_m(double& coeff, unsigned int& deg) {

	coeff = correct_coeff();

	std::cout << "\nEnter the degree of the monomial (Integer. No more than 3 digits, minimum value: 0).\n";
	std::cout << "The number of hundreds is the degree of the variable x, the number of tens is the degree of the variable y,";
	std::cout << " and the number of units is the degree of the variable z.\n";
	std::cout << "Example:" << '\n';
	std::cout << "123 -> x^1, y^2, z^2;" << '\n';
	std::cout << "10 -> x^0, y^1, z^0;" << '\n';
	std::cout << "2 -> x^0, y^0, z^2" << '\n';


	deg = correct_deg();
	std::cout << '\n';
}

void rules_add_monom() {
	std::cout << "\nInput rules: the first number is the coefficient, followed by a colon, and the second number is the power.\n";
	std::cout << "Enter the monomials separated by commas (spaces are not allowed!).\n";
	std::cout << "Example: 12:123,5:11,4:789 -> polynomial: 4x7y8z9 + 12x1y2z3 + 5x0y1z1.\n";
	std::cout << "(pressing only \"enter\" will create an empty polynomial)\n";
}


/////////// New features

void input_key(std::string& key) {

	while (true) {
		std::cout << "\nKey: ";
		std::getline(std::cin, key);
		if (key.size() == 0) {
			std::cout << "Error. The key cannot be an empty string. Please try again\n";
		}
		else {
			break;
		}
	}
}

int Choice_mode(char left, char right) {
	std::string ch;
	while (true) {

		std::cout << "\nYour choice: ";

		std::getline(std::cin, ch);
		if (ch.size() == 1 && ch[0] >= left && ch[0] <= right) {
			break;
		}
		else {
			std::cout << "Incorrect data. Please try again" << '\n';
		}
	}

	int choice = std::stoi(ch);
	return choice;
}

int choice_replace_polinom() {
	std::cout << "Please select the polynomial you want to replace with a polynomial from the table\n";
	std::cout << "1 - polinom_1 will be changed\n";
	std::cout << "2 - polinom_2 will be changed\n";
	std::cout << "0 - cancel the operation (the data obtained from the table will not be applied)\n";

	int choice = Choice_mode('0', '2');
	return choice;
}



int choice_insert_polinom() {
	std::cout << "Please select the polynomial you want to insert into the table\n";
	std::cout << "1 - polinom_1\n";
	std::cout << "2 - polinom_2\n";
	std::cout << "3 - polynomial is the result of an arithmetic operation between polinom_1\n";
	std::cout << "and polinom_2\n";
	std::cout << "If no arithmetic operations were performed between polinom_1 and polinom_2,\nan empty polynomial will be added to the table\n";

	int choice = Choice_mode('1', '3');
	return choice;
}



void work_with_table(Polinom& pol_res, Polinom& change_pol1, Polinom& change_pol2, unordered_table<Polinom>& t1, AVLtable<Polinom>& t2, HashTable<Polinom>& t3, Logging_operations<Polinom>& l) {


	bool flag_stop = 0;

	do {

		std::cout << "\nPlease select an operation\n";
		std::cout << "0 - Delete a record\n";
		std::cout << "1 - Insert a polynomial into the table\n";
		std::cout << "2 - Add a new polynomial to the table\n";
		std::cout << "3 - Find the record data (find the polynomial by the key)\n";
		std::cout << "4 - Extract the record (after that, the record will be deleted from the table)\n";
		std::cout << "5 - Finish work (return to polynomial operations)\n";


		int operation = Choice_mode('0', '5');

		std::string key;

		switch (operation) {

		case 0:
		{
			std::cout << "\nPlease enter the key of the record you want to delete from the table\n";
			input_key(key);

			t1.Delete(key);
			t2.Delete(key);
			t3.Delete(key);
			l.Delete_operations(t1, t2, t3);
			l.Writing_to_file(0);

			std::cout << "The record has been deleted (if the table does not contain this key, then nothing has happened)\n";
		}

		break;

		case 1:
		{
			std::cout << "\nPlease enter the key for the record where the polynomial will be stored\n";
			input_key(key);

			int choice = choice_insert_polinom();
			if (choice == 1) { //polinom_1
				t1.Insert(key, change_pol1);
				t2.Insert(key, change_pol1);
				t3.Insert(key, change_pol1);
			}
			else if (choice == 2) { //polinom_2
				t1.Insert(key, change_pol2);
				t2.Insert(key, change_pol2);
				t3.Insert(key, change_pol2);
			}
			else {

				t1.Insert(key, pol_res);
				t2.Insert(key, pol_res);
				t3.Insert(key, pol_res);
			}
			l.Insert_operations(t1, t2, t3);
			l.Writing_to_file(1);

			std::cout << "The record is inserted into the table\n";
		}
		break;

		case 2:

		{

			std::cout << "\nYou need to create a polynomial\n";
			Polinom new_polinom;
			rules_add_monom();
			input_polinom(new_polinom);

			std::cout << "\nPlease enter the key for the record where the polynomial will be stored\n";
			input_key(key);

			t1.Insert(key, new_polinom);
			t2.Insert(key, new_polinom);
			t3.Insert(key, new_polinom);

			l.Insert_operations(t1, t2, t3);
			l.Writing_to_file(1);

			std::cout << "The record is inserted into the table\n";

		}
		break;

		case 3:
		{
			std::cout << "\nPlease enter the key of the record from which you want to retrieve data (polynomial)\n";
			input_key(key);

			Polinom* temp = t1.Find(key);
			t2.Find(key);
			t3.Find(key);
			l.Find_operations(t1, t2, t3);
			l.Writing_to_file(2);


			if (temp == nullptr) {
				std::cout << "This key is not in the table\n";
			}
			else {

				int choice = choice_replace_polinom();

				if (choice == 0) {
					break;
				}
				else if (choice == 1) {

					change_pol1 = *temp;
				}
				else {

					change_pol2 = *temp;
				}

				std::cout << "Data received\n";
			}

		}

		break;

		case 4:
		{

			std::cout << "\nPlease enter the key of the record you want to extract\n";
			input_key(key);

			Polinom extr;

			try {
				extr = t1.Extract(key);
			}
			catch (...) {}

			try {

				t2.Extract(key);

			}
			catch (...) {}

			try {
				t3.Extract(key);

			}
			catch (...) {

				std::cout << "This key is not in the table\n";

				l.Extract_operations(t1, t2, t3);
				l.Writing_to_file(3);

				break;
			}


			l.Extract_operations(t1, t2, t3);
			l.Writing_to_file(3);

			int choice = choice_replace_polinom();

			if (choice == 0) {
				break;
			}
			else if (choice == 1) {

				change_pol1 = extr;
			}
			else {

				change_pol2 = extr;
			}
		}
		break;

		case 5:

			flag_stop = 1;
			break;
		}


	} while (!flag_stop);


}

void main_job(Polinom& pol1, Polinom& pol2, Logging_operations<Polinom>& l) {

	unordered_table<Polinom> t1;
	AVLtable<Polinom> t2;
	HashTable<Polinom> t3;

	Polinom result;

	bool  flag_stop = 0;
	std::string ch;
	do {

		std::cout << "\nAvailable operations:" << '\n';
		std::cout << "1 - Add monom to polinom_1;" << '\n';
		std::cout << "2 - Add monom to polinom_2;" << '\n';
		std::cout << "3 - Add polinom_1 and polinom_2 (sum);" << '\n';
		std::cout << "4 - Subtract from polinom_1 polinom_2;" << '\n';
		std::cout << "5 - Subtract from polinom_2 polinom_1;" << '\n';
		std::cout << "6 - Multiply polinom_1 by a constant;" << '\n';
		std::cout << "7 - Multiply polinom_2 by a constant;" << '\n';
		std::cout << "8 - Multiply polinom_1 and polinom_2;" << '\n';
		std::cout << "9 - Output polinom_1" << '\n';
		std::cout << "10 - Output polinom_2" << '\n';

		std::cout << "11 - Working with tables" << '\n';

		std::cout << "0 - Stop the program" << '\n';

		std::cout << "\nSelect an operation: ";
		bool flag_correct = 0;
		while (!flag_correct) {

			std::getline(std::cin, ch);

			if ((ch.size() == 1 && ch[0] >= '0' && ch[0] <= '9') || ch == "10" || ch == "11") {
				flag_correct = 1;

			}
			else {
				std::cout << "Incorrect data. Please try again" << '\n';
			}
		}

		double coeff;
		unsigned int deg;
		int operation = std::stoi(ch);
		try {
			switch (operation) {
			case 1:
				std::cout << "\nEnter a coefficient (a real number). Note: If you enter 0, the monomial will not be added to polinom_1\n";

				add_m(coeff, deg);

				pol1.add_monom(coeff, deg);

				std::cout << "Monom is added to polinom_1\n";
				break;
			case 2:
				std::cout << "\nEnter a coefficient (a real number). Note: If you enter 0, the monomial will not be added to polinom_2\n";

				add_m(coeff, deg);

				pol2.add_monom(coeff, deg);

				std::cout << '\n';
				std::cout << "Monom is added to polinom_1\n";

				break;
			case 3:

				result = pol1 + pol2;
				std::cout << "\nThe result of the operation: " << result << '\n';

				break;
			case 4:

				result = pol1 - pol2;
				std::cout << "\nThe result of the operation: " << result << '\n';

				break;
			case 5:

				result = pol2 - pol1;
				std::cout << "\nThe result of the operation: " << result << '\n';

				break;
			case 6:

				std::cout << "\nAttention: when you multiply a polynomial by a constant, the polynomial itself will be changed.\n (If you don't want to change anything, multiply it by 1)\n";

				double constant1;
				constant1 = correct_coeff();
				pol1 = pol1 * constant1;
				std::cout << "Polinom_1: " << pol1 << '\n';

				break;
			case 7:

				std::cout << "\nAttention: when you multiply a polynomial by a constant, the polynomial itself will be changed.\n (If you don't want to change anything, multiply it by 1)\n";

				double constant2;
				constant2 = correct_coeff();
				pol2 = pol2 * constant2;

				std::cout << "Polinom_2: " << pol2 << '\n';

				break;
			case 8:

				result = pol1 * pol2;
				std::cout << "\nThe result of the operation: " << result << '\n';

				break;
			case 9:

				std::cout << "\nPolinom_1: " << pol1 << '\n';

				break;
			case 10:

				std::cout << "\nPolinom_2: " << pol2 << '\n';
				break;
			case 11:

				std::cout << "Attention!! You can enter polinom_1, polinom_2, and the result of the arithmetic operation between polinom_1\n";
				std::cout << "and polinom_2 (the result of the last operation) in the table\n";

				work_with_table(result, pol1, pol2, t1, t2, t3, l);

				break;

			case 0:
				l.Writing_end_work();
				flag_stop = 1;
				break;
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		}



	} while (!flag_stop);
}


int main() {

	Logging_operations<Polinom> logg;

	bool flag_stop = 0;

	std::cout << "Hello. You need to create two polynomials: polinom_1 and polinom_2. Please select the mode:" << '\n';
	std::cout << "1 - Create polinom_1 and polinom_2\n";
	std::cout << "0 - Stop the program" << '\n';
	std::cout << "When working with tables, the number of operations performed was recorded in the file: \"Logging operation.txt\".\n";
	std::cout << "If you want to clear the file, enter: -1 (the program will complete its work)\n";

	std::string ch;
	while (flag_stop != 1) {
		std::cout << "Your choice: ";

		std::getline(std::cin, ch);
		if ((ch.size() == 1 && ch[0] >= '0' && ch[0] <= '1') || ch == "-1") {
			flag_stop = 1;

		}
		else {
			std::cout << "Incorrect data. Please try again" << '\n';
			flag_stop = 0;
		}
	}

	int m = std::stoi(ch);
	switch (m) {
	case 1:
	{

		Polinom pol1;
		std::cout << "\nCreate polinom_1\n";
		rules_add_monom();

		input_polinom(pol1);
		Polinom pol2;
		std::cout << "\nNow polinom_2\n";
		input_polinom(pol2);

		std::cout << "Polynomials: polinom_1 and polinom_2 are created" << '\n';

		main_job(pol1, pol2, logg);
	}
	break;

	case 0:

		logg.Writing_end_work();
		break;

	case -1:

		logg.Clear(); //Logging operation в консольном проложении и в тестах - разные файлы 
		break;
	}
}