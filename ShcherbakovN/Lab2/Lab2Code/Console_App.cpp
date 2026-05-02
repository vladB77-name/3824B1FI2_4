// Copyright 2026 Nikita

#include <iostream>
#include <string>
#include <vector>

#include "MainTable.h"
#include "Polynom.h"

void print_menu() {
  std::cout << "+------------------------+MENU+------------------------+\n";
  std::cout << "1 - Creating/Replacing the first polynomial\n"
            << "2 - Creating/Replacing the second polynomial\n";
  std::cout << "3 - Add polynomials\n"
            << "4 - Subtract polynomials\n"
            << "5 - Multiply a polynomial by a constant\n";
  std::cout << "6 - Multiply polynomials\n"
            << "7 - Insert data into a table\n"
            << "8 - Querying data from a table by key\n";
  std::cout << "9 - Delete data from the table by key\n"
            << "10 - Outputting the program menu to the console\n";
  std::cout << "0 - Saving logs AND Shutting down the program\n";
  std::cout << "+------------------------------------------------------+\n";
}

std::string input_key() {
  std::string user_key = "";
  std::cout << "Enter the key for this record:\n";

  std::getline(std::cin, user_key);
  return user_key;
}

void saveResultOfOperationWithPolynomials(
    Polynomial &polynom, MainTable<std::string, Polynomial> &table) {
  std::cout << "0 - put the result of adding polynomials in a table (insert), "
               "1 - do nothing.\nEnter:\n";
  std::string user_input_save = "";

  bool flag_save = true;
  int temp_save = -1;
  while (flag_save) {
    std::getline(std::cin, user_input_save);
    try {
      temp_save = std::stoi(user_input_save);

      if (temp_save == 0) {
        std::string key = input_key();

        table.insertALL(key, polynom);
        std::cout << "The insertion is completed.\n";
        flag_save = false;
      } else if (temp_save == 1) {
        flag_save = false;
      } else {
        std::cout << "INVALID INPUT.\n0 - put the result of adding polynomials "
                     "in a table (insert), 1 - do nothing.\nEnter:\n";
        continue;
      }
    } catch (std::exception) {
      std::cout << "INVALID INPUT.\n0 - put the result of adding polynomials "
                   "in a table (insert), 1 - do nothing.\nEnter:\n";
      continue;
    }
  }
}

Polynomial input_polynom() {
  std::cout << "The coefficients of monomials are real numbers.\nThe degree of "
               "a monomial is a three-digit number, where the hundreds digit "
               "is the degree of variable X, the tens digit is the degree of "
               "variable Y, and the units digit is the degree of variable Z.\n";

  bool flag_input = true;

  std::vector<std::pair<double, size_t>> source;

  std::string user_input = "";

  while (flag_input) {
    std::cout << "Enter the coefficient of the monomial:\n";

    double temp_coeff = 0.0;
    std::getline(std::cin, user_input);
    try {
      temp_coeff = std::stod(user_input);

      if (std::fabs(temp_coeff) < EPSILON) {
        std::cout << "INVALID INPUT. The coefficient of the monomial must be "
                     "different from 0.\n";
        continue;
      }
    } catch (std::exception) {
      std::cout << "INVALID INPUT. Please enter a real number as the "
                   "coefficient of the monomial.\n";
      continue;
    }

    std::cout << "Enter the degree of the monomial:\n";

    size_t temp_deg = -1;
    std::getline(std::cin, user_input);
    try {
      temp_deg = std::stoull(user_input);

      if (temp_deg > 999 || temp_deg < 0) {
        std::cout << "INVALID INPUT. Please enter an integer between 0 and 999 "
                     "as the degree of the monomial.\n";
        continue;
      }
    } catch (std::exception) {
      std::cout << "INVALID INPUT. Please enter an integer between 0 and 999 "
                   "as the degree of the monomial.\n";
      continue;
    }

    source.push_back({temp_coeff, temp_deg});

    std::cout << "1 - Continue entering the polynomial, 0 - Finish entering "
                 "the polynomial.\nEnter:\n";
    bool flag_temp = true;
    while (flag_temp) {
      int temp = -1;
      std::getline(std::cin, user_input);
      try {
        temp = std::stoi(user_input);

        if (temp == 0) {
          flag_input = false;
          flag_temp = false;
        } else if (temp == 1) {
          flag_temp = false;
        } else {
          std::cout << "INVALID INPUT.\n1 - Continue entering the polynomial, "
                       "0 - Finish entering the polynomial.\nEnter:\n";
          continue;
        }
      } catch (std::exception) {
        std::cout << "INVALID INPUT.\n1 - Continue entering the polynomial, 0 "
                     "- Finish entering the polynomial.\nEnter:\n";
        continue;
      }
    }
  }

  Polynomial temp_p1(source);
  return temp_p1;
}

