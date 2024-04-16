#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;
class WareHouse;
enum class ActionStatus
{
    COMPLETED,
    ERROR
};

enum class CustomerType
{
    Soldier,
    Civilian
};

class BaseAction
{ /*done*/
public:
    BaseAction();
    virtual ~BaseAction() = 0;
    ActionStatus getStatus() const;
    virtual void act(WareHouse &wareHouse) = 0;
    virtual string toString() const = 0;
    virtual BaseAction *clone() const = 0;
    void setActionStatus(ActionStatus newStatus);
    // string toString1(CustomerType type) const;

protected:
    void complete();
    void error(string errorMsg);
    string getErrorMsg() const;

private:
    string errorMsg;
    ActionStatus status;
};
// for later
class SimulateStep : public BaseAction
{

public:
    ~SimulateStep() = default;
    SimulateStep(int numOfSteps);            //
    void act(WareHouse &wareHouse) override; //
    std::string toString() const override;
    SimulateStep *clone() const override;
    void stage1(WareHouse &wareHouse); //
    void stage2(WareHouse &wareHouse);
    void stage3(WareHouse &wareHouse);
    void stage4(WareHouse &wareHouse);
    int getNumOfSteps() const;

private:
    const int numOfSteps;
};

class AddOrder : public BaseAction
{ // done
public:
    ~AddOrder() = default;
    AddOrder(int id);                        //
    void act(WareHouse &wareHouse) override; //
    string toString() const override;        //
    AddOrder *clone() const override;
    int getCustomerId() const; //
private:
    const int customerId;
};

class AddCustomer : public BaseAction
{ /*done*/
public:
    ~AddCustomer() = default;
    AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders); /**/
    void act(WareHouse &wareHouse) override;                                                          /**/
    AddCustomer *clone() const override;                                                              /**/
    string toString() const override;                                                                 //
    int getMaxOrders() const;                                                                         //
    int getDistance() const;
    CustomerType getCustomerType() const;      //
    string getCustomerName() const;            //
    string toString1(CustomerType type) const; //
private:
    const string customerName;
    const CustomerType customerType;
    const int distance;
    const int maxOrders;
};

class PrintOrderStatus : public BaseAction
{ /**/
public:
    ~PrintOrderStatus() = default;
    PrintOrderStatus(int id);                 // id of order
    void act(WareHouse &wareHouse) override;  //
    PrintOrderStatus *clone() const override; //
    string toString() const override;         //
private:
    const int orderId;
};

class PrintCustomerStatus : public BaseAction
{
public:
    ~PrintCustomerStatus() = default;
    PrintCustomerStatus(int customerId);         //
    void act(WareHouse &wareHouse) override;     //
    PrintCustomerStatus *clone() const override; //
    string toString() const override;            //
private:
    const int customerId;
};

class PrintVolunteerStatus : public BaseAction
{
public:
    ~PrintVolunteerStatus() = default;
    PrintVolunteerStatus(int id);                 //
    void act(WareHouse &wareHouse) override;      //
    PrintVolunteerStatus *clone() const override; //
    string toString() const override;             //
private:
    const int volunteerId;
};

class PrintActionsLog : public BaseAction
{
public:
    ~PrintActionsLog() = default;
    PrintActionsLog();
    void act(WareHouse &wareHouse) override;
    PrintActionsLog *clone() const override;
    string toString() const override;

private:
};

class Close : public BaseAction
{
public:
    ~Close() = default;
    Close();
    void act(WareHouse &wareHouse) override;
    Close *clone() const override;
    string toString() const override;

private:
};

class BackupWareHouse : public BaseAction
{
public:
    ~BackupWareHouse() = default;
    BackupWareHouse();
    void act(WareHouse &wareHouse) override;
    BackupWareHouse *clone() const override;
    string toString() const override;

private:
};

class RestoreWareHouse : public BaseAction
{
public:
    ~RestoreWareHouse() = default;
    RestoreWareHouse();
    void act(WareHouse &wareHouse) override;
    RestoreWareHouse *clone() const override;
    string toString() const override;

private:
};