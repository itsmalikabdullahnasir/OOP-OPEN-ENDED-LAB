#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

class Student;
class Teacher;
class Course;

class Student {
public:
    Student(int id, std::string name, std::string email) : studentID(id), studentName(name), studentEmail(email) {}

    void enrollCourse(Course* course);
    void dropCourse(Course* course);
    void viewCourses();


    int studentID;
    std::string studentName;
    std::string studentEmail;
    std::vector<Course*> coursesEnrolled;
};

class Teacher {
public:
    Teacher(int id, std::string name, std::string email) : teacherID(id), teacherName(name), teacherEmail(email) {}

    void assignCourse(Course* course);
    void removeCourse(Course* course);
    void viewCourses();

    int teacherID;
    std::string teacherName;
    std::string teacherEmail;
    std::vector<Course*> coursesTaught;
};


class Course {
public:
    Course(std::string code, std::string name, Teacher* teacher, int capacity) : courseCode(code), courseName(name), teacher(teacher), maxCapacity(capacity) {}

    void addStudent(Student* student);
    void removeStudent(Student* student);
    void viewStudents();

    std::string courseCode;
    std::string courseName;
    Teacher* teacher;
    std::vector<Student*> studentsEnrolled;
    int maxCapacity;
};

std::vector<std::string> split(std::string basicString, char i);

void Student::enrollCourse(Course* course) {
    if (course->studentsEnrolled.size() < course->maxCapacity) {
        course->studentsEnrolled.push_back(this);
        coursesEnrolled.push_back(course);
        std::cout << "Successfully enrolled in " << course->courseName << std::endl;
    } else {
        std::cout << "Course " << course->courseName << " is full." << std::endl;
    }
}

void Student::dropCourse(Course* course) {
    for (auto it = coursesEnrolled.begin(); it != coursesEnrolled.end(); ++it) {
        if (*it == course) {
            coursesEnrolled.erase(it);
            course->studentsEnrolled.erase(std::remove(course->studentsEnrolled.begin(), course->studentsEnrolled.end(), this), course->studentsEnrolled.end());
            std::cout << "Successfully dropped " << course->courseName << std::endl;
            break;
        }
    }
}

void Student::viewCourses() {
    std::cout << "Enrolled Courses:" << std::endl;
    for (auto course : coursesEnrolled) {
        std::cout << course->courseName << std::endl;
    }
}

void Teacher::assignCourse(Course* course) {
    coursesTaught.push_back(course);
    course->teacher = this;
}

void Teacher::removeCourse(Course* course) {
    for (auto it = coursesTaught.begin(); it != coursesTaught.end(); ++it) {
        if (*it == course) {
            coursesTaught.erase(it);
            course->teacher = nullptr;
            break;
        }
    }
}

void Teacher::viewCourses() {
    std::cout << "Taught Courses:" << std::endl;
    for (auto course : coursesTaught) {
        std::cout << course->courseName << std::endl;
    }
}

void Course::addStudent(Student* student) {
    if (studentsEnrolled.size() < maxCapacity) {
        studentsEnrolled.push_back(student);
        student->enrollCourse(this);
    } else {
        std::cout << "Course is full, cannot enroll student." << std::endl;
    }
}

void Course::removeStudent(Student* student) {
    for (auto it = studentsEnrolled.begin(); it != studentsEnrolled.end(); ++it) {
        if (*it == student) {
            studentsEnrolled.erase(it);
            student->dropCourse(this);
            break;
        }
    }
}

void Course::viewStudents() {
    std::cout << "Enrolled Students:" << std::endl;
    for (auto student : studentsEnrolled) {
        std::cout << student->studentName << std::endl;
    }
}

void saveData(const std::vector<Student>& students, const std::vector<Teacher>& teachers, const std::vector<Course>& courses) {
    std::ofstream studentsFile("students.txt");
    std::ofstream teachersFile("teachers.txt");
    std::ofstream coursesFile("courses.txt");

    for (const auto& student : students) {
        studentsFile << student.studentID << "|" << student.studentName << "|" << student.studentEmail;
        for (const auto& course : student.coursesEnrolled) {
            studentsFile << "|" << course->courseCode;
        }
        studentsFile << "\n";
    }

    for (const auto& teacher : teachers) {
        teachersFile << teacher.teacherID << "|" << teacher.teacherName << "|" << teacher.teacherEmail;
        for (const auto& course : teacher.coursesTaught) {
            teachersFile << "|" << course->courseCode;
        }
        teachersFile << "\n";
    }

    for (const auto& course : courses) {
        coursesFile << course.courseCode << "|" << course.courseName << "|" << course.teacher->teacherID;
        coursesFile << "|" << course.maxCapacity << "|";
        for (const auto& student : course.studentsEnrolled) {
            coursesFile << student->studentID << "|";
        }
        coursesFile << "\n";
    }

    studentsFile.close();
    teachersFile.close();
    coursesFile.close();
}

