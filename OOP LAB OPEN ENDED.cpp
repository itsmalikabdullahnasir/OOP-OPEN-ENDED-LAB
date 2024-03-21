#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Course;

class Student {
	public: string studentID;
	string name;
	string email;
	vector < Course * > coursesEnrolled;
	void enrollCourse(Course * course);
	void dropCourse(Course * course);
	void viewCourses();
	void save();
	void load();
};
class Teacher {
	public: string teacherID;
	string name;
	string email;
	vector < Course * > coursesTaught;
	void assignCourse(Course * course);
	void removeCourse(Course * course);
	void viewCourses();
	void save();
	void load();
};
class Course {
	public: string courseCode;
	string courseName;
	Teacher * teacher;
	vector < Student * > studentsEnrolled;
	int maxCapacity = 30;
	void addStudent(Student * student);
	void removeStudent(Student * student);
	void viewStudents();
	void save();
	void load();
};
void Student::enrollCourse(Course * course) {
	if(course - > studentsEnrolled.size() < course - > maxCapacity) {
		coursesEnrolled.push_back(course);
		course - > addStudent(this);
	} else {
		cout << "Course is full." << endl;
	}
}
void Student::dropCourse(Course * course) {
	for(int i = 0; i < coursesEnrolled.size(); i++) {
		if(coursesEnrolled[i] == course) {
			coursesEnrolled.erase(coursesEnrolled.begin() + i);
			course - > removeStudent(this);
			break;
		}
	}
}

void Student::viewCourses() {
	for(Course * course: coursesEnrolled) {
		cout << course - > courseName << endl;
	}
}
void Student::save() {
	ofstream file("student.txt", ios::app);
	file << studentID << endl;
	file << name << endl;
	file << email << endl;
	for(Course * course: coursesEnrolled) {
		file << course - > courseCode << endl;
	}
	file.close();
}

void Teacher::save() {
	ofstream file("Teacher.txt", ios::app);
	file << teacherID << endl;
	file << name << endl;
	file << email << endl;
	for(Course * course: coursesTaught) {
		file << course - > courseCode << endl;
	}
	file.close();
}
void Course::save() {
	ofstream file("Course.txt", ios::app);
	file << courseCode << endl;
	file << courseName << endl;
	if(teacher != nullptr) {
		file << teacher - > teacherID << endl;
	}
	for(Student * student: studentsEnrolled) {
		file << student - > studentID << endl;
	}
	file.close();
}

void Student::load() {
	ifstream file("student.txt", ios::app);
	if(file.is_open()) {
		getline(file, studentID);
		getline(file, name);
		getline(file, email);
		string courseCode;
		while(getline(file, courseCode)) {
			Course * course = new Course();
			course - > courseCode = courseCode;
			coursesEnrolled.push_back(course);
		}
		file.close();
	} else {
		cout << "No saved data found." << endl;
	}
}
void Teacher::load() {
	ifstream file("Teacher.txt", ios::app);
	if(file.is_open()) {
		getline(file, teacherID);
		getline(file, name);
		getline(file, email);
		string courseCode;
		while(getline(file, courseCode)) {
			Course * course = new Course();
			course - > courseCode = courseCode;
			coursesTaught.push_back(course);
		}
		file.close();
	} else {
		cout << "No saved data found." << endl;
	}
}

