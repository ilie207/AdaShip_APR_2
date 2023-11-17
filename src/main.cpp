#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

char *mySecret = getenv("GITHUB_TOKEN");

class Task {
public:
std::string title;
std::string description;
std::time_t deadline;
int priority;

Task(const std::string& t, const std::string& desc, std::time_t dl, int p) : title(t), description(desc), deadline(dl), priority(p) {}
};

class User {
public:
std::string username;
std::vector<Task> tasks;

User(const std::string& name) : username(name) {}
};

bool compareTasks(const Task& a, const Task& b) {
  return a.deadline < b.deadline;
}

    int main() {

  std::cout << "Hello World!\n";
}