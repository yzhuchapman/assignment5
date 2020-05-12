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
#include "menu.h"

Menu::Menu()
{
	masterStudent = new BST<Student>("studentTable.txt");
	masterFaculty = new BST<Faculty>("facultyTable.txt");
}

Menu::~Menu()
{
	delete masterStudent;
	delete masterFaculty;
}

//helper methods for menu functions
int Menu::getInt(string prompt) //prompts user for int and returns value
{
	cout << prompt << ": ";
	int value;
	cin >> value;
	return value;
}

string Menu::getString (string prompt) //prompts user for string and returns value
{
	cout << prompt << ": ";
	string value;
	cin >> value;
	return value;
}

float Menu::getFloat (string prompt) // prompts user for float and returns value
{
	cout << prompt << ": ";
	float value;
	cin >> value;
	return value;
}

bool Menu::hasStudent(Faculty * faculty, int ID) // returns true if faculty contains student with given ID as advisee, or false otherwise
{
	for (int i = 0; i < faculty->students.size(); i++)
		if (faculty->students[i] == ID)
			return true;
	return false;
}

void Menu::doRemoveStudentAdviseeFromFaculty(int studentID, int advisorID) // removes student advisee from faculty
{
	Faculty *advisor = masterFaculty->search(advisorID);

	if (advisor)
	{
		for (int i=0; i<advisor->students.size(); i++)
		{
			if (advisor->students[i] == studentID)
			{
				advisor->students.erase(advisor->students.begin()+i);
				break;
			}
		}
	}
}

void Menu::pushAction(int db) // pushes tree to rollback
{
	dbActions.push_front(db);

	while(dbActions.size() > MAXROLLBACKS)
		dbActions.pop_back();
}

// MENU METHODS

void Menu::printAllStudentsAndInfo() // prints all students info
{
	masterStudent->displayAll();
}

void Menu::printAllFacultyAndInfo() // prints all faculty info
{
	masterFaculty->displayAll();
}

void Menu::findAndDisplayStudentByID() // find and displays student by ID
{
	int studentID = getInt ("\nEnter StudentID to search for");
	if (studentID == 0) return;
	Student *student = masterStudent->search (studentID);
	if (student == NULL) cout << "Student ID #" << studentID << " is not on file.\n";
	else
		student->display();
}

void Menu::findAndDisplayFacultyByID() // finds and displays faculty by ID, if faculty is on file
{
	int facultyID = getInt("\nEnter facultyID to search for");
	if (facultyID == 0) return;
	Faculty * faculty = masterFaculty->search(facultyID);
	if (faculty == NULL) cout << "Faculty ID #" << facultyID << " is not on file.\n";
	else
		faculty->display();
}

void Menu::getFacultyByStudentID() // gets faculty by student ID, if student id is onfile
{
	int studentID = getInt("\nEnter student ID");
	Student *student = masterStudent->search(studentID);
	if (studentID == 0) return;
	if(student == NULL)
	{
		cout << "Student ID #" << studentID << " is not on file.\n";
		return;
	}
	Faculty *advisor = masterFaculty->search(student->advisorID);
	if (advisor == NULL) cout << "No advisor on record for student ID# " << studentID << '\n';
	else advisor->display();
}

void Menu::printStudentAdvisees() // prints advisees info, given faculty ID
{
	int facultyID = getInt("\nEnter faculty ID");
	if (facultyID == 0) return;
	Faculty * faculty = masterFaculty->search(facultyID);
	if (faculty == NULL)
	{
		cout << "Faculty ID " << facultyID << " is not on file\n";
		return;
	}
	cout << "Student advisees for " << faculty->name << '\n';
	for (int i = 0; i < faculty->students.size(); i++)
	{
		Student *student = masterStudent->search(faculty->students[i]);
		if (student == NULL) cout << "Advisee's student ID #" << faculty->students[i] << " is not on file.\n";
		else
		{
			student->display();
			cout << '\n';
		}
	}
}

