#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
using namespace std;

class Account {
public:
    int balance;
    int id;
    int password;
};

class Transaction {
public:
    string type;
    int amount;
    string date;
    int newBalance;
};

class Customer {
public:
    Account s;
    Transaction trans[100];
    int transCount;
    
    Customer() {
        transCount = 0;
    }
    
    void login();
    void createAccount();
    void after_access();
    bool check_acc(int id, Account &a);
    void update(Account a);
    void addTransaction(string type, int amount, int newBalance);
    void showTransactionHistory();
};

Customer obj;

void select() {
    char op;
    cout << "Please select one of the following options : " << endl;
    cout << "l- login" << endl << "c- Create new account" << endl << "q- Quit" << endl << endl;
    cout << "Enter your choice : ";
    cin >> op;
    
    if (op == 'l') {
        obj.login();
        obj.after_access();
    }
    else if (op == 'c') {
        obj.createAccount();
        cout << "login now: " << endl;
        obj.login();
        obj.after_access();
    }
    else if (op == 'q') {
        cout << "PROCESS ENDED SUCCESSFULLY";
    }
    else {
        cout << "Invalid option";
        select();
    }
}

int main() {
    cout << "WELCOME TO TEHREEM's ATM MACHINE!" << endl << endl;
    select();
    return 0;
}

void Customer::createAccount() {
    Account a;
    ofstream ofile("cdata.txt", ios::app);
    
    if (!ofile) {
        cout << "file does not exist!!" << endl;
        return;
    }
    
    cout << "Enter user-id : " << endl;
    cin >> a.id;
    cout << "Enter your Password (only integers) : " << endl;
    cin >> a.password;
    a.balance = 0;
    
    cout << "THANK YOU! Your account has been Created" << endl << endl;
    ofile << a.id << "  " << a.password << "  " << a.balance << endl;
    ofile.close();
}

bool Customer::check_acc(int id, Account &a) {
    ifstream ifile("cdata.txt");
    if (!ifile) {
        return false;
    }
    
    while (ifile >> a.id >> a.password >> a.balance) {
        if (a.id == id) {
            ifile.close();
            return true;
        }
    }
    ifile.close();
    return false;
}

void Customer::update(Account a) {
    ifstream ifile("cdata.txt");
    ofstream nfile("copy.txt");
    Account copy;
    
    while (ifile >> copy.id >> copy.password >> copy.balance) {
        if (copy.id == a.id) {
            nfile << a.id << "  " << a.password << "  " << a.balance << endl;
        }
        else {
            nfile << copy.id << "  " << copy.password << "  " << copy.balance << endl;
        }
    }
    
    ifile.close();
    nfile.close();
    remove("cdata.txt");
    rename("copy.txt", "cdata.txt");
}

void Customer::login() {
    int x, d;
    cout << "Enter id : ";
    cin >> x;
    cout << "Enter password : ";
    cin >> d;
    cout << endl;
    
    if (check_acc(x, s) && s.password == d) {
        cout << "Access Granted!" << endl;
    }
    else {
        cout << "*******LOGIN FAILED!!*******" << endl << endl;
        select();
    }
}

void Customer::addTransaction(string type, int amount, int newBalance) {
    trans[transCount].type = type;
    trans[transCount].amount = amount;
    trans[transCount].newBalance = newBalance;
    
    time_t now = time(0);
    trans[transCount].date = ctime(&now);
    
    transCount++;
}

void Customer::showTransactionHistory() {
    cout << "\n========== TRANSACTION HISTORY ==========\n";
    cout << "-------------------------------------------\n";
    
    if (transCount == 0) {
        cout << "No transactions yet!\n";
    }
    else {
        for (int i = 0; i < transCount; i++) {
            cout << i + 1 << ". " << trans[i].type << " Rs." << trans[i].amount
                 << " | Balance: Rs." << trans[i].newBalance
                 << " | Date: " << trans[i].date;
        }
    }
    cout << "===========================================\n";
}

void Customer::after_access() {
    char select;
    int deposit;
    int withdraw;
    
    do {
        cout << "\n=====================================\n";
        cout << "d -> Deposit Money" << endl;
        cout << "w -> Withdraw Money" << endl;
        cout << "r -> Request Balance" << endl;
        cout << "t -> Transaction History" << endl;
        cout << "q -> Quit" << endl;
        cout << "Enter your choice: ";
        cin >> select;
        
        if (select == 'd') {
            cout << "Amount to Deposit = ";
            cin >> deposit;
            
            if (deposit > 0) {
                s.balance += deposit;
                cout << "Deposit successful!" << endl;
                update(s);
                addTransaction("DEPOSIT", deposit, s.balance);
            } else {
                cout << "Invalid amount!" << endl;
            }
        }
        else if (select == 'w') {
            cout << "Amount to Withdraw = ";
            cin >> withdraw;
            
            if (withdraw > 0 && withdraw <= s.balance) {
                s.balance -= withdraw;
                cout << "Withdrawal successful!" << endl;
                update(s);
                addTransaction("WITHDRAWAL", withdraw, s.balance);
            }
            else if (withdraw <= 0) {
                cout << "Invalid amount!" << endl;
            }
            else {
                cout << "Insufficient balance!" << endl;
            }
        }
        else if (select == 'r') {
            cout << "Your Balance is = Rs." << s.balance << endl;
        }
        else if (select == 't') {
            showTransactionHistory();
        }
        else if (select == 'q') {
            cout << "Thanks for stopping by!" << endl;
            break;
        }
        else {
            cout << "Invalid option!" << endl;
        }
    } while (select == 'd' || select == 'w' || select == 'r' || select == 't');
}
