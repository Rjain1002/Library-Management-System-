#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

class Book {
public:
    string title;
    string author;
    int isbn;
    bool available;

    Book(string t, string a, int i) : title(t), author(a), isbn(i), available(true) {}
};

class Student {
public:
    int roll;
    string name;
    double balance;
    vector<int> issued_books;

    Student(int r, string n, double b) : roll(r), name(n), balance(b) {}
};

class Library {
private:
    vector<Student> students;
    vector<Book> books;
    map<int, int> isbnToBookIndex;

public:
    void addBooks() {
            books.emplace_back("C++ Programming", "Bjarne Stroustrup", 12345);
            books.emplace_back("Data Structures", "Mark Allen Weiss", 67890);
            books.emplace_back("Algorithms", "Robert Sedgewick", 11223);
            books.emplace_back("Operating Systems", "Andrew S. Tanenbaum", 44556);
            books.emplace_back("Database Systems", "Abraham Silberschatz", 77889);

            for (int i = 0; i < books.size(); ++i) {
                isbnToBookIndex[books[i].isbn] = i;
            }
    }
    void createAccount() {
        int roll;
        string name;
        double deposit;

        cout << "Enter roll number: ";
        cin >> roll;

        if (findStudentIndex(roll) != -1) {
            cout << "Account already exists.\n";
            return;
        }

        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter initial deposit (min $50): ";
        cin >> deposit;

        if (deposit < 50) {
            cout << "Minimum deposit is $50.\n";
            return;
        }

        students.emplace_back(roll, name, deposit - 50); // $20 account + $30 security
        cout << "Account created successfully.\n";
    }

    void viewBalance(int roll) {
        int index = findStudentIndex(roll);
        if (index == -1) {
            cout << "Student not found.\n";
            return;
        }

        cout << "Roll No: " << students[index].roll
             << "\nName: " << students[index].name
             << "\nBalance: $" << fixed << setprecision(2) << students[index].balance << "\n";
    }

    void depositAmount(int roll, double amount) {
        int index = findStudentIndex(roll);
        if (index == -1) {
            cout << "Student not found.\n";
            return;
        }
        students[index].balance += amount;
        cout << "Deposit successful. New Balance: $" << students[index].balance << "\n";
    }

    void issueBook(int roll) {
        int studentIndex = findStudentIndex(roll);
        if (studentIndex == -1) {
            cout << "Student not found.\n";
            return;
        }

        viewAvailableBooks();

        int isbn;
        cout << "Enter ISBN of the book to issue: ";
        cin >> isbn;

        if (isbnToBookIndex.find(isbn) == isbnToBookIndex.end() || !books[isbnToBookIndex[isbn]].available) {
            cout << "Book not available.\n";
            return;
        }

        if (students[studentIndex].balance < 2) {
            cout << "Insufficient balance.\n";
            return;
        }

        books[isbnToBookIndex[isbn]].available = false;
        students[studentIndex].balance -= 2;
        students[studentIndex].issued_books.push_back(isbn);

        cout << "Book issued successfully.\n";
    }

    void returnBook(int roll, int isbn) {
        int studentIndex = findStudentIndex(roll);
        if (studentIndex == -1) {
            cout << "Student not found.\n";
            return;
        }

        auto& issued = students[studentIndex].issued_books;
        auto it = find(issued.begin(), issued.end(), isbn);
        if (it == issued.end()) {
            cout << "Book not found in issued list.\n";
            return;
        }

        issued.erase(it);
        books[isbnToBookIndex[isbn]].available = true;

        cout << "Book returned successfully.\n";
    }

    void addBook() {
        string title, author;
        int isbn;

        cout << "Enter book title: ";
        cin.ignore();
        getline(cin, title);

        cout << "Enter author: ";
        getline(cin, author);

        cout << "Enter ISBN: ";
        cin >> isbn;

        if (isbnToBookIndex.find(isbn) != isbnToBookIndex.end()) {
            cout << "Book with this ISBN already exists.\n";
            return;
        }

        books.emplace_back(title, author, isbn);
        isbnToBookIndex[isbn] = books.size() - 1;

        cout << "Book added successfully.\n";
    }

    void editBook() {
        int isbn;
        cout << "Enter ISBN to edit: ";
        cin >> isbn;

        if (isbnToBookIndex.find(isbn) == isbnToBookIndex.end()) {
            cout << "Book not found.\n";
            return;
        }

        int index = isbnToBookIndex[isbn];
        cout << "Current title: " << books[index].title << "\nEnter new title: ";
        cin.ignore();
        getline(cin, books[index].title);

        cout << "Current author: " << books[index].author << "\nEnter new author: ";
        getline(cin, books[index].author);

        cout << "Book updated.\n";
    }

