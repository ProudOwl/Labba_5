#define TROLLEY_H

#include <vector>
#include <string>
#include <map>
using namespace std;

enum class CommandType {
    CREATE_TRL,
    TRL_IN_STOP,
    STOPS_IN_TRL,
    TRLS
};

class TrolleySystem {
private:
    map<std::string, vector<std::string>> trolleys;
    map<std::string, vector<std::string>> stops;

public:
    void createTrolley(const string& name, const vector<std::string>& stopsList);
    void trolleysForStop(const string& stop) const;
    void stopsForTrolley(const string& trolley) const;
    void allTrolleys() const;
    bool isTrolleyExist(const string& name) const;
    bool isStopExist(const string& name) const;
};

CommandType parseCommand(const string& cmd);