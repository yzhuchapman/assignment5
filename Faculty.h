#ifndef Faculty_h
#define Faculty_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>

using namespace std;

class Faculty{
public:
    int facultyID;

    string name;
    string level;
    string department;

    vector<int> students; //used vector for advisee list

    //overloaded operators to compare Faculty based on ID
    bool operator == ( const Faculty &other ) { return this->facultyID == other.facultyID; }
    bool operator != ( const Faculty &other ) { return this->facultyID != other.facultyID; }
    bool operator <  ( const Faculty &other ) { return this->facultyID < other.facultyID;  }
    bool operator >  ( const Faculty &other ) { return this->facultyID > other.facultyID;  }

    bool operator == ( const int ID ) { return this->facultyID == ID; }
    bool operator != ( const int ID ) { return this->facultyID != ID; }
    bool operator <  ( const int ID ) { return this->facultyID <  ID; }
    bool operator >  ( const int ID ) { return this->facultyID >  ID; }

    // Constructor
    Faculty(){
        facultyID = 0;
        name = "unknown";
        level = "unknown";
        department = "unknown";
    }

    // Overloaded Contructor
    Faculty(int facultyID, string name, string level, string department, vector<int> students){
        this->facultyID = facultyID;
        this->name = name;
        this->level = level;
        this->department = department;
        this->students = students;
    }

    // Stores varaibles for faculty
    Faculty(string dataRec) {
        istringstream data(dataRec);
        int numStudents;
        data >> facultyID >> name >> level >> department >> numStudents;
        for (int i = 0; i < numStudents; i++) {
            int studentID;
            data >> studentID;
            students.push_back(studentID);
        }
    }

    void display(){ //method to display Faculty info
        cout << "\nFaculty ID " << facultyID << '\n';
        cout << "Name " << name << '\n';
        cout << "Level " << level << '\n';
        cout << "Department " << department << '\n';
        cout << "Student IDs \n";
        for (int i = 0; i < students.size(); i++){ //traverse through advisee list
            int id = students[i];
            cout << "\t\t " << id << '\n';
        }
    }

    void removeAdvisee(int id)
    {
        int index = 0;
        while(index < students.size())
        {
            if(students[index] == id) break;
            index++;
        }

        students.erase(students.begin() + index);
    }

    virtual string ToString(){ //print method for faculty
        stringstream s;
        string sep = " ";
        s << facultyID << sep << name << sep << level << sep << department << sep << students.size() << sep;
        for (int i = 0; i < students.size (); i++)
            s << students[i] << sep;
        s << '\n';
        string result = s.str();
        return result;
    }
};


#endif
