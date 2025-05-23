#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <limits>
using namespace std;

class StudentSystem {
private:
    vector<int> students;
    set<int> suspicious_students;
    set<int> immortal_students;

public:
    void addStudents(int number) {
        if (number > 0) {
            int start = students.empty() ? 1 : students.back() + 1;
            for (int i = start; i < start + number; ++i) {
                students.push_back(i);
            }
            cout << "Welcome " << number << " clever students!" << endl;
        } else if (number < 0) {
            if (abs(number) > students.size()) {
                cout << "Incorrect" << endl;
                return;
            }
            for (int i = 0; i < abs(number); ++i) {
                int student = students.back();
                students.pop_back();
                suspicious_students.erase(student);
                immortal_students.erase(student);
            }
            cout << "GoodBye " << abs(number) << " clever students!" << endl;
        }
    }

    void suspicious(int student_number) {
        if (find(students.begin(), students.end(), student_number) == students.end()) {
            cout << "Incorrect" << endl;
            return;
        }
        if (immortal_students.count(student_number) == 0) {
            suspicious_students.insert(student_number);
            cout << "The suspected student " << student_number << endl;
        }
    }

    void immortal(int student_number) {
        if (find(students.begin(), students.end(), student_number) == students.end()) {
            cout << "Incorrect" << endl;
            return;
        }
        immortal_students.insert(student_number);
        suspicious_students.erase(student_number);
        cout << "Student " << student_number << " is immortal!" << endl;
    }

    void topList() {
        cout << "List of students for expulsion:";
        bool first = true;
        for (int student : suspicious_students) {
            if (!first) cout << ",";
            cout << " Student " << student;
            first = false;
        }
        cout << endl;
    }

    void suspiciousCount() {
        cout << "List of students for expulsion consists of " << suspicious_students.size() << " students" << endl;
    }
};

int main() {
    StudentSystem system;
    int N;
    cin >> N;

    for (int i = 0; i < N; ++i) {
        string command;
        cin >> command;

        if (command == "NEW_STUDENTS") {
            int number;
            cin >> number;
            system.addStudents(number);
        } else if (command == "SUSPICIOUS") {
            int student_number;
            cin >> student_number;
            system.suspicious(student_number);
        } else if (command == "IMMORTIAL") {
            int student_number;
            cin >> student_number;
            system.immortal(student_number);
        } else if (command == "TOP-LIST") {
            system.topList();
        } else if (command == "SCOUNT") {
            system.suspiciousCount();
        } else {
            cout << "Incorrect" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}