#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include <vector>
#include <string>

class Student : public Person {
private:
    std::vector<int> homeworkScores;
    int examScore;
    double finalAvg;
    double finalMed;

    double calculateAverage() const;
    double calculateMedian() const;

public:
    Student();
    Student(std::string f, std::string s, std::vector<int> hw, int exam);

    // Rule of Three
    Student(const Student& other) = default;
    Student& operator=(const Student& other) = default;
    ~Student() override = default;

    void computeFinals();
    double getFinalAvg() const;

    static bool compareByName(const Student& a, const Student& b);
};

#endif // STUDENT_H