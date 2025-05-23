#include "trolley.h"
#include <iostream>
#include <algorithm>
using namespace std;

void TrolleySystem::createTrolley(const string& name, const vector<string>& stopsList) {
    trolleys[name] = stopsList;

    for (const auto& stop : stopsList) {
        stops[stop].push_back(name);
    }
}

void TrolleySystem::trolleysForStop(const string& stop) const {
    if (stops.empty()) {
        cout << "Stops is absent" << endl;
        return;
    }

    auto it = stops.find(stop);
    if (it == stops.end()) {
        cout << "Stop " << stop << ": 0" << endl;
        return;
    }

    cout << stop << ": ";
    for (size_t i = 0; i < it->second.size(); ++i) {
        if (i != 0) cout << " ";
        cout << it->second[i];
    }
    cout << endl;
}

void TrolleySystem::stopsForTrolley(const string& trolley) const {
    if (trolleys.empty()) {
        cout << "Trolleys is absent" << endl;
        return;
    }

    auto it = trolleys.find(trolley);
    if (it == trolleys.end()) {
        cout << "Trolley " << trolley << " is absent" << endl;
        return;
    }

    for (const auto& stop : it->second) {
        cout << "Stop " << stop << ": ";
        bool first = true;
        for (const auto& trl : stops.at(stop)) {
            if (trl != trolley) {
                if (!first) cout << " ";
                cout << trl;
                first = false;
            }
        }
        if (first) cout << "0";
        cout << endl;
    }
}

void TrolleySystem::allTrolleys() const {
    if (trolleys.empty()) {
        cout << "Trolleys is absent" << endl;
        return;
    }

    for (const auto& [name, stopsList] : trolleys) {
        cout << "TRL " << name << ": ";
        for (size_t i = 0; i < stopsList.size(); ++i) {
            if (i != 0) cout << " ";
            cout << stopsList[i];
        }
        cout << endl;
    }
}

bool TrolleySystem::isTrolleyExist(const string& name) const {
    return trolleys.find(name) != trolleys.end();
}

bool TrolleySystem::isStopExist(const string& name) const {
    return stops.find(name) != stops.end();
}

CommandType parseCommand(const string& cmd) {
    if (cmd == "CREATE_TRL") return CommandType::CREATE_TRL;
    if (cmd == "TRL_IN_STOP") return CommandType::TRL_IN_STOP;
    if (cmd == "STOPS_IN_TRL") return CommandType::STOPS_IN_TRL;
    if (cmd == "TRLS") return CommandType::TRLS;
    throw invalid_argument("Unknown command");
}