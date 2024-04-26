/*
CS-300 Project Two by Dalton Rose
Note: In Project One, I recommended using a hash table. However, I encountered some problems in my initial implementation using the hash tables.
Note (continued): I revised it using a vector to help me troubleshoot, although the issue ended up being unrelated (Byte-Order Mark being left on the downloaded file).
Note (continued): However, at that point, I decided to continue using a vector instead of rewriting it to use hash tables. I hope this is okay.
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <cstring>
using namespace std;

struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;

    // Default Constructor
    Course() {

    }

    // Constructor for creating a Course with a courseNumber, courseName, and a prerequisites vector
    Course(string courseNumber, string courseName, vector<string> prerequisites) {
        this->courseNumber = courseNumber;
        this->courseName = courseName;
        this->prerequisites = prerequisites;
    }
};

/*
Skips byte-order mark

Description: Code to Ignore Byte-Order Mark
Author: Stack Overflow/bames53
Date: 2012/01/16
URL: https://stackoverflow.com/questions/8880548/ignore-byte-order-marks-in-c-reading-from-a-stream
*/
void SkipBOM(ifstream& in) {
    {
        char test[3] = { 0 };
        in.read(test, 3);
        if ((unsigned char)test[0] == 0xEF &&
            (unsigned char)test[1] == 0xBB &&
            (unsigned char)test[2] == 0xBF)
        {
            return;
        }
        in.seekg(0);
    }
}

/*
Reads data from the input file and creates a vector storing Course objects based on the specifications in the file
*/
vector<Course> importData(string fileName) {
    // Initialize vector to store Courses
    vector<Course> courseList;

    // Initialize file stream and skip byte-order mark
    ifstream inputFile(fileName, ios::in | ios::binary);
    SkipBOM(inputFile);
    
    // Inform the user if there are issues opening the file
    if (!inputFile) {
        cout << endl << "Error loading file." << endl;
    }

    string inputLine;

    // Until eof reached, separate the elements in the line and store them in courseNumber, courseName, and the prerequisiteCourses vector
    while (getline(inputFile, inputLine)) {
        istringstream iss(inputLine);
        string courseNumber;
        string courseName;
        vector<string> prerequisiteCourses;

        if (getline(iss, courseNumber, ',') && getline(iss, courseName, ',')) {
            string prereq;
            while (getline(iss, prereq, ',')) {
                prerequisiteCourses.push_back(prereq);
            }
        }
        else {
            cout << endl << "Invalid line format." << endl;
        }

        // Create a new Course object with the parameters
        Course newCourse = Course(courseNumber, courseName, prerequisiteCourses);

        // Add the Course object to courseList
        courseList.push_back(newCourse);
    }

    inputFile.close();
    return courseList;
}

/*
Prints a list of all courses including the course number and course name
*/
void printCourses(vector<Course> courseList) {
    cout << endl;
    // Iterate through courseList, printing all course's numbers and names
    for (unsigned int i = 0; i < courseList.size(); i++) {
        cout << courseList.at(i).courseNumber;
        cout << ", ";
        cout << courseList.at(i).courseName;
        cout << endl;
    }
}

/*
Prompts user for a course number, searches for that course, and prints information about that course (if found)
*/
void searchCourse(vector<Course> courseList, string courseNumber) {
    // Initialize bool to track if a match is found
    bool found = false;

    // Convert input courseNumber to upper case
    for (unsigned int i = 0; i < courseNumber.size(); i++) {
        courseNumber[i] = ::toupper(courseNumber[i]);
    }

    // Search the vector for a match
    // If a match is found, print out the course details
    for (unsigned int i = 0; i < courseList.size(); i++) {
        if (courseList.at(i).courseNumber == courseNumber) {
            found = true;
            cout << endl;
            cout << courseList.at(i).courseNumber << ", " << courseList.at(i).courseName << endl;
            cout.flush();
            cout << "Prerequisites: ";
            if (courseList.at(i).prerequisites.size() == 0) {
                cout << "None";
            }
            else if (courseList.at(i).prerequisites.size() == 1) {
                cout << courseList.at(i).prerequisites.at(0);
            }
            else {
                for (unsigned int j = 0; j < courseList.at(i).prerequisites.size(); j++) {
                    cout << courseList.at(i).prerequisites.at(j);
                    if ((courseList.at(i).prerequisites.size() - j) != 1 || courseList.at(i).prerequisites.size() == 1) {
                        cout << ", ";
                    }
            }
            }
        }
    }
    // If a match is not found, inform the user that no match was found
    if (found == false) {
        cout << "Course not found.";
    }
    cout << endl << endl;
}

/*
Partitions the vector for sorting
*/
int partition(vector<Course>& courses, int begin, int end) {
    // Initialize variables based on input parameters and calculate the midpoint
    int low = begin;
    int high = end;
    int midpoint = begin + (end - begin) / 2;

    // Create a Course object based on the midpoint element
    Course pivot = courses[midpoint];

    bool done = false;

    while (done == false) {
        // Increment low while low element course number is less than that of pivot course
        while (courses[low].courseNumber < pivot.courseNumber) {
            low += 1;
        }
        // Decrement high while pivot element course number is less than that of high element
        while (pivot.courseNumber < courses[high].courseNumber) {
            high -= 1;
        }
        // Exit loop once all elements have been checked
        if (low >= high) {
            done = true;
        }
        // Swap low and high elements and move low and high closer
        else {
            swap(courses[low], courses[high]);
            low += 1;
            high -= 1;
        }
    }
    return high;
}

/*

*/
void quickSort(vector<Course>& courses, int begin, int end) {
    // Initialize mid variable
    int mid = 0;

    // Return if there are only 1 or 0 elements to sort
    if (begin >= (end - 1)) {
        return;
    }

    // Partition bids into low and high sections and find the pivot point
    mid = partition(courses, begin, end);

    // Recursively sort elements left of pivot
    quickSort(courses, begin, mid);

    // Recursively sort elements right of pivot
    quickSort(courses, mid + 1, end);

    return;
}

int main()
{
    vector<Course> courseList;

    int menu = 0;
    while (menu != 9) {
        cout << "Welcome to the course planner." << endl << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl << endl;
        cout << "What would you like to do? ";
        cin >> menu;

        switch (menu) {
        case 1:
        {            
        cout << "What's the name of the file you would like to load? ";
        string fileName;
        cin >> fileName;
        courseList = importData(fileName);
        quickSort(courseList, 0, courseList.size() - 1);
        break; 
        }
        case 2:
            printCourses(courseList);
            break;
        case 3: {
            cout << "What course do you want to know about? ";
            string inputCourse;
            cin >> inputCourse;
            searchCourse(courseList, inputCourse);
            break;
        }
        case 9:
            menu = 9;
        }
    }
}
