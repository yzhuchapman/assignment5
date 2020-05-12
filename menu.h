#ifndef __MAIN_H
#define __MAIN_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <deque>
#include "Faculty.h"
#include "Student.h"
#include "TreeNode.h"
#include "BST.h"

#define MARKER "NULL"
#define STUDENTFILENAME "students.txt"
#define FACULTYFILENAME "faculty.txt"

#define STUDENTS 0
#define FACULTY 1
#define BOTH 2

#define MAXROLLBACKS 5

class Menu
{

	private:
		BST<Student> *masterStudent;
		BST<Faculty> *masterFaculty;

		deque<int> dbActions; // double ended queue hold up to MAXROLLBACKS transaction types (STUDENTS, FACULTY, or BOTH)

		//helper methods for menu functions
		//prompts user for int and returns value
		int getInt(string prompt);

		//prompts user for string and returns value
		string getString (string prompt);

		// prompts user for float and returns value
		float getFloat (string prompt);

		// Returns true if faculty contains student with given ID as advisee, or false otherwise
		bool hasStudent(Faculty * faculty, int ID);

		// Removes student advisee from faculty
		void doRemoveStudentAdviseeFromFaculty(int studentID, int advisorID);

		// pushes tree to rollback
		void pushAction(int db);

		// prints all students info
		void printAllStudentsAndInfo();
		// prints all faculty info
		void printAllFacultyAndInfo();
		// find and displays student by ID
		void findAndDisplayStudentByID();

		// finds and displays faculty by ID, if faculty is on file
		void findAndDisplayFacultyByID();

		// gets faculty by student ID, if student id is onfile
		void getFacultyByStudentID();

		// Prints advisees info, given faculty ID
		void printStudentAdvisees();

		// Creates student, asks user to enter information and Adds student to tree
		void addNewStudent();

		// deletes student by ID
		void deleteStudentByID();

		// Delete faculty by ID and asks user to assign new advisors to advisees
		void deleteFacultyByID();
		// Creates faculty and asks user for info, including advisee ID's -
		//let's user know if a student already has a faculty advisor
		void addNewFaculty();

		// Changes student's advisor, and changes faculty's advisees corerespondingly
		void changeStudentsAdvisor();

		// removes advisee from faculty and
		void removeAdviseeFromFaculty();

		// Manipulates DB's stored
		void rollback();

		// prints options for user
		void printMenu();

		// performs menu option based on passed in command
		bool performCommand ( int command );

	public:
		Menu();
		~Menu();
		// Loop that prints menu and allows user to interact with database until exits
		void doMenu();

		//deserializes student/faculty trees
		void initDatabases();

		//serializes student/faculty trees
		void saveDatabases();
};

#endif
