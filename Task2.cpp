#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>

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

        // найти окно с минимальным total_time
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
    int windows_count;
    string command;
    queue<Visitor> visitors_queue;
    cout << ">>> Введите кол-во окон" << endl;
    cout << "<<< ";
    cin >> windows_count;

    while (true) {
        cout << "<<< ";
        cin >> command;
        if (command == "ENQUEUE") {
            int duration;
            cin >> duration;
            string ticket = generate_ticket();
            visitors_queue.push({ticket, duration});
            cout << ">>> " << ticket << endl;
        } else if (command == "DISTRIBUTE") {
            break;
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