# student-grade-calculator
C++ application designed to manage student records, calculate final grades using either average or median scores, and process data dynamically from text files or random score generators.
# Calculation Formula
The final grade for each student is computed using a weighted combination of their homework assignments and the final exam:
### $$\text{Final Grade} = (\text{Homework Score} \times 0.4) + (\text{Exam Score} \times 0.6)$$
Students can choose whether the homework score is calculated using the average or the median of their submitted homework assignments.
# Features
Object-Oriented Design: Implements Person and Student classes adhering to the Rule of Three (copy constructor, copy assignment operator, and destructor).
Dynamic Homework Handling: Uses std::vector<int> to support an arbitrary number of homework assignments.
File Processing: Automatically reads student data, homework scores, and exam results from a Students.txt input file.
Random Generation: Includes built-in support to randomly generate scores for testing and demonstration.
Formatted Output: Displays results sorted alphabetically by name with clean column alignment and two-decimal precision.
