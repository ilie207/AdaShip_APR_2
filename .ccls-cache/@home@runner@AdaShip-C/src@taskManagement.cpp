#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include "board.h"

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
  std::cout << std::left << std::setw(18) 
    << "Title" << std::setw(20) 
    << "Description" << std::setw(20) 
    << "Deadline" << std::setw(10) 
    << "Priority\n";

  if(user.tasks.empty()) {
    std::cout << "No tasks found.\n";
  } else {
    for (const auto& task : user.tasks) {
      
      std::tm* tm = std::localtime(&task.deadline);
      std::cout << std::left << std::setw(18)
        << task.title << std::setw(19)
        << task.description << std::setw(18)
        << std::put_time(tm, "%Y-%m-%d") 
        << std::setw(40) 
        << task.priority << "\n";
    }
  }
std::cout << "-----------------------------------\n";
}

void addTask(User& user, const std::string& title, const std::string& description, std::time_t deadline, int priority){
  user.tasks.emplace_back(title, description, deadline, priority);
}

void saveTasksToCSV(const User& user, const std::string& filename) {
  std::ofstream outfile(filename, std::ios::trunc);
  if (!outfile.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return;
  }
  
  for (const auto& task : user.tasks) {
    outfile 
      << task.title << "," 
      << task.description << ","
      << std::put_time(std::localtime(&task.deadline), "%Y-%m-%d") << "," 
      << task.priority << "\n";
  }
  outfile.close();
}

void loadTasksFromCSV(User& user, const std::string& filename) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << "\n";
    return;
  }

    std::string line;
  
  while (std::getline(infile, line)) {
    std::stringstream ss(line);
    std::string title, description, dateStr, priorityStr;
    std::getline(ss, title, ',');
    std::getline(ss, description, ',');
    std::getline(ss, dateStr, ',');
    std::getline(ss, priorityStr, ',');

    std::tm tm = {};
    std::istringstream dateStream(dateStr);
    dateStream >> std::get_time(&tm, "%Y-%m-%d");

    int priority = std::stoi(priorityStr);

    std::time_t deadline = std::mktime(&tm);

    user.tasks.emplace_back(title, description, deadline, priority);
  }
  infile.close();
}

  int main() {
    User user("John Schmoe");
    loadTasksFromCSV(user, "data/tasks.csv");

    while (true) {
      std::cout << "Welcome to the task management system \n";
      std::cout << "Please select an option: \n";
      std::cout << "1. Add Task\n";
      std::cout << "2. Display Tasks\n";
      std::cout << "3. Save Tasks to CSV\n";
      std::cout << "4. Exit\n";

      int choice;
      std::cin >> choice;

      switch(choice) {
        case 1: {
          std::string title, description;
          std::time_t deadline;
          int priority;

          std::cout << "Enter task title: ";
          std::cin.ignore();
          std::getline(std::cin, title);

          std::cout << "Enter task description: ";
          std::getline(std::cin, description);

          std::cout << "Enter task deadline (YYYY-MM-DD): ";
          std::tm tm = {};
          std::cin >> std::get_time(&tm, "%Y-%m-%d");
          deadline = std::mktime(&tm);

          std::cout << "Enter task priority: ";
          std::cin >> priority;

          addTask(user, title, description, deadline, priority);
          break;
        }
        case 2: 
        std::sort(user.tasks.begin(), user.tasks.end(), compareTasks);
        displayTasks(user);
        break;
        case 3: 
        saveTasksToCSV(user, "data/tasks.csv");
        std::cout << "Tasks saved to tasks.csv\n";
        break;
        case 4:
        return 0;
        default:
        std::cout << "Invalid choice. Please try again.\n";
      }
    }
    return 0;
}