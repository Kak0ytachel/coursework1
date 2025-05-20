#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <functional>
using namespace std;

const int CURRENT_YEAR = 2025;

struct Date {
    int day;
    int month;
    int year;

    Date(int d, int m, int y) {
        day = d;
        month = m;
        if (y < 100) {
            if (y <= CURRENT_YEAR % 100) {
                year = (CURRENT_YEAR / 100) * 100 + y;
            } else {
                year = (CURRENT_YEAR / 100 - 1) * 100 + y;
            }
        } else {
            year = y;
        }
    }

    operator string() {
        stringstream ss;
        ss << setw(2) << setfill('0') << this->day << "." << setw(2) << setfill('0') << this->month << "." << setw(4) << setfill('0') << this->year;
        return ss.str();
    }

    Date(string s) {
        this->day = stoi(s.substr(0, 2));
        this->month = stoi(s.substr(3, 2));
        this->year = stoi(s.substr(6, 4));
    }
};

std::ostream& operator << (std::ostream &os, const Date &date)
{
    return os << setw(2) << setfill('0') << date.day << "." << setw(2) << setfill('0') << date.month << "." << setw(4) << setfill('0') <<  date.year;
}

struct FullName {
    string surName;
    string firstName;
    string lastName;

    FullName(string surName, string firstName, string lastName): surName(surName), firstName(firstName), lastName(lastName) {}

    operator string() {
        return this->surName + " " + this->firstName + " " + this->lastName;
    }
};


ostream& operator << (ostream &os, const FullName &n) {
    return (os << n.surName + " " + n.firstName + " " + n.lastName);
}


struct CustomerAccount {
    string phoneNumber;
    FullName name;
    Date birthDate;
    Date registrationDate;
    double accountBalance;
    string plan;

    CustomerAccount(string phone, FullName name, Date birth, Date registration, string plan, double balance): 
        phoneNumber(phone), name(name), birthDate(birth), registrationDate(registration), plan(plan), accountBalance(balance) {}

    CustomerAccount(): phoneNumber(""), name(FullName("", "", "")), birthDate(Date(0, 0, 0)), registrationDate(Date(0, 0, 0)), 
        accountBalance(0.0), plan("") {}
};

template<typename T>
struct MyArray {
    // dynamic array itself
    T *arr = nullptr;
    // real amount of items in array
    int _size = 0;
    // size of the container
    int capacity;

    MyArray(int capacity): capacity(capacity), _size(0) {
        arr = new T[capacity];
    }

    MyArray(): MyArray(8) {}

    void resize(int new_capacity) {
        T *new_arr = new T[new_capacity];
        for (int i = 0; i < _size; i++) {
            new_arr[i] = arr[i];
        }
        this->arr = new_arr;
        this->capacity = new_capacity;
    }

    void push(T e) {
        if (this->_size == this->capacity) {
            this->resize(this->capacity * 2);
        }
        this->arr[this->_size] = e;
        this->_size++;
    }

    void pop(int ind) {
        for (; ind < this->_size - 1; ind++) {
            arr[ind] = arr[ind+1];
        }
        this->_size--;
    }

    T operator [](int i) const {
        return this->arr[i];
    }

    T & operator [] (int i) {
        return this->arr[i];
    }

    int size() {
        return this->_size;
    }

    T get(int i) {
        return this->operator[](i);
    }

    void swap(int a, int b) {
        T t = this->arr[a];
        this->arr[a] = this->arr[b];
        this->arr[b] = t;
    }

    MyArray<T>* copy() {
        MyArray<T> *new_array = new MyArray<T>(this->capacity);
        for (int i = 0; i < this->_size; i++) {
            new_array->push(this->arr[i]);
        }
        return new_array;
    }
};

template <typename T>
ostream& operator << (ostream &os, const MyArray<T> &arr) {
    os << "[";
    int i = 0;
    while (i < arr._size) {
        os << arr.arr[i];
        i++;
        if (i >= arr._size) {
            break;
        }
        os << ", ";
    }
    os << "]";
    return os;
}

int getInt(int upto, string message) {
    cout << message << endl;
    int value;
    cin >> value;
    cin.clear();
	cin.ignore(99999, '\n');
    if (1 > value || value > upto) {
        cout << "Wrong input. Please provide integer number in range [" << 1 << ", " << upto << "]" << endl;
    } else {
        return value;
    }
    return getInt(upto, message);
}