void Menu::addNewStudent() // creates student, asks user to enter information and Adds student to tree
{
	masterStudent->snapshot();

	int studentID = getInt("\nStudent ID..........");
	if (studentID == 0) return;
	string name = getString("Name................ ");
	string level = getString("Level............... ");
	string major = getString("Major............... ");
	int advisorID = getInt("Advisor ID.......... ");
	Faculty *faculty = masterFaculty->search(advisorID);
	if (faculty == NULL)
	{
		cout << "Faculty ID " << advisorID << " is not on file. Redo command.\n";
		return;
	}
	faculty->students.push_back(studentID);
	float gpa = getFloat("GPA................. ");
	Student student = Student(studentID, name, level, major, advisorID, gpa);
	masterStudent->insert(student);

	pushAction(STUDENTS);
}

void Menu::deleteStudentByID() // deletes student by ID
{
	masterStudent->snapshot();

	int studentID = getInt("\nEnter studentID to delete");
	Student *student = masterStudent->search (studentID);
	if (studentID == 0) return;
	if(student == NULL)
	{
		cout << "Student ID #" << studentID << " is not on file.\n";
		return;
	}
	Faculty *faculty = masterFaculty->search (student->advisorID);

	faculty->removeAdvisee(studentID);
	masterStudent->remove(studentID);

	pushAction(STUDENTS);

	doRemoveStudentAdviseeFromFaculty(studentID, student->advisorID);
}

void Menu::deleteFacultyByID() // delete faculty by ID and asks user to assign new advisors to advisees
{
	int facultyID = getInt("\nEnter Faculty ID to delete");
	if (facultyID == 0) return;

	Faculty * faculty = masterFaculty->search(facultyID);
	if (faculty == NULL)
	{
		cout << "Faculty ID " << facultyID << " is not on file\n";
		return;
	}

	int newFacultyID = getInt("Must assign new advisor to advisees. Enter new advisorID for advisees");
	Faculty *newFaculty = masterFaculty->search(newFacultyID);
	if (newFaculty == NULL)
	{
		cout << "Faculty ID " << facultyID << " is not on file\n";
		return;
	}

	masterFaculty->snapshot();

	int action = FACULTY;

	if (!faculty->students.empty())
	{
		action = BOTH;
		masterStudent->snapshot();
	}

	for (int i = 0; i < faculty->students.size(); i++)
	{
		Student *student = masterStudent->search(faculty->students[i]);
		newFaculty->students.push_back(student->studentID);
		student->advisorID = newFacultyID;
	}
	masterFaculty->remove(facultyID);

	pushAction(action);
}

void Menu::addNewFaculty() // creates faculty and asks user for info, including advisee ID's
{
	masterFaculty->snapshot();

	int facultyID = getInt("\nFaculty ID.......... ");
	if ( facultyID == 0 ) return;
	string name = getString("Name................ ");
	string level = getString("Level............... ");
	string dept = getString("Department.......... ");
	cout << "Student advisee IDs (enter 0 when finished)\n";

	vector<int> advisees;
	int studentID = getInt(">");
	while ( studentID != 0 )
	{
		Student *student = masterStudent->search(studentID);
		if(student == NULL)
		{
			cout << "Student ID #" << studentID << " is not on file.\n";
			return;
		}
		if(student->advisorID != 0) // let's user know if a student already has a faculty advisor
  {
		{
			cout << "WARNING: Student already has advisor. INSTEAD: Redo command and add new faculty without current student then change current student's advisor."<<endl;
			return;
		}

		if(studentID > 0) advisees.push_back(studentID);

		studentID = getInt ( ">" );
	}
 }
	Faculty faculty = Faculty(facultyID, name, level, dept, advisees);
	masterFaculty->insert(faculty);

	pushAction(FACULTY);
}
void Menu::changeStudentsAdvisor() // changes student's advisor, and changes faculty's advisees corerespondingly
{
	int studentID = getInt("\nEnter student ID");
	Student *student = masterStudent->search(studentID);
	if (student == NULL) return;
	int facultyID = getInt("Enter new advisor ID");
	Faculty *newFaculty = masterFaculty->search(facultyID);
	if (newFaculty == NULL)
	{
		cout << "Faculty ID " << facultyID << " is not on file\n";
		return;
	}
	else
	{
		masterStudent->snapshot();
		student->advisorID = newFaculty->facultyID;

		masterFaculty->snapshot();
		newFaculty->students.push_back(studentID);
	}

	pushAction(BOTH);
}

