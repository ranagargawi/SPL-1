#include <string>
#include "Customer.h"
#include <vector>
using std::vector;

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId()
{
}
Customer::~Customer() {}
const string &Customer::getName() const
{
   return name;
}

int Customer::getId() const
{
   return id;
}

int Customer::getCustomerDistance() const
{
   return locationDistance;
}

int Customer::getMaxOrders() const
{
   return maxOrders;
}
// Returns maxOrders
int Customer::getNumOrders() const
{
   return ordersId.size();
}

// Returns num of orders the customer has made so far
bool Customer::canMakeOrder() const
{
   // return !(getNumOrders() == getMaxOrders());
   return getNumOrders() < getMaxOrders();
}
// Returns true if the customer didn't reach max orders
const vector<int> &Customer::getOrdersIds() const
{
   return ordersId;
}

int Customer::addOrder(int orderId)
{
   if (canMakeOrder())
   {
      for (unsigned int i = 0; i < ordersId.size(); i++)
      {
         if (ordersId[i] == orderId)
         {
            return -1;
         }
      }
      ordersId.push_back(orderId);
      return orderId;
   }
   return -1;
}
/*
for (unsigned int i = 0; i < ordersId.size(); i++)
{
if (ordersId[i] == orderId)
{
   return -1;
}
}
ordersId.push_back(orderId);
return orderId;
}*/
// return OrderId if order was added successfully, -1 otherwise

SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders)
{
}

SoldierCustomer *SoldierCustomer::clone() const
{
   return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders)
{
}
CivilianCustomer *CivilianCustomer::clone() const
{
   return new CivilianCustomer(*this);
}