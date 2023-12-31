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

void displayTasks(const User& user) {
  std::cout << "Tasks for user: " << user.username << "\n";
  std::cout << "----------------------------------\n";

  if(user.tasks.empty()) {
    std::cout << "No tasks found.\n";
  } else {
    std::cout << "Title\tDeadline\tPriority\n";
    for (const auto& task : user.tasks) {
      std::cout << task.title << "\t" << std::put_time(std::localtime(&task.deadline), "%Y-%m-%d") << "\t" << task.priority << "\n";
    }
  }
std::cout << "-----------------------------------\n";
}

void addTask(User& user, const std::string& title, const std::string& description, std::time_t deadline, int priority){
  user.tasks.emplace_back(title, description, deadline, priority);
}

void saveTasksToCSV(const User& user, const std::string& filename) {
  std::ofstream outfile("tasks.csv", std::ios::app);
  for (const auto& task : user.tasks) {
    outfile << task.title << "," << std::time_t (&task.deadline) << "," << task.description << "," << task.priority << "\n";
  }
  outfile.close();
}

void loadTasksFromCSV(User& user, const std::string& filename) {
  std::ifstream infile("tasks.csv");
  if (!infile.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << "\n";
    return;
  }

  std::string line;
  while (std::getline(infile, line)) {
    std::stringstream ss(line);
    std::string title, dateStr, description, priorityStr;
    std::getline(ss, title, ',');
    std::getline(ss, dateStr, ',');
    std::getline(ss, description, ',');
    std::getline(ss, priorityStr, ',');

    std::tm tm = {};
    std::istringstream dateStream(dateStr);
    dateStream >> std::get_time(&tm, "%c");

    int priority = std::stoi(priorityStr);

    std::time_t deadline = std::mktime(&tm);

    user.tasks.emplace_back(title, description, deadline, priority);
  }
  infile.close();
}

    int main() {

  std::cout << "Hello World!\n";
}