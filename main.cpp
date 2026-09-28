#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <iomanip>
#include <numeric>

class Person {
protected:
    std::string firstName;
    std::string surname;

public:
    // Constructor
    Person(std::string f = "", std::string s = "") : firstName(f), surname(s) {}

    // Virtual destructor for safe inheritance
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

    // Helper to calculate average
    double calculateAverage() const {
        if (homeworkScores.empty()) return examScore * 0.6; // fallback if no HW
        double sumHw = std::accumulate(homeworkScores.begin(), homeworkScores.end(), 0.0);
        double avgHw = sumHw / homeworkScores.size();
        return (avgHw * 0.4) + (examScore * 0.6);
    }

    // Helper to calculate median
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
    // Default constructor
    Student() : Person(), examScore(0), finalAvg(0.0), finalMed(0.0) {}

    // Parameterized constructor
    Student(std::string f, std::string s, std::vector<int> hw, int exam)
        : Person(f, s), homeworkScores(hw), examScore(exam) {
        computeFinals();
    }

    // --- Rule of Three Implementation ---

    // 1. Copy Constructor
    Student(const Student& other)
        : Person(other.firstName, other.surname),
          homeworkScores(other.homeworkScores),
          examScore(other.examScore),
          finalAvg(other.finalAvg),
          finalMed(other.finalMed) {}

    // 2. Copy Assignment Operator
    Student& operator=(const Student& other) {
        if (this != &other) {
            Person::operator=(other);
            homeworkScores = other.homeworkScores;
            examScore = other.examScore;
            finalAvg = other.finalAvg;
            finalMed = other.finalMed;
        }
        return *this;
    }

    // 3. Destructor
    ~Student() override = default;

    // --- Computation ---
    void computeFinals() {
        finalAvg = calculateAverage();
        finalMed = calculateMedian();
    }

    // --- Overloaded Input (cin) ---
    friend std::istream& operator>>(std::istream& is, Student& s) {
        std::cout << "Enter First Name and Surname: ";
        is >> s.firstName >> s.surname;

        s.homeworkScores.clear();
        std::cout << "Enter homework scores (-1 to stop): ";
        int score;
        while (is >> score && score != -1) {
            s.homeworkScores.push_back(score);
        }

        std::cout << "Enter Exam score: ";
        is >> s.examScore;

        s.computeFinals();
        return is;
    }

    // --- Overloaded Output (cout) ---
    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        os << std::left << std::setw(12) << s.firstName
           << std::setw(12) << s.surname
           << std::setw(15) << std::fixed << std::setprecision(2) << s.finalAvg
           << std::setw(12) << s.finalMed;
        return os;
    }

    // Random generator for scores
    void generateRandomScores(int numHw) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, 10);

        homeworkScores.clear();
        for (int i = 0; i < numHw; ++i) {
            homeworkScores.push_back(distrib(gen));
        }
        examScore = distrib(gen);
        computeFinals();
    }

    // Sorting comparator by name
    static bool compareByName(const Student& a, const Student& b) {
        if (a.firstName != b.firstName)
            return a.firstName < b.firstName;
        return a.surname < b.surname;
    }
};

int main() {
    std::vector<Student> students;

    // Option to read from "Students.txt" if available, or populate/demo
    std::ifstream file("Students.txt");
    if (file.is_open()) {
        std::string line;
        // Skip header
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string f, s;
            ss >> f >> s;
            std::vector<int> hw;
            int score;
            // Read all tokens except the last one (exam)
            std::vector<int> tokens;
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
    } else {
        // Fallback demo: Create sample students with random scores if file doesn't exist yet
        students.emplace_back("John", "Doe", std::vector<int>(), 0);
        students.back().generateRandomScores(5);

        students.emplace_back("Alice", "Smith", std::vector<int>(), 0);
        students.back().generateRandomScores(5);
    }

    // Sort students by name
    std::sort(students.begin(), students.end(), Student::compareByName);

    // Display formatted results
    std::cout << std::left << std::setw(12) << "Name"
              << std::setw(12) << "Surname"
              << std::setw(15) << "Final (Avg.)"
              << std::setw(12) << "Final (Med.)" << "\n";
    std::cout << "----------------------------------------------------\n";

    for (const auto& student : students) {
        std::cout << student << "\n";
    }

    return 0;
}