void loadData(std::vector<Student>& students, std::vector<Teacher>& teachers, std::vector<Course>& courses) {
    std::ifstream studentsFile("students.txt");
    std::ifstream teachersFile("teachers.txt");
    std::ifstream coursesFile("courses.txt");

    std::string line;

    while (std::getline(studentsFile, line)) {
        std::vector<std::string> tokens = split(line, '|');
        int id = std::stoi(tokens[0]);
        std::string name = tokens[1];
        std::string email = tokens[2];

        Student student(id, name, email);
        students.push_back(student);

        std::vector<std::string> courseCodes;
        for (int i = 3; i < tokens.size(); ++i) {
            courseCodes.push_back(tokens[i]);
        }

        for (const auto& courseCode : courseCodes) {
            for (auto& course : courses) {
                if (course.courseCode == courseCode) {
                    student.coursesEnrolled.push_back(&course);
                    course.studentsEnrolled.push_back(&students.back());
                    break;
                }
            }
        }
    }

    while (std::getline(teachersFile, line)) {
        std::vector<std::string> tokens = split(line, '|');
        int id = std::stoi(tokens[0]);
        std::string name = tokens[1];
        std::string email = tokens[2];

        Teacher teacher(id, name, email);
        teachers.push_back(teacher);

        std::vector<std::string> courseCodes;
        for (int i = 3; i < tokens.size(); ++i) {
            courseCodes.push_back(tokens[i]);
        }

        for (const auto& courseCode : courseCodes) {
            for (auto& course : courses) {
                if (course.courseCode == courseCode) {
                    teacher.coursesTaught.push_back(&course);
                    course.teacher = &teachers.back();
                    break;
                }
            }
        }
    }

    while (std::getline(coursesFile, line)) {
        std::vector<std::string> tokens = split(line, '|');
        std::string courseCode = tokens[0];
        std::string courseName = tokens[1];
        int teacherID = std::stoi(tokens[2]);
        int maxCapacity = std::stoi(tokens[3]);

        Teacher* teacher = nullptr;
        for (auto& t : teachers) {
            if (t.teacherID == teacherID) {
                teacher = &t;
                break;
            }
        }

        if (teacher != nullptr) {
            Course course(courseCode, courseName, teacher, maxCapacity);
            courses.push_back(course);

            std::vector<std::string> studentIDs;
            for (int i = 4; i < tokens.size(); ++i) {
                studentIDs.push_back(tokens[i]);
            }

            for (const auto& studentID : studentIDs) {
                int id = std::stoi(studentID);
                for (auto& student : students) {
                    if (student.studentID == id) {
                        student.coursesEnrolled.push_back(&courses.back());
                        courses.back().studentsEnrolled.push_back(&student);
                        break;
                    }
                }
            }
        }
    }

    studentsFile.close();
    teachersFile.close();
    coursesFile.close();
}

std::vector<std::string> split(std::string basicString, char i) {
    return std::vector<std::string>();
}

int main() {
    std::vector<Student> students;
    std::vector<Teacher> teachers;
    std::vector<Course> courses;

    loadData(students, teachers, courses);

    int id = 123;
    std::string name = "Zainab Asif";
    std::string email = "Zasif9204@gmail.com";
    Student newStudent(id, name, email);

    students.push_back(newStudent);
    saveData(students, teachers, courses);

    int choice;
    do {
        std::cout << "\nTeacher M.Waleed Khan\n"
                  << "\nTeacher' Email 'mwaleedkhan@gmail.com'\n"
                  << "1. Enroll in a course\n"
                  << "2. Drop a course\n"
                  << "3. View enrolled courses\n"
                  << "4. Exit\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        if (choice >= 1 && choice <= 3) {
            int studentID;
            std::cout << "Enter student ID: ";
            std::cin >> studentID;



            Student* currentStudent = nullptr;
            for (auto& student : students) {
                if (student.studentID == studentID) {
                    currentStudent = &student;
                    break;
                }
            }

            if (currentStudent == nullptr) {
                std::cout << "Student not found.\n";
                continue;
            }

            std::string courseCode;
            std::cout << "Enter course code: ";
            std::cin >> courseCode;

            Course* currentCourse = nullptr;
            for (auto& course : courses) {
                if (course.courseCode == courseCode) {
                    currentCourse = &course;
                    break;
                }
            }

            if (currentCourse == nullptr) {
                std::cout << "Course not found.\n";
                continue;
            }

            switch (choice) {
                case 1:
                    currentCourse->addStudent(currentStudent);
                    break;
                case 2:
                    currentCourse->removeStudent(currentStudent);
                    break;
                case 3:
                    currentStudent->viewCourses();
                    break;
            }
        }
    } while (choice != 4);

    saveData(students, teachers, courses);

    return 0;
}
