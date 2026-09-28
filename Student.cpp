#include "Student.h"
#include <numeric>
#include <algorithm>

double Student::calculateAverage() const {
    if (homeworkScores.empty()) return examScore * 0.6;
    double sumHw = std::accumulate(homeworkScores.begin(), homeworkScores.end(), 0.0);
    return (sumHw / homeworkScores.size() * 0.4) + (examScore * 0.6);
}

double Student::calculateMedian() const {
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

Student::Student() : Person(), examScore(0), finalAvg(0.0), finalMed(0.0) {}

Student::Student(std::string f, std::string s, std::vector<int> hw, int exam)
    : Person(f, s), homeworkScores(hw), examScore(exam) {
    computeFinals();
}

void Student::computeFinals() {
    finalAvg = calculateAverage();
    finalMed = calculateMedian();
}

double Student::getFinalAvg() const { return finalAvg; }

bool Student::compareByName(const Student& a, const Student& b) {
    if (a.firstName != b.firstName)
        return a.firstName < b.firstName;
    return a.surname < b.surname;
}