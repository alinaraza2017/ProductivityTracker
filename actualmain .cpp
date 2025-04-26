#include <iostream>
#include <string>
#include <ctime>  //timing operations
#include <fstream> //for file handling
#include <sstream>
using namespace std;

//structure of subtask using tree
struct subtask {
    int id;
    int priority;
    string name;
    bool iscompleted;

    subtask* left; //left for lower priority
    subtask* right;//right for higher priority
};

//structure of task node using doubly linked list
struct tasknode {
    int id;
    int priority;
    string name;
    bool iscompleted;
    bool isFavorite;
    string dueDate;
    subtask* subRoot; //subtask ka obj


    tasknode* next;
    tasknode* prev;
};

//queue for displaying the remaining/uncompleted tasks separately
class Queue {
private:
    struct Queuenode { // Structure of QueueNode
        tasknode* task;
        Queuenode* next;
    };

    Queuenode* front; // Pointer to the front of the queue
    Queuenode* rear;  // Pointer to the rear of the queue

public:
    Queue(){
         front=NULL;
         rear=NULL; 
    }

    // Enqueue a tasknode
    void enqueue(tasknode* task) {
        Queuenode* newNode = new Queuenode;
        newNode->task = task;
        newNode->next = NULL;

        if (rear == NULL) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // Dequeue a tasknode
    tasknode* dequeue() {
        if (front == NULL) {
            cout << "Queue is empty!" << endl;
            return NULL;
        }

        Queuenode* temp = front;
        tasknode* task = temp->task;
        front = front->next;

        if (front == NULL) {
            rear = NULL;
        }
        delete temp;
        return task;
    }

    // Check if the queue is empty
    bool isEmpty(){
        return front == NULL;
    }

    void displayTasksFromQueue() {
    while (!isEmpty()) {
        tasknode* task = dequeue();
        cout << "Task ID: " << task->id 
             << ", Name: " << task->name 
             << ", Priority: " << task->priority << endl;
    }
}

};

// Max size for the heap
const int MAX_HEAP_SIZE = 100;


//favourites ki functionality keliye heap structure
// Task Heap Class to manage favorite tasks using a heap
class TaskHeap {
private:
    tasknode* favoriteHeap[MAX_HEAP_SIZE];  // Array to store tasks in heap
    int heapSize;

    // Heapify up to maintain the heap property (min-heap based on priority)
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (favoriteHeap[index]->priority >= favoriteHeap[parent]->priority) break;
            swap(favoriteHeap[index], favoriteHeap[parent]);
            index = parent;
        }
    }

    // Heapify down to maintain the heap property
    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < heapSize && favoriteHeap[left]->priority < favoriteHeap[smallest]->priority) {
                smallest = left;
            }
            if (right < heapSize && favoriteHeap[right]->priority < favoriteHeap[smallest]->priority) {
                smallest = right;
            }
            if (smallest == index) break;

            swap(favoriteHeap[index], favoriteHeap[smallest]);
            index = smallest;
        }
    }

public:
    TaskHeap() : heapSize(0) {}

    // Mark task as favorite and add it to the heap
    void markAsFavorite(tasknode* task) {
        if (!task->isFavorite) {
            task->isFavorite = true;
            favoriteHeap[heapSize++] = task;
            heapifyUp(heapSize - 1);
            cout << "Task " << task->id << " marked as favorite." << endl;
        } else {
            cout << "Task " << task->id << " is already marked as favorite." << endl;
        }
    }

    // Remove a task from favorites
    void removeFavorite(int taskId) {
        for (int i = 0; i < heapSize; i++) {
            if (favoriteHeap[i]->id == taskId) {
                favoriteHeap[i]->isFavorite = false;
                favoriteHeap[i] = favoriteHeap[--heapSize];
                heapifyDown(i);
                cout << "Task " << taskId << " removed from favorites." << endl;
                return;
            }
        }
        cout << "Task with ID " << taskId << " not found in favorites." << endl;
    }

    // Display all favorite tasks (sorted by priority)
    void displayFavorites() {
        if (heapSize == 0) {
            cout << "No favorite tasks." << endl;
            return;
        }

        cout << "Favorite Tasks (sorted by priority):" << endl;
        for (int i = 0; i < heapSize; i++) {
            cout << "Task ID: " << favoriteHeap[i]->id
                 << ", Name: " << favoriteHeap[i]->name
                 << ", Priority: " << favoriteHeap[i]->priority << endl;
        }
    }
};


