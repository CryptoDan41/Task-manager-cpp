#include <iostream>
#include <windows.h>
#include <vector>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

namespace {
    struct Task {
        string taskName;
        char taskType = ' ';
    };
}

static int GetNumberFromUser();

static int GetIntFromUser();

static string GetStringFromUser();

static void Tasks(const vector<Task> &tasks);

static string trim(const string &str);

static bool PutTaskToFile(const vector<Task> &tasks, const string &fileName);

static bool GetTaskFromFile(vector<Task> &tasks, const string &fileName);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const string fileName = "database.txt";
    vector<Task> tasks;
    GetTaskFromFile(tasks, fileName);

    while (true) {
        cout << endl << "=== Менеджер задач ===" << endl << endl;

        cout << "1. Добавить задачу" << endl;
        cout << "2. Показать задачи" << endl;
        cout << "3. Выполнить задачу" << endl;
        cout << "4. Удалить задачу" << endl;
        cout << "0. Выход" << endl << endl;

        cout << "Введите действие: ";

        switch (GetNumberFromUser()) {
            case 1: {
                string userTask = GetStringFromUser();
                tasks.emplace_back(Task{userTask, ' '});
                cout << "Задача добавлена..." << endl;

                PutTaskToFile(tasks, fileName);
                break;
            }
            case 2:
                cout << "=== МОИ ЗАДАЧИ ===" << endl << endl;
                if (!tasks.empty()) {
                    Tasks(tasks);
                    cout << "[ ] = не выполнено" << endl;
                    cout << "[X] = выполнено" << endl;
                } else {
                    cout << "Список задач пуст" << endl;
                }
                break;
            case 3: {
                if (!tasks.empty()) {
                    Tasks(tasks);

                    cout << "Введите номер выполненной задачи: ";
                    int number = GetIntFromUser();

                    if (number > tasks.size() || number <= 0) {
                        cout << "Такой задачи нет..." << endl;
                    } else {
                        tasks[number - 1].taskType = 'X';
                        cout << "Задача отмечена как выполненная..." << endl;
                        PutTaskToFile(tasks, fileName);
                    }
                } else {
                    cout << "Список задач пуст" << endl;
                }
                break;
            }
            case 4:
                if (!tasks.empty()) {
                    Tasks(tasks);

                    cout << "Введите номер задачи для удаления: ";
                    int number = GetIntFromUser();

                    if (number > tasks.size() || number <= 0) {
                        cout << "Такой задачи нет..." << endl;
                    } else {
                        tasks.erase(tasks.begin() + (number - 1));
                        cout << "Задача удалена..." << endl;
                        PutTaskToFile(tasks, fileName);
                    }
                } else {
                    cout << "Список задач пуст" << endl;
                }
                break;
            default:
                cout << "До свидания!" << endl;
                return 0;
        }
    }
    return 0;
}

int GetNumberFromUser() {
    int number;
    while (true) {
        if (cin >> number && number <= 4 && number >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        }
        cout << "Введите число от 0 до 4: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Tasks(const vector<Task> &tasks) {
    for (size_t i = 0; i < tasks.size(); ++i) {
        cout << i + 1 << ". [" << tasks[i].taskType << "] " << tasks[i].taskName << endl;
    }
    cout << endl;
}

int GetIntFromUser() {
    int number;
    while (true) {
        if (cin >> number) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        }
        cout << "Введите корректный номер задачи: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string GetStringFromUser() {
    string userInput;
    while (true) {
        cout << "Введите задачу: ";
        getline(cin, userInput);

        if (string cleanTask = trim(userInput); cleanTask.empty()) {
            cout << "Ошибка: вы ничего не ввели или ввели одни пробелы!" << endl << endl;
        } else {
            return cleanTask;
        }
    }
}

string trim(const string &str) {
    const size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) {
        return "";
    }
    const size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool PutTaskToFile(const vector<Task> &tasks, const string &fileName) {
    ofstream outFile(fileName);
    if (!outFile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << fileName << " для записи!\n";
        return false;
    }

    outFile << tasks.size() << "\n";
    for (const auto &[taskName, taskType]: tasks) {
        outFile << taskName << "\n";
        outFile << taskType << "\n";
    }

    outFile.close();
    return true;
}

bool GetTaskFromFile(vector<Task> &tasks, const string &fileName) {
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        return false;
    }

    if (size_t count = 0; inFile >> count) {
        inFile.ignore();
        tasks.clear();
        tasks.resize(count);

        for (size_t i = 0; i < count; ++i) {
            getline(inFile, tasks[i].taskName);
            inFile.get(tasks[i].taskType);
            inFile.ignore();
        }
    }

    inFile.close();
    return true;
}
