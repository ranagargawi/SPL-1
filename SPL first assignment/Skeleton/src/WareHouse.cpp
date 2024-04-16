#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm> // for std::swap
#include <stdexcept>
using std::cerr;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
/*defaultOrder(new Order(-1, -1, -1));
Volunteer *defaultVolunteer= CollectorVolunteer(-1, "", -1);
*/
#include "WareHouse.h"

WareHouse::WareHouse(const string &configFilePath) : isOpen(true), orderCounter(0), customerCounter(0), volunteerCounter(0), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers()
{
    ifstream configFile(configFilePath); // Open the config file

    if (!configFile.is_open())
    {
        cerr << "Error: Unable to open configuration file: " << configFilePath << endl;
        return; // Handle the error
    }

    string line;

    while (getline(configFile, line))
    {
        istringstream iss(line);
        string type;
        iss >> type;
        if (type == "customer")
        {
            // Parse and create customer
            string customerName, customerType;
            int distance, maxOrders;
            iss >> customerName >> customerType >> distance >> maxOrders; // adding values into defined variables

            // Create and add the customer to the warehouse
            Customer *newCustomer = nullptr;
            if (customerType == "soldier")
            {
                newCustomer = new SoldierCustomer(customerCounter, customerName, distance, maxOrders);
            }
            else if (customerType == "civilian")
            {
                newCustomer = new CivilianCustomer(customerCounter, customerName, distance, maxOrders);
            }
            addCustomer(newCustomer);
        }
        else if (type == "volunteer")
        {
            // Parse and create volunteer
            string volunteerName, volunteerRole;
            iss >> volunteerName >> volunteerRole;

            // Create and add the volunteer to the warehouse
            Volunteer *newVolunteer = nullptr;
            if (volunteerRole == "collector")
            {
                int coolDown;
                iss >> coolDown;
                newVolunteer = new CollectorVolunteer(volunteerCounter, volunteerName, coolDown);
            }
            else if (volunteerRole == "limited_collector")
            {
                int coolDown, maxOrders;
                iss >> coolDown >> maxOrders;
                newVolunteer = new LimitedCollectorVolunteer(volunteerCounter, volunteerName, coolDown, maxOrders);
            }
            else if (volunteerRole == "driver")
            {
                int maxDistance, distancePerStep;
                iss >> maxDistance >> distancePerStep;
                newVolunteer = new DriverVolunteer(volunteerCounter, volunteerName, maxDistance, distancePerStep);
            }
            else if (volunteerRole == "limited_driver")
            {
                int maxDistance, distancePerStep, maxOrders;
                iss >> maxDistance >> distancePerStep >> maxOrders;
                newVolunteer = new LimitedDriverVolunteer(volunteerCounter, volunteerName, maxDistance, distancePerStep, maxOrders);
            }
            addVolunteer(newVolunteer);
        }
    }
    configFile.close();
    std::cout << "Warehouse initialized from config file: " << configFilePath << endl;
}
/********************************************************************************/
// Destructor
WareHouse::~WareHouse()
{
    clearWareHouse();
}
// Assignment Operator
WareHouse &WareHouse::operator=(WareHouse &other)
{
    if (&other != this)
    {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        clearWareHouse();
        for (unsigned int i = 0; i < other.actionsLog.size(); i++)
        {
            actionsLog.push_back(other.actionsLog[i]->clone());
        }
        for (unsigned int i = 0; i < other.volunteers.size(); i++)
        {
            volunteers.push_back(other.volunteers[i]->clone());
        }
        for (unsigned int i = 0; i < other.customers.size(); i++)
        {
            customers.push_back(other.customers[i]->clone());
        }
        for (unsigned int i = 0; i < other.pendingOrders.size(); i++)
        {
            pendingOrders.push_back(other.pendingOrders[i]->clone());
        }
        for (unsigned int i = 0; i < other.inProcessOrders.size(); i++)
        {
            inProcessOrders.push_back(other.inProcessOrders[i]->clone());
        }
        for (unsigned int i = 0; i < other.completedOrders.size(); i++)
        {
            completedOrders.push_back(other.completedOrders[i]->clone());
        }
    }
    return *this;
}
// Copy Constructor
WareHouse::WareHouse(WareHouse &other) : isOpen(other.isOpen), orderCounter(other.orderCounter), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers()
{

    for (const auto &action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
    for (const auto &volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer->clone());
    }
    for (const auto &customer : other.customers)
    {
        customers.push_back(customer->clone());
    }
    for (const auto &order : other.pendingOrders)
    {
        pendingOrders.push_back(order->clone());
    }
    for (const auto &order : other.inProcessOrders)
    {
        inProcessOrders.push_back(order->clone());
    }
    for (const auto &order : other.completedOrders)
    {
        completedOrders.push_back(order->clone());
    }
}
// Move Assignment Operator
WareHouse &WareHouse::operator=(WareHouse &&other)
{
    if (&other != this)
    {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        clearWareHouse();

        for (const auto &action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }
        for (const auto &volunteer : other.volunteers)
        {
            volunteers.push_back(volunteer->clone());
        }
        for (const auto &customer : other.customers)
        {
            customers.push_back(customer->clone());
        }
        for (const auto &order : other.pendingOrders)
        {
            pendingOrders.push_back(order->clone());
        }
        for (const auto &order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order->clone());
        }
        for (const auto &order : other.completedOrders)
        {
            completedOrders.push_back(order->clone());
        }
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
        other.clearWareHouse();
    }
    return *this;
}
// Move Constructor
WareHouse::WareHouse(WareHouse &&other) : isOpen(other.isOpen), orderCounter(other.orderCounter), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers()
{
    other.clearWareHouse();
    other.isOpen = false;
    other.volunteerCounter = 0;
    other.customerCounter = 0;
    other.orderCounter = 0;
}
void WareHouse::clearWareHouse()
{
    // printf("deleting a warehouse\n");
    for (unsigned int i = 0; i < actionsLog.size(); i++)
    {
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }
    for (unsigned int i = 0; i < volunteers.size(); i++)
    {
        delete volunteers[i];
        volunteers[i] = nullptr;
    }
    for (unsigned int i = 0; i < customers.size(); i++)
    {
        delete customers[i];
        customers[i] = nullptr;
    }
    for (unsigned int i = 0; i < pendingOrders.size(); i++)
    {
        delete pendingOrders[i];
        pendingOrders[i] = nullptr;
    }
    for (unsigned int i = 0; i < inProcessOrders.size(); i++)
    {

        delete inProcessOrders[i];
        inProcessOrders[i] = nullptr;
    }
    for (unsigned int i = 0; i < completedOrders.size(); i++)
    {
        delete completedOrders[i];
        completedOrders[i] = nullptr;
    }
    actionsLog.clear();
    volunteers.clear();
    customers.clear();
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    ;
}
void WareHouse::increaseVolCounter()
{
    volunteerCounter++;
}
void WareHouse::decreaseVolCounter()
{
    volunteerCounter--;
}
int WareHouse::getVolCounter()
{
    return volunteerCounter;
}
/**********************/
void WareHouse::start() /********************/
{
    open();
    std::cout << "The WareHouse is Open!" << endl;
    while (isOpen == true)
    {
        // get a line from user
        string line;
        getline(std::cin, line);
        // parse -> create action object
        BaseAction *action;
        istringstream sstream(line);
        string type;
        sstream >> type;
        if (type == "order")
        {
            int val1;
            sstream >> val1;
            action = new AddOrder(val1);
        }
        else if (type == "step")
        {
            int val1;
            sstream >> val1;
            action = new SimulateStep(val1);
        }
        else if (type == "customer")
        {
            string str1, str2;
            int val1, val2;
            sstream >> str1 >> str2 >> val1 >> val2;
            action = new AddCustomer(str1, str2, val1, val2);
        }
        else if (type == "orderStatus")
        {
            int val1;
            sstream >> val1;
            action = new PrintOrderStatus(val1);
        }
        else if (type == "customerStatus")
        {
            int val1;
            sstream >> val1;
            action = new PrintCustomerStatus(val1);
        }
        else if (type == "volunteerStatus")
        {
            int val1;
            sstream >> val1;
            action = new PrintVolunteerStatus(val1);
        }
        else if (type == "log")
        {
            action = new PrintActionsLog();
        }
        else if (type == "close")
        {
            action = new Close();
        }
        else if (type == "backup")
        {
            action = new BackupWareHouse();
        }
        else if (type == "restore")
        {
            action = new RestoreWareHouse();
        }
        else
        {
            cerr << "Unknown command " << type << endl;
            continue;
        }
        // act the action object
        action->act(*this);
        // add to actions log
        addAction(action);
    }
}