//classes easier functionality keliye 
class SubtaskManager {
public:
    subtask* createSubtask(int subId, string subName, int subPriority) {
        subtask* newSubtask = new subtask;
        newSubtask->id = subId;
        newSubtask->name = subName;
        newSubtask->priority = subPriority;
        newSubtask->iscompleted = false;
        newSubtask->left = NULL;
        newSubtask->right = NULL;
        return newSubtask;
    }

    subtask* insertSubtask(subtask* root, subtask* newSubtask) {
        if (root == NULL) {
            return newSubtask;
        }
        if (newSubtask->priority < root->priority) {
            root->left = insertSubtask(root->left, newSubtask);
        } else {
            root->right = insertSubtask(root->right, newSubtask);
        }
        return root;
    }

    void editSubtask(subtask* root, int subId, string newName, int newPriority) {
        if (root == NULL) {
            return;
        }
        if (root->id == subId) {
            root->name = newName;
            root->priority = newPriority;
            cout << "Subtask updated successfully!" << endl;
            return;
        }
        if (subId < root->id) {
            editSubtask(root->left, subId, newName, newPriority);
        } else {
            editSubtask(root->right, subId, newName, newPriority);
        }
    }

    subtask* deleteSubtask(subtask* root, int subId) {
        if (root == NULL) return NULL;
        if (subId < root->id) {
            root->left = deleteSubtask(root->left, subId);
        } else if (subId > root->id) {
            root->right = deleteSubtask(root->right, subId);
        } else {
            if (root->left == NULL) {
                subtask* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == NULL) {
                subtask* temp = root->left;
                delete root;
                return temp;
            } else {
                subtask* temp = root->right;
                while (temp->left != NULL) temp = temp->left;
                root->id = temp->id;
                root->name = temp->name;
                root->priority = temp->priority;
                root->iscompleted = temp->iscompleted;
                root->right = deleteSubtask(root->right, temp->id);
            }
        }
        return root;
    }
    
    //method to mark subtask as completed
    void markSubtaskComplete(subtask* root, int subtaskId) {
    if (root == NULL) {
        cout << "Subtask with ID " << subtaskId << " not found!" << endl;
        return;
    }

    if (root->id == subtaskId) {
        if (!root->iscompleted) {
            root->iscompleted = true;
            cout << "Subtask ID " << subtaskId << " marked as complete." << endl;
        } else {
            cout << "Subtask ID " << subtaskId << " is already complete." << endl;
        }
        return;
    }

    if (subtaskId < root->id) {
        markSubtaskComplete(root->left, subtaskId);
    } else {
        markSubtaskComplete(root->right, subtaskId);
    }
}
    
    //method to check if all subtasks are completed
    bool areAllSubtasksCompleted(subtask* root) {
        if (root == NULL) return true;

        if (!root->iscompleted) return false;

        return areAllSubtasksCompleted(root->left) && areAllSubtasksCompleted(root->right);
    }

    // Method to display only the completed subtasks 
    void displayCompletedSubtasks(subtask* root, int taskId) {
        if (root == NULL) return;

        displayCompletedSubtasks(root->left, taskId);
        if (root->iscompleted) {
            cout << "Task ID: " << taskId 
                << ", Subtask ID: " << root->id 
                << ", Name: " << root->name 
                << ", Priority: " << root->priority << endl;
        }
        displayCompletedSubtasks(root->right, taskId);
    } 

    // Method to display only the remaining/incomplete subtasks
    void displayRemainingSubtasks(subtask* root, int taskId) {
        if (root == NULL) return;

        displayRemainingSubtasks(root->left, taskId);
        if (!root->iscompleted) {
            cout << "Task ID: " << taskId 
                << ", Subtask ID: " << root->id 
                << ", Name: " << root->name 
                << ", Priority: " << root->priority << endl;
        }
        displayRemainingSubtasks(root->right, taskId);
    } 

    void displaySubtasks(subtask* root) {
        if (root != NULL) {
            displaySubtasks(root->left);
            cout << "    Subtask ID: " << root->id 
                 << ", Name: " << root->name 
                 << ", Priority: " << root->priority << endl;
            displaySubtasks(root->right);
        }
    }

    
};

class TaskManager {
    //attributes of the task class
private:
    tasknode* head;
    SubtaskManager subtaskManager; //containership bcz task contains the subtasks
    
    // Utility function to get the current date as a string
    string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char dateStr[11];
        snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d", 
                 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
        return string(dateStr);
    }

