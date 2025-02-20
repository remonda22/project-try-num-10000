#include <iostream>// نستخدم هذه المكتبة لادخال واخراج البيانات
#include <vector>//نستخدم هذه المكتبة لاستخدام المصفوفات الديناميكية لتخزين الحسابات
#include <string>//نستخدم هذه المكتبة للتعامل مع النصوص
#include <limits>//نستخدم هذه المكتبة للتعامل مع الادخال غير الصحيح
#include <algorithm>//نستخدم هذه المكتبة لاستخدام بعض الدوال التي تسهل علينا الكود
#include <windows.h>//نستخدم هذه المكتبة لتغيير الوان النصوص في الكونسول
#include <conio.h> // لإضافة دالة getch() لايقاف الشاشة حتى يضغط المستخدم علي زر

using namespace std;

// الألوان: هذه القيم تمثل الوان النصوص في الكونسول
const int DEFAULT_COLOR = 7;
const int TITLE_COLOR = 11;
const int ERROR_COLOR = 12;
const int SUCCESS_COLOR = 10;
const int MENU_COLOR = 14;
const int INFO_COLOR = 9;

// دالة لتغيير لون النص في الكونسول
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// دالة لمسح الشاشة عند الحاجة حتى لا تكون الشاشة مزدحمة وتكون سلسة في التعامل
void clearScreen() {
    system("cls");
}

// تعريف كلاس BankAccount يحتوي الكلاس علي اسم العميل ,كلمة المرور,والرصيد
// يتم تهيئة البيانات باستخدام constructor
class BankAccount {
public:
    string name;
    string password;
    int accountNumber;
    double balance;

    // Constructor لتهيئة الحساب
    BankAccount(string n, string p, double b, int accNum) {
        name = n;
        password = p;
        balance = b;
        accountNumber = accNum;
    }
};

// قائمة لتخزين الحسابات accounts

vector<BankAccount> accounts;
int nextAccountNumber = 1000; // رقم الحساب التالي الذي سيتم تعيينه وذلك لاعطاء كل حساب رقم مميز غير متكرر


// دالة للتعامل مع الإدخال الخاطئ

bool handleInvalidInput(bool isRetryAllowed = true) {
    cin.clear(); //لمسح حالة الخطأ
    cin.ignore(numeric_limits<streamsize>::max(), '\n');//لتجاهل المدخلات الخطأ

    if (!isRetryAllowed) return false;

    setColor(MENU_COLOR);
    cout << "\n1. Try again\n0. Back to main menu\nChoose: ";
    int choice;
    cin >> choice;

    if (choice == 1) return true;
    return false;
}

// دالة لطباعة رسائل الخطأ
void printError(string msg) //تطبع رسالة خطأ باللون الاحمر
{
    setColor(ERROR_COLOR);
    cout << "[!] " << msg << endl;
    setColor(DEFAULT_COLOR);
}

// دالة لطباعة رسائل النجاح
void printSuccess(string msg) //تطبع رسالة نجاح باللون الأخضر
{
    setColor(SUCCESS_COLOR);
    cout << "[*] " << msg << endl;
    setColor(DEFAULT_COLOR);
}

// دالة للحصول على مبلغ صحيح
// تمنع ادخال أرقام سالبة أو  غير رقمية
//إذا كان الادخال خاطئا, تعطي المستخدم خيار اعادة المحاولة
double getValidAmount() {
    double amount;
    while (true) {
        if (!(cin >> amount) || amount < 0) {
            printError("Invalid amount! Must be non-negative number");
            if (!handleInvalidInput()) return -1; // إرجاع -1 للإشارة إلى إلغاء العملية
        }
        else {
            return amount;
        }
    }
}

// nullptr دالة للبحث عن حساب باستخدام رقم الحساب وتعيد مؤشرا للحساب إذا وجد ,وإلا تعيد
BankAccount* findAccount(int accNum) {
    for (auto& acc : accounts) {
        if (acc.accountNumber == accNum)
            return &acc;
    }
    return nullptr;
}