string getString(string message) {
    cout << message << endl;
    string result;
    cin >> result;
    return result;
}

double getDouble(string message) {
    cout << message << endl;
    double value;
    cin >> value;
    return value;
    cin.clear();
	cin.ignore(99999, '\n');
}

Date getDate(string message) {
    cout << message << endl;
    int d = getInt(31, "Enter day number: ");
    int m = getInt(12, "Enter month number: ");
    int y = getInt(CURRENT_YEAR, "Enter year number: ");
    if (false) {
        // check if this date could exist
        cout << "This date could not exist! There are only 30 days in April";
        cout << "Please try once again";
        return getDate(message);
    }
    Date date = Date(d, m, y);
    return date;
}

string centered(string s, int l) {
    if (s.size() >= l) {
        return s;
    }
    s = string((l - s.size()) / 2, ' ') + s + string((l - s.size()) / 2 + (l - s.size()) % 2, ' ');
    return s;
}

string fromDouble(double value, int precision) {
    stringstream s;  
    s << fixed << setprecision(precision) << value; 
    return s.str(); 
}

template <typename T, typename K>
MyArray<T>* linearSearch(MyArray<T> *arr, function<bool(T, K)> func, K key) {
    MyArray<T> *result = new MyArray<T>();
    for (int i = 0; i < arr->size(); i++) {
        if (func(arr->get(i), key)) {
            result->push(arr->get(i));
        }
    }
    return result;
}

template <typename T>
void choiceSort(MyArray<T> *arr, function<bool(T, T)> compare) {
    for (int start = 0; start < arr->size() - 1; start++) {
        int peakIndex = start;
        for (int i = start + 1; i < arr->size(); i++) {
            if (compare(arr->get(peakIndex), arr->get(i)))  {
                peakIndex = i;
            }
        }
        arr->swap(start, peakIndex);
    }
}

template <typename T, typename K>
int tripleComparator(T obj, K min_val, K max_val, function<K(T)> extractor) {
    K val = extractor(obj);
    if (val < min_val) {
        return -1;
    }
    if (val > max_val) {
        return 1;
    }
    return 0;
}

template <typename T, typename K>
MyArray<T>* binarySearch(MyArray<T> *arr, function<K(T)> func, K min_key, K max_key) {
    MyArray<T> *result = new MyArray<T>();
    int start = binarySearchLeft(arr, func, min_key, max_key);
    int finish = binarySearchRight(arr, func, min_key, max_key);
    cout << "start " << start << " finish " << finish << endl;
    if (start == 0 && finish == 0 && func(arr->get(0)) < min_key) {
        return result;
    }
    if (start == arr->size() && finish == arr->size() && func(arr->get(0)) > max_key) {
        return result;
    }
    for (int i = start; i < finish; i++) {
        result->push(arr->get(i));
    }
    return result;
}

template <typename T, typename K>
int binarySearchLeft(MyArray<T> *arr, function<K(T)> func, K min_key, K max_key) {
    int left = 0;
    int right = arr->size() - 1;
    int mid = (left + right) / 2;
    while (right - left > 1) {
        if (tripleComparator(arr->get(mid), min_key, max_key, func) == -1) {
            left = mid;
        } else {
            right = mid;
        }
        mid = (left + right) / 2;
    }
    if (tripleComparator(arr->get(mid), min_key, max_key, func) == -1) {
            return right;
        } else {
            return left;
        }
}

template <typename T, typename K>
int binarySearchRight(MyArray<T> *arr, function<K(T)> func, K min_key, K max_key) {
    int left = 0;
    int right = arr->size() - 1;
    int mid = (left + right) / 2 + (left + right) % 2 ;
    while (right - left > 1) {
        if (tripleComparator(arr->get(mid), min_key, max_key, func) != 1) {
            left = mid;
        } else {
            right = mid;
        }
        mid = (left + right) / 2 + (left + right) % 2 ;
    }
    if (tripleComparator(arr->get(mid), min_key, max_key, func) != 1) {
            return right;
        } else {
            return left;
        }
}