public:
//constructor
    TaskManager(){
        head=NULL;

    }

    subtask* getSubtaskRoot(int taskId) {
    tasknode* task = findTaskById(taskId);
    if (task) {
        return task->subRoot; // Assuming each task has a subtask root node
    }
    return nullptr;
}

    tasknode* findTaskById(int taskId) {
    tasknode* current = head; // Assuming tasks are stored in a linked list
    while (current != nullptr) {
        if (current->id == taskId) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // Task not found
}

    void writeTasksToFile(const string& fileName) {
    // Open the file in truncation mode to clear previous data
    ofstream outFile(fileName, ios::trunc); // Use `ios::trunc` to clear the file
    if (!outFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    tasknode* current = head;
    while (current != NULL) {
        // Write task details
        outFile << "Task|" << current->id << "|" << current->name << "|"
                << current->priority << "|" << current->dueDate << "|"
                << (current->iscompleted ? "1" : "0") << endl;

        // Write subtasks (if any)
        writeSubtasksToFile(outFile, current->subRoot); // Pass the same `ofstream`

        // Add task separator
        outFile << "---" << endl;

        current = current->next;
    }

    outFile.close();
    cout << "Tasks and subtasks written to file successfully!" << endl;
}



    void writeSubtasksToFile(ofstream& outFile, subtask* root) {
    if (root == NULL) return;

    // Recursive in-order traversal
    writeSubtasksToFile(outFile, root->left); // Traverse left subtree

    // Write subtask details
    outFile << "Subtask|" << root->id << "|" << root->name << "|"
            << root->priority << "|" << (root->iscompleted ? "1" : "0") << endl;

    writeSubtasksToFile(outFile, root->right); // Traverse right subtree
}


    //reading tasks and subatasks from the file
    void readTasksFromFile(const string& fileName) {
    ifstream inFile(fileName);
    if (!inFile) {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    string line;
    tasknode* currentTask = NULL;

    while (getline(inFile, line)) {
        if (line == "---") {
            // End of a task and its subtasks
            currentTask = NULL;
        } else {
            processLine(line, currentTask);
        }
    }

    inFile.close();
    cout << "Tasks and subtasks read from file successfully!" << endl;
}

void processLine(const string& line, tasknode*& currentTask) {
    stringstream ss(line);
    string token;
    string taskType;
    int taskId, priority, subId, subPriority;
    string taskName, dueDate, subName;
    bool isCompleted;

    // Read the task/subtask type
    getline(ss, taskType, '|'); // Task or Subtask

    if (taskType == "Task") {
        // Read task details
        getline(ss, token, '|');
        taskId = stoi(token);
        getline(ss, taskName, '|');
        getline(ss, token, '|');
        priority = stoi(token);
        getline(ss, dueDate, '|');
        getline(ss, token, '|');
        isCompleted = (token == "1");

        // Create and add a task
        addTask(taskName, taskId, priority, dueDate);
        currentTask = findTaskById(taskId);
        currentTask->iscompleted = isCompleted;
    } else if (taskType == "Subtask" && currentTask != NULL) {
        // Read subtask details
        getline(ss, token, '|');
        subId = stoi(token);
        getline(ss, subName, '|');
        getline(ss, token, '|');
        subPriority = stoi(token);
        getline(ss, token, '|');
        isCompleted = (token == "1");

        // Add a subtask to the current task
        subtask* newSubtask = subtaskManager.createSubtask(subId, subName, subPriority);
        newSubtask->iscompleted = isCompleted;
        currentTask->subRoot = subtaskManager.insertSubtask(currentTask->subRoot, newSubtask);
    }
}

    //method to add task to the list
    void addTask(string taskName, int taskId, int taskPriority,string dueDate = "") {
        //pehle new node banayi hai
        tasknode* newNode = new tasknode;
        newNode->id = taskId;
        newNode->name = taskName;
        newNode->priority = taskPriority;
        newNode->subRoot = NULL;
        newNode->iscompleted = false;
        //? and : is the if else condition
        newNode->dueDate = (dueDate.empty()) ? getCurrentDate() : dueDate;
        newNode->next = NULL;
        newNode->prev = NULL;
        //now implementing the conditions
        if (head == NULL) {
            head = newNode;
        } else {
            tasknode* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->prev = temp;
        }
       
    }
    
    //method to edit or update the tasks in the list
    void editTask(string taskName, int taskId) {
        if (head == NULL) {
            cout << "There are no tasks to edit." << endl;
            return;
        }
        tasknode* temp = head;
        while (temp != NULL) {
            if (temp->id == taskId) {
                temp->name = taskName;
                cout << "Task updated successfully." << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Task not found." << endl;
    }
    
    //method to delete the task
    void deleteTask(int taskId) {
        if (head == NULL) {
            cout << "No tasks to delete." << endl;
            return;
        }
        tasknode* temp = head;
        while (temp != NULL) {
            if (temp->id == taskId) {
                if (temp == head) {
                    head = temp->next;
                    if (head != NULL) head->prev = NULL;
                } else {
                    temp->prev->next = temp->next;
                    if (temp->next != NULL) temp->next->prev = temp->prev;
                }
                delete temp;
                cout << "Task deleted successfully." << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Task not found." << endl;
    }
    
    //method to add the subtask to the specific tasks
    void addSubtask(int taskId, int subId, string subName, int subPriority) {
        tasknode* temp = head;
        while (temp != NULL) {
            if (temp->id == taskId) {
                subtask* newSubtask = subtaskManager.createSubtask(subId, subName, subPriority);
                temp->subRoot = subtaskManager.insertSubtask(temp->subRoot, newSubtask);
                cout << "Subtask added to task " << taskId << ": " << subName << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Task with ID " << taskId << " not found." << endl;
    }
    
    // Method to mark a subtask as complete for a specific task
    void markSubtaskComplete(int taskId, int subtaskId) {
        tasknode* temp = head; // Start from the head of the task list
        while (temp != NULL) {
            if (temp->id == taskId) {
                if (temp->subRoot == NULL) {
                    cout << "No subtasks exist for Task ID " << taskId << "." << endl;
                    return;
                }
                subtaskManager.markSubtaskComplete(temp->subRoot, subtaskId); // Delegate to SubtaskManager
                return;
            }
            temp = temp->next; // Move to the next task
        }
        cout << "Task with ID " << taskId << " not found!" << endl;
    }

    
    //method to mark task complete only if all its subtasks are complete
    void markTaskComplete(int taskId) {
        tasknode* temp = head;
        while (temp != NULL) {
            if (temp->id == taskId) {
                if (subtaskManager.areAllSubtasksCompleted(temp->subRoot)) {
                    temp->iscompleted = true;
                    cout << "Task ID " << taskId << " marked as completed." << endl;
                } else {
                    cout << "Cannot mark Task ID " << taskId 
                         << " as completed. Not all subtasks are completed." << endl;
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Task not found!" << endl;
    }


    //method to display the completed and remaining tasks 
    void displayCompletedAndRemainingTasks(){
    if (head == NULL) {
        cout << "No tasks available." << endl;
        return;
    }

    Queue completedQueue, remainingQueue;

    tasknode* temp = head;
    while (temp != NULL) {
        if (temp->iscompleted) {
            completedQueue.enqueue(temp);
        } else {
            remainingQueue.enqueue(temp);
        }
        temp = temp->next;
    }

    cout << "\nCompleted Tasks:" << endl;
    if (completedQueue.isEmpty()) {
        cout << "  No completed tasks." << endl;
    } else {
        completedQueue.displayTasksFromQueue();
    }

    cout << "\nRemaining Tasks:" << endl;
    if (remainingQueue.isEmpty()) {
        cout << "  No remaining tasks." << endl;
    } else {
        remainingQueue.displayTasksFromQueue();
    }
}


    //method to display the tasks
    void displayTasks() {
        if (head == NULL) {
            cout << "No tasks available." << endl;
            return;
        }
        tasknode* temp = head;
        while (temp != NULL) {
            cout << "Task ID: " << temp->id 
                 << ", Name: " << temp->name 
                 << ", Priority: " << temp->priority 
                 << ", Due Date: " << temp->dueDate
                 << ", Completed: " << (temp->iscompleted ? "Yes" : "No") << endl;

            if (temp->subRoot != NULL) {
                cout << "  Subtasks:" << endl;
                subtaskManager.displaySubtasks(temp->subRoot);
            } else {
                cout << "  No subtasks available." << endl;
            }
            temp = temp->next;
        }
    }
    

    //merge sort function for tasks bcz we are using the linked list
    //subtasks are already sorted bcz of bst tree that is used
    tasknode* split(tasknode* head) {
        tasknode* slow = head;
        tasknode* fast = head;

        while (fast->next != NULL && fast->next->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }

        tasknode* secondHalf = slow->next;
        slow->next = NULL;
        if (secondHalf != NULL) {
            secondHalf->prev = NULL;
        }

        return secondHalf;
    }

    tasknode* merge(tasknode* first, tasknode* second) {
        if (!first) return second;
        if (!second) return first;

        if (first->priority < second->priority) {
            first->next = merge(first->next, second);
            first->next->prev = first;
            first->prev = NULL;
            return first;
        } else {
            second->next = merge(first, second->next);
            second->next->prev = second;
            second->prev = NULL;
            return second;
        }
    }

    tasknode* mergeSort(tasknode* head) {
        if (!head || !head->next) {
            return head;
        }

        tasknode* secondHalf = split(head);
        head = mergeSort(head);
        secondHalf = mergeSort(secondHalf);

        return merge(head, secondHalf);
    }

    void sortTasksByPriority() {
        head = mergeSort(head);
        
    }
};


int main() {
    const string fileName = "task.txt";
    std::ofstream outFile(fileName); // Open file using ofstream

    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return 1;
    }

    TaskManager taskManager;
    SubtaskManager subtaskManager;
    TaskHeap taskheap;
    int choice;
    string taskName, subName;
    int taskId, subId, taskPriority, subPriority;

    do {
        cout << "\n--- Productivity Tracker Menu ---\n";
        cout << "1. Add Task\n";
        cout << "2. Edit Task\n";
        cout << "3. Delete Task\n";
        cout << "4. Add Subtask\n";
        cout << "5. Edit Subtask\n";
        cout << "6. Delete Subtask\n";
        cout << "7. Mark Task Complete\n";
        cout << "8. Mark Subtask Complete\n";
        cout << "9. Display Completed and Incompleted Tasks\n";
        cout << "10. Display All Subtasks\n";
        cout << "11. Display Completed and Incompleted Subtasks\n";
        cout << "12. Display All Tasks and Subtasks\n";
        cout << "13. Sort Tasks by Priority\n";
        cout << "14. Mark Tasks As Favourite\n";
        cout << "15. Remove Tasks From Favourites\n";
        cout << "16. Display Favourite Tasks List\n";
        cout << "17. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:{
                cout << "Enter Task ID: ";
                cin >> taskId;
                cout << "Enter Task Name: ";
                cin.ignore(); // Ignore newline
                getline(cin, taskName); // Get the entire line for input
                cout << "Enter Task Priority: ";
                cin >> taskPriority;
                cout << "Do you want to set the due date?\n1. Yes \n2. No" << endl;
                int dueDateChoice;
                cin >> dueDateChoice; // Take the choice input

                if (dueDateChoice == 1) {
                    string dueDate;
                    cout << "Enter the due date in the format DD-MM-YYYY: ";
                    cin.ignore(); // Ignore leftover newline from previous input
                    getline(cin, dueDate); // Read the full due date as a string
                    taskManager.addTask(taskName, taskId, taskPriority, dueDate);
                    taskManager.writeTasksToFile(fileName);
                    break;
                } else if (dueDateChoice == 2) {
                    taskManager.addTask(taskName, taskId, taskPriority);
                    taskManager.writeTasksToFile(fileName);
                    break;
                } else {
                    cout << "Invalid choice. Task not added." << endl;
                }
                break;
                
            }  
            case 2:{
                cout << "Enter Task ID to Edit: ";
                cin >> taskId;
                cout << "Enter New Task Name: ";
                cin.ignore();
                getline(cin, taskName);
                taskManager.editTask(taskName, taskId);
                taskManager.writeTasksToFile(fileName);
                break;
            }
            case 3:{
                cout << "Enter Task ID to Delete: ";
                cin >> taskId;
                taskManager.deleteTask(taskId);
                taskManager.writeTasksToFile(fileName);
                break;
            }
            case 4:{
                cout << "Enter Task ID to Add Subtask To: ";
                cin >> taskId;
                cout << "Enter Subtask ID: ";
                cin >> subId;
                cout << "Enter Subtask Name: ";
                cin.ignore();
                getline(cin, subName);
                cout << "Enter Subtask Priority: ";
                cin >> subPriority;
                taskManager.addSubtask(taskId, subId, subName, subPriority);
                taskManager.writeTasksToFile(fileName);
                break;
            }
            case 5:{ // Edit Subtask
                cout << "Enter Task ID to Edit Subtask In: ";
                cin >> taskId;
                cout << "Enter Subtask ID to Edit: ";
                cin >> subId;
                cout << "Enter New Subtask Name: ";
                cin.ignore();
                getline(cin, subName);
                cout << "Enter New Subtask Priority: ";
                cin >> subPriority;

                {
                    subtask* root = taskManager.getSubtaskRoot(taskId); // Get subtask tree root
                    if (root == nullptr) {
                        cout << "Task ID not found or no subtasks exist for this task." << endl;
                        break;
                    } else {
                        subtaskManager.editSubtask(root, subId, subName, subPriority); // Edit the subtask
                        taskManager.writeTasksToFile(fileName);
                        break;
                    }
                }
                break;
            }
            case 6:{
                cout << "Enter Task ID of Subtask to Delete: ";
                cin >> taskId;
                cout << "Enter Subtask ID to Delete: ";
                cin >> subId;
                subtaskManager.deleteSubtask(taskManager.findTaskById(taskId)->subRoot, subId);
                taskManager.writeTasksToFile(fileName);
                break;
            }
            case 7:{
                cout << "Enter Task ID to Mark Complete: ";
                cin >> taskId;
                taskManager.markTaskComplete(taskId);
                taskManager.writeTasksToFile(fileName);
                break;
            }
            case 8:{
                cout << "Enter Task ID of Subtask to Mark Complete: ";
                cin >> taskId;
                cout << "Enter Subtask ID to Mark Complete: ";
                cin >> subId;
                taskManager.markSubtaskComplete(taskId, subId);
                taskManager.writeTasksToFile(fileName);
                break;
            }
            case 9:{
                taskManager.displayCompletedAndRemainingTasks();
                break;
            }
            case 10:{
                cout << "Enter Task ID to Display All Subtasks: ";
                cin >> taskId;
                subtask* root = taskManager.getSubtaskRoot(taskId); // Get root of the subtask tree
                if (root) {
                    cout << "--- Subtasks for Task ID: " << taskId << " ---" << endl;
                    subtaskManager.displaySubtasks(root);
                } else {
                    cout << "No subtasks found for Task ID " << taskId << "." << endl;
                }
                break;
            }
            case 11:{
                cout << "Enter Task ID to Display Completed and Remaining Subtasks: ";
                cin >> taskId;
                subtask* rootForDisplay = taskManager.getSubtaskRoot(taskId); // Get root of the subtask tree
                if (rootForDisplay) {
                    cout << "--- Completed Subtasks ---" << endl;
                    subtaskManager.displayCompletedSubtasks(rootForDisplay, taskId);
                    cout << "--- Remaining Subtasks ---" << endl;
                    subtaskManager.displayRemainingSubtasks(rootForDisplay, taskId);
                } else {
                    cout << "No subtasks found for Task ID " << taskId << "." << endl;
                }
                break;
            }
            case 12:{
                taskManager.displayTasks();
                break;
            }
            case 13:{
                cout << "Sorting tasks by priority..." << endl;
                taskManager.sortTasksByPriority();
                taskManager.writeTasksToFile(fileName);
                break;
            }
            case 14: { // Mark Task As Favorite
                cout << "Enter Task ID to Mark as Favorite: ";
                cin >> taskId;

                // Search for the task and mark it as a favorite
                tasknode* task = taskManager.findTaskById(taskId);
                if (task) {
                    taskheap.markAsFavorite(task);  // Mark task as favorite
                    taskManager.writeTasksToFile(fileName);  // Write updated tasks to file
                } else {
                    cout << "Task with ID " << taskId << " not found." << endl;
                }
                break;
            }

            case 15: { // Remove Task from Favorites
                cout << "Enter Task ID to Remove from Favorites: ";
                cin >> taskId;

                
                if (taskId) {
                    taskheap.removeFavorite(taskId);  // Remove task from favorite list
                    taskManager.writeTasksToFile(fileName);  // Write updated tasks to file
                } else {
                    cout << "Task with ID " << taskId << " not found." << endl;
                }
                break;
            }

            case 16: { // Display Favorite Tasks List
                cout << "--- Favorite Tasks ---" << endl;
                taskheap.displayFavorites();  // Display the list of favorite tasks
                break;
            }

            case 17:{
                cout << "Exiting Productivity Tracker. Goodbye!" << endl;
                break;
            }
            default:{
                cout << "Invalid choice. Please try again." << endl;
            }
}
     } while (choice != 17);

    return 0;
}
