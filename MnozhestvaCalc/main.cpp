#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <time.h>
#include <stack>
#include <sstream>

using namespace std;

//template<typename T>
class Set {
private: 

	int _size;
	string _name;
	int _containerCapacity;
	int* _elements;
public:
	int _maxUniv;
	int _minUniv;
	Set() : _elements(nullptr), _size(0), _containerCapacity(0), _name("") {

	}
	~Set() {
		delete[] _elements;
		_elements = nullptr;
	}
	void Insert(const int& element) {
		if (!Contains(element)) {
			if (_size >= _containerCapacity) {
				_containerCapacity = (_containerCapacity == 0) ? 1 : _containerCapacity * 2;
				int* newElements = new int[_containerCapacity];
				for (int i = 0; i < _size; i++) {
					newElements[i] = _elements[i];
				}
				delete[] _elements;
				_elements = newElements;
			}
			_elements[_size++] = element;
		}
	}

	bool Contains(const int& element) const {
		for (int i = 0; i < _size; ++i) {
			if (_elements[i] == element) {
				return true;
			}
		}
		return false;
	}

	void Erase(const int& element) {
		int index = -1;
		for (int i = 0; i < _size; ++i) {
			if (_elements[i] == element) {
				index = i;
				break;
			}
		}
		if (index != -1) {
			for (int i = index; i < _size - 1; ++i) {
				_elements[i] = _elements[i + 1];
			}
			--_size;
		}
	}

	void SetName(string name) {
		_name = name;
	}

	string GetName() {
		return _name;
	}

	int GetSize() const {
		return _size;
	}

	void Clear() {
		_size = 0;
		_containerCapacity = 0;
		delete[] _elements;
		_elements = nullptr;
	}

	void Display() const {
		for (int i = 0; i < _size; i++) {
			cout << _elements[i] << " ";
		}
	}


	Set operator*(const Set& other) const {
		Set intersection;
		for (int i = 0; i < _size; i++) {
			if (other.Contains(_elements[i])) {
				intersection.Insert(_elements[i]);
			}
		}
		return intersection;
	}

	Set operator+(const Set& other) const {
		Set unionSet;
		for (int i = 0; i < _size; i++) {
			unionSet.Insert(_elements[i]);
		}
		for (int i = 0; i < other._size; i++) {
			unionSet.Insert(other._elements[i]);
		}
		return unionSet;
	}

	Set operator-(const Set& other) const {
		Set difference;
		for (int i = 0; i < _size; i++) {
			if (!other.Contains(_elements[i])) {
				difference.Insert(_elements[i]);
			}
		}
		return difference;
	}

	Set operator!() const {
		Set complement;
		for (int i = _minUniv; i <= _maxUniv; i++) {
			if (!Contains(i)) {
				complement.Insert(i);
			}
		}
		return complement;
	}

	Set& operator=(const Set& other) {
		if (this == &other) {
			return *this; // �������� �� ����������������
		}

		// ����������� ������� �������
		Clear();

		// �������� ������ �� ������� �������
		_size = other._size;
		_name = other._name;
		_containerCapacity = other._containerCapacity;
		_minUniv = other._minUniv;
		_maxUniv = other._maxUniv;

		// �������� ������ � �������� ��������
		if (_size > 0) {
			_elements = new int[_containerCapacity];
			for (int i = 0; i < _size; i++) {
				_elements[i] = other._elements[i];
			}
		}
		else {
			_elements = nullptr;
		}

		return *this;
	}

	friend ostream& operator<<(ostream& os, const Set& container) {
		for (int i = 0; i < container._size; i++) {
			os << container._elements[i] << " ";
		}
		return os;
	}
};


template<typename T>
void Remove(std::vector<T>& v, size_t index) {
	v.erase(v.begin() + index);
}


void ShowAllVector(vector<Set*>& v) {
	for (int i = 0; i < v.size(); i++) {
		cout << i << ". " << (*v[i]).GetName() << " = {" << *v[i] << "}\n";
	}
}

bool CheckNumberOrNot(string str) {
	bool isNumber = true;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] < '0' or str[i] > '9') {
			isNumber = false;
		}
	}
	return isNumber;
}

void ClearAll(vector<Set*>& v) {
	for (int i = 0; i < v.size(); i++) {
		delete v[i]; // �������� ���������� �� ���������
	}
	v.clear();
}