template <typename T>
void quickSort(MyArray<T> *arr, function<bool(T, T)> compare) {
    quickSortRecursive(arr, compare, 0, arr->size()-1);
}

template <typename T>
void quickSortRecursive(MyArray<T> *arr, function<bool(T, T)> compare, int start, int finish) {
    if (start >= finish) {
        return;
    }
    int base = (start + finish) / 2;
    int i = start;
    while (i <= finish) {
        if (i < base && compare(arr->get(i), arr->get(base))) {
            arr->swap(i, base-1);
            arr->swap(base-1, base);
            base--;
        } else if (i > base && !compare(arr->get(i), arr->get(base))) {
            arr->swap(i, base+1);
            arr->swap(base+1, base);
            base++;
        } else {
            i++;
        }
    }
    quickSortRecursive(arr, compare, start, base-1);
    quickSortRecursive(arr, compare, base+1, finish);
} 

bool dateComparator(Date a, Date b) {
    if (a.year > b.year || (a.year == b.year && a.month > b.month) || (a.year == b.year && a.month == b.month && a.day > b.day)) {
        return true;
    }
    return false;
}
 
void showTable(MyArray<CustomerAccount> *accounts) {
    cout << centered("No.", 5) << "|" << centered("Name", 25) << "|" << centered("Phone number", 15) << "|" << centered("Registration", 15) << 
        "|" << centered("Birth date", 15) << "|" << centered("Plan", 15) << "|" << centered("Balance", 10) << endl;
    for (int i = 0; i < accounts->size(); i++) {
        CustomerAccount account = accounts->operator[](i);
        cout << centered(to_string(i+1) + ".", 5) << "|" << centered(account.name, 25) << "|" << centered(account.phoneNumber, 15) << "|" << 
            centered(account.registrationDate, 15) << "|" << centered(account.birthDate, 15) << "|" << centered(account.plan, 15) 
            << "|" << centered(fromDouble(account.accountBalance, 2), 10) << endl;
    }
}

void displayCustomers(MyArray<CustomerAccount> *accounts) {
    MyArray<CustomerAccount> *sorted = accounts->copy();
    while (true) {
        showTable(sorted);
        int option2 = getInt(3, "Options: \n1. View account\n2. Sort\n3. Return");
        switch (option2) {
            case 1: {
                int option3 = getInt(sorted->size(), "Enter account No.");
                CustomerAccount account = sorted->get(option3-1);
                cout << "--------------------------------" << endl;
                cout << "Viewing account details:" << endl;
                cout << "Owner: " << account.name << endl;
                cout << "Phone number: " << account.phoneNumber << endl;
                cout << "Registred on: " << account.registrationDate << endl;
                cout << "Owner birth date: " << account.birthDate << endl;
                cout << "Plan: " << account.plan << endl;
                cout << "Balance: " << account.accountBalance << endl; 
                cout << "--------------------------------" << endl; 
                break;
            }
            case 2: {
                int option3 = getInt(3, "Choose field to sort by: \n1. Name\n2. Registration date\n3. Balance");
                string fieldName;
                string sortName;
                function<bool(CustomerAccount a, CustomerAccount b)> cmp;
                cmp = [](CustomerAccount a, CustomerAccount b) {return true;};
                switch (option3) {
                    case 1: {
                        cmp = [](CustomerAccount a, CustomerAccount b) {return ((string)a.name > (string)b.name);};
                        fieldName = "name";
                        break;
                    }
                    case 2: {
                        cmp = [](CustomerAccount a, CustomerAccount b) {return dateComparator(a.registrationDate, b.registrationDate);};
                        fieldName = "regisstration date";
                        break;
                    }
                    case 3: {
                        cmp = [](CustomerAccount a, CustomerAccount b) {return (a.accountBalance > b.accountBalance);};
                        fieldName = "balance";
                        break;
                    }
                }
                int option4 = getInt(2, "Choose sorting algorithm to use: \n1. Choice sort\n2. Quick sort");
                switch (option4) {
                    case 1: {
                        choiceSort<CustomerAccount>(sorted, cmp);
                        sortName = "choice sort";
                        break;
                    }
                    case 2: {
                        quickSort<CustomerAccount>(sorted, cmp);
                        sortName = "quick sort";
                        break;
                    }
                }
                cout << "Succesffully sorted by " << fieldName << " using " << sortName << endl;
                break;
            }
            case 3: {
                return;
            }
        }
    }
}

