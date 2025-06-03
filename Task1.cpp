#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

class Warehouse {
private:
    struct Cell {
        string address;
        string product;
        int quantity;
    };

    int zones;
    int shelves_per_zone;
    int sections_per_shelf;
    int shelves_per_section;
    int total_capacity;
    int used_capacity = 0;

    map<string, Cell> cells;

public:
    Warehouse(int z, int spz, int sec, int spl, int cap) 
        : zones(z), shelves_per_zone(spz), sections_per_shelf(sec), 
          shelves_per_section(spl), total_capacity(cap) {}

    bool Address(const string& address) {
        if (address.length() != 5) return false;
        
        char zone = address[0];
        if (zone < 'A' || zone >= 'A' + zones) return false;
        
        int shelf = stoi(address.substr(1, 2));
        if (shelf < 1 || shelf > shelves_per_zone) return false;
        
        int section = address[3] - '0';
        if (section < 1 || section > sections_per_shelf) return false;
        
        int shelf_num = address[4] - '0';
        if (shelf_num < 1 || shelf_num > shelves_per_section) return false;
        
        return true;
    }

    void ADD(const string& product, int quantity, const string& address) {
        if (!Address(address)) {
            cout << "Ошибка: Неверный адрес: " << address << endl;
            return;
        }

        if (quantity > 10) {
            cout << "Ошибка: Нельзя добавить более 10 единиц в ячейку" << endl;
            return;
        }

        if (cells.count(address)) {
            Cell& cell = cells[address];
            if (cell.product != product) {
                cout << "Ошибка: Ячейка " << address << " уже содержит " << cell.product << endl;
                return;
            }
            if (cell.quantity + quantity > 10) {
                cout << "Ошибка: Ячейка " << address << " не может содержать более 10 единиц (сейчас: " << cell.quantity << ")" << endl;
                return;
            }
            cell.quantity += quantity;
        } else {
            cells[address] = {address, product, quantity};
        }
        used_capacity += quantity;
        cout << "Добавлено " << quantity << " единиц " << product << " в " << address << endl;
    }

    void REMOVE(const string& product, int quantity, const string& address) {
        if (!Address(address)) {
            cout << "Ошибка: Неверный адрес: " << address << endl;
            return;
        }

        if (!cells.count(address)) {
            cout << "Ошибка: Ячейка " << address << " пуста" << endl;
            return;
        }

        Cell& cell = cells[address];
        if (cell.product != product) {
            cout << "Ошибка: Ячейка " << address << " содержит " << cell.product << ", а не " << product << endl;
            return;
        }

        if (cell.quantity < quantity) {
            cout << "Ошибка: Недостаточно " << product << " в ячейке " << address << " (доступно: " << cell.quantity << ")" << endl;
            return;
        }

        cell.quantity -= quantity;
        used_capacity -= quantity;
        cout << "Удалено " << quantity << " единиц " << product << " из " << address << endl;

        if (cell.quantity == 0) {
            cells.erase(address);
        }
    }

    void INFO() {
        double total_percent = (static_cast<double>(used_capacity) / total_capacity) * 100;
        cout << fixed << setprecision(1);
        cout << "Информация о складе:" << endl;
        cout << "Общая заполненность: " << total_percent << "%" << endl;

        map<char, int> zone_capacity;
        map<char, int> zone_used;
        
        for (char zone = 'A'; zone < 'A' + zones; ++zone) {
            zone_capacity[zone] = shelves_per_zone * sections_per_shelf * shelves_per_section * 10;
            zone_used[zone] = 0;
        }

        for (const auto& pair : cells) {
            char zone = pair.first[0];
            zone_used[zone] += pair.second.quantity;
        }

        for (char zone = 'A'; zone < 'A' + zones; ++zone) {
            double zone_percent = (static_cast<double>(zone_used[zone]) / zone_capacity[zone]) * 100;
            cout << "Зона " << zone << " заполнена на " << zone_percent << "%" << endl;
        }

        cout << "\nЗанятые ячейки:" << endl;
        for (const auto& pair : cells) {
            cout << pair.first << ": " << pair.second.product << " (" << pair.second.quantity << ")" << endl;
        }

        int total_cells = zones * shelves_per_zone * sections_per_shelf * shelves_per_section;
        int empty_cells = total_cells - cells.size();
        cout << "\nПустые ячейки: " << empty_cells << endl;
    }
};

int main() {
    Warehouse warehouse(1, 10, 7, 4, 2800);
    string command;
    while (true) {
        cout << ">>> ";
        if (!getline(cin, command)) {
            cin.clear();
            cout << "Ошибка: Неправильный ввод команды" << endl;
            continue;
        }

        if (command.empty()) {
            continue;
        }

        // Разбиваем команду на части
        size_t space1 = command.find(' ');
        string cmd = command.substr(0, space1);

        if (cmd == "ADD") {
            size_t space2 = command.find(' ', space1 + 1);
            if (space1 == string::npos || space2 == string::npos) {
                cout << "Ошибка: Неправильный формат команды ADD. Используйте: ADD <продукт> <количество> <адрес>" << endl;
                continue;
            }

            string product = command.substr(space1 + 1, space2 - space1 - 1);
            string rest = command.substr(space2 + 1);

            size_t space3 = rest.find(' ');
            if (space3 == string::npos) {
                cout << "Ошибка: Неправильный формат команды ADD. Используйте: ADD <продукт> <количество> <адрес>" << endl;
                continue;
            }

            string quantityStr = rest.substr(0, space3);
            string address = rest.substr(space3 + 1);

            try {
                int quantity = stoi(quantityStr);
                warehouse.ADD(product, quantity, address);
            } catch (...) {
                cout << "Ошибка: Неправильный формат количества. Должно быть целое число" << endl;
            }
        } 
        else if (cmd == "REMOVE") {
            size_t space2 = command.find(' ', space1 + 1);
            if (space1 == string::npos || space2 == string::npos) {
                cout << "Ошибка: Неправильный формат команды REMOVE. Используйте: REMOVE <продукт> <количество> <адрес>" << endl;
                continue;
            }

            string product = command.substr(space1 + 1, space2 - space1 - 1);
            string rest = command.substr(space2 + 1);

            size_t space3 = rest.find(' ');
            if (space3 == string::npos) {
                cout << "Ошибка: Неправильный формат команды REMOVE. Используйте: REMOVE <продукт> <количество> <адрес>" << endl;
                continue;
            }

            string quantityStr = rest.substr(0, space3);
            string address = rest.substr(space3 + 1);

            try {
                int quantity = stoi(quantityStr);
                warehouse.REMOVE(product, quantity, address);
            } catch (...) {
                cout << "Ошибка: Неправильный формат количества. Должно быть целое число" << endl;
            }
        }
        else if (cmd == "INFO") {
            warehouse.INFO();
        }
        else if (cmd == "EXIT") {
            break;
        }
        else {
            cout << "Ошибка: Неизвестная команда '" << cmd << "'. Доступные команды: ADD, REMOVE, INFO, EXIT" << endl;
        }
    }
    return 0;
}