bool processExpression(vector<Set*>& v, string expression) {
    auto IsOperator = [](const string token) {
        return token == "+" || token == "*" || token == "-" || token == "!";
    };

    auto IsOperand = [&IsOperator](const string token) {
        return !IsOperator(token);
    };

    auto GetPriority = [](const string op) {
        if (op == "+" || op == "-") {
            return 1;
        }
        if (op == "*") {
            return 2;
        }
        if (op == "!") {
            return 3; // ������������� ��������� "!" ����, ����� �� ���������� ������
        }
        return 0;
    };

    istringstream iss(expression);
    string token;
    stack<string> operators;
    stack<Set*> operands;
    stack<string> parentheses;

    while (iss >> token) {
        if (token == "(") {
            operators.push(token);
            parentheses.push(token);
        }
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                string top = operators.top();
                operators.pop();

                Set* operand2 = operands.top();
                operands.pop();
                Set* operand1 = operands.top();
                operands.pop();

                if (top == "+") {
                    operands.push(new Set(*operand1 + *operand2));
                }
                else if (top == "*") {
                    operands.push(new Set(*operand1 * *operand2));
                }
                else if (top == "-") {
                    operands.push(new Set(*operand1 - *operand2));
                }
                else if (top == "!") {
                    operands.push(new Set(!*operand1));
                }
            }
            if (!operators.empty() && operators.top() == "(") {
                operators.pop(); // ������� "(" �� ����� ����������
                parentheses.pop(); // ������� "(" �� ����� ������
            }
            else {
                cout << "������: ��������������� ������." << endl;
                return false;
            }
        }
		else if (IsOperator(token)) {
			while (!operators.empty() && operators.top() != "(" && GetPriority(token) <= GetPriority(operators.top())) {
				string top = operators.top();
				operators.pop();

				if (top == "!") {
					if (operands.empty()) {
						cout << "������: ����������� ������� ��� ��������� '!" << endl;
						return false;
					}
					Set* operand1 = operands.top();
					operands.pop();
					operands.push(new Set(!*operand1));
				}
				else {
					if (operands.size() < 2) {
						cout << "������: ������������ ��������� ��� ��������� '" << top << "'" << endl;
						return false;
					}
					Set* operand2 = operands.top();
					operands.pop();
					Set* operand1 = operands.top();
					operands.pop();

					if (top == "+") {
						operands.push(new Set(*operand1 + *operand2));
					}
					else if (top == "*") {
						operands.push(new Set(*operand1 * *operand2));
					}
					else if (top == "-") {
						operands.push(new Set(*operand1 - *operand2));
					}
				}
			}
			operators.push(token);
		}
        else if (IsOperand(token)) {
            bool found = false;
            for (Set* set : v) {
                if (set->GetName() == token) {
                    operands.push(set);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "������: ��������� '" << token << "' �� �������." << endl;
                return false;
            }
        }
    }

	while (!operators.empty()) {
		string top = operators.top();
		operators.pop();

		if (top == "!") {
			Set* operand = operands.top();
			operands.pop();
			operands.push(new Set(!*operand));
		}
		else {
			Set* operand2 = operands.top();
			operands.pop();
			Set* operand1 = operands.top();
			operands.pop();

			if (top == "+") {
				operands.push(new Set(*operand1 + *operand2));
			}
			else if (top == "*") {
				operands.push(new Set(*operand1 * *operand2));
			}
			else if (top == "-") {
				operands.push(new Set(*operand1 - *operand2));
			}
		}
	}

    if (!parentheses.empty()) {
        cout << "������: ��������������� ������." << endl;
        return false;
    }

    if (operands.size() != 1) {
        cout << "������: �������� ���������� ��������� � ����������." << endl;
        return false;
    }

    cout << "���������: " << *operands.top() << endl;

    // ����������� ���������� ������
    while (!operands.empty()) {
        if (std::find(v.begin(), v.end(), operands.top()) == v.end()) {
            delete operands.top();
        }
        operands.pop();
    }

    return true;
}



