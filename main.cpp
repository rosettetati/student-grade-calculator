#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "Student.h"

int main(int argc, char* argv[]) {
    // Default to 10k if no argument is provided, otherwise use the filename passed in the terminal
    std::string filename = (argc > 1) ? argv[1] : "students10000.txt";

    std::vector<Student> students;
    
    // Dynamic reserve hint based on file name to optimize memory allocation for large scales
    if (filename.find("1000000") != std::string::npos) {
        students.reserve(1000000); // Reserve for 1 Million
    } else if (filename.find("100000") != std::string::npos) {
        students.reserve(100000);  // Reserve for 100k
    } else {
        students.reserve(10000);   // Reserve for 10k / default
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    std::ifstream file(filename);
    if (!file.is_open()) {
        filename = "Students.txt"; // Fallback
        file.open(filename);
    }

    if (!file.is_open()) {
        std::cerr << "Error: Could not open data file: " << filename << "\n";
        return 1;
    }

    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string f, s;
        ss >> f >> s;
        std::vector<int> tokens;
        int score;
        while (ss >> score) {
            tokens.push_back(score);
        }
        if (!tokens.empty()) {
            int exam = tokens.back();
            tokens.pop_back();
            students.emplace_back(f, s, tokens, exam);
        }
    }
    file.close();

    // Sort students alphabetically
    std::sort(students.begin(), students.end(), Student::compareByName);

    // Split into passing (>= 5.0) and failing (< 5.0) groups
    std::vector<Student> passingStudents;
    std::vector<Student> failingStudents;
    passingStudents.reserve(students.size());
    failingStudents.reserve(students.size());

    for (const auto& student : students) {
        if (student.getFinalAvg() >= 5.0) {
            passingStudents.push_back(student);
        } else {
            failingStudents.push_back(student);
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // Output Benchmarking Results
    std::cout << "--- Version 0.3 Performance Report ---\n";
    std::cout << "Data source file: " << filename << "\n";
    std::cout << "Total students processed: " << students.size() << "\n";
    std::cout << "Passing students: " << passingStudents.size() << "\n";
    std::cout << "Failing students: " << failingStudents.size() << "\n";
    std::cout << "Total processing time (Read + Sort + Split): " << elapsed.count() << " seconds\n";

    return 0;
}