/******************************/
vector<Order *> &WareHouse::getPendingOrders()
{
    return pendingOrders;
}

vector<Order *> &WareHouse::getInProcessOrders()
{
    return inProcessOrders;
}
int WareHouse::getOrderCounter() const
{
    return orderCounter;
}

vector<Order *> &WareHouse::getCompletedOrders()
{
    return completedOrders;
}
vector<Volunteer *> &WareHouse::getVolunteers()
{
    return volunteers;
}
vector<Customer *> &WareHouse::getCustomers()
{
    return customers;
}
vector<BaseAction *> &WareHouse::getActions()
{
    return actionsLog;
}

/*******************************************/
void WareHouse::addOrder(Order *order)
{
    addPendingOrder(order);
    incOrderCounter();
}

void WareHouse::incOrderCounter()
{
    orderCounter++;
}
void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (unsigned int i = 0; i < customers.size(); i++)
    {
        if (customers[i]->getId() == customerId)
        {
            return *customers[i];
        }
    }

    Customer *c = new SoldierCustomer(-1, "", -1, -1); /***********************/
    return *c;
}

int WareHouse::getCustomerCounter() const
{
    return customerCounter;
}
Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (unsigned int i = 0; i < volunteers.size(); i++)
    {
        if (volunteers[i]->getId() == volunteerId)
        {
            return *volunteers[i];
        }
    }
    Volunteer *vol = new CollectorVolunteer(-1, "", -1); /***********************/

    return *vol;
}