int input_value_int() {
  std::cout << "Enter the integer value for this record:\n";
  std::string user_input = "";
  while (true) {
    std::getline(std::cin, user_input);
    try {
      int result = std::stoi(user_input);
      return result;
    } catch (std::exception) {
      std::cout << "Invalid input. Integer is expected\nEnter:\n";
      continue;
    }
  }
}

double input_value_double() {
  std::cout << "Enter the double value for this record:\n";
  std::string user_input = "";
  while (true) {
    std::getline(std::cin, user_input);
    try {
      double result = std::stod(user_input);
      return result;
    } catch (std::exception) {
      std::cout << "Invalid input. Double is expected\nEnter:\n";
      continue;
    }
  }
}

std::string input_value_string() {
  std::cout << "Enter the string value for this record:\n";
  std::string result = "";
  std::getline(std::cin, result);
  return result;
}

bool input_value_bool() {
  std::cout << "Enter the bool value for this record:\n";
  while (true) {
    int result = -1;
    std::string user_input = "";
    bool flag_stoi = true;
    while (flag_stoi) {
      std::getline(std::cin, user_input);
      try {
        result = std::stoi(user_input);
        flag_stoi = false;
      } catch (std::exception) {
        std::cout << "Invalid input. Bool(0/1) is expected\nEnter:\n";
        continue;
      }
    }

    if (result == 0) {
      return false;
    } else if (result == 1) {
      return true;
    } else {
      std::cout << "Invalid input. 0/1 is expected\nEnter:\n";
    }
  }
}

char input_value_char() {
  std::cout << "Enter the char value for this record:\n";
  std::string user_input = "";
  while (true) {
    std::getline(std::cin, user_input);
    if (user_input.length() == 1) {
      return user_input[0];
    } else {
      std::cout << "Invalid input. Char is expected\nEnter:\n";
    }
  }
}

template <typename TKey, typename TValue>
void insertToTable(MainTable<TKey, TValue> &table_, TValue (*input_func)()) {
  std::string key = input_key();

  TValue value = input_func();

  table_.insertALL(key, value);

  std::cout << "The insertion is completed." << std::endl;
}

std::ostream &operator<<(std::ostream &out, const Polynomial &polynom_) {
  polynom_.print_polynom();
  return out;
}

template <typename TKey, typename TValue>
void findInTable(MainTable<TKey, TValue> &table_, const bool &outputFactor_) {
  if (table_.is_emptyALL()) {
    std::cout << "The table is empty." << std::endl;
    return;
  }

  std::string key = input_key();

  TValue *value = table_.findALL(key).value;
  if (value != nullptr) {
    std::cout << "The search is completed: " << *value << std::endl;
    if (outputFactor_) {
      table_.removeALL(key);
      std::cout << "The remove is completed." << std::endl;
    }
  } else {
    std::cout << "The record was not found." << std::endl;
  }
}

