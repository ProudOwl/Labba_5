#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "trolley.h"
using namespace std;

vector<string> split(const string& s) {
    vector<string> result;
    istringstream iss(s);
    string token;
    while (iss >> token) {
        result.push_back(token);
    }
    return result;
}

int main() {
    TrolleySystem system;
    string line;

    while (getline(cin, line)) {
        if (line.empty()) continue;

        vector<string> tokens = split(line);
        if (tokens.empty()) continue;

        try {
            CommandType cmd = parseCommand(tokens[0]);

            switch (cmd) {
                case CommandType::CREATE_TRL: {
                    if (tokens.size() < 3) {
                        cout << "Invalid command format" << endl;
                        break;
                    }
                    string name = tokens[1];
                    vector<string> stops(tokens.begin() + 2, tokens.end());
                    system.createTrolley(name, stops);
                    break;
                }
                case CommandType::TRL_IN_STOP: {
                    if (tokens.size() != 2) {
                        cout << "Invalid command format" << endl;
                        break;
                    }
                    system.trolleysForStop(tokens[1]);
                    break;
                }
                case CommandType::STOPS_IN_TRL: {
                    if (tokens.size() != 2) {
                        cout << "Invalid command format" << endl;
                        break;
                    }
                    system.stopsForTrolley(tokens[1]);
                    break;
                }
                case CommandType::TRLS: {
                    system.allTrolleys();
                    break;
                }
            }
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    return 0;
}