    void viewAvailableBooks() {
        cout << "Available Books:\n";
        for (const auto& book : books) {
            if (book.available) {
                cout << "ISBN: " << book.isbn << " | " << book.title << " by " << book.author << "\n";
            }
        }
    }

    void viewAllBooks() {
        cout << "All Books:\n";
        for (const auto& book : books) {
            cout << "ISBN: " << book.isbn
                 << " | " << book.title
                 << " by " << book.author
                 << " | Available: " << (book.available ? "Yes" : "No") << "\n";
        }
    }

    void listAllStudents() {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.roll < b.roll;
        });

        for (const auto& student : students) {
            cout << "Roll: " << student.roll << " | Name: " << student.name
                 << " | Balance: $" << fixed << setprecision(2) << student.balance << "\n";
        }
    }

    void calculateFine(int roll) {
        int index = findStudentIndex(roll);
        if (index == -1) {
            cout << "Student not found.\n";
            return;
        }

        int fine = students[index].issued_books.size() * 5; // $5 fine per book
        cout << "Fine for " << students[index].name << " is: $" << fine << "\n";
    }

    // New Feature 1: Remove student
    void removeStudent(int roll) {
        auto it = find_if(students.begin(), students.end(), [&](Student& s) {
            return s.roll == roll;
        });

        if (it != students.end()) {
            students.erase(it);
            cout << "Student removed successfully.\n";
        } else {
            cout << "Student not found.\n";
        }
    }

    // New Feature 2: View total library balance
    void totalLibraryBalance() {
        double total = 0;
        for (auto& s : students) {
            total += s.balance;
        }
        cout << "Total balance in library: $" << fixed << setprecision(2) << total << "\n";
    }

    // New Feature 3: View all issued books
    void viewIssuedBooks() {
        for (auto& s : students) {
            for (int isbn : s.issued_books) {
                cout << "Student: " << s.name << " | ISBN: " << isbn << " | " << books[isbnToBookIndex[isbn]].title << "\n";
            }
        }
    }

private:
    int findStudentIndex(int roll) {
        for (int i = 0; i < students.size(); ++i) {
            if (students[i].roll == roll) return i;
        }
        return -1;
    }
};

// ========================= Main =========================

int main() {
    Library lib;
    int choice;
    string password;
    lib.addBooks(); // Preload some books
    while (true) {
        cout << "\nLogin as:\n1. Admin\n2. Student\n0. Exit\nEnter choice: ";
        cin >> choice;

        if (choice == 0) break;

        cout << "Enter password: ";
        cin >> password;

        if (password != "123") {
            cout << "Incorrect password.\n";
            continue;
        }

        if (choice == 1) {
            int adminOption;
            cout << "\nAdmin Menu:\n"
                 << "1. Add Book\n2. Edit Book\n3. View All Books\n4. View All Students\n5. Remove Student\n"
                 << "6. View Total Library Balance\n7. View Issued Books\nEnter option: ";
            cin >> adminOption;

            switch (adminOption) {
                case 1: lib.addBook(); break;
                case 2: lib.editBook(); break;
                case 3: lib.viewAllBooks(); break;
                case 4: lib.listAllStudents(); break;
                case 5: {
                    int roll;
                    cout << "Enter roll number to remove: ";
                    cin >> roll;
                    lib.removeStudent(roll);
                    break;
                }
                case 6: lib.totalLibraryBalance(); break;
                case 7: lib.viewIssuedBooks(); break;
                default: cout << "Invalid option.\n";
            }

        } else {
            int roll;
            cout << "Enter roll number: ";
            cin >> roll;

            int studentMenu;
            cout << "\nStudent Menu:\n"
                 << "1. Create Account\n2. View Balance\n3. Deposit Amount\n4. Issue Book\n5. Return Book\n6. Calculate Fine\nEnter option: ";
            cin >> studentMenu;

            switch (studentMenu) {
                case 1: lib.createAccount(); break;
                case 2: lib.viewBalance(roll); break;
                case 3: {
                    double amt;
                    cout << "Enter amount: ";
                    cin >> amt;
                    lib.depositAmount(roll, amt);
                    break;
                }
                case 4: lib.issueBook(roll); break;
                case 5: {
                    int isbn;
                    cout << "Enter ISBN to return: ";
                    cin >> isbn;
                    lib.returnBook(roll, isbn);
                    break;
                }
                case 6: lib.calculateFine(roll); break;
                default: cout << "Invalid option.\n";
            }
        }
    }

    return 0;
}