MyArray<CustomerAccount>* filterCustomers(MyArray<CustomerAccount> *arr) {
    int option = getInt(3, "Choose field to filter by: \n1. Name (linear search, contains substring)\n2. Balance (binary search, from min value to max value)\n3. Plan + balance below 0 (linear, individual task)");
    MyArray<CustomerAccount> *result;
    switch (option) {
        case 1: {
            string name = getString("Enter name to look for");
            function<bool(CustomerAccount, string)> comparator = [](CustomerAccount acc, string name) {
                string name2 = (string)acc.name;
                return (name.find(name2) != string::npos || name2.find(name) != string::npos);
            };
            result = linearSearch<CustomerAccount, string>(arr, comparator, name);
            break;
        }
        case 2: {
            double min_val = getDouble("Enter minimal value to look for");
            double max_val = getDouble("Enter maximal value to look for");
            function <double(CustomerAccount)> extractor = [](CustomerAccount acc) {
                return acc.accountBalance;
            };
            function<bool(CustomerAccount, CustomerAccount)> comparator = [](CustomerAccount a, CustomerAccount b) {
                return (a.accountBalance > b.accountBalance);
            };
            quickSort(arr, comparator);
            result = binarySearch<CustomerAccount, double>(arr, extractor, min_val, max_val);
            break;
        }
        case 3: {
            string plan = getString("Enter plan to look for");
            function<bool(CustomerAccount, string)> comparator = [](CustomerAccount acc, string plan) {
                string plan2 = (string)acc.plan;
                return (acc.accountBalance > 0 && (plan.find(plan2) != string::npos || plan2.find(plan) != string::npos));
            };
            result = linearSearch<CustomerAccount, string>(arr, comparator, plan);
            break;
        }
    }
    return result;
}

string serializeData(MyArray<CustomerAccount> *accounts) {
    string result = "[";
    for (int i = 0; i < accounts->size(); i++) {
        CustomerAccount acc = accounts->get(i);
        string st = "{" + acc.phoneNumber + "|" + acc.name.surName + "|" + acc.name.firstName + "|" + acc.name.lastName + "|" + 
        (string)(acc.birthDate) + "|" + (string)(acc.registrationDate) + "|" + acc.plan + "|" + to_string(acc.accountBalance) + "|}";
        result += st;
        if (i + 1 != accounts->size()) {
            result += ",";
        }
    }
    result += "]";
    return result;
}

MyArray<CustomerAccount> *deserializeData(string s) {
    int start = s.find_first_of('[');
    int i = start;
    MyArray<CustomerAccount> *accounts = new MyArray<CustomerAccount>();
    while (i < s.size()) {
        if (s[i] == '{') {
            int begin = i;
            i++;
            MyArray<string> elements = MyArray<string>(8);
            string buf;
            while (true) {
                if (s[i] == '|') {
                    elements.push(buf);
                    buf = "";
                } else if (s[i] == '}') {
                    break;
                } else {
                    buf += s[i];
                }
                i++;
            }
            cout << s.substr(begin, i-begin+1) << endl;
            string phoneNumber = elements[0];
            string surname = elements[1];
            string firstname = elements[2];
            string lastname = elements[3];
            Date born = Date(elements[4]);
            Date registered = Date(elements[5]);
            string plan = elements[6];
            double balance = stod(elements[7]);
            CustomerAccount acc = CustomerAccount(phoneNumber, FullName(surname, firstname, lastname), born, registered, plan, balance);
            accounts->push(acc);
        }
        i++;
        if (s[i] == ',') {
            i++;
        }
        if (s[i] == ']') {
            break;
        }
    }
    return accounts;
}

string loadFile(string filename) {
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open()) {
        return "";
    }
    string s;
    fin >> s;
    fin.close();
    return s;
}

void saveFile(string filename, string data) {
    ofstream fout;
    fout.open(filename);
    fout << data;
    fout.close();
}