// دالة لعرض تفاصيل الحساب
void displayAccountInfo(BankAccount* acc) {
    setColor(INFO_COLOR);
    cout << "\nAccount Details:\n";
    cout << "----------------\n";
    cout << "Account Number: " << acc->accountNumber << endl;
    cout << "Account Holder: " << acc->name << endl;
    cout << "Balance: $" << acc->balance << endl;
    setColor(DEFAULT_COLOR);
}

// دالة لإنشاء حساب جديد
//يطلب اسم العميل, كلمة المرور , والرصيد
//يتحقق من صحة المدخلات
// accounts ينشئ الحساب ويخزنه في

void createAccount() {
    clearScreen();
    string name, password;
    double initialDeposit;

    setColor(TITLE_COLOR);
    cout << "=== Create New Account ===\n\n";
    setColor(DEFAULT_COLOR);

    while(true) {
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name);
        if(name.empty()) {
            printError("Name cannot be empty!");
            if(!handleInvalidInput()) return;
            continue;
        }
        break;
    }

    while(true) {
        cout << "Enter password (min 4 chars): ";
        cin >> password;
        if(password.length() < 4) {
            printError("Password must be at least 4 characters!");
            if(!handleInvalidInput()) return;
            continue;
        }
        break;
    }

    cout << "Enter initial deposit: ";
    initialDeposit = getValidAmount();
    if(initialDeposit == -1) return;

    accounts.emplace_back(name, password, initialDeposit, nextAccountNumber++);
    printSuccess("Account created successfully!");
    displayAccountInfo(&accounts.back());
}

// دالة لحذف حساب
void deleteAccount() {
    clearScreen();
    int accNum;
    string pwd;

    setColor(TITLE_COLOR);
    cout << "=== Delete Account ===\n\n";
    setColor(DEFAULT_COLOR);
    while(true) {
        cout << "Enter account number: ";
        if(!(cin >> accNum)) {
            printError("Invalid account number!");
            if(!handleInvalidInput()) return;
            continue;
        }
        break;
    }

    BankAccount* acc = findAccount(accNum);
    if (!acc) {
        printError("Account not found!");
        return;
    }

    int tries = 3;
    while(tries > 0) {
        cout << "Enter password (" << tries << " tries left): ";
        cin >> pwd;
        if(acc->password != pwd) {
            printError("Incorrect password!");
            tries--;
            if(tries == 0) return;
            continue;
        }
        break;
    }

    accounts.erase(remove_if(accounts.begin(), accounts.end(),
        [accNum](const BankAccount& a) { return a.accountNumber == accNum; }), accounts.end());

    printSuccess("Account deleted successfully!");
}

// دالة لتغيير كلمة المرور
void changePassword(BankAccount* acc) {
    clearScreen();
    string oldPwd, newPwd;

    setColor(TITLE_COLOR);
    cout << "=== Change Password ===\n\n";
    setColor(DEFAULT_COLOR);

    while(true) {
        cout << "Enter old password: ";
        cin >> oldPwd;
        if(oldPwd != acc->password) {
            printError("Incorrect password!");
            if(!handleInvalidInput()) return;
            continue;
        }
        break;
    }

    while(true) {
        cout << "Enter new password (min 4 chars): ";
        cin >> newPwd;
        if(newPwd.length() < 4) {
            printError("Password must be at least 4 characters!");
            if(!handleInvalidInput()) return;
            continue;
        }
        break;
    }

    acc->password = newPwd;
    printSuccess("Password changed successfully!");
}

