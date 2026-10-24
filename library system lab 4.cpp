#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct BookRequestNode
{
    int studentID;
    string studentName;
    int bookID;
    string bookTitle;
    int priority;
    BookRequestNode* next;
};

class PriorityQueue
{
private:
    BookRequestNode* front;
    BookRequestNode* rear;
    int count;

    void CopyFrom(const PriorityQueue& other)
    {
        BookRequestNode* current = other.front;
        while (current != nullptr)
        {
            BookRequestNode* newNode = new BookRequestNode;
            newNode->studentID = current->studentID;
            newNode->studentName = current->studentName;
            newNode->bookID = current->bookID;
            newNode->bookTitle = current->bookTitle;
            newNode->priority = current->priority;
            newNode->next = nullptr;
            Add(newNode);
            current = current->next;
        }
    }

    void Clear()
    {
        while (front != nullptr)
        {
            BookRequestNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        count = 0;
    }

public:
    PriorityQueue()
    {
        front = nullptr;
        rear = nullptr;
        count = 0;
    }

    void Add(BookRequestNode* request)
    {
        request->next = nullptr;

        if (front == nullptr)
        {
            front = request;
            rear = request;
        }
        else if (request->priority < front->priority)
        {
            request->next = front;
            front = request;
        }
        else
        {
            BookRequestNode* current = front;
            while (current->next != nullptr &&
                   current->next->priority <= request->priority)
            {
                current = current->next;
            }
            request->next = current->next;
            current->next = request;

            if (request->next == nullptr)
            {
                rear = request;
            }
        }
        count++;
    }

    BookRequestNode* Remove()
    {
        if (front == nullptr)
        {
            return nullptr;
        }
        BookRequestNode* request = front;
        front = front->next;
        if (front == nullptr)
        {
            rear = nullptr;
        }
        request->next = nullptr;
        count--;
        return request;
    }

    bool IsEmpty() { return front == nullptr; }
    bool IsNotEmpty() { return front != nullptr; }
    int GetCount() { return count; }

    void PrintQueue()
    {
        BookRequestNode* current = front;
        while (current != nullptr)
        {
            cout << "Student ID:   " << current->studentID << endl;
            cout << "Student Name: " << current->studentName << endl;
            cout << "Book ID:      " << current->bookID << endl;
            cout << "Book Title:   " << current->bookTitle << endl;
            cout << "Priority:     " << current->priority << endl;
            cout << "-----------------------------" << endl;
            current = current->next;
        }
    }

    PriorityQueue(const PriorityQueue& other)
    {
        front = nullptr;
        rear = nullptr;
        count = 0;
        CopyFrom(other);
    }

    PriorityQueue& operator=(const PriorityQueue& other)
    {
        if (this == &other)
        {
            return *this;
        }
        Clear();
        CopyFrom(other);
        return *this;
    }

    ~PriorityQueue()
    {
        Clear();
    }
};

BookRequestNode* CreateRequest(int studentID, string studentName,
                               int bookID, string bookTitle, int priority)
{
    BookRequestNode* request = new BookRequestNode;
    request->studentID = studentID;
    request->studentName = studentName;
    request->bookID = bookID;
    request->bookTitle = bookTitle;
    request->priority = priority;
    request->next = nullptr;
    return request;
}

int ReadInt(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number." << endl;
    }
}

void ShowMenu()
{
    cout << endl;
    cout << "===== Library Book Request System =====" << endl;
    cout << "1. Add Book Request" << endl;
    cout << "2. Process Highest Priority Request" << endl;
    cout << "3. Display All Requests" << endl;
    cout << "4. Check Queue Status" << endl;
    cout << "5. Exit" << endl;
}

void AddRequest(PriorityQueue& requests)
{
    int studentID = ReadInt("Enter Student ID: ");

    string studentName;
    cout << "Enter Student Name: ";
    getline(cin, studentName);

    int bookID = ReadInt("Enter Book ID: ");

    string bookTitle;
    cout << "Enter Book Title: ";
    getline(cin, bookTitle);

    int priority;
    do
    {
        priority = ReadInt("Enter Priority (1 = Exam/Urgent, 2 = Assignment/Project, 3 = Normal Reading): ");
        if (priority < 1 || priority > 3)
        {
            cout << "Priority must be 1, 2, or 3." << endl;
        }
    } while (priority < 1 || priority > 3);

    requests.Add(CreateRequest(studentID, studentName, bookID, bookTitle, priority));
    cout << "Request added successfully." << endl;
}

void ProcessRequest(PriorityQueue& requests)
{
    BookRequestNode* processed = requests.Remove();
    if (processed == nullptr)
    {
        cout << "No requests to process." << endl;
        return;
    }

    cout << endl << "===== Processing Request =====" << endl;
    cout << "Student ID:   " << processed->studentID << endl;
    cout << "Student Name: " << processed->studentName << endl;
    cout << "Book ID:      " << processed->bookID << endl;
    cout << "Book Title:   " << processed->bookTitle << endl;
    cout << "Priority:     " << processed->priority << endl;
    delete processed;
}

void DisplayRequests(PriorityQueue& requests)
{
    if (requests.IsEmpty())
    {
        cout << "No pending requests." << endl;
        return;
    }
    cout << endl << "===== Pending Requests (Highest to Lowest Priority) =====" << endl << endl;
    requests.PrintQueue();
}

void CheckStatus(PriorityQueue& requests)
{
    if (requests.IsEmpty())
    {
        cout << "Queue Status: EMPTY (no pending requests)." << endl;
    }
    else if (requests.IsNotEmpty())
    {
        cout << "Queue Status: NOT EMPTY (" << requests.GetCount()
             << " pending request(s))." << endl;
    }
}

int main()
{
    PriorityQueue requests;
    int choice;

    do
    {
        ShowMenu();
        choice = ReadInt("Enter your choice: ");

        switch (choice)
        {
        case 1: AddRequest(requests);      break;
        case 2: ProcessRequest(requests);  break;
        case 3: DisplayRequests(requests); break;
        case 4: CheckStatus(requests);     break;
        case 5:
            cout << "Exiting. Releasing memory..." << endl;
            break;
        default:
            cout << "Invalid choice. Please select 1-5." << endl;
        }
    } while (choice != 5);

    return 0;
}