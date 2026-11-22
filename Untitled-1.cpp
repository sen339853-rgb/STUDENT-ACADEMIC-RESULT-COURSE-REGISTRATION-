/**
 * ========================================================================================
 * PROJECT TITLE: STUDENT ACADEMIC RESULT & COURSE REGISTRATION SYSTEM (STUDENT ACADEMIC PORTAL)
 * ACADEMIC CONTEXT: Phase III College Project-Based Learning (PBL)
 * LANGUAGE: Pure C++ (C++11/C++14/C++17 Compatible)
 * 
 * CORE CONCEPTS DEMONSTRATED:
 *  1. C Programming Concepts:
 *     - Dynamic memory allocation & pointer-based Singly Linked List (struct StudentNode* head)
 *     - Low-level pointer traversal with memory address tracing
 *     - Modular record-based file handling streams
 *  2. C++ Object-Oriented Programming (OOP) Pillars:
 *     - Encapsulation: Private & protected data members with validation in getters/setters
 *     - Inheritance: `class Student : public Person`
 *     - Polymorphism: Pure virtual methods (displayDetails, serialize, deserialize)
 *     - Abstraction: Separation of interface from implementation
 *  3. Data Structures & Algorithms (DSA):
 *     - Singly Linked List (insertHead, insertTail, deleteByEnrollment, searchByEnrollment)
 *     - Searching: Linear Search O(N) vs Binary Search O(log N) with comparison counters
 *     - Sorting: QuickSort O(N log N), Insertion Sort O(N^2), Bubble Sort O(N^2)
 *  4. Automated Course Prerequisite Verification Engine:
 *     - Cross-verifies past academic transcripts before allowing enrollment
 *     - Enforces passing criteria: Minimum Grade C (40%)
 *     - Prevents duplicate registrations
 *  5. Academic Result Processing & Official Marksheet Generator:
 *     - UGC/AICTE 10-Point Grading Scale (O, A+, A, B+, B, C, F)
 *     - Semester Grade Point Average (SGPA) calculation: Sum(Credits * GP) / Sum(Credits)
 *     - ASCII University Transcript with division classification and signatures
 *  6. File Handling (Persistent Storage):
 *     - Auto-seeds 8 students, 8 courses, results, and registrations into text files
 *  7. Automated Diagnostic Test Suite:
 *     - 8 end-to-end integration assertions for college faculty viva demonstration
 * ========================================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>

// ========================================================================================
// 1. BASE CLASS: Person (Demonstrates Encapsulation, Abstract Base, & Virtual Polymorphism)
// ========================================================================================
class Person {
protected:
    std::string id;
    std::string name;
    std::string email;
    std::string phone;

public:
    Person() : id(""), name(""), email(""), phone("") {}

    Person(const std::string& id, const std::string& name, const std::string& email, const std::string& phone)
        : id(id), name(name), email(email), phone(phone) {}

    virtual ~Person() {}

    // Encapsulated Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }

    // Validated Setters
    void setName(const std::string& n) {
        if (n.length() >= 2) name = n;
    }
    void setEmail(const std::string& e) {
        if (e.find('@') != std::string::npos) email = e;
    }
    void setPhone(const std::string& p) {
        if (p.length() >= 7) phone = p;
    }

    // Pure virtual methods to be overridden by derived classes (Polymorphism)
    virtual void displayDetails() const = 0;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& line) = 0;
};

// ========================================================================================
// 2. DERIVED CLASS: Student (Demonstrates Inheritance, Polymorphism, & Data Hiding)
// ========================================================================================
class Student : public Person {
private:
    std::string enrollmentNo;
    std::string rollNo;
    std::string branch;
    int semester;

public:
    Student() : Person(), enrollmentNo(""), rollNo(""), branch(""), semester(1) {}

    Student(const std::string& name, const std::string& enrollmentNo, const std::string& rollNo,
            const std::string& branch, int semester, const std::string& email, const std::string& phone)
        : Person(enrollmentNo, name, email, phone),
          enrollmentNo(enrollmentNo), rollNo(rollNo), branch(branch), semester(semester) {}

    // Getters
    std::string getEnrollmentNo() const { return enrollmentNo; }
    std::string getRollNo() const { return rollNo; }
    std::string getBranch() const { return branch; }
    int getSemester() const { return semester; }

    // Setters
    void setRollNo(const std::string& r) { rollNo = r; }
    void setBranch(const std::string& b) { branch = b; }
    void setSemester(int s) {
        if (s >= 1 && s <= 8) semester = s;
    }

    // Polymorphic display override
    void displayDetails() const override {
        std::cout << "┌────────────────────────────────────────────────────────┐\n";
        std::cout << "│ Student Profile: " << std::left << std::setw(37) << name << "│\n";
        std::cout << "├──────────────────────────┬─────────────────────────────┤\n";
        std::cout << "│ Enrollment: " << std::left << std::setw(13) << enrollmentNo 
                  << "│ Roll No:  " << std::left << std::setw(17) << rollNo << "│\n";
        std::cout << "│ Branch:     " << std::left << std::setw(13) << branch 
                  << "│ Semester: " << std::left << std::setw(17) << ("Semester " + std::to_string(semester)) << "│\n";
        std::cout << "│ Email:      " << std::left << std::setw(39) << email << "│\n";
        std::cout << "│ Contact:    " << std::left << std::setw(39) << phone << "│\n";
        std::cout << "└──────────────────────────┴─────────────────────────────┘\n";
    }

    // Serialization format: enrollment|roll|name|branch|semester|email|phone
    std::string serialize() const override {
        std::stringstream ss;
        ss << enrollmentNo << "|" << rollNo << "|" << name << "|" << branch << "|"
           << semester << "|" << email << "|" << phone;
        return ss.str();
    }

    void deserialize(const std::string& line) override {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        if (tokens.size() >= 7) {
            enrollmentNo = tokens[0];
            id = enrollmentNo;
            rollNo = tokens[1];
            name = tokens[2];
            branch = tokens[3];
            semester = std::stoi(tokens[4]);
            email = tokens[5];
            phone = tokens[6];
        }
    }
};

// ========================================================================================
// 3. ENTITY CLASS: Course (Curriculum Catalog & Prerequisite Mapping)
// ========================================================================================
class Course {
private:
    std::string courseCode;
    std::string courseName;
    int credits;
    int semester;
    std::string prerequisiteCode;

public:
    Course() : courseCode(""), courseName(""), credits(3), semester(1), prerequisiteCode("NONE") {}

    Course(const std::string& code, const std::string& name, int credits, int semester, const std::string& prereq = "NONE")
        : courseCode(code), courseName(name), credits(credits), semester(semester), prerequisiteCode(prereq) {}

    std::string getCourseCode() const { return courseCode; }
    std::string getCourseName() const { return courseName; }
    int getCredits() const { return credits; }
    int getSemester() const { return semester; }
    std::string getPrerequisiteCode() const { return prerequisiteCode; }

    bool hasPrerequisite() const {
        return !prerequisiteCode.empty() && prerequisiteCode != "NONE";
    }

    void setCourseName(const std::string& name) { courseName = name; }
    void setCredits(int c) { if (c >= 1 && c <= 6) credits = c; }
    void setSemester(int s) { if (s >= 1 && s <= 8) semester = s; }
    void setPrerequisiteCode(const std::string& p) { prerequisiteCode = p.empty() ? "NONE" : p; }

    void display() const {
        std::cout << "│ " << std::left << std::setw(10) << courseCode
                  << "│ " << std::left << std::setw(34) << (courseName.length() > 33 ? courseName.substr(0, 31) + ".." : courseName)
                  << "│ " << std::setw(7) << credits
                  << "│ " << std::setw(8) << semester
                  << "│ " << std::left << std::setw(13) << prerequisiteCode << "│\n";
    }

    std::string serialize() const {
        std::stringstream ss;
        ss << courseCode << "|" << courseName << "|" << credits << "|" << semester << "|" << prerequisiteCode;
        return ss.str();
    }

    void deserialize(const std::string& line) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        if (tokens.size() >= 5) {
            courseCode = tokens[0];
            courseName = tokens[1];
            credits = std::stoi(tokens[2]);
            semester = std::stoi(tokens[3]);
            prerequisiteCode = tokens[4];
        }
    }
};

// ========================================================================================
// 4. ENTITY & ENGINE: Result (UGC 10-Point Scale, Marksheet Generator, SGPA Computation)
// ========================================================================================
struct SubjectMarks {
    std::string courseCode;
    std::string courseName;
    int credits;
    int internalMarks; // Max 40
    int externalMarks; // Max 60
    int totalMarks;    // Max 100
    std::string grade;
    int gradePoint;
    std::string status; // PASS or FAIL
};

class Result {
private:
    std::string enrollmentNo;
    int semester;
    std::vector<SubjectMarks> subjects;
    std::string remarks;

public:
    Result() : enrollmentNo(""), semester(1), remarks("") {}

    Result(const std::string& enrollmentNo, int semester, const std::string& remarks = "")
        : enrollmentNo(enrollmentNo), semester(semester), remarks(remarks) {}

    std::string getEnrollmentNo() const { return enrollmentNo; }
    int getSemester() const { return semester; }
    const std::vector<SubjectMarks>& getSubjects() const { return subjects; }

    static void computeGrade(int marks, std::string& grade, int& point, std::string& status) {
        marks = std::max(0, std::min(100, marks));
        if (marks >= 90) { grade = "O"; point = 10; status = "PASS"; }
        else if (marks >= 80) { grade = "A+"; point = 9; status = "PASS"; }
        else if (marks >= 70) { grade = "A";  point = 8; status = "PASS"; }
        else if (marks >= 60) { grade = "B+"; point = 7; status = "PASS"; }
        else if (marks >= 50) { grade = "B";  point = 6; status = "PASS"; }
        else if (marks >= 40) { grade = "C";  point = 5; status = "PASS"; }
        else { grade = "F"; point = 0; status = "FAIL"; }
    }

    void addSubject(const std::string& code, const std::string& name, int credits, int internalM, int externalM) {
        SubjectMarks sub;
        sub.courseCode = code;
        sub.courseName = name;
        sub.credits = credits;
        sub.internalMarks = std::max(0, std::min(40, internalM));
        sub.externalMarks = std::max(0, std::min(60, externalM));
        sub.totalMarks = sub.internalMarks + sub.externalMarks;
        computeGrade(sub.totalMarks, sub.grade, sub.gradePoint, sub.status);

        for (auto& s : subjects) {
            if (s.courseCode == code) {
                s = sub;
                return;
            }
        }
        subjects.push_back(sub);
    }

    bool hasPassedCourse(const std::string& code, int& marksObtained, std::string& gradeSecured) const {
        for (const auto& s : subjects) {
            if (s.courseCode == code) {
                marksObtained = s.totalMarks;
                gradeSecured = s.grade;
                return (s.status == "PASS");
            }
        }
        return false;
    }

    bool hasTakenCourse(const std::string& code) const {
        for (const auto& s : subjects) {
            if (s.courseCode == code) return true;
        }
        return false;
    }

    void calculateSummary(int& totalObtained, int& maxMarks, double& percentage,
                          int& totalCredits, double& sgpa, std::string& resultStatus, std::string& division) const {
        totalObtained = 0;
        maxMarks = subjects.size() * 100;
        totalCredits = 0;
        int weightedPoints = 0;
        bool hasFailed = false;

        for (const auto& s : subjects) {
            totalObtained += s.totalMarks;
            totalCredits += s.credits;
            weightedPoints += (s.credits * s.gradePoint);
            if (s.status == "FAIL") hasFailed = true;
        }

        percentage = maxMarks > 0 ? (static_cast<double>(totalObtained) / maxMarks) * 100.0 : 0.0;
        sgpa = totalCredits > 0 ? static_cast<double>(weightedPoints) / totalCredits : 0.0;
        resultStatus = hasFailed ? "FAIL" : "PASS";

        if (hasFailed) division = "FAIL / REAPPEAR";
        else if (percentage >= 75.0) division = "FIRST CLASS WITH DISTINCTION";
        else if (percentage >= 60.0) division = "FIRST CLASS";
        else if (percentage >= 50.0) division = "SECOND CLASS";
        else division = "PASS CLASS";
    }

    void displayMarksheet(const std::string& studentName, const std::string& rollNo, const std::string& branch) const {
        int totalObtained, maxMarks, totalCredits;
        double percentage, sgpa;
        std::string resultStatus, division;
        calculateSummary(totalObtained, maxMarks, percentage, totalCredits, sgpa, resultStatus, division);

        std::cout << "\n========================================================================================\n";
        std::cout << "                        NATIONAL INSTITUTE OF TECHNOLOGY\n";
        std::cout << "                     OFFICE OF CONTROLLER OF EXAMINATIONS\n";
        std::cout << "                      SEMESTER GRADE REPORT / MARKSHEET\n";
        std::cout << "========================================================================================\n";
        std::cout << " Student Name:  " << std::left << std::setw(30) << studentName
                  << " Enrollment No: " << enrollmentNo << "\n";
        std::cout << " Roll Number:   " << std::left << std::setw(30) << rollNo
                  << " Academic Term: Semester " << semester << "\n";
        std::cout << " Department:    " << std::left << std::setw(30) << branch
                  << " Examination:   Regular 2026-27\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        std::cout << " #  Course Code  Course Title                       Credits  Int(40)  Ext(60)  Total  Grade  Status\n";
        std::cout << "----------------------------------------------------------------------------------------\n";

        int idx = 1;
        int totalCreditPoints = 0;
        for (const auto& s : subjects) {
            int cp = s.credits * s.gradePoint;
            totalCreditPoints += cp;
            std::cout << " " << std::left << std::setw(3) << idx++
                      << std::setw(13) << s.courseCode
                      << std::setw(35) << (s.courseName.length() > 33 ? s.courseName.substr(0, 31) + ".." : s.courseName)
                      << std::right << std::setw(5) << s.credits
                      << std::setw(9) << s.internalMarks
                      << std::setw(9) << s.externalMarks
                      << std::setw(8) << s.totalMarks
                      << std::setw(7) << s.grade
                      << std::setw(9) << s.status << "\n";
        }
        std::cout << "----------------------------------------------------------------------------------------\n";
        std::cout << " Aggregate Score: " << totalObtained << " / " << maxMarks 
                  << " (" << std::fixed << std::setprecision(2) << percentage << "%)\n";
        std::cout << " Total Credits:   " << totalCredits << "  |  Total Credit Points: " << totalCreditPoints << "\n";
        std::cout << " Semester SGPA:   " << std::fixed << std::setprecision(2) << sgpa << " / 10.00\n";
        std::cout << " Final Result:    " << resultStatus << " [" << division << "]\n";
        std::cout << "========================================================================================\n";
        std::cout << " Grading Scale: O(10) [>=90], A+(9) [80-89], A(8) [70-79], B+(7) [60-69], B(6) [50-59],\n";
        std::cout << "                C(5) [40-49 Minimum Passing Threshold], F(0) [<40 Fail / Reappear]\n";
        std::cout << "========================================================================================\n\n";
    }

    std::string serialize() const {
        std::stringstream ss;
        ss << enrollmentNo << "|" << semester << "|" << remarks << "|" << subjects.size();
        for (const auto& s : subjects) {
            ss << "#" << s.courseCode << "," << s.courseName << "," << s.credits << ","
               << s.internalMarks << "," << s.externalMarks;
        }
        return ss.str();
    }

    void deserialize(const std::string& line) {
        subjects.clear();
        std::stringstream ss(line);
        std::string part;
        if (!std::getline(ss, part, '#')) return;

        std::stringstream pss(part);
        std::string token;
        std::vector<std::string> mainTokens;
        while (std::getline(pss, token, '|')) {
            mainTokens.push_back(token);
        }
        if (mainTokens.size() >= 3) {
            enrollmentNo = mainTokens[0];
            semester = std::stoi(mainTokens[1]);
            remarks = mainTokens[2];
        }

        std::string subStr;
        while (std::getline(ss, subStr, '#')) {
            std::stringstream subSS(subStr);
            std::string sToken;
            std::vector<std::string> subTokens;
            while (std::getline(subSS, sToken, ',')) {
                subTokens.push_back(sToken);
            }
            if (subTokens.size() >= 5) {
                addSubject(subTokens[0], subTokens[1], std::stoi(subTokens[2]),
                           std::stoi(subTokens[3]), std::stoi(subTokens[4]));
            }
        }
    }
};

// ========================================================================================
// 5. ENTITY CLASS: Registration (Enrollment Record & Prerequisite Audit Trail)
// ========================================================================================
class Registration {
private:
    std::string registrationId;
    std::string enrollmentNo;
    std::string courseCode;
    int semester;
    std::string status; // CONFIRMED or CANCELLED
    std::string prerequisiteNote;

public:
    Registration() : registrationId(""), enrollmentNo(""), courseCode(""), semester(1),
                     status("CONFIRMED"), prerequisiteNote("") {}

    Registration(const std::string& enrollmentNo, const std::string& courseCode, int semester,
                 const std::string& status = "CONFIRMED", const std::string& note = "Prerequisite Verified")
        : enrollmentNo(enrollmentNo), courseCode(courseCode), semester(semester),
          status(status), prerequisiteNote(note) {
        registrationId = "REG_" + enrollmentNo + "_" + courseCode;
    }

    std::string getRegistrationId() const { return registrationId; }
    std::string getEnrollmentNo() const { return enrollmentNo; }
    std::string getCourseCode() const { return courseCode; }
    int getSemester() const { return semester; }
    std::string getStatus() const { return status; }
    std::string getPrerequisiteNote() const { return prerequisiteNote; }

    void cancel() { status = "CANCELLED"; }

    void display() const {
        std::cout << "│ " << std::left << std::setw(22) << registrationId
                  << "│ " << std::setw(14) << enrollmentNo
                  << "│ " << std::setw(12) << courseCode
                  << "│ " << std::setw(8) << semester
                  << "│ " << std::setw(12) << status << "│\n";
    }

    std::string serialize() const {
        std::stringstream ss;
        ss << registrationId << "|" << enrollmentNo << "|" << courseCode << "|"
           << semester << "|" << status << "|" << prerequisiteNote;
        return ss.str();
    }

    void deserialize(const std::string& line) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        if (tokens.size() >= 6) {
            registrationId = tokens[0];
            enrollmentNo = tokens[1];
            courseCode = tokens[2];
            semester = std::stoi(tokens[3]);
            status = tokens[4];
            prerequisiteNote = tokens[5];
        }
    }
};

// ========================================================================================
// 6. DATA STRUCTURE: Singly Linked List (C-Style Pointer Memory Manipulation)
// ========================================================================================
struct StudentNode {
    Student data;
    StudentNode* next;

    StudentNode(const Student& s) : data(s), next(nullptr) {}
};

class StudentLinkedList {
private:
    StudentNode* head;
    StudentNode* tail;
    int count;

public:
    StudentLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~StudentLinkedList() {
        clear();
    }

    void clear() {
        StudentNode* current = head;
        while (current != nullptr) {
            StudentNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    int getSize() const { return count; }

    // O(1) Tail Insertion with tail pointer
    void insertTail(const Student& s) {
        StudentNode* newNode = new StudentNode(s);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    // O(1) Head Insertion
    void insertHead(const Student& s) {
        StudentNode* newNode = new StudentNode(s);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        count++;
    }

    // O(N) Deletion by Key
    bool deleteByEnrollment(const std::string& enrollmentNo) {
        if (head == nullptr) return false;

        if (head->data.getEnrollmentNo() == enrollmentNo) {
            StudentNode* temp = head;
            head = head->next;
            if (head == nullptr) tail = nullptr;
            delete temp;
            count--;
            return true;
        }

        StudentNode* current = head;
        while (current->next != nullptr) {
            if (current->next->data.getEnrollmentNo() == enrollmentNo) {
                StudentNode* temp = current->next;
                current->next = current->next->next;
                if (current->next == nullptr) tail = current;
                delete temp;
                count--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // O(N) Search with comparison tracking
    Student* searchByEnrollment(const std::string& enrollmentNo, int& comparisons) const {
        comparisons = 0;
        StudentNode* current = head;
        while (current != nullptr) {
            comparisons++;
            if (current->data.getEnrollmentNo() == enrollmentNo) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    // Live Step-by-Step Memory Pointer Traversal
    void traverseWithSteps() const {
        std::cout << "\n--- Linked List Pointer Traversal (Memory Address Trace) ---\n";
        std::cout << "HEAD Pointer Address: " << head << "\n";
        if (head == nullptr) {
            std::cout << "List is empty (HEAD points to NULL).\n";
            return;
        }

        StudentNode* current = head;
        int hop = 1;
        while (current != nullptr) {
            std::cout << " [Hop " << hop++ << "] Node Address: " << current
                      << " | Student: " << current->data.getEnrollmentNo() << " (" << current->data.getName() << ")"
                      << " | Next Pointer: " << current->next << "\n";
            current = current->next;
        }
        std::cout << " Reached NULL tail pointer. Total nodes visited: " << count << " (O(N) operations)\n\n";
    }

    std::vector<Student> toVector() const {
        std::vector<Student> result;
        StudentNode* current = head;
        while (current != nullptr) {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }
};

// ========================================================================================
// 7. ALGORITHMS ENGINE: Searching & Sorting
// ========================================================================================
class DSAEngine {
public:
    // Linear Search: O(N) Time, O(1) Space
    static int linearSearch(const std::vector<Student>& list, const std::string& targetEnrollment, int& comparisons) {
        comparisons = 0;
        for (size_t i = 0; i < list.size(); ++i) {
            comparisons++;
            if (list[i].getEnrollmentNo() == targetEnrollment) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    // Binary Search: O(log N) Time, O(1) Space (Operates on sorted vector)
    static int binarySearch(const std::vector<Student>& sortedList, const std::string& targetEnrollment, int& comparisons) {
        comparisons = 0;
        int low = 0;
        int high = static_cast<int>(sortedList.size()) - 1;

        while (low <= high) {
            comparisons++;
            int mid = low + (high - low) / 2;
            if (sortedList[mid].getEnrollmentNo() == targetEnrollment) {
                return mid;
            }
            if (sortedList[mid].getEnrollmentNo() < targetEnrollment) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return -1;
    }

    // Bubble Sort: O(N^2)
    static void bubbleSort(std::vector<Student>& arr, bool sortByRoll, int& comparisons, int& swaps) {
        comparisons = 0; swaps = 0;
        int n = static_cast<int>(arr.size());
        for (int i = 0; i < n - 1; ++i) {
            bool swapped = false;
            for (int j = 0; j < n - i - 1; ++j) {
                comparisons++;
                bool condition = sortByRoll 
                    ? (arr[j].getRollNo() > arr[j + 1].getRollNo())
                    : (arr[j].getName() > arr[j + 1].getName());
                if (condition) {
                    std::swap(arr[j], arr[j + 1]);
                    swaps++;
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    // Insertion Sort: O(N^2)
    static void insertionSort(std::vector<Student>& arr, bool sortByRoll, int& comparisons, int& shifts) {
        comparisons = 0; shifts = 0;
        int n = static_cast<int>(arr.size());
        for (int i = 1; i < n; ++i) {
            Student key = arr[i];
            int j = i - 1;
            while (j >= 0) {
                comparisons++;
                bool condition = sortByRoll 
                    ? (arr[j].getRollNo() > key.getRollNo())
                    : (arr[j].getName() > key.getName());
                if (condition) {
                    arr[j + 1] = arr[j];
                    shifts++;
                    j--;
                } else {
                    break;
                }
            }
            arr[j + 1] = key;
        }
    }

    // QuickSort: O(N log N) Average
    static void quickSort(std::vector<Student>& arr, int low, int high, bool sortByRoll, int& comparisons, int& swaps) {
        if (low < high) {
            int pi = partition(arr, low, high, sortByRoll, comparisons, swaps);
            quickSort(arr, low, pi - 1, sortByRoll, comparisons, swaps);
            quickSort(arr, pi + 1, high, sortByRoll, comparisons, swaps);
        }
    }

private:
    static int partition(std::vector<Student>& arr, int low, int high, bool sortByRoll, int& comparisons, int& swaps) {
        Student pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            comparisons++;
            bool condition = sortByRoll 
                ? (arr[j].getRollNo() <= pivot.getRollNo())
                : (arr[j].getName() <= pivot.getName());
            if (condition) {
                i++;
                std::swap(arr[i], arr[j]);
                swaps++;
            }
        }
        std::swap(arr[i + 1], arr[high]);
        swaps++;
        return i + 1;
    }
};

// ========================================================================================
// 8. MAIN PORTAL CONTROLLER CLASS (Coordinator, UI Menus, & Storage)
// ========================================================================================
class AcademicPortal {
private:
    StudentLinkedList studentList;
    std::vector<Course> courses;
    std::vector<Result> results;
    std::vector<Registration> registrations;

    const std::string studentFile = "data_students.txt";
    const std::string courseFile = "data_courses.txt";
    const std::string resultFile = "data_results.txt";
    const std::string regFile = "data_registrations.txt";

    void printHeader(const std::string& title) const {
        std::cout << "\n========================================================================================\n";
        std::cout << "                         " << title << "\n";
        std::cout << "========================================================================================\n";
    }

    void pauseConsole() const {
        std::cout << "\nPress Enter to return to menu...";
        std::cin.ignore(10000, '\n');
        std::string dummy;
        std::getline(std::cin, dummy);
    }

public:
    AcademicPortal() {
        if (!loadAllData()) {
            std::cout << "[INFO] Initializing with default Phase III demonstration sample data...\n";
            seedDefaultData();
            saveAllData();
        }
    }

    // --- 1. Dashboard ---
    void displayDashboard() const {
        printHeader("WELCOME TO STUDENT ACADEMIC PORTAL");
        std::cout << "  Manage Students, Courses, Academic Results & Registrations in One Place.\n";
        std::cout << "----------------------------------------------------------------------------------------\n";

        int totalStudents = studentList.getSize();
        int totalCourses = static_cast<int>(courses.size());
        int activeRegs = 0;
        for (const auto& r : registrations) {
            if (r.getStatus() == "CONFIRMED") activeRegs++;
        }

        double totalPct = 0.0, totalSgpa = 0.0;
        for (const auto& res : results) {
            int tot, maxM, cr;
            double pct, sg;
            std::string st, div;
            res.calculateSummary(tot, maxM, pct, cr, sg, st, div);
            totalPct += pct;
            totalSgpa += sg;
        }

        double avgPct = results.empty() ? 0.0 : totalPct / results.size();
        double avgSgpa = results.empty() ? 0.0 : totalSgpa / results.size();

        std::cout << " ┌──────────────────────┬──────────────────────┬──────────────────────┬──────────────────────┐\n";
        std::cout << " │ TOTAL STUDENTS       │ TOTAL COURSES        │ ACTIVE REGISTRATIONS │ ACADEMIC PERFORMANCE │\n";
        std::cout << " ├──────────────────────┼──────────────────────┼──────────────────────┼──────────────────────┤\n";
        std::cout << " │ " << std::left << std::setw(21) << (std::to_string(totalStudents) + " Students")
                  << "│ " << std::setw(21) << (std::to_string(totalCourses) + " Courses")
                  << "│ " << std::setw(21) << (std::to_string(activeRegs) + " Enrolled")
                  << "│ " << std::setw(21) << (std::to_string(static_cast<int>(avgPct)) + "% (SGPA " + std::to_string(avgSgpa).substr(0,4) + ")") << "│\n";
        std::cout << " └──────────────────────┴──────────────────────┴──────────────────────┴──────────────────────┘\n";

        std::cout << "\n [Recent Course Registrations]\n";
        int shownRegs = 0;
        for (int i = static_cast<int>(registrations.size()) - 1; i >= 0 && shownRegs < 4; --i, ++shownRegs) {
            std::cout << "   * " << registrations[i].getRegistrationId() << " | Student: " 
                      << registrations[i].getEnrollmentNo() << " -> Course: " << registrations[i].getCourseCode() 
                      << " (" << registrations[i].getStatus() << ")\n";
        }

        std::cout << "\n [Recent Result Declarations]\n";
        int shownRes = 0;
        for (int i = static_cast<int>(results.size()) - 1; i >= 0 && shownRes < 4; --i, ++shownRes) {
            int tot, maxM, cr;
            double pct, sg;
            std::string st, div;
            results[i].calculateSummary(tot, maxM, pct, cr, sg, st, div);
            std::cout << "   * Student: " << results[i].getEnrollmentNo() << " (Sem " << results[i].getSemester() 
                      << ") -> Aggregate: " << std::fixed << std::setprecision(1) << pct << "% | SGPA: " << sg 
                      << " | Status: " << st << "\n";
        }
        pauseConsole();
    }

    // --- 2. Student CRUD ---
    void studentManagementMenu() {
        while (true) {
            printHeader("STUDENT MANAGEMENT MODULE (CRUD & LINKED LIST)");
            std::cout << " [1] View All Students (Traverse Linked List)\n";
            std::cout << " [2] Add New Student\n";
            std::cout << " [3] Search Student (by Name, Enrollment, or Roll No)\n";
            std::cout << " [4] Update Student Record\n";
            std::cout << " [5] Delete Student Record\n";
            std::cout << " [6] View Individual Student Profile Dossier\n";
            std::cout << " [0] Return to Main Menu\n";
            std::cout << " Enter choice: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear(); std::cin.ignore(10000, '\n'); continue;
            }
            if (choice == 0) break;

            if (choice == 1) {
                viewAllStudents();
            } else if (choice == 2) {
                addNewStudent();
            } else if (choice == 3) {
                searchStudent();
            } else if (choice == 4) {
                updateStudent();
            } else if (choice == 5) {
                deleteStudent();
            } else if (choice == 6) {
                viewStudentProfile();
            }
        }
    }

    void viewAllStudents() const {
        printHeader("REGISTERED STUDENT ROSTER");
        std::vector<Student> list = studentList.toVector();
        if (list.empty()) {
            std::cout << "No student records found.\n";
            pauseConsole();
            return;
        }

        std::cout << "┌─────────────┬──────────────────────┬──────────┬─────────────────────────────┬──────────┐\n";
        std::cout << "│ Enrollment  │ Student Full Name    │ Roll No  │ Branch / Program            │ Semester │\n";
        std::cout << "├─────────────┼──────────────────────┼──────────┼─────────────────────────────┼──────────┤\n";
        for (const auto& s : list) {
            std::cout << "│ " << std::left << std::setw(12) << s.getEnrollmentNo()
                      << "│ " << std::setw(21) << (s.getName().length() > 20 ? s.getName().substr(0, 18) + ".." : s.getName())
                      << "│ " << std::setw(9) << s.getRollNo()
                      << "│ " << std::setw(28) << (s.getBranch().length() > 27 ? s.getBranch().substr(0, 25) + ".." : s.getBranch())
                      << "│ Sem " << std::setw(5) << s.getSemester() << "│\n";
        }
        std::cout << "└─────────────┴──────────────────────┴──────────┴─────────────────────────────┴──────────┘\n";
        std::cout << " Total Count: " << list.size() << " student node(s) stored in Singly Linked List.\n";
        pauseConsole();
    }

    void addNewStudent() {
        std::cin.ignore(10000, '\n');
        printHeader("ADD NEW STUDENT RECORD");
        std::string enrollment, roll, name, branch, email, phone;
        int sem;

        std::cout << "Enter Enrollment No (e.g. EN2024010): ";
        std::getline(std::cin, enrollment);

        int comps = 0;
        if (studentList.searchByEnrollment(enrollment, comps) != nullptr) {
            std::cout << "\n[ERROR] Duplicate Enrollment: A student with ID " << enrollment << " already exists!\n";
            pauseConsole();
            return;
        }

        std::cout << "Enter Roll Number (e.g. CS-24-10): ";
        std::getline(std::cin, roll);
        std::cout << "Enter Full Name: ";
        std::getline(std::cin, name);
        std::cout << "Enter Branch (e.g. Computer Science & Engineering): ";
        std::getline(std::cin, branch);
        std::cout << "Enter Current Semester (1-8): ";
        std::cin >> sem;
        std::cin.ignore(10000, '\n');
        std::cout << "Enter Email Address: ";
        std::getline(std::cin, email);
        std::cout << "Enter Contact Phone: ";
        std::getline(std::cin, phone);

        Student s(name, enrollment, roll, branch, sem, email, phone);
        studentList.insertTail(s);
        saveAllData();

        std::cout << "\n[SUCCESS] Student " << name << " (" << enrollment << ") successfully added to Linked List!\n";
        pauseConsole();
    }

    void searchStudent() const {
        std::cin.ignore(10000, '\n');
        printHeader("SEARCH STUDENT RECORDS");
        std::cout << "Enter Search Query (Name, Enrollment No, or Roll No): ";
        std::string query;
        std::getline(std::cin, query);

        std::vector<Student> list = studentList.toVector();
        int foundCount = 0;
        for (const auto& s : list) {
            if (s.getName().find(query) != std::string::npos ||
                s.getEnrollmentNo().find(query) != std::string::npos ||
                s.getRollNo().find(query) != std::string::npos) {
                s.displayDetails();
                foundCount++;
            }
        }
        if (foundCount == 0) std::cout << "No matching records found for query \"" << query << "\".\n";
        else std::cout << "Found " << foundCount << " matching student(s).\n";
        pauseConsole();
    }

    void updateStudent() {
        std::cin.ignore(10000, '\n');
        printHeader("UPDATE STUDENT RECORD");
        std::cout << "Enter Enrollment Number to update: ";
        std::string enrollment;
        std::getline(std::cin, enrollment);

        int comps = 0;
        Student* s = studentList.searchByEnrollment(enrollment, comps);
        if (s == nullptr) {
            std::cout << "[ERROR] Student with Enrollment " << enrollment << " not found.\n";
            pauseConsole();
            return;
        }

        std::cout << "\nExisting Record Found:\n";
        s->displayDetails();

        std::cout << "\nEnter New Name (or press Enter to keep current): ";
        std::string newName;
        std::getline(std::cin, newName);
        if (!newName.empty()) s->setName(newName);

        std::cout << "Enter New Roll No (or press Enter to keep current): ";
        std::string newRoll;
        std::getline(std::cin, newRoll);
        if (!newRoll.empty()) s->setRollNo(newRoll);

        std::cout << "Enter New Branch (or press Enter to keep current): ";
        std::string newBranch;
        std::getline(std::cin, newBranch);
        if (!newBranch.empty()) s->setBranch(newBranch);

        std::cout << "Enter New Semester (1-8, or 0 to keep current): ";
        int newSem;
        std::cin >> newSem;
        if (newSem >= 1 && newSem <= 8) s->setSemester(newSem);

        saveAllData();
        std::cout << "\n[SUCCESS] Student record updated successfully!\n";
        pauseConsole();
    }

    void deleteStudent() {
        std::cin.ignore(10000, '\n');
        printHeader("DELETE STUDENT RECORD");
        std::cout << "Enter Enrollment Number to delete: ";
        std::string enrollment;
        std::getline(std::cin, enrollment);

        std::cout << "Are you sure you want to delete student " << enrollment << "? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            if (studentList.deleteByEnrollment(enrollment)) {
                for (auto& r : registrations) {
                    if (r.getEnrollmentNo() == enrollment) r.cancel();
                }
                saveAllData();
                std::cout << "\n[SUCCESS] Student " << enrollment << " deleted from Linked List and registrations updated.\n";
            } else {
                std::cout << "\n[ERROR] Student " << enrollment << " not found.\n";
            }
        } else {
            std::cout << "Deletion cancelled.\n";
        }
        pauseConsole();
    }

    void viewStudentProfile() const {
        std::cin.ignore(10000, '\n');
        printHeader("VIEW STUDENT PROFILE DOSSIER");
        std::cout << "Enter Student Enrollment Number: ";
        std::string enrollment;
        std::getline(std::cin, enrollment);

        int comps = 0;
        Student* s = studentList.searchByEnrollment(enrollment, comps);
        if (s == nullptr) {
            std::cout << "[ERROR] Student " << enrollment << " not found.\n";
            pauseConsole();
            return;
        }

        s->displayDetails();

        std::cout << "\n--- Currently Registered Courses ---\n";
        bool hasRegs = false;
        for (const auto& r : registrations) {
            if (r.getEnrollmentNo() == enrollment && r.getStatus() == "CONFIRMED") {
                std::cout << "  * " << r.getCourseCode() << " (Semester " << r.getSemester() 
                          << ") | Status: " << r.getStatus() << "\n";
                hasRegs = true;
            }
        }
        if (!hasRegs) std::cout << "  No active course registrations on file.\n";

        std::cout << "\n--- Academic Examination Transcripts ---\n";
        bool hasRes = false;
        for (const auto& res : results) {
            if (res.getEnrollmentNo() == enrollment) {
                int tot, maxM, cr;
                double pct, sg;
                std::string st, div;
                res.calculateSummary(tot, maxM, pct, cr, sg, st, div);
                std::cout << "  * Semester " << res.getSemester() << " Result: Aggregate " 
                          << std::fixed << std::setprecision(1) << pct << "% | SGPA: " << sg 
                          << " | Status: " << st << " (" << div << ")\n";
                hasRes = true;
            }
        }
        if (!hasRes) std::cout << "  No examination records recorded yet.\n";
        pauseConsole();
    }

    // --- 3. Course Management ---
    void courseManagementMenu() {
        while (true) {
            printHeader("COURSE MANAGEMENT MODULE (CURRICULUM CATALOG)");
            std::cout << " [1] Display All Available Courses\n";
            std::cout << " [2] Add New Course\n";
            std::cout << " [3] Edit Course Details\n";
            std::cout << " [4] Delete Course\n";
            std::cout << " [5] Search Courses\n";
            std::cout << " [0] Return to Main Menu\n";
            std::cout << " Enter choice: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear(); std::cin.ignore(10000, '\n'); continue;
            }
            if (choice == 0) break;

            if (choice == 1) displayAllCourses();
            else if (choice == 2) addNewCourse();
            else if (choice == 3) editCourse();
            else if (choice == 4) deleteCourse();
            else if (choice == 5) searchCourses();
        }
    }

    void displayAllCourses() const {
        printHeader("AVAILABLE CURRICULUM COURSES");
        if (courses.empty()) {
            std::cout << "No courses in catalog.\n";
            pauseConsole();
            return;
        }

        std::cout << "┌───────────┬───────────────────────────────────┬────────┬─────────┬──────────────┐\n";
        std::cout << "│ Code      │ Course Title                      │Credits │Semester │ Prerequisite │\n";
        std::cout << "├───────────┼───────────────────────────────────┼────────┼─────────┼──────────────┤\n";
        for (const auto& c : courses) {
            c.display();
        }
        std::cout << "└───────────┴───────────────────────────────────┴────────┴─────────┴──────────────┘\n";
        std::cout << " Total Catalog Courses: " << courses.size() << "\n";
        pauseConsole();
    }

    void addNewCourse() {
        std::cin.ignore(10000, '\n');
        printHeader("ADD NEW COURSE");
        std::string code, name, prereq;
        int credits, sem;

        std::cout << "Enter Course Code (e.g. CS201): ";
        std::getline(std::cin, code);

        for (const auto& c : courses) {
            if (c.getCourseCode() == code) {
                std::cout << "[ERROR] Duplicate Course Code: " << code << " already exists!\n";
                pauseConsole();
                return;
            }
        }

        std::cout << "Enter Course Title: ";
        std::getline(std::cin, name);
        std::cout << "Enter Academic Credits (1-6): ";
        std::cin >> credits;
        std::cout << "Enter Semester (1-8): ";
        std::cin >> sem;
        std::cin.ignore(10000, '\n');
        std::cout << "Enter Prerequisite Course Code (or 'NONE'): ";
        std::getline(std::cin, prereq);

        courses.push_back(Course(code, name, credits, sem, prereq));
        saveAllData();
        std::cout << "\n[SUCCESS] Course " << code << " (" << name << ") added to catalog!\n";
        pauseConsole();
    }

    void editCourse() {
        std::cin.ignore(10000, '\n');
        printHeader("EDIT COURSE DETAILS");
        std::cout << "Enter Course Code to Edit: ";
        std::string code;
        std::getline(std::cin, code);

        Course* target = nullptr;
        for (auto& c : courses) {
            if (c.getCourseCode() == code) {
                target = &c;
                break;
            }
        }

        if (target == nullptr) {
            std::cout << "[ERROR] Course " << code << " not found.\n";
            pauseConsole();
            return;
        }

        std::cout << "Current Title: " << target->getCourseName() << "\n";
        std::cout << "Enter New Title (press Enter to keep): ";
        std::string newTitle;
        std::getline(std::cin, newTitle);
        if (!newTitle.empty()) target->setCourseName(newTitle);

        std::cout << "Current Credits: " << target->getCredits() << "\n";
        std::cout << "Enter New Credits (1-6, or 0 to keep): ";
        int newCr;
        std::cin >> newCr;
        if (newCr >= 1 && newCr <= 6) target->setCredits(newCr);

        std::cin.ignore(10000, '\n');
        std::cout << "Current Prerequisite: " << target->getPrerequisiteCode() << "\n";
        std::cout << "Enter New Prerequisite (or 'NONE', press Enter to keep): ";
        std::string newPrereq;
        std::getline(std::cin, newPrereq);
        if (!newPrereq.empty()) target->setPrerequisiteCode(newPrereq);

        saveAllData();
        std::cout << "\n[SUCCESS] Course details updated!\n";
        pauseConsole();
    }

    void deleteCourse() {
        std::cin.ignore(10000, '\n');
        printHeader("DELETE COURSE");
        std::cout << "Enter Course Code to delete: ";
        std::string code;
        std::getline(std::cin, code);

        for (const auto& c : courses) {
            if (c.getPrerequisiteCode() == code) {
                std::cout << "[ERROR] Cannot delete course " << code << ": Course " 
                          << c.getCourseCode() << " requires it as a prerequisite!\n";
                pauseConsole();
                return;
            }
        }

        auto it = std::remove_if(courses.begin(), courses.end(), [&](const Course& c) {
            return c.getCourseCode() == code;
        });

        if (it != courses.end()) {
            courses.erase(it, courses.end());
            saveAllData();
            std::cout << "\n[SUCCESS] Course " << code << " removed from curriculum catalog.\n";
        } else {
            std::cout << "[ERROR] Course " << code << " not found.\n";
        }
        pauseConsole();
    }

    void searchCourses() const {
        std::cin.ignore(10000, '\n');
        printHeader("SEARCH COURSES");
        std::cout << "Enter Search Query (Code, Title, or Prerequisite): ";
        std::string query;
        std::getline(std::cin, query);

        std::cout << "┌───────────┬───────────────────────────────────┬────────┬─────────┬──────────────┐\n";
        std::cout << "│ Code      │ Course Title                      │Credits │Semester │ Prerequisite │\n";
        std::cout << "├───────────┼───────────────────────────────────┼────────┼─────────┼──────────────┤\n";
        int matchCount = 0;
        for (const auto& c : courses) {
            if (c.getCourseCode().find(query) != std::string::npos ||
                c.getCourseName().find(query) != std::string::npos ||
                c.getPrerequisiteCode().find(query) != std::string::npos) {
                c.display();
                matchCount++;
            }
        }
        std::cout << "└───────────┴───────────────────────────────────┴────────┴─────────┴──────────────┘\n";
        std::cout << "Matches found: " << matchCount << "\n";
        pauseConsole();
    }

    // --- 4. Course Registration & Automated Prerequisite Engine ---
    bool verifyPrerequisite(const std::string& enrollmentNo, const std::string& courseCode,
                            std::string& statusMessage, bool& isAllowed) const {
        const Course* targetCourse = nullptr;
        for (const auto& c : courses) {
            if (c.getCourseCode() == courseCode) {
                targetCourse = &c;
                break;
            }
        }

        if (targetCourse == nullptr) {
            statusMessage = "Course " + courseCode + " does not exist in curriculum catalog.";
            isAllowed = false;
            return false;
        }

        if (!targetCourse->hasPrerequisite()) {
            statusMessage = "Course " + courseCode + " (" + targetCourse->getCourseName() + ") has NO prerequisite requirement. Direct enrollment approved.";
            isAllowed = true;
            return true;
        }

        std::string prereqCode = targetCourse->getPrerequisiteCode();
        bool hasTaken = false;
        bool hasPassed = false;
        int marksObtained = 0;
        std::string gradeSecured = "";
        int semPassed = 0;

        for (const auto& res : results) {
            if (res.getEnrollmentNo() == enrollmentNo) {
                if (res.hasTakenCourse(prereqCode)) {
                    hasTaken = true;
                    if (res.hasPassedCourse(prereqCode, marksObtained, gradeSecured)) {
                        hasPassed = true;
                        semPassed = res.getSemester();
                        break;
                    }
                }
            }
        }

        if (!hasTaken) {
            statusMessage = "REGISTRATION REJECTED: Missing Prerequisite! Student has NOT taken required prerequisite " + prereqCode + ". Minimum passing threshold is Grade C (40%).";
            isAllowed = false;
            return false;
        }

        if (!hasPassed) {
            statusMessage = "REGISTRATION REJECTED: Prerequisite Failed! Student took prerequisite " + prereqCode + " but scored " + std::to_string(marksObtained) + "% (Grade " + gradeSecured + "). Minimum passing threshold is Grade C (40%).";
            isAllowed = false;
            return false;
        }

        statusMessage = "PREREQUISITE VERIFIED: Student passed prerequisite " + prereqCode + " in Semester " + std::to_string(semPassed) + " with " + std::to_string(marksObtained) + "% (Grade " + gradeSecured + "). Registration approved!";
        isAllowed = true;
        return true;
    }

    void registrationMenu() {
        while (true) {
            printHeader("COURSE REGISTRATION & PREREQUISITE VALIDATION ENGINE");
            std::cout << " [1] Register Student for Course (Automatic Prerequisite Check)\n";
            std::cout << " [2] View All Active Course Registrations\n";
            std::cout << " [3] Cancel Course Registration\n";
            std::cout << " [4] Quick PBL Viva Evaluation Demo Scenarios (1-Click Test)\n";
            std::cout << " [0] Return to Main Menu\n";
            std::cout << " Enter choice: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear(); std::cin.ignore(10000, '\n'); continue;
            }
            if (choice == 0) break;

            if (choice == 1) registerStudentWorkflow();
            else if (choice == 2) viewAllRegistrations();
            else if (choice == 3) cancelRegistrationWorkflow();
            else if (choice == 4) runVivaDemoScenarios();
        }
    }

    void registerStudentWorkflow() {
        std::cin.ignore(10000, '\n');
        printHeader("ENROLL STUDENT IN COURSE");
        std::cout << "Enter Student Enrollment No: ";
        std::string enrollment;
        std::getline(std::cin, enrollment);

        int comps = 0;
        Student* s = studentList.searchByEnrollment(enrollment, comps);
        if (s == nullptr) {
            std::cout << "[ERROR] Student " << enrollment << " not found.\n";
            pauseConsole();
            return;
        }

        std::cout << "Student: " << s->getName() << " | Branch: " << s->getBranch() 
                  << " | Current Semester: " << s->getSemester() << "\n";

        std::cout << "\nEnter Course Code to Register (e.g. CS201): ";
        std::string code;
        std::getline(std::cin, code);

        for (const auto& r : registrations) {
            if (r.getEnrollmentNo() == enrollment && r.getCourseCode() == code && r.getStatus() == "CONFIRMED") {
                std::cout << "\n[ERROR] Duplicate Registration: Student " << enrollment 
                          << " is ALREADY actively registered for course " << code << "!\n";
                pauseConsole();
                return;
            }
        }

        std::cout << "\n[PREREQUISITE ENGINE] Scanning student academic transcripts...\n";
        std::string statusMsg;
        bool isAllowed = false;
        verifyPrerequisite(enrollment, code, statusMsg, isAllowed);

        std::cout << "\n" << std::string(80, '-') << "\n";
        if (isAllowed) {
            std::cout << ">>> STATUS: [APPROVED] <<<\n";
            std::cout << statusMsg << "\n";
            std::cout << std::string(80, '-') << "\n";

            registrations.push_back(Registration(enrollment, code, s->getSemester(), "CONFIRMED", statusMsg));
            saveAllData();
            std::cout << "\n[SUCCESS] Registration confirmed and recorded in database!\n";
        } else {
            std::cout << ">>> STATUS: [BLOCKED / REJECTED] <<<\n";
            std::cout << statusMsg << "\n";
            std::cout << std::string(80, '-') << "\n";
            std::cout << "Advisory: Student must clear prerequisite before enrolling.\n";
        }
        pauseConsole();
    }

    void viewAllRegistrations() const {
        printHeader("ACTIVE COURSE REGISTRATIONS");
        if (registrations.empty()) {
            std::cout << "No registrations on record.\n";
            pauseConsole();
            return;
        }

        std::cout << "┌───────────────────────┬──────────────┬────────────┬────────┬────────────┐\n";
        std::cout << "│ Registration ID       │ Enrollment   │ Course Code│ Semester│ Status    │\n";
        std::cout << "├───────────────────────┼──────────────┼────────────┼────────┼────────────┤\n";
        for (const auto& r : registrations) {
            r.display();
        }
        std::cout << "└───────────────────────┴──────────────┴────────────┴────────┴────────────┘\n";
        std::cout << " Total Registrations: " << registrations.size() << "\n";
        pauseConsole();
    }

    void cancelRegistrationWorkflow() {
        std::cin.ignore(10000, '\n');
        printHeader("CANCEL COURSE REGISTRATION");
        std::cout << "Enter Student Enrollment Number: ";
        std::string enrollment;
        std::getline(std::cin, enrollment);

        std::cout << "Enter Course Code to Cancel: ";
        std::string code;
        std::getline(std::cin, code);

        bool found = false;
        for (auto& r : registrations) {
            if (r.getEnrollmentNo() == enrollment && r.getCourseCode() == code && r.getStatus() == "CONFIRMED") {
                r.cancel();
                found = true;
                break;
            }
        }

        if (found) {
            saveAllData();
            std::cout << "\n[SUCCESS] Registration for student " << enrollment << " in course " << code << " has been cancelled.\n";
        } else {
            std::cout << "\n[ERROR] No active registration found matching student " << enrollment << " and course " << code << ".\n";
        }
        pauseConsole();
    }

    void runVivaDemoScenarios() const {
        printHeader("PHASE III VIVA PRESET DEMONSTRATION SCENARIOS");
        std::cout << "Target Advanced Course: CS201 (Data Structures & Algorithms)\n";
        std::cout << "Mandatory Prerequisite: CS101 (Programming in C & Problem Solving)\n";
        std::cout << "Passing Requirement:    Grade C (Minimum 40%)\n";
        std::cout << "----------------------------------------------------------------------------------------\n\n";

        // Demo 1
        std::cout << "[DEMO SCENARIO 1 - SUCCESSFUL PREREQUISITE]\n";
        std::cout << "Student: Aarav Sharma (EN2024001)\n";
        std::cout << "Past Result: Scored 86% (Grade A+) in prerequisite CS101 during Semester 1.\n";
        std::cout << "Attempting Registration for CS201...\n";
        std::string msg1; bool allowed1 = false;
        verifyPrerequisite("EN2024001", "CS201", msg1, allowed1);
        std::cout << "Result: " << (allowed1 ? "[PASSED / APPROVED]" : "[REJECTED]") << " -> " << msg1 << "\n\n";

        // Demo 2
        std::cout << "[DEMO SCENARIO 2 - FAILED PREREQUISITE]\n";
        std::cout << "Student: Rohan Mehta (EN2024003)\n";
        std::cout << "Past Result: Scored 32% (Grade F) in prerequisite CS101 during Semester 1.\n";
        std::cout << "Attempting Registration for CS201...\n";
        std::string msg2; bool allowed2 = false;
        verifyPrerequisite("EN2024003", "CS201", msg2, allowed2);
        std::cout << "Result: " << (allowed2 ? "[PASSED / APPROVED]" : "[REJECTED / BLOCKED]") << " -> " << msg2 << "\n\n";

        // Demo 3
        std::cout << "[DEMO SCENARIO 3 - MISSING / UNATTEMPTED PREREQUISITE]\n";
        std::cout << "Student: Vikram Singh (EN2024005)\n";
        std::cout << "Past Result: Enrolled in ECE, has NEVER taken introductory CS101.\n";
        std::cout << "Attempting Registration for CS201...\n";
        std::string msg3; bool allowed3 = false;
        verifyPrerequisite("EN2024005", "CS201", msg3, allowed3);
        std::cout << "Result: " << (allowed3 ? "[PASSED / APPROVED]" : "[REJECTED / BLOCKED]") << " -> " << msg3 << "\n";

        pauseConsole();
    }

    // --- 5. Result Processing & Marksheets ---
    void resultManagementMenu() {
        while (true) {
            printHeader("RESULT PROCESSING & MARKSHEET MODULE");
            std::cout << " [1] Record / Update Student Subject Marks\n";
            std::cout << " [2] Generate & Print Official University Marksheet\n";
            std::cout << " [3] Search Results by Enrollment Number\n";
            std::cout << " [4] View UGC 10-Point Grading Scale Criteria\n";
            std::cout << " [0] Return to Main Menu\n";
            std::cout << " Enter choice: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear(); std::cin.ignore(10000, '\n'); continue;
            }
            if (choice == 0) break;

            if (choice == 1) recordStudentMarks();
            else if (choice == 2) generateMarksheetWorkflow();
            else if (choice == 3) searchResultsWorkflow();
            else if (choice == 4) displayGradingScale();
        }
    }

    void recordStudentMarks() {
        std::cin.ignore(10000, '\n');
        printHeader("RECORD STUDENT EXAMINATION MARKS");
        std::cout << "Enter Student Enrollment Number: ";
        std::string enrollment;
        std::getline(std::cin, enrollment);

        int comps = 0;
        Student* s = studentList.searchByEnrollment(enrollment, comps);
        if (s == nullptr) {
            std::cout << "[ERROR] Student " << enrollment << " not found.\n";
            pauseConsole();
            return;
        }

        std::cout << "Enter Examination Semester (1-8): ";
        int sem;
        std::cin >> sem;
        std::cin.ignore(10000, '\n');

        std::cout << "Enter Course Code (e.g. CS101): ";
        std::string code;
        std::getline(std::cin, code);

        std::string courseName = code;
        int credits = 3;
        for (const auto& c : courses) {
            if (c.getCourseCode() == code) {
                courseName = c.getCourseName();
                credits = c.getCredits();
                break;
            }
        }

        std::cout << "Enter Internal Marks (Max 40): ";
        int intMarks;
        std::cin >> intMarks;
        std::cout << "Enter External Marks (Max 60): ";
        int extMarks;
        std::cin >> extMarks;

        Result* targetRes = nullptr;
        for (auto& r : results) {
            if (r.getEnrollmentNo() == enrollment && r.getSemester() == sem) {
                targetRes = &r;
                break;
            }
        }

        if (targetRes == nullptr) {
            Result newRes(enrollment, sem, "Regular Examination");
            newRes.addSubject(code, courseName, credits, intMarks, extMarks);
            results.push_back(newRes);
        } else {
            targetRes->addSubject(code, courseName, credits, intMarks, extMarks);
        }

        saveAllData();
        std::cout << "\n[SUCCESS] Marks recorded and GPA recalculation updated!\n";
        pauseConsole();
    }

    void generateMarksheetWorkflow() const {
        std::cin.ignore(10000, '\n');
        printHeader("GENERATE OFFICIAL UNIVERSITY MARKSHEET");
        std::cout << "Enter Student Enrollment Number: ";
        std::string enrollment;
        std::getline(std::cin, enrollment);

        int comps = 0;
        Student* s = studentList.searchByEnrollment(enrollment, comps);
        if (s == nullptr) {
            std::cout << "[ERROR] Student " << enrollment << " not found.\n";
            pauseConsole();
            return;
        }

        std::cout << "Enter Semester to Print (1-8): ";
        int sem;
        std::cin >> sem;

        bool found = false;
        for (const auto& res : results) {
            if (res.getEnrollmentNo() == enrollment && res.getSemester() == sem) {
                res.displayMarksheet(s->getName(), s->getRollNo(), s->getBranch());
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "[ERROR] No examination transcript found for " << enrollment 
                      << " in Semester " << sem << ".\n";
        }
        pauseConsole();
    }

    void searchResultsWorkflow() const {
        std::cin.ignore(10000, '\n');
        printHeader("SEARCH ACADEMIC RESULTS");
        std::cout << "Enter Enrollment Number: ";
        std::string enrollment;
        std::getline(std::cin, enrollment);

        int comps = 0;
        Student* s = studentList.searchByEnrollment(enrollment, comps);
        std::string stuName = s ? s->getName() : enrollment;

        bool found = false;
        for (const auto& res : results) {
            if (res.getEnrollmentNo() == enrollment) {
                int tot, maxM, cr;
                double pct, sg;
                std::string st, div;
                res.calculateSummary(tot, maxM, pct, cr, sg, st, div);

                std::cout << "\n------------------------------------------------------------------------\n";
                std::cout << " Result for: " << stuName << " (" << enrollment << ") | Semester: " << res.getSemester() << "\n";
                std::cout << " Aggregate Marks: " << tot << " / " << maxM << " (" << std::fixed << std::setprecision(2) << pct << "%)\n";
                std::cout << " Semester SGPA:   " << sg << " / 10.00\n";
                std::cout << " Outcome:         " << st << " [" << div << "]\n";
                std::cout << " Subjects Count:  " << res.getSubjects().size() << "\n";
                std::cout << "------------------------------------------------------------------------\n";
                found = true;
            }
        }

        if (!found) {
            std::cout << "No academic results found on file for " << enrollment << ".\n";
        }
        pauseConsole();
    }

    void displayGradingScale() const {
        printHeader("UGC / AICTE 10-POINT GRADING SCALE CRITERIA");
        std::cout << "┌──────────┬──────────────────┬──────────────┬────────────────────────────┐\n";
        std::cout << "│ Marks %  │ Letter Grade     │ Grade Points │ Performance Classification │\n";
        std::cout << "├──────────┼──────────────────┼──────────────┼────────────────────────────┤\n";
        std::cout << "│ 90 - 100 │ O                │ 10           │ Outstanding                │\n";
        std::cout << "│ 80 - 89  │ A+               │ 9            │ Excellent                  │\n";
        std::cout << "│ 70 - 79  │ A                │ 8            │ Very Good                  │\n";
        std::cout << "│ 60 - 69  │ B+               │ 7            │ Good                       │\n";
        std::cout << "│ 50 - 59  │ B                │ 6            │ Above Average              │\n";
        std::cout << "│ 40 - 49  │ C                │ 5            │ Pass (Passing Threshold)   │\n";
        std::cout << "│ 0 - 39   │ F                │ 0            │ Fail (Mandatory Reappear)  │\n";
        std::cout << "└──────────┴──────────────────┴──────────────┴────────────────────────────┘\n";
        std::cout << " SGPA Formula: Sum(Course Credits * Grade Points) / Sum(Course Credits)\n";
        pauseConsole();
    }

    // --- 6. DSA Showcase ---
    void dsaShowcaseMenu() {
        while (true) {
            printHeader("DATA STRUCTURES & ALGORITHMS (DSA) SHOWCASE");
            std::cout << " [1] Singly Linked List: Live Memory Pointer Traversal (O(N))\n";
            std::cout << " [2] Search Benchmark: Linear Search O(N) vs Binary Search O(log N)\n";
            std::cout << " [3] Sort Benchmark: QuickSort vs Insertion Sort vs Bubble Sort\n";
            std::cout << " [4] View DSA Time & Space Complexity Matrix\n";
            std::cout << " [0] Return to Main Menu\n";
            std::cout << " Enter choice: ";

            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear(); std::cin.ignore(10000, '\n'); continue;
            }
            if (choice == 0) break;

            if (choice == 1) {
                studentList.traverseWithSteps();
                pauseConsole();
            } else if (choice == 2) {
                runSearchBenchmark();
            } else if (choice == 3) {
                runSortBenchmark();
            } else if (choice == 4) {
                displayComplexityMatrix();
            }
        }
    }

    void runSearchBenchmark() const {
        std::cin.ignore(10000, '\n');
        printHeader("SEARCHING ALGORITHMS BENCHMARK");
        std::cout << "Enter Enrollment Number to search (e.g. EN2024004): ";
        std::string target;
        std::getline(std::cin, target);

        std::vector<Student> original = studentList.toVector();
        if (original.empty()) {
            std::cout << "Dataset empty.\n";
            pauseConsole();
            return;
        }

        int linComps = 0;
        auto start1 = std::chrono::high_resolution_clock::now();
        int linIdx = DSAEngine::linearSearch(original, target, linComps);
        auto end1 = std::chrono::high_resolution_clock::now();
        double timeLin = std::chrono::duration<double, std::micro>(end1 - start1).count();

        std::vector<Student> sorted = original;
        std::sort(sorted.begin(), sorted.end(), [](const Student& a, const Student& b) {
            return a.getEnrollmentNo() < b.getEnrollmentNo();
        });

        int binComps = 0;
        auto start2 = std::chrono::high_resolution_clock::now();
        int binIdx = DSAEngine::binarySearch(sorted, target, binComps);
        auto end2 = std::chrono::high_resolution_clock::now();
        double timeBin = std::chrono::duration<double, std::micro>(end2 - start2).count();

        std::cout << "\n----------------------------------------------------------------------------------------\n";
        std::cout << " ALGORITHM       FOUND?     INDEX     COMPARISONS     TIME COMPLEXITY     EXECUTION TIME\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        std::cout << " Linear Search   " << std::left << std::setw(11) << (linIdx != -1 ? "Yes" : "No")
                  << std::setw(10) << linIdx
                  << std::setw(16) << linComps
                  << std::setw(20) << "O(N)"
                  << timeLin << " microseconds\n";
        std::cout << " Binary Search   " << std::left << std::setw(11) << (binIdx != -1 ? "Yes" : "No")
                  << std::setw(10) << binIdx
                  << std::setw(16) << binComps
                  << std::setw(20) << "O(log N)"
                  << timeBin << " microseconds\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        pauseConsole();
    }

    void runSortBenchmark() const {
        printHeader("SORTING ALGORITHMS BENCHMARK");
        std::vector<Student> base = studentList.toVector();

        std::cout << "Dataset Size: " << base.size() << " student records.\n";
        std::cout << "Sorting criteria: By Student Full Name\n\n";

        std::vector<Student> arrBubble = base;
        int bComps = 0, bSwaps = 0;
        auto t1 = std::chrono::high_resolution_clock::now();
        DSAEngine::bubbleSort(arrBubble, false, bComps, bSwaps);
        auto t2 = std::chrono::high_resolution_clock::now();
        double timeBubble = std::chrono::duration<double, std::micro>(t2 - t1).count();

        std::vector<Student> arrInsert = base;
        int iComps = 0, iShifts = 0;
        auto t3 = std::chrono::high_resolution_clock::now();
        DSAEngine::insertionSort(arrInsert, false, iComps, iShifts);
        auto t4 = std::chrono::high_resolution_clock::now();
        double timeInsert = std::chrono::duration<double, std::micro>(t4 - t3).count();

        std::vector<Student> arrQuick = base;
        int qComps = 0, qSwaps = 0;
        auto t5 = std::chrono::high_resolution_clock::now();
        if (!arrQuick.empty()) {
            DSAEngine::quickSort(arrQuick, 0, static_cast<int>(arrQuick.size()) - 1, false, qComps, qSwaps);
        }
        auto t6 = std::chrono::high_resolution_clock::now();
        double timeQuick = std::chrono::duration<double, std::micro>(t6 - t5).count();

        std::cout << "----------------------------------------------------------------------------------------\n";
        std::cout << " ALGORITHM        TIME COMPLEXITY     SPACE COMPLEXITY   COMPARISONS   SWAPS/SHIFTS   TIME\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        std::cout << " Bubble Sort      O(N^2)              O(1)               "
                  << std::left << std::setw(14) << bComps << std::setw(15) << bSwaps << timeBubble << " us\n";
        std::cout << " Insertion Sort   O(N^2)              O(1)               "
                  << std::left << std::setw(14) << iComps << std::setw(15) << iShifts << timeInsert << " us\n";
        std::cout << " QuickSort        O(N log N) Avg      O(log N)           "
                  << std::left << std::setw(14) << qComps << std::setw(15) << qSwaps << timeQuick << " us\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        pauseConsole();
    }

    void displayComplexityMatrix() const {
        printHeader("DSA TIME & SPACE COMPLEXITY ANALYSIS (PHASE III PBL)");
        std::cout << "┌──────────────────────┬──────────────────────┬──────────────────┬─────────────────┐\n";
        std::cout << "│ Concept / Structure  │ Where Applied        │ Time Complexity  │ Space Complexity│\n";
        std::cout << "├──────────────────────┼──────────────────────┼──────────────────┼─────────────────┤\n";
        std::cout << "│ Singly Linked List   │ Student Dynamic Nodes│ Insert: O(1)     │ O(N)            │\n";
        std::cout << "│                      │                      │ Search: O(N)     │                 │\n";
        std::cout << "│ Linear Search        │ Multi-Field Querying │ O(N)             │ O(1)            │\n";
        std::cout << "│ Binary Search        │ Enrollment Lookups   │ O(log N)         │ O(1)            │\n";
        std::cout << "│ Bubble / Insertion   │ Small Record Sets    │ O(N^2)           │ O(1)            │\n";
        std::cout << "│ QuickSort            │ Roster Ordering      │ O(N log N) Avg   │ O(log N)        │\n";
        std::cout << "└──────────────────────┴──────────────────────┴──────────────────┴─────────────────┘\n";
        pauseConsole();
    }

    // --- 7. OOP Architecture Deep-Dive ---
    void oopArchitectureMenu() const {
        printHeader("OBJECT-ORIENTED PROGRAMMING (OOP) ARCHITECTURE");
        std::cout << "1. ENCAPSULATION:\n";
        std::cout << "   - Class Person and Student enforce data hiding via private/protected members.\n";
        std::cout << "   - Access is restricted exclusively through public getters and validated setters.\n\n";

        std::cout << "2. INHERITANCE:\n";
        std::cout << "   - Class Student derives from Person: `class Student : public Person`\n";
        std::cout << "   - Reuses base demographic parameters (name, email, phone) and extends academic fields.\n\n";

        std::cout << "3. POLYMORPHISM:\n";
        std::cout << "   - Base class Person defines pure virtual functions:\n";
        std::cout << "     `virtual void displayDetails() const = 0;`\n";
        std::cout << "     `virtual string serialize() const = 0;`\n";
        std::cout << "   - Student overrides these methods providing custom academic formatting.\n\n";

        std::cout << "4. ABSTRACTION:\n";
        std::cout << "   - Complex linked list pointer manipulation, file I/O streams, and prerequisite\n";
        std::cout << "     verification algorithms are abstracted behind clean, intuitive APIs in the portal.\n";
        pauseConsole();
    }

    // --- 8. Automated Diagnostic Test Suite ---
    void runAutomatedTestSuite() const {
        printHeader("RUNNING PHASE III AUTOMATED DIAGNOSTIC TEST SUITE");
        std::cout << "Executing 8 automated unit & integration assertions...\n\n";

        int passed = 0;
        int total = 8;

        // Test 1: Student Creation
        {
            Student s("Test Student", "TEST001", "TST-01", "CSE", 1, "t@c.edu", "9999999999");
            if (s.getEnrollmentNo() == "TEST001" && s.getName() == "Test Student" && s.getId() == "TEST001") {
                std::cout << " [PASS] Test 1: Student Entity Creation & Encapsulation Verified\n";
                passed++;
            } else std::cout << " [FAIL] Test 1: Student Entity Creation Failed\n";
        }

        // Test 2: Linked List Operations
        {
            StudentLinkedList ll;
            ll.insertTail(Student("A", "E1", "R1", "CSE", 1, "a@c.edu", "1111111111"));
            ll.insertTail(Student("B", "E2", "R2", "CSE", 1, "b@c.edu", "2222222222"));
            int c = 0;
            bool found = (ll.searchByEnrollment("E2", c) != nullptr);
            bool del = ll.deleteByEnrollment("E1");
            if (found && del && ll.getSize() == 1) {
                std::cout << " [PASS] Test 2: Linked List Dynamic Insert, Search & Delete Verified\n";
                passed++;
            } else std::cout << " [FAIL] Test 2: Linked List Operations Failed\n";
        }

        // Test 3: Search Engine Parity
        {
            std::vector<Student> v;
            v.push_back(Student("A", "EN01", "R1", "CSE", 1, "a@c.edu", "1111111111"));
            v.push_back(Student("B", "EN02", "R2", "CSE", 1, "b@c.edu", "2222222222"));
            int lC = 0, bC = 0;
            int lIdx = DSAEngine::linearSearch(v, "EN02", lC);
            int bIdx = DSAEngine::binarySearch(v, "EN02", bC);
            if (lIdx == 1 && bIdx == 1) {
                std::cout << " [PASS] Test 3: Linear vs Binary Search Parity Verified\n";
                passed++;
            } else std::cout << " [FAIL] Test 3: Search Algorithm Failed\n";
        }

        // Test 4: Course Creation
        {
            Course c("CS101", "Programming in C", 4, 1, "NONE");
            if (c.getCourseCode() == "CS101" && !c.hasPrerequisite()) {
                std::cout << " [PASS] Test 4: Course Entity & Prerequisite Rules Verified\n";
                passed++;
            } else std::cout << " [FAIL] Test 4: Course Creation Failed\n";
        }

        // Test 5: Result & SGPA Formula Calculation
        {
            Result res("EN2024001", 1);
            res.addSubject("C1", "Course 1", 4, 36, 54); // 90 -> O (10 pts) -> 40 cp
            res.addSubject("C2", "Course 2", 4, 28, 42); // 70 -> A (8 pts)  -> 32 cp
            int tot, maxM, cr;
            double pct, sg;
            std::string st, div;
            res.calculateSummary(tot, maxM, pct, cr, sg, st, div);
            if (std::abs(sg - 9.0) < 0.05 && st == "PASS") {
                std::cout << " [PASS] Test 5: UGC SGPA Calculation (Exact Match: 9.00) Verified\n";
                passed++;
            } else std::cout << " [FAIL] Test 5: Result Processing Failed\n";
        }

        // Test 6: Successful Course Registration Prerequisite
        {
            std::string msg; bool allowed = false;
            verifyPrerequisite("EN2024001", "CS201", msg, allowed);
            if (allowed) {
                std::cout << " [PASS] Test 6: Prerequisite Met Approval (Aarav -> CS201) Verified\n";
                passed++;
            } else std::cout << " [FAIL] Test 6: Prerequisite Met Approval Failed\n";
        }

        // Test 7: Failed / Missing Prerequisite Rejection
        {
            std::string msgFailed; bool allowedFailed = true;
            verifyPrerequisite("EN2024003", "CS201", msgFailed, allowedFailed);

            std::string msgMissing; bool allowedMissing = true;
            verifyPrerequisite("EN2024005", "CS201", msgMissing, allowedMissing);

            if (!allowedFailed && !allowedMissing) {
                std::cout << " [PASS] Test 7: Automated Prerequisite Rejection (Failed & Missing) Verified\n";
                passed++;
            } else std::cout << " [FAIL] Test 7: Prerequisite Rejection Failed\n";
        }

        // Test 8: File Persistence
        {
            std::ofstream out("test_persist.tmp");
            out << "TEST_RECORD|SUCCESS";
            out.close();

            std::ifstream in("test_persist.tmp");
            std::string line;
            std::getline(in, line);
            in.close();
            std::remove("test_persist.tmp");

            if (line == "TEST_RECORD|SUCCESS") {
                std::cout << " [PASS] Test 8: File Handling I/O Persistence Verified\n";
                passed++;
            } else std::cout << " [FAIL] Test 8: File Handling Failed\n";
        }

        std::cout << "\n========================================================================================\n";
        std::cout << " TEST EXECUTION SUMMARY: " << passed << " / " << total << " Assertions PASSED (" 
                  << (passed == total ? "100% SUCCESS" : "DEFECTS DETECTED") << ")\n";
        std::cout << "========================================================================================\n";
        pauseConsole();
    }

    // --- File Handling & Initial Sample Seeds ---
    void seedDefaultData() {
        studentList.clear();
        courses.clear();
        results.clear();
        registrations.clear();

        // 8 Realistic Students
        studentList.insertTail(Student("Aarav Sharma", "EN2024001", "CS-24-01", "Computer Science & Engineering", 2, "aarav.sharma@campus.edu", "+91 98765 43210"));
        studentList.insertTail(Student("Priya Patel", "EN2024002", "CS-24-02", "Computer Science & Engineering", 3, "priya.patel@campus.edu", "+91 98765 43211"));
        studentList.insertTail(Student("Rohan Mehta", "EN2024003", "CS-24-03", "Computer Science & Engineering", 2, "rohan.mehta@campus.edu", "+91 98765 43212"));
        studentList.insertTail(Student("Ananya Iyer", "EN2024004", "IT-24-15", "Information Technology", 2, "ananya.iyer@campus.edu", "+91 98765 43213"));
        studentList.insertTail(Student("Vikram Singh", "EN2024005", "EC-24-08", "Electronics & Communication", 2, "vikram.singh@campus.edu", "+91 98765 43214"));
        studentList.insertTail(Student("Sneha Reddy", "EN2024006", "CS-24-06", "Computer Science & Engineering", 4, "sneha.reddy@campus.edu", "+91 98765 43215"));
        studentList.insertTail(Student("Devansh Gupta", "EN2024007", "ME-24-21", "Mechanical Engineering", 1, "devansh.gupta@campus.edu", "+91 98765 43216"));
        studentList.insertTail(Student("Meera Nair", "EN2024008", "IT-24-22", "Information Technology", 3, "meera.nair@campus.edu", "+91 98765 43217"));

        // 8 Courses
        courses.push_back(Course("CS101", "Programming in C & Problem Solving", 4, 1, "NONE"));
        courses.push_back(Course("CS102", "Discrete Mathematics & Linear Algebra", 4, 1, "NONE"));
        courses.push_back(Course("CS201", "Data Structures & Algorithms", 4, 2, "CS101"));
        courses.push_back(Course("CS202", "Object-Oriented Programming with C++", 3, 2, "CS101"));
        courses.push_back(Course("CS301", "Database Management Systems", 3, 3, "NONE"));
        courses.push_back(Course("CS302", "Operating Systems Architecture", 4, 3, "CS201"));
        courses.push_back(Course("CS303", "Computer Networks & Protocols", 3, 3, "CS101"));
        courses.push_back(Course("CS401", "Advanced Software Engineering", 3, 4, "CS202"));

        // Results
        Result r1("EN2024001", 1, "Exemplary performance");
        r1.addSubject("CS101", "Programming in C & Problem Solving", 4, 36, 50); // 86 (A+)
        r1.addSubject("CS102", "Discrete Mathematics & Linear Algebra", 4, 32, 48); // 80 (A+)
        results.push_back(r1);

        Result r2("EN2024002", 1, "High Honors");
        r2.addSubject("CS101", "Programming in C & Problem Solving", 4, 38, 54); // 92 (O)
        r2.addSubject("CS102", "Discrete Mathematics & Linear Algebra", 4, 35, 53); // 88 (A+)
        results.push_back(r2);

        Result r3("EN2024003", 1, "Needs remedial study");
        r3.addSubject("CS101", "Programming in C & Problem Solving", 4, 12, 20); // 32 (F)
        r3.addSubject("CS102", "Discrete Mathematics & Linear Algebra", 4, 22, 35); // 57 (B)
        results.push_back(r3);

        Result r4("EN2024004", 1, "Good academic standing");
        r4.addSubject("CS101", "Programming in C & Problem Solving", 4, 30, 45); // 75 (A)
        r4.addSubject("CS102", "Discrete Mathematics & Linear Algebra", 4, 28, 44); // 72 (A)
        results.push_back(r4);

        // Registrations
        registrations.push_back(Registration("EN2024001", "CS201", 2, "CONFIRMED", "Prerequisite CS101 Verified (86% A+)"));
        registrations.push_back(Registration("EN2024001", "CS202", 2, "CONFIRMED", "Prerequisite CS101 Verified (86% A+)"));
        registrations.push_back(Registration("EN2024002", "CS301", 3, "CONFIRMED", "No prerequisite required"));
        registrations.push_back(Registration("EN2024002", "CS302", 3, "CONFIRMED", "Prerequisite CS201 Verified"));
        registrations.push_back(Registration("EN2024004", "CS201", 2, "CONFIRMED", "Prerequisite CS101 Verified (75% A)"));
        registrations.push_back(Registration("EN2024007", "CS101", 1, "CONFIRMED", "Introductory course"));
        registrations.push_back(Registration("EN2024007", "CS102", 1, "CONFIRMED", "Introductory course"));
    }

    void saveAllData() const {
        std::ofstream sOut(studentFile);
        if (sOut.is_open()) {
            for (const auto& s : studentList.toVector()) sOut << s.serialize() << "\n";
            sOut.close();
        }

        std::ofstream cOut(courseFile);
        if (cOut.is_open()) {
            for (const auto& c : courses) cOut << c.serialize() << "\n";
            cOut.close();
        }

        std::ofstream rOut(resultFile);
        if (rOut.is_open()) {
            for (const auto& res : results) rOut << res.serialize() << "\n";
            rOut.close();
        }

        std::ofstream regOut(regFile);
        if (regOut.is_open()) {
            for (const auto& r : registrations) regOut << r.serialize() << "\n";
            regOut.close();
        }
    }

    bool loadAllData() {
        std::ifstream sIn(studentFile);
        if (!sIn.is_open()) return false;

        studentList.clear();
        std::string line;
        while (std::getline(sIn, line)) {
            if (!line.empty()) {
                Student s;
                s.deserialize(line);
                studentList.insertTail(s);
            }
        }
        sIn.close();

        std::ifstream cIn(courseFile);
        courses.clear();
        while (std::getline(cIn, line)) {
            if (!line.empty()) {
                Course c;
                c.deserialize(line);
                courses.push_back(c);
            }
        }
        cIn.close();

        std::ifstream rIn(resultFile);
        results.clear();
        while (std::getline(rIn, line)) {
            if (!line.empty()) {
                Result res;
                res.deserialize(line);
                results.push_back(res);
            }
        }
        rIn.close();

        std::ifstream regIn(regFile);
        registrations.clear();
        while (std::getline(regIn, line)) {
            if (!line.empty()) {
                Registration r;
                r.deserialize(line);
                registrations.push_back(r);
            }
        }
        regIn.close();

        return (studentList.getSize() > 0);
    }

    void resetDemoDataWorkflow() {
        std::cout << "Are you sure you want to reset all data back to default Phase III demo records? (y/n): ";
        char c;
        std::cin >> c;
        if (c == 'y' || c == 'Y') {
            seedDefaultData();
            saveAllData();
            std::cout << "\n[SUCCESS] System database reset to default PBL Phase III state!\n";
        } else {
            std::cout << "Reset cancelled.\n";
        }
        pauseConsole();
    }
};

// ========================================================================================
// 9. MAIN ENTRY POINT (Terminal Menu Loop)
// ========================================================================================
int main() {
    AcademicPortal portal;

    while (true) {
        std::cout << "\n========================================================================================\n";
        std::cout << "                  STUDENT ACADEMIC RESULT & COURSE REGISTRATION SYSTEM\n";
        std::cout << "                 Phase III College Project-Based Learning (Native C++)\n";
        std::cout << "========================================================================================\n";
        std::cout << " [1] Dashboard Overview (Live Statistics & Quick Feeds)\n";
        std::cout << " [2] Student Management Module (CRUD, Profile & Search)\n";
        std::cout << " [3] Course Curriculum Management Module (Catalog & Prerequisites)\n";
        std::cout << " [4] Course Registration Module (Automated Prerequisite Checking)\n";
        std::cout << " [5] Result Processing & Official University Marksheet Generator\n";
        std::cout << " [6] Data Structures & Algorithms (DSA) Interactive Showcase\n";
        std::cout << " [7] Object-Oriented Programming (OOP) Architecture Deep-Dive\n";
        std::cout << " [8] Run Automated Diagnostic Test Suite (8 Integration Assertions)\n";
        std::cout << " [9] Reset Demonstration Sample Data\n";
        std::cout << " [0] Exit Application\n";
        std::cout << "----------------------------------------------------------------------------------------\n";
        std::cout << " Enter choice [0-9]: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 0) {
            std::cout << "\nThank you for evaluating the Student Academic Portal. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: portal.displayDashboard(); break;
            case 2: portal.studentManagementMenu(); break;
            case 3: portal.courseManagementMenu(); break;
            case 4: portal.registrationMenu(); break;
            case 5: portal.resultManagementMenu(); break;
            case 6: portal.dsaShowcaseMenu(); break;
            case 7: portal.oopArchitectureMenu(); break;
            case 8: portal.runAutomatedTestSuite(); break;
            case 9: portal.resetDemoDataWorkflow(); break;
            default:
                std::cout << "Invalid selection. Please choose between 0 and 9.\n";
                break;
        }
    }

    return 0;
}
