#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <numeric>

class Person {
protected:
    std::string firstName;
    std::string surname;

public:
    Person(std::string f = "", std::string s = "") : firstName(f), surname(s) {}
    virtual ~Person() = default;
    std::string getFirstName() const { return firstName; }
    std::string getSurname() const { return surname; }
};

class Student : public Person {
private:
    std::vector<int> homeworkScores;
    int examScore;
    double finalAvg;
    double finalMed;

    double calculateAverage() const {
        if (homeworkScores.empty()) return examScore * 0.6;
        double sumHw = std::accumulate(homeworkScores.begin(), homeworkScores.end(), 0.0);
        return (sumHw / homeworkScores.size() * 0.4) + (examScore * 0.6);
    }

    double calculateMedian() const {
        if (homeworkScores.empty()) return examScore * 0.6;
        std::vector<int> temp = homeworkScores;
        std::sort(temp.begin(), temp.end());
        double medHw = 0.0;
        size_t size = temp.size();
        if (size % 2 == 0) {
            medHw = (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
        } else {
            medHw = temp[size / 2];
        }
        return (medHw * 0.4) + (examScore * 0.6);
    }

public:
    Student() : Person(), examScore(0), finalAvg(0.0), finalMed(0.0) {}

    Student(std::string f, std::string s, std::vector<int> hw, int exam)
        : Person(f, s), homeworkScores(hw), examScore(exam) {
        computeFinals();
    }

    // Rule of Three
    Student(const Student& other) = default;
    Student& operator=(const Student& other) = default;
    ~Student() override = default;

    void computeFinals() {
        finalAvg = calculateAverage();
        finalMed = calculateMedian();
    }

    double getFinalAvg() const { return finalAvg; }

    static bool compareByName(const Student& a, const Student& b) {
        if (a.firstName != b.firstName)
            return a.firstName < b.firstName;
        return a.surname < b.surname;
    }
};

int main() {
    // Set to test the 10,000 dataset file
    std::string filename = "students10000.txt";

    std::vector<Student> students;
    students.reserve(10000); // Pre-allocate memory for efficiency

    auto start_time = std::chrono::high_resolution_clock::now();

    std::ifstream file(filename);
    if (!file.is_open()) {
        filename = "Students.txt"; // Fallback
        file.open(filename);
    }

    if (!file.is_open()) {
        std::cerr << "Error: Could not open data file!\n";
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
    std::cout << "--- Performance Report ---\n";
    std::cout << "Data source file: " << filename << "\n";
    std::cout << "Total students processed: " << students.size() << "\n";
    std::cout << "Passing students: " << passingStudents.size() << "\n";
    std::cout << "Failing students: " << failingStudents.size() << "\n";
    std::cout << "Total processing time (Read + Sort + Split): " << elapsed.count() << " seconds\n";

    return 0;
}