int main() {
	setlocale(0, "");
	srand(time(NULL));

	string nameOfSet;
	string numberOfDeletingSet;
	string numberOfFillingSet;
	string elementOfSet;
	string temp;

	string input;

	bool flag = true;
	int randomElementOfSet;
	string countOfFillingElements;
	int maxSetValue, minSetValue;
	int countOfAdded = 0;
	cout << "������� ����������� �������� ����������: ";
	cin >> minSetValue;
	cout << "������� ������������ �������� ����������: ";
	cin >> maxSetValue;

	vector<Set*> tempParametricVector;
	vector<Set*> vectorMnozhestv; //������ �������� ��� ��������� ��������
	bool programmIsWorking = true;

	int n;

	char userChoice = '0';
	char howFillSet;
	while (userChoice != '8') {
		system("cls");
		cout << "��� �� ������ �������?\n";
		cout << "1 - ������� ���������.\n";
		cout << "2 - ������� ���������.\n";
		cout << "3 - ��������� ���������.\n";
		cout << "4 - ������ ������ � ��������� �����������.\n";
		cout << "5 - �������� � ������� ���������.\n";
		cout << "6 - ������� �������� ���������.\n";
		cout << "7 - ������� ��������� � �����������.\n";
		cout << "8 - ������� ���������.\n\n\n";
		cout << "������������ ���������: ";
		cout << vectorMnozhestv.size() << "\n";
		ShowAllVector(vectorMnozhestv);
		cin >> userChoice;
		system("cls");
		cin.ignore(10000, '\n');
		switch (userChoice)
		{
		case '1':
			nameOfSet = "";
			vectorMnozhestv.push_back(new Set());
			cout << "������� �������� ���������: ";
			cin >> nameOfSet;
			for (int i = 0; i < nameOfSet.length(); i++) {
				if ((nameOfSet[i] >= '�' and nameOfSet[i] <= '�') or nameOfSet[i] == '�' or nameOfSet[i] == '�' or nameOfSet[i] == 32) {
					cout << "������� �������� ��� ��������� ��� ��������: ";
					cin >> nameOfSet;
					i = 0;
				}
			}
			(*vectorMnozhestv[vectorMnozhestv.size() - 1]).SetName(nameOfSet);
			(*vectorMnozhestv[vectorMnozhestv.size() - 1])._minUniv = minSetValue;
			(*vectorMnozhestv[vectorMnozhestv.size() - 1])._maxUniv = maxSetValue;
			break;
		case '2':
			cout << "������� ����� ��������� ������� ������ �������: \n";
			ShowAllVector(vectorMnozhestv);
			cin >> numberOfDeletingSet;
			if (CheckNumberOrNot(numberOfDeletingSet)) {
				if (vectorMnozhestv.size() > stoi(numberOfDeletingSet) and vectorMnozhestv.size() != 0 and stoi(numberOfDeletingSet) >= 0) {
					(*vectorMnozhestv[stoi(numberOfDeletingSet)]).Clear();
					Remove(vectorMnozhestv, stoi(numberOfDeletingSet));
				}
			}
			break;
		case '3':
			cout << "������� ����� ��������� � ������� ������ �������� ��������:\n";
			ShowAllVector(vectorMnozhestv);
			cin >> numberOfFillingSet;
			countOfAdded = 0;
			if (CheckNumberOrNot(numberOfFillingSet)) {
				if (vectorMnozhestv.size() > stoi(numberOfFillingSet) and vectorMnozhestv.size() != 0
					and stoi(numberOfFillingSet) >= 0) {
					system("cls");
					cout << "��� �� ������ ��������� ���������?\n";
					cout << "1 - ���������� �������\n2 - ��������� ������� �������\n";
					cin >> howFillSet;
					switch (howFillSet) {
					case '1':
						cout << "���-�� ��������� ��� ����������: ";
						cin >> countOfFillingElements;
						if (CheckNumberOrNot(countOfFillingElements)) {
							if (stoi(countOfFillingElements) > 0) {
								while (stoi(countOfFillingElements) != countOfAdded) {
									randomElementOfSet = rand() % (maxSetValue - minSetValue + 1) + minSetValue;
									(*vectorMnozhestv[stoi(numberOfFillingSet)]).Insert(randomElementOfSet);
									countOfAdded++;
								}
							}
						}
						break;
					case '2':
						cout << "���-�� ��������� ��� ����������: ";
						cin >> countOfFillingElements;
						if (CheckNumberOrNot(countOfFillingElements)) {
							if (stoi(countOfFillingElements) > 0) {
								cout << "������� �������� ������� ������ �������� � " + (*vectorMnozhestv[stoi(numberOfFillingSet)]).GetName() + " :\n";
								while (stoi(countOfFillingElements) != countOfAdded) {
									cin >> elementOfSet;
									if (CheckNumberOrNot(elementOfSet)) {
										if (stoi(elementOfSet) > minSetValue and stoi(elementOfSet) < maxSetValue) {
											(*vectorMnozhestv[stoi(numberOfFillingSet)]).Insert(stoi(elementOfSet));
											countOfAdded++;
										}
										else {
											cout << "����� ��������� ��� ������ ����������.";
										}
									}
								}
							}
						}
						break;
					}
				}
			}
			break;
		case '4':
				system("cls");
				cout << "������� ���������. ������: A + B, A * B, !A, A - B\n";
				getline(cin, input);
				//cin.ignore(10000, '\n');
				processExpression(vectorMnozhestv, input);
				cin >> temp;
			break;
		case '5':
			ClearAll(vectorMnozhestv);
			break;
		case '6':
			cout << "������� ����� ��������� � ������� ������ ������� ��������:\n";
			ShowAllVector(vectorMnozhestv);
			cin >> numberOfDeletingSet;
			countOfAdded = 0;
			if (CheckNumberOrNot(numberOfDeletingSet)) {
				if (vectorMnozhestv.size() > stoi(numberOfDeletingSet) and vectorMnozhestv.size() != 0
					and stoi(numberOfDeletingSet) >= 0) {
					while (userChoice != '1') {
						cout << "������� �������� ��������, ������� ������ �������: ";
						cin >> elementOfSet;
						(*vectorMnozhestv[stoi(numberOfDeletingSet)]).Erase(stoi(elementOfSet));
						cout << "���������?\n1 - ��\n2 - ���";
						cin >> userChoice;
					}
				}
			}
			break;
		case '7':
			while (flag) {
				cout << "�������� ��������� ������������ ���������:\n";
				cout << "1 - ������� n\n2 - ������\n3 - ��������\n4 - ������ n\n5 - ������ n\n6 - ��������� �������� ���������.\n";
				cin >> userChoice;
				switch (userChoice) {
				case '1':
					cout << "������� n: ";
					cin >> n;
					tempParametricVector.push_back(new Set());
					
					for (int i = minSetValue; i <= abs(maxSetValue); i++) {
						if (i % n == 0) {
							(*tempParametricVector[tempParametricVector.size() - 1]).Insert(i);
						}
					}
					system("cls");
					break;
				case '2':
					tempParametricVector.push_back(new Set());
					for (int i = minSetValue; i <= abs(maxSetValue); i++) {
						if (i % 2 == 0) {
							(*tempParametricVector[tempParametricVector.size() - 1]).Insert(i);
						}
					}
					break;
				case '3':
					tempParametricVector.push_back(new Set());
					for (int i = minSetValue; i <= abs(maxSetValue); i++) {
						if (i % 2 != 0) {
							(*tempParametricVector[tempParametricVector.size() - 1]).Insert(i);
						}
					}
					break;
				case '4':
					cout << "������� n: ";
					cin >> n;
					tempParametricVector.push_back(new Set());
					for (int i = minSetValue; i <= abs(maxSetValue); i++) {
						if (i > n) {
							(*tempParametricVector[tempParametricVector.size() - 1]).Insert(i);
						}
					}
					break;
				case '5':
					cout << "������� n: ";
					cin >> n;
					tempParametricVector.push_back(new Set());
					for (int i = minSetValue; i <= abs(maxSetValue); i++) {
						if (i < n) {
							(*tempParametricVector[tempParametricVector.size() - 1]).Insert(i);
						}
					}
					break;
				case '6':
					if (tempParametricVector.size() != 0) {
						cout << "������� �������� ���������: ";
						cin >> nameOfSet;

						for (int i = tempParametricVector.size() - 1; i >= 0; i--) {
							*tempParametricVector[0] = *tempParametricVector[0] * (*tempParametricVector[i]);
						}

						
						vectorMnozhestv.push_back(new Set());
						*vectorMnozhestv[vectorMnozhestv.size() - 1] = *tempParametricVector[0];
						vectorMnozhestv[vectorMnozhestv.size() - 1]->SetName(nameOfSet);
						(*vectorMnozhestv[vectorMnozhestv.size() - 1])._maxUniv = maxSetValue;
						(*vectorMnozhestv[vectorMnozhestv.size() - 1])._minUniv = minSetValue;
						

						tempParametricVector[0] = NULL;
						ClearAll(tempParametricVector);
					}
					flag = false;
					break;
				}
			}
			flag = true;
			break;
		case '8':
			for (auto set_ptr : vectorMnozhestv) {
				delete set_ptr;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}