CustomerAccount create_account() {
    cout << "Enter customer full name" << endl;
    string surname = getString("Enter surname: ");
    string firstname = getString("Enter first name: ");
    string lastname = getString("Enter last name: ");
    FullName name = FullName(surname, firstname, lastname);
    Date birth = getDate("Enter birth date: ");
    Date registration = getDate("Enter registration date: ");
    string phoneNumber = getString("Enter phone number: ");
    string plan = getString("Enter current plan: ");
    double balance = getDouble("Enter start balance");
    CustomerAccount acc = CustomerAccount(phoneNumber, name, birth, registration, plan, balance);
    cout << "Successfully created account" << endl;
    return acc;
}

MyArray<CustomerAccount>* load_data() {
    MyArray<CustomerAccount>* accounts = new MyArray<CustomerAccount>();
    string filename = getString("Enter filename or \"-\" to load sample dataset");
    if (filename == "-") {
        accounts->push(CustomerAccount("+14159823472", FullName("Thompson", "Olivia", "James"), Date(14, 8, 1992), Date(21, 3, 2021), "Unlimited 5G", -23.45));
        accounts->push(CustomerAccount("+12127431195", FullName("Rodriguez", "Ethan", "Morales"), Date(3, 5, 1987), Date(7, 11, 2022), "Family Share", 5.00));
        accounts->push(CustomerAccount("+16176652048", FullName("Nguyen", "Sophia", "Tran"), Date(25, 12, 1999), Date(13, 1, 2023), "Prepaid Basic", 0.00));
        accounts->push(CustomerAccount("+13054497883", FullName("Patel", "Arjun", "Mehta"), Date(19, 2, 1990), Date(30, 7, 2020), "Unlimited Plus", 92.13));
        accounts->push(CustomerAccount("+12063986771", FullName("Kim", "Hannah", "Park"), Date(8, 4, 2001), Date(2, 9, 2021), "Plus", 11.76));
        accounts->push(CustomerAccount("+491712345678", FullName("Muler", "Lucas", "Schmidt"), Date(11, 10, 1985), Date(15, 11, 2022), "Unlimited EU", 8.50));
        accounts->push(CustomerAccount("+336688453321", FullName("Dubois", "Camille", "Moreau"), Date(20, 7, 1996), Date(4, 5, 2022), "Basic", 2.90));
        accounts->push(CustomerAccount("+5521987654321", FullName("Silva", "Mariana", "Costa"), Date(30, 11, 1993), Date(10, 1, 2023), "Unlimited", 14.25));
        accounts->push(CustomerAccount("+79165558842", FullName("Ivanov", "Alexei", "Petrov"), Date(17, 3, 1988), Date(26, 10, 2020), "Unlimited 5G", 310.00));
        accounts->push(CustomerAccount("+45204547788", FullName("Jensen", "Freja", "Larsen"), Date(9, 6, 1995), Date(19, 6, 2021), "Unlimited 5G", 47.00));
        cout << "Loaded sample dataset" << endl;
    } else {
        string data = loadFile(filename);
        if (data == "") {
            cout << "Unable to find file or load data, please try again" << endl;
        } else {
            accounts = deserializeData(data);
            cout << "Succesfully loaded data" << endl;
        }
    }
    return accounts;
}

void saveData(MyArray<CustomerAccount>* accounts) {
    string filename = getString("Enter filename");
    saveFile(filename, serializeData(accounts));
    cout << "Successfully saved data" << endl;
}

int main() {
    MyArray<CustomerAccount> *accounts = new MyArray<CustomerAccount>();
    while (true){    
        int option = getInt(6, "Menu: \n1. Create customer account\n2. View customer data\n3. Apply filter\n4. Load from file\n5. Save to file\n6. Exit\nEnter option index: ");
        switch (option) {
            case 1: {
                CustomerAccount acc = create_account();
                accounts->push(acc);
                break;
            }
            case 2: {
                displayCustomers(accounts);
                break;
            }
            case 3: {
                MyArray<CustomerAccount> *filtered = filterCustomers(accounts);
                displayCustomers(filtered);
                break;
            }
            case 4: {
                accounts = load_data();
                break;
            }
            case 5: {
                saveData(accounts);
                break;
            }
            case 6: {
                cout << "Exiting..." << endl;
                return 0;
            }
        }
    }
    return 0;
}