void Menu::removeAdviseeFromFaculty() // removes advisee from faculty and
{
	int studentID = getInt("\nEnter student ID");
	Student *student = masterStudent->search(studentID);
	if (studentID == 0) return;

	masterFaculty->snapshot();
	masterStudent->snapshot();

	if(student != NULL)
	{
		Faculty *advisor = masterFaculty->search(student->advisorID);
		advisor->removeAdvisee(studentID);
		student->advisorID = 0;

		pushAction(BOTH);
	}else
	{
		cout << "Student " << studentID << " is not on file\n";
	}
}

void Menu::rollback() // manipulates DB's stored
{
	if (dbActions.empty()) return;

	int lastDB = dbActions.front();

	if (lastDB == STUDENTS || lastDB == BOTH)
		masterStudent->rollback();

	if (lastDB == FACULTY || lastDB == BOTH)
		masterFaculty->rollback();

	dbActions.pop_front();
}

void Menu::printMenu() // prints options for user
{
	cout << '\n' << "Menu:" << '\n' << '\n';
	cout << " 1. Print all students and their information (sorted by ascending id #)" << '\n';
	cout << " 2. Print all faculty and their information (sorted by ascending id #)" << '\n';
	cout << " 3. Find and display student information given the students id" << '\n';
	cout << " 4. Find and display faculty information given the faculty id" << '\n';
	cout << " 5. Given a student’s id, print the name and info of their faculty advisor" << '\n';
	cout << " 6. Given a faculty id, print ALL the names and info of his/her advisees." << '\n';
	cout << " 7. Add a new student" << '\n';
	cout << " 8. Delete a student given the id" << '\n';
	cout << " 9. Add a new faculty member" << '\n';
	cout << "10. Delete a faculty member given the id." << '\n';
	cout << "11. Change a student’s advisor given the student id and the new faculty id." << '\n';
	cout << "12. Remove an advisee from a faculty member given the ids" << '\n';
	cout << "13. Rollback" << '\n';
	cout << "14. Exit" << '\n' << '\n';
}

bool Menu::performCommand (int command) // performs menu option based on passed in command
{
	switch(command)
	{
		case 1:
			printAllStudentsAndInfo();
			break;
		case 2:
			printAllFacultyAndInfo();
			break;
		case 3:
			findAndDisplayStudentByID();
			break;
		case 4:
			findAndDisplayFacultyByID();
			break;
		case 5:
			getFacultyByStudentID();
			break;
		case 6:
			printStudentAdvisees();
			break;
		case 7:
			addNewStudent();
			break;
		case 8:
			deleteStudentByID();
			break;
		case 9:
			addNewFaculty();
			break;
		case 10:
			deleteFacultyByID();
			break;
		case 11:
			changeStudentsAdvisor();
			break;
		case 12:
			removeAdviseeFromFaculty();
			break;
		case 13:
			rollback();
			break;
		case 14: cout << "Exit" << '\n';
			return false;
			break;
	}

	return true;
}

void Menu::doMenu() // loop that prints menu and allows user to interact with database until exits
{
	while(true){
		printMenu();
		int command = getInt("Select Command: ");
		if (!performCommand(command))
			break;
	}
}

void Menu::initDatabases() //deserializes student/faculty trees
{
	masterStudent->deserialize();
	masterFaculty->deserialize();
}

void Menu::saveDatabases() //serializes student/faculty trees
{
	masterStudent->serialize();
	masterFaculty->serialize();
}
