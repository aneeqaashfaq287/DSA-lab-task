#include <iostream>
#include <string>
using namespace std;

struct OrderNode
{
    int orderID;
    string customerName;
    string items;
    string address;
    int priority;
    OrderNode* next;
};

class PriorityQueue
{
private:
    OrderNode* front;
    OrderNode* rear;
    int count;

    void CopyFrom(const PriorityQueue& other)
    {
        OrderNode* current = other.front;
        while (current != nullptr)
        {
            OrderNode* newNode = new OrderNode;
            newNode->orderID = current->orderID;
            newNode->customerName = current->customerName;
            newNode->items = current->items;
            newNode->address = current->address;
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
            OrderNode* temp = front;
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

    void Add(OrderNode* order)
    {
        order->next = nullptr;

        if (front == nullptr)
        {
            front = order;
            rear = order;
        }
        else if (order->priority < front->priority)
        {
            order->next = front;
            front = order;
        }
        else
        {
            OrderNode* current = front;
            while (current->next != nullptr &&
                   current->next->priority <= order->priority)
            {
                current = current->next;
            }
            order->next = current->next;
            current->next = order;

            if (order->next == nullptr)
            {
                rear = order;
            }
        }
        count++;
    }

    OrderNode* Remove()
    {
        if (front == nullptr)
        {
            return nullptr;
        }
        OrderNode* order = front;
        front = front->next;
        if (front == nullptr)
        {
            rear = nullptr;
        }
        order->next = nullptr;
        count--;
        return order;
    }

    bool IsEmpty()
    {
        return front == nullptr;
    }

    bool IsNotEmpty()
    {
        return front != nullptr;
    }

    int GetCount()
    {
        return count;
    }

    void PrintQueue()
    {
        OrderNode* current = front;
        while (current != nullptr)
        {
            cout << "Order ID:  " << current->orderID << endl;
            cout << "Customer:  " << current->customerName << endl;
            cout << "Items:     " << current->items << endl;
            cout << "Address:   " << current->address << endl;
            cout << "Priority:  " << current->priority << endl;
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

OrderNode* CreateOrder(int orderID, string customerName, string items,
                       string address, int priority)
{
    OrderNode* order = new OrderNode;
    order->orderID = orderID;
    order->customerName = customerName;
    order->items = items;
    order->address = address;
    order->priority = priority;
    order->next = nullptr;
    return order;
}

string PriorityName(int priority)
{
    if (priority == 1) return "Express Delivery";
    if (priority == 2) return "Standard Delivery";
    return "Scheduled Delivery";
}

int main()
{
    PriorityQueue orders;

    orders.Add(CreateOrder(1001, "Ali", "Chicken Biryani", "House 12, Model Town", 3));
    orders.Add(CreateOrder(1002, "Sara", "Zinger Burger Meal", "Flat 5, Gulberg", 1));
    orders.Add(CreateOrder(1003, "Bilal", "Pizza Large", "Street 7, DHA", 2));
    orders.Add(CreateOrder(1004, "Hamza", "Pasta and Soup", "House 3, Johar Town", 1));
    orders.Add(CreateOrder(1005, "Ayesha", "BBQ Platter", "Block B, Faisal Town", 2));

    cout << "===== Food Delivery Orders (Priority Order) =====" << endl << endl;
    orders.PrintQueue();

    cout << "Total pending orders: " << orders.GetCount() << endl << endl;

    cout << "===== Dispatching Orders =====" << endl << endl;
    while (orders.IsNotEmpty())
    {
        OrderNode* dispatched = orders.Remove();
        cout << "Dispatching Order " << dispatched->orderID
             << " | " << dispatched->customerName
             << " | " << dispatched->items
             << " | " << PriorityName(dispatched->priority) << endl;
        delete dispatched;
    }

    cout << endl;
    if (orders.IsEmpty())
    {
        cout << "All orders dispatched. Queue is empty." << endl;
    }

    return 0;
}