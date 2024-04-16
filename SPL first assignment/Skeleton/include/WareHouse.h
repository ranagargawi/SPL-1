#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"
#include "Action.h"
#include "Volunteer.h"

class BaseAction;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.

class WareHouse
{

public:
    WareHouse(const string &configFilePath);
    ~WareHouse();
    WareHouse &operator=(WareHouse &other);
    WareHouse(WareHouse &other);
    WareHouse &operator=(WareHouse &&other);
    WareHouse(WareHouse &&other);
    void start();
    int getOrderCounter() const;
    void incOrderCounter();
    void addOrder(Order *order);
    void addAction(BaseAction *action);
    Customer &getCustomer(int customerId) const;
    Volunteer &getVolunteer(int volunteerId) const;
    Order& getOrder(int orderId);
    void close();
    void open();
    void addCustomer(Customer *newCustomer);
    vector<BaseAction *>& getActions();
    vector<Order *>& getPendingOrders();
    vector<Order *>& getInProcessOrders();
    vector<Order *>& getCompletedOrders();
    vector<Volunteer *>& getVolunteers();
    vector<Customer *>& getCustomers();
    int getCustomerCounter() const;
    bool orderExists(int orderId);
    bool volunteerExists(int volunteerId);
    bool customerExists(int customerId);
    void addVolunteer(Volunteer *newVolunteer);
    void clearWareHouse();
    void addInProcessOrder(Order *order);
    void addPendingOrder(Order *order);
    void addCompletedOrder(Order *order);
    void increaseVolCounter();
    int getVolCounter();
    void decreaseVolCounter();

private:
    bool isOpen;
    int orderCounter;
    int customerCounter;  // For assigning unique customer IDs
    int volunteerCounter; // For assigning unique volunteer IDs
    vector<BaseAction *> actionsLog;
    vector<Volunteer *> volunteers;
    vector<Order *> pendingOrders;   //
    vector<Order *> inProcessOrders; //
    vector<Order *> completedOrders; //
    vector<Customer *> customers;
};