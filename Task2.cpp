#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>
using namespace std;

struct Visitor {
    string ticket;
    int duration;
};

struct Window {
    int total_time = 0;
    vector<Visitor> visitors;
};

string generate_ticket() {
    static int counter = 1;
    stringstream ss;
    ss << "T" << setw(3) << setfill('0') << counter++;
    return ss.str();
}

vector<Window> distribute_queue(queue<Visitor>& q, int windows_count) {
    vector<Window> windows(windows_count);

    while (!q.empty()) {
        Visitor visitor = q.front();
        q.pop();

        auto min_window = min_element(windows.begin(), windows.end(),
            [](const Window& a, const Window& b) {
                return a.total_time < b.total_time;
            });

        min_window->visitors.push_back(visitor);
        min_window->total_time += visitor.duration;
    }
    return windows;
}

int main() {
    int windows_count = 0;
    string command;
    queue<Visitor> visitors_queue;
    
    while (true) {
        cout << ">>> Введите кол-во окон" << endl;
        cout << "<<< ";
        try {
            cin >> windows_count;
            if (cin.fail() || windows_count <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Неверный ввод. Введите положительное целое число");
            }
            break;
        } catch (const exception& e) {
            cerr << ">>> Ошибка: " << e.what() << endl;
        }
    }

    while (true) {
        cout << "<<< ";
        cin >> command;
        
        if (command == "ENQUEUE") {
            while (true) {
                int duration;
                try {
                    cin >> duration;
                    if (cin.fail() || duration <= 0) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw invalid_argument("Неверный ввод. Введите положительное целое число");
                    }
                    
                    string ticket = generate_ticket();
                    visitors_queue.push({ticket, duration});
                    cout << ">>> " << ticket << endl;
                    break;
                } catch (const exception& e) {
                    cerr << ">>> Ошибка: " << e.what() << endl;
                }
            }
        } 
        else if (command == "DISTRIBUTE") {
            break;
        } 
        else {
            cout << ">>> Неизвестная команда. Используйте ENQUEUE или DISTRIBUTE" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    vector<Window> windows = distribute_queue(visitors_queue, windows_count);

    for (size_t i = 0; i < windows.size(); ++i) {
        cout << ">>> Окно " << i + 1 << " (" << windows[i].total_time << " минут): ";
        for (size_t j = 0; j < windows[i].visitors.size(); ++j) {
            if (j != 0) cout << ", ";
            cout << windows[i].visitors[j].ticket;
        }
        cout << endl;
    }
    return 0;
}
