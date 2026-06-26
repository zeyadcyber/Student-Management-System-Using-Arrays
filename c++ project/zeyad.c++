#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;

struct Student {
    string studentID;
    string fullName;
    string programme;
    int year;
    double cgpa;
};

class StudentArray {

private:

    Student* students;
    int size;
    int capacity;

    // Merge Sort Functions

    void merge(int left, int mid, int right) {

        int n1 = mid - left + 1;
        int n2 = right - mid;

        Student* L = new Student[n1];
        Student* R = new Student[n2];

        for (int i = 0; i < n1; i++)
            L[i] = students[left + i];

        for (int j = 0; j < n2; j++)
            R[j] = students[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {

            if (L[i].cgpa <= R[j].cgpa)
                students[k++] = L[i++];
            else
                students[k++] = R[j++];
        }

        while (i < n1)
            students[k++] = L[i++];

        while (j < n2)
            students[k++] = R[j++];

        delete[] L;
        delete[] R;
    }

    void mergeSortHelper(int left, int right) {

        if (left < right) {

            int mid = left + (right - left) / 2;

            mergeSortHelper(left, mid);
            mergeSortHelper(mid + 1, right);

            merge(left, mid, right);
        }
    }

public:

    StudentArray(int cap = 50000) {

        capacity = cap;
        size = 0;

        students = new Student[capacity];
    }

    ~StudentArray() {

        delete[] students;
    }

    int getSize() {

        return size;
    }

    bool addStudent(Student s) {

        if (size >= capacity) {

            cout << "Array Full!\n";
            return false;
        }

        if (s.cgpa < 0 || s.cgpa > 4) {

            cout << "Invalid CGPA!\n";
            return false;
        }

        if (linearSearchID(s.studentID) != -1) {

            cout << "Duplicate ID!\n";
            return false;
        }

        students[size++] = s;

        return true;
    }

    int linearSearchID(string id) {

        for (int i = 0; i < size; i++) {

            if (students[i].studentID == id)
                return i;
        }

        return -1;
    }

    int linearSearchName(string name) {

        for (int i = 0; i < size; i++) {

            if (students[i].fullName == name)
                return i;
        }

        return -1;
    }

    bool deleteStudent(string id) {

        int index = linearSearchID(id);

        if (index == -1) {

            cout << "Student Not Found!\n";
            return false;
        }

        for (int i = index; i < size - 1; i++) {

            students[i] = students[i + 1];
        }

        size--;

        return true;
    }

    void displayAll() {

        if (size == 0) {

            cout << "No Records.\n";
            return;
        }

        for (int i = 0; i < size; i++) {

            cout << "\n--------------------------\n";
            cout << "ID: " << students[i].studentID << endl;
            cout << "Name: " << students[i].fullName << endl;
            cout << "Programme: " << students[i].programme << endl;
            cout << "Year: " << students[i].year << endl;
            cout << "CGPA: " << students[i].cgpa << endl;
        }
    }

    void mergeSort() {

        if (size > 1)
            mergeSortHelper(0, size - 1);
    }

    bool loadCSV(string filename) {

        size =  0;

        ifstream file(filename);

        if (!file.is_open()) {

            cout << "Cannot Open File.\n";
            return false;
        }

        string line;

        getline(file, line); // skip header

        while (getline(file, line)) {

            stringstream ss(line);

            Student s;

            string yearStr, cgpaStr;

            getline(ss, s.studentID, ',');
            getline(ss, s.fullName, ',');
            getline(ss, s.programme, ',');
            getline(ss, yearStr, ',');
            getline(ss, cgpaStr, ',');

            try {

                s.year = stoi(yearStr);
                s.cgpa = stod(cgpaStr);

                students[size++] = s;
            }

            catch (...) {

                cout << "Malformed CSV line skipped.\n";
            }
        }

        file.close();

        cout << size << "students loaded successfully.\n";

        return true;
    }

    void measureOperations() {

        if (size == 0) {

            cout << "Load dataset first.\n";
            return;
        }

        auto start = chrono::high_resolution_clock::now();

        linearSearchID(students[size / 2].studentID);

        auto stop = chrono::high_resolution_clock::now();

        auto duration =
            chrono::duration_cast<chrono::microseconds>
            (stop - start);

        cout << "\nSearch Time: "
             << duration.count()
             << " us\n";


        start = chrono::high_resolution_clock::now();

        mergeSort();

        stop = chrono::high_resolution_clock::now();

        duration =
            chrono::duration_cast<chrono::microseconds>
            (stop - start);

        cout << "Sort Time: "
             << duration.count()
             << " us\n";
    }

    void estimateMemory() {

        long long memory =
            sizeof(Student) * size;

        cout << "Approx Memory Usage: "
             << memory
             << " bytes\n";
    }
};

int main() {

    StudentArray system;

    int choice;

    do {

        cout << "\n====== STUDENT SYSTEM ======\n";
        cout << "1. Load CSV\n";
        cout << "2. Add Student\n";
        cout << "3. Delete Student\n";
        cout << "4. Search by ID\n";
        cout << "5. Search by Name\n";
        cout << "6. Display All\n";
        cout << "7. Sort by CGPA\n";
        cout << "8. Measure Performance\n";
        cout << "9. Memory Usage\n";
        cout << "0. Exit\n";

        cin >> choice;
        cin.ignore();

        switch (choice) {

        case 1: {

            string file;

            cout << "Enter CSV filename: ";
            getline(cin, file);

            system.loadCSV(file);
            break;
        }

        case 2: {

            Student s;

            cout << "ID: ";
            getline(cin, s.studentID);

            cout << "Name: ";
            getline(cin, s.fullName);

            cout << "Programme: ";
            getline(cin, s.programme);

            cout << "Year: ";
            cin >> s.year;

            cout << "CGPA: ";
            cin >> s.cgpa;

            cin.ignore();

            system.addStudent(s);

            break;
        }

        case 3: {

            string id;

            cout << "Enter ID: ";
            getline(cin, id);

            system.deleteStudent(id);

            break;
        }

        case 4: {

            string id;

            cout << "Enter ID: ";
            getline(cin, id);

            int pos = system.linearSearchID(id);

            if (pos == -1)
                cout << "Not Found\n";
            else
                cout << "Found at index " << pos << endl;

            break;
        }

        case 5: {

            string name;

            cout << "Enter Name: ";
            getline(cin, name);

            int pos = system.linearSearchName(name);

            if (pos == -1)
                cout << "Not Found\n";
            else
                cout << "Found at index " << pos << endl;

            break;
        }

        case 6:
            system.displayAll();
            break;

        case 7:
            system.mergeSort();
            cout << "Sorted Successfully.\n";
            break;

        case 8:
            system.measureOperations();
            break;

        case 9:
            system.estimateMemory();
            break;

        case 0:
            cout << "Program Ended.\n";
            break;

        default:
            cout << "Invalid Choice.\n";
        }

    } while (choice != 0);

    return 0;
}