// دالة لتسجيل الدخول
//يطلب رقم الحساب وكلمة المرور
//يتحقق من صحة المدخلات ويسمح ب 3 محاولات
// اذا نجح تسجيل الدخول , يتم عرض القائمة الخاصة بالحساب
void login() {
    clearScreen();
    int accNum;
    string pwd;

    setColor(TITLE_COLOR);
    cout << "=== Login ===\n\n";
    setColor(DEFAULT_COLOR);

    while(true) {
        cout << "Enter account number: ";
        if(!(cin >> accNum)) {
            printError("Invalid input! Numbers only");
            if(!handleInvalidInput()) return;
            continue;
        }
        break;
    }

    BankAccount* acc = findAccount(accNum);
    if (!acc) {
        printError("Account not found!");
        return;
    }

    int tries = 3;
    while(tries > 0) {
        cout << "Enter password (" << tries << " tries left): ";
        cin >> pwd;
        if(acc->password != pwd) {
            printError("Incorrect password!");
            tries--;
            if(tries == 0) return;
            continue;
        }
        break;
    }

    int choice;
    do {
        clearScreen();
        setColor(MENU_COLOR);
        cout << "\nLogged in as: " << acc->name << endl;
        cout << "1. Deposit\n2. Withdraw\n3. View Balance\n4. Account Details\n";
        cout << "5. Change Password\n6. Logout\nChoose: ";
        setColor(DEFAULT_COLOR);

        if(!(cin >> choice)) {
            handleInvalidInput(false);
            choice = 0;
            continue;
        }

        switch(choice) {
            case 1: {
                clearScreen();
                cout << "Enter deposit amount: ";
                double amount = getValidAmount();
                if(amount == -1) break;
                acc->balance += amount;
                printSuccess("Deposit successful!");
                displayAccountInfo(acc);
                break;
            }
            case 2: {
                clearScreen();
                while (true) {
                    cout << "Enter withdrawal amount: ";
                    double amount = getValidAmount();
                    if (amount == -1) break; // إذا تم إلغاء العملية

                    if (amount > acc->balance) {
    printError("Sorry , you are requesting to withdraw an amount larger than your current balance!");
                        setColor(MENU_COLOR);
                        cout << "\n1. Try again\n0. Back to menu\nChoose: ";
                        int retryChoice;
                        cin >> retryChoice;
                        if (retryChoice == 1) continue; // المحاولة مرة أخرى
                        else break; // العودة للقائمة
                    }

                    acc->balance -= amount;
                    printSuccess("Withdrawal successful!");
                    displayAccountInfo(acc);
                    break; // الخروج من الحلقة بعد السحب الناجح
                }
                break;
            }
            case 3:
                clearScreen();
                displayAccountInfo(acc);
                break;
            case 4:
                clearScreen();
                displayAccountInfo(acc);
                break;
            case 5:
                changePassword(acc);
                break;
            case 6:
                printSuccess("Logged out successfully!");
                break;
            default:
                printError("Invalid choice!");
        }

        if(choice != 6) {
            cout << "\nPress any key to continue...";
            _getch();
        }

    } while(choice != 6);
}

// دالة لعرض الرسالة الترحيبية
void displayWelcomeMessage() {
    setColor(TITLE_COLOR);
    cout << "*******************(0_0)******************\n";
    cout << "*                                        *\n";
    cout << "*  Welcome to the Bank Management        *\n";
    cout << "*            System                      *\n";
    cout << "*                                        *\n";
    cout << "*******************(0_0)******************\n\n";
    setColor(DEFAULT_COLOR);
}

// الدالة الرئيسية
int main() {
    int choice;
    do {
        clearScreen();
        displayWelcomeMessage(); // عرض الرسالة الترحيبية
        setColor(MENU_COLOR);
        cout << "1. Create Account\n2. Login\n3. Delete Account\n4. Exit\nChoose: ";
        setColor(DEFAULT_COLOR);

        if(!(cin >> choice)) {
            handleInvalidInput(false);
            choice = 0;
            continue;
        }

        switch(choice) {
            case 1: createAccount(); break;
            case 2: login(); break;
            case 3: deleteAccount(); break;
            case 4: break;
            default: printError("Invalid choice!");
        }

        if(choice != 4) {
            cout << "\nPress any key to continue...";
            _getch();
        }

    } while(choice != 4);

    setColor(INFO_COLOR);
    cout << "\nThank you for using our banking system!\n";
    setColor(DEFAULT_COLOR);
    return 0;
}