template <typename TKey, typename TValue>
void removeFromTable(MainTable<TKey, TValue> &table_) {
  if (table_.is_emptyALL()) {
    std::cout << "The table is empty." << std::endl;
    return;
  }

  std::string key = input_key();

  table_.removeALL(key);
  std::cout << "The removal is completed.\n";
}

enum ValueType { INTEGER, DOUBLE, STRING, POLYNOMIAL, BOOL, CHAR };

int main() {
  bool flag = true;

  Polynomial p1, p2, res_p;

  MainTable<std::string, int> tableInt;
  MainTable<std::string, double> tableDouble;
  MainTable<std::string, std::string> tableString;
  MainTable<std::string, Polynomial> tablePolynomial;
  MainTable<std::string, bool> tableBool;
  MainTable<std::string, char> tableChar;

  ValueType typeValue;

  std::cout << "+---------------------+TYPE_VALUE+---------------------+\n";
  std::cout << "0 - integer\n"
            << "1 - double\n"
            << "2 - std::string\n"
            << "3 - Polynomial\n"
            << "4 - bool\n"
            << "5 - char\n";
  std::cout << "+--------------------+ENTER_CHOICE+--------------------+\n";
  std::string user_input_type = "";

  bool flag_type = true;
  int temp_type = -1;
  while (flag_type) {
    std::getline(std::cin, user_input_type);
    try {
      temp_type = std::stoi(user_input_type);

      switch (temp_type) {
      case 0: {
        typeValue = INTEGER;
        flag_type = false;
        break;
      }
      case 1: {
        typeValue = DOUBLE;
        flag_type = false;
        break;
      }
      case 2: {
        typeValue = STRING;
        flag_type = false;
        break;
      }
      case 3: {
        typeValue = POLYNOMIAL;
        flag_type = false;
        break;
      }
      case 4: {
        typeValue = BOOL;
        flag_type = false;
        break;
      }
      case 5: {
        typeValue = CHAR;
        flag_type = false;
        break;
      }
      default: {
        std::cout
            << "INVALID INPUT. Select the data type to work with the table.\n";
        std::cout
            << "+--------------------+ENTER_CHOICE+--------------------+\n";
        break;
      }
      }
    } catch (std::exception) {
      std::cout
          << "INVALID INPUT. Select the data type to work with the table.\n";
      std::cout << "+--------------------+ENTER_CHOICE+--------------------+\n";
      continue;
    }
  }

  print_menu();

  while (flag) {
    std::cout << "+--------------------+ENTER_CHOICE+--------------------+\n";

    std::string choice = "";
    int num_choice = 0;
    std::getline(std::cin, choice);

    try {
      num_choice = std::stoi(choice);
    } catch (std::exception) {
      std::cout << "INVALID INPUT. Please enter a number from 0 to 10\n";
      continue;
    }

    switch (num_choice) {
    case 1:
      p1 = input_polynom();
      p1.print_polynom();
      break;
    case 2:
      p2 = input_polynom();
      p2.print_polynom();
      break;
    case 3: {
      if (!p1.is_empty() && !p2.is_empty()) {
        res_p = p1 + p2;
        res_p.print_polynom();

        if (typeValue == POLYNOMIAL) {
          saveResultOfOperationWithPolynomials(res_p, tablePolynomial);
        }
      } else {
        if (p1.is_empty()) {
          std::cout << "The first polynomial is empty.\n";
        }
        if (p2.is_empty()) {
          std::cout << "The second polynomial is empty.\n";
        }
      }

      break;
    }
    case 4: {
      if (!p1.is_empty() && !p2.is_empty()) {
        std::cout << "0 - subtract the second from the first polynomial (p1 - "
                     "p2), 1 - subtract the first from the second polynomial "
                     "(p2 - p1).\nEnter:\n";
        std::string user_input = "";

        bool flag_temp = true;
        int temp = -1;
        while (flag_temp) {
          std::getline(std::cin, user_input);
          try {
            temp = std::stoi(user_input);

            if (temp == 0 || temp == 1) {
              if (temp == 0) {
                res_p = p1 - p2;
                res_p.print_polynom();
              } else {
                res_p = p2 - p1;
                res_p.print_polynom();
              }

              if (typeValue == POLYNOMIAL) {
                saveResultOfOperationWithPolynomials(res_p, tablePolynomial);
              }
              flag_temp = false;
            } else {
              std::cout << "INVALID INPUT.\n0 - subtract the second from the "
                           "first polynomial (p1 - p2), 1 - subtract the first "
                           "from the second polynomial (p2 - p1).\nEnter:\n";
              continue;
            }
          } catch (std::exception) {
            std::cout << "INVALID INPUT.\n0 - subtract the second from the "
                         "first polynomial (p1 - p2), 1 - subtract the first "
                         "from the second polynomial (p2 - p1).\nEnter:\n";
            continue;
          }
        }
      } else {
        if (p1.is_empty()) {
          std::cout << "The first polynomial is empty.\n";
        }
        if (p2.is_empty()) {
          std::cout << "The second polynomial is empty.\n";
        }
      }

      break;
    }
    case 5: {
      std::cout
          << "0 - multiply the first(p1) polynomial by a constant, 1 - "
             "multiply the second(p2) polynomial by a constant.\nEnter:\n";
      std::string user_input = "";

      bool flag_temp = true;
      int temp = -1;
      double temp_const = -1;
      while (flag_temp) {
        std::getline(std::cin, user_input);
        try {
          temp = std::stoi(user_input);

          if (temp == 0 && !p1.is_empty() || temp == 1 && !p2.is_empty()) {
            std::cout << "Enter a constant (real number):\n";

            bool flag_const = true;
            while (flag_const) {
              std::getline(std::cin, user_input);
              try {
                temp_const = std::stod(user_input);
                flag_const = false;
              } catch (std::exception) {
                std::cout << "INVALID INPUT.\nEnter a constant (real number):";
                continue;
              }
            }

            if (temp == 0) {
              res_p = p1 * temp_const;
              res_p.print_polynom();
            } else if (temp == 1) {
              res_p = p2 * temp_const;
              res_p.print_polynom();
            }

            if (typeValue == POLYNOMIAL) {
              saveResultOfOperationWithPolynomials(res_p, tablePolynomial);
            }
            flag_temp = false;
          } else if (temp == 0 && p1.is_empty()) {
            std::cout << "The first polynomial is empty.\n";
            flag_temp = false;
          } else if (temp == 1 && p2.is_empty()) {
            std::cout << "The second polynomial is empty.\n";
            flag_temp = false;
          } else {
            std::cout << "INVALID INPUT.\n0 - multiply the first(p1) "
                         "polynomial by a constant, 1 - multiply the "
                         "second(p2) polynomial by a constant.\nEnter:\n";
            continue;
          }
        } catch (std::exception) {
          std::cout << "INVALID INPUT.\n0 - multiply the first(p1) polynomial "
                       "by a constant, 1 - multiply the second(p2) polynomial "
                       "by a constant.\nEnter:\n";
          continue;
        }
      }

      break;
    }
    case 6: {
      if (!p1.is_empty() && !p2.is_empty()) {
        try {
          res_p = p1 * p2;
          res_p.print_polynom();

          if (typeValue == POLYNOMIAL) {
            saveResultOfOperationWithPolynomials(res_p, tablePolynomial);
          }
        } catch (std::range_error) {
          std::cout << "The degrees of the obtained variables are greater than "
                       "9. The maximum degree of a polynomial is 999.\n";
        }
      } else {
        if (p1.is_empty()) {
          std::cout << "The first polynomial is empty.\n";
        }
        if (p2.is_empty()) {
          std::cout << "The second polynomial is empty.\n";
        }
      }

      break;
    }
    case 7: {
      switch (typeValue) {
      case INTEGER: {
        insertToTable(tableInt, input_value_int);
        break;
      }
      case DOUBLE: {
        insertToTable(tableDouble, input_value_double);
        break;
      }
      case STRING: {
        insertToTable(tableString, input_value_string);
        break;
      }
      case POLYNOMIAL: {
        insertToTable(tablePolynomial, input_polynom);
        break;
      }
      case BOOL: {
        insertToTable(tableBool, input_value_bool);
        break;
      }
      case CHAR: {
        insertToTable(tableChar, input_value_char);
        break;
      }
      }

      break;
    }
    case 8: {
      std::cout << "0 - output the record data by key from the table, 1 - "
                   "extract record data by key from the table.\nEnter:\n";
      std::string user_input = "";

      bool outputFactor = false;

      bool flag_temp = true;
      int temp = -1;
      while (flag_temp) {
        std::getline(std::cin, user_input);
        try {
          temp = std::stoi(user_input);

          if (temp == 0) {
            outputFactor = false;
            flag_temp = false;
          } else if (temp == 1) {
            outputFactor = true;
            flag_temp = false;
          } else {
            std::cout << "INVALID INPUT.\n0 - output the record data by key "
                         "from the table, 1 - extract record data by key from "
                         "the table.\nEnter:\n";
            continue;
          }
        } catch (std::exception) {
          std::cout << "INVALID INPUT.\n0 - output the record data by key from "
                       "the table, 1 - extract record data by key from the "
                       "table.\nEnter:\n";
          continue;
        }
      }

      switch (typeValue) {
      case INTEGER: {
        findInTable(tableInt, outputFactor);
        break;
      }
      case DOUBLE: {
        findInTable(tableDouble, outputFactor);
        break;
      }
      case STRING: {
        findInTable(tableString, outputFactor);
        break;
      }
      case POLYNOMIAL: {
        findInTable(tablePolynomial, outputFactor);
        break;
      }
      case BOOL: {
        findInTable(tableBool, outputFactor);
        break;
      }
      case CHAR: {
        findInTable(tableChar, outputFactor);
        break;
      }
      }

      break;
    }
    case 9: {
      switch (typeValue) {
      case INTEGER: {
        removeFromTable(tableInt);
        break;
      }
      case DOUBLE: {
        removeFromTable(tableDouble);
        break;
      }
      case STRING: {
        removeFromTable(tableString);
        break;
      }
      case POLYNOMIAL: {
        removeFromTable(tablePolynomial);
        break;
      }
      case BOOL: {
        removeFromTable(tableBool);
        break;
      }
      case CHAR: {
        removeFromTable(tableChar);
        break;
      }
      }

      break;
    }
    case 10: {
      print_menu();
      break;
    }
    case 0: {
      std::string file_name = "";
      std::cout << "Enter the file name:\n";

      std::getline(std::cin, file_name);

      while (true) {
          if (file_name.empty()) {
			  std::cout << "INVALID INPUT. The file name cannot be empty.\n";
              std::cout << "Enter the file name:\n";
              std::getline(std::cin, file_name);
          }
          else {
              break;
          }
      }

      switch (typeValue) {
      case INTEGER: {
        tableInt.saveLogs(file_name);
        break;
      }
      case DOUBLE: {
        tableDouble.saveLogs(file_name);
        break;
      }
      case STRING: {
        tableString.saveLogs(file_name);
        break;
      }
      case POLYNOMIAL: {
        tablePolynomial.saveLogs(file_name);
        break;
      }
      case BOOL: {
        tableBool.saveLogs(file_name);
        break;
      }
      case CHAR: {
        tableChar.saveLogs(file_name);
        break;
      }
      }

      std::cout << "Logs are saved.\n";
      std::cout << "Shutdown program.\n";
      flag = false;
      break;
    }
    default: {
      std::cout << "INVALID INPUT. Please enter a number from 0 to 10\n";
      break;
    }
    }
  }
  return 0;
}