Order &WareHouse::getOrder(int orderId)
{
    for (unsigned int i = 0; i < pendingOrders.size(); i++)
    {
        if (pendingOrders[i]->getId() == orderId)
        {
            return *pendingOrders[i];
        }
    }
    for (unsigned int i = 0; i < inProcessOrders.size(); i++)
    {
        if (inProcessOrders[i]->getId() == orderId)
        {
            return *inProcessOrders[i];
        }
    }
    for (unsigned int i = 0; i < completedOrders.size(); i++)
    {
        if (completedOrders[i]->getId() == orderId)
        {
            return *completedOrders[i];
        }
    }
    Order *order = new Order(-1, -1, -1); /***********************/

    return *order;
}

void WareHouse::addCustomer(Customer *newCustomer)
{
    customers.push_back(newCustomer);
    customerCounter++;
}

bool WareHouse::customerExists(int customerId)
{
    for (Customer *cus : customers)
    {
        if (cus->getId() == customerId)
        {
            return true;
        }
    }
    return false;
}

void WareHouse::addVolunteer(Volunteer *newVolunteer)
{
    volunteers.push_back(newVolunteer);
    volunteerCounter++;
}
bool WareHouse::volunteerExists(int volunteerId)
{
    for (Volunteer *c : volunteers)
    {
        if (c->getId() == volunteerId)
            return true;
    }
    return false;
}

bool WareHouse::orderExists(int orderId)
{
    for (Order *c : pendingOrders)
    {
        if (c->getId() == orderId)
            return true;
    }
    for (Order *c : inProcessOrders)
    {
        if (c->getId() == orderId)
            return true;
    }
    for (Order *c : completedOrders)
    {
        if (c->getId() == orderId)
            return true;
    }
    return false;
}

void WareHouse::addInProcessOrder(Order *order)
{
    inProcessOrders.push_back(order);
}
void WareHouse::addCompletedOrder(Order *order)
{
    completedOrders.push_back(order);
}
void WareHouse::addPendingOrder(Order *order)
{
    pendingOrders.push_back(order);
}

void WareHouse::open()
{
    isOpen = true;
}
void WareHouse::close()
{
    // for (Order *order : pendingOrders)
    // {
    //     PrintOrderStatus(order->getId());
    // }
    // for (Order *order : inProcessOrders)
    // {
    //     PrintOrderStatus(order->getId());
    // }
    // for (Order *order : completedOrders)
    // {
    //     PrintOrderStatus(order->getId());
    // }
    isOpen = false;
    std::cout << "WareHouse is closed!" << std::endl;
}