void Course::load() {
	ifstream file("Course.txt", ios::app);
	if(file.is_open()) {
		getline(file, courseCode);
		getline(file, courseName);
		string teacherId;
		getline(file, teacherId);
		teacher = new Teacher();
		teacher - > teacherID = teacherId;
		string studentId;
		while(getline(file, studentId)) {
			Student * student = new Student();
			student - > studentID = studentId;
			studentsEnrolled.push_back(student);
		}
		file.close();
	} else {
		cout << "No saved data found." << endl;
	}
}
void Teacher::assignCourse(Course * course) {
	coursesTaught.push_back(course);
	course - > teacher = this;
}
void Teacher::removeCourse(Course * course) {
	for(int i = 0; i < coursesTaught.size(); i++) {
		if(coursesTaught[i] == course) {
			coursesTaught.erase(coursesTaught.begin() + i);
			course - > teacher = nullptr;
			break;
		}
	}
}
void Teacher::viewCourses() {
	for(Course * course: coursesTaught) {
		cout << course - > courseName << endl;
	}
}
void Course::addStudent(Student * student) {
	studentsEnrolled.push_back(student);
}
void Course::removeStudent(Student * student) {
	for(int i = 0; i < studentsEnrolled.size(); i++) {
		if(studentsEnrolled[i] == student) {
			studentsEnrolled.erase(studentsEnrolled.begin() + i);
			break;
		}
	}
}
void Course::viewStudents() {
	for(Student * student: studentsEnrolled) {
		cout << student - > name << endl;
	}
}
int main() {
	vector < Student > students;
	vector < Teacher > teachers;
	vector < Course > courses;
	int choice;
	do {
		cout << "***********University Management System*********:" << endl;
		cout << "1. ** Add student **" << endl;
		cout << "2. ** Add teacher **" << endl;
		cout << "3. ** Add course **" << endl;
		cout << "4. ** student in course**" << endl;
		cout << "5. ** Assign teacher to course **" << endl;
		cout << "6. ** See students **" << endl;
		cout << "7. ** See faculty **" << endl;
		cout << "8. ** Browse courses **" << endl;
		cout << "9. ** Save all data **" << endl;
		cout << "10. ** Import all data **" << endl;
		cout << "0. ** Exit **" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		switch(choice) {
			case 1: {
				string id, name, email;
				cout << "Enter student ID: ";
				cin >> id;
				cin.ignore();
				cout << "Enter student name: ";
				getline(cin, name);
				cout << "Enter student email: ";
				getline(cin, email);
				students.push_back({
					id,
					name,
					email
				});
				break;
			}
			case 2: {
				string id, name, email;
				cout << "Enter teacher ID: ";
				cin >> id;
				cin.ignore();
				cout << "Enter teacher name: ";
				getline(cin, name);
				cout << "Enter teacher email: ";
				getline(cin, email);
				teachers.push_back({
					id,
					name,
					email
				});
				break;
			}
			case 3: {
				string code, name;
				cout << "Enter Course code: ";
				cin >> code;
				cout << "Enter Course name: ";
				cin >> name;
				courses.push_back({
					code,
					name,
					nullptr
				});
				break;
			}
			case 4: {
				string studentId, courseCode;
				cout << "Students:" << endl;
				for(const Student & student: students) {
					cout << student.name << ": " << student.studentID << endl;
				}
				cout << "Courses:" << endl;
				for(const Course & course: courses) {
					cout << course.courseName << ": " << course.courseCode << endl;
				}
				cout << "Enter student ID: ";
				cin >> studentId;
				cout << "Enter course code: ";
				cin >> courseCode;
				students[0].enrollCourse( & courses[0]);
				break;
			}
			case 5: {
				string teacherId, courseCode;
				cout << "Teachers:" << endl;
				for(const Teacher & teacher: teachers) {
					cout << teacher.name << endl;
				}
				cout << "Courses:" << endl;
				for(const Course & course: courses) {
					cout << course.courseName << ": " << course.courseCode << endl;
				}
				cout << "Enter teacher ID: ";
				cin >> teacherId;
				cout << "Enter course code: ";
				cin >> courseCode;
				teachers[0].assignCourse( & courses[0]);
				break;
			}
			case 6: {
				cout << "Students:" << endl;
				for(const Student & student: students) {
					cout << student.name << ": " << student.studentID << endl;
				}
				break;
			}
			case 7: {
				cout << "Teachers:" << endl;
				for(const Teacher & teacher: teachers) {
					cout << teacher.name << ":" << teacher.teacherID << endl;
				}
				break;
			}
			case 8: {
				cout << "Courses:" << endl;
				for(const Course & course: courses) {
					cout << course.courseName << ": " << course.courseCode << endl;
				}
				break;
			}
			case 9:
				if(!students.empty()) {
					for(int i = 0; i < students.size(); i++) {
						students[i].save();
					}
					for(int i = 0; i < teachers.size(); i++) {
						teachers[i].save();
					}
					for(int i = 0; i < courses.size(); i++) {
						courses[i].save();
					}
				}
				break;
			case 10:
				if(!students.empty()) {
					for(int i = 0; i < students.size(); i++) {
						students[0].load();
					}
					for(int i = 0; i < teachers.size(); i++) {
						teachers[0].load();
					}
					for(int i = 0; i < courses.size(); i++) {
						courses[0].load();
					}
				}
				break;
			case 0:
				cout << "Exiting..." << endl;
				break;
			default:
				cout << "Invalid choice. Please try again." << endl;
				break;
		}
	} while(choice != 0);
	system("pause");
	return 0;
}
