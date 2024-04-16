using namespace std;
#include "Order.h"
#include <iostream>
#include <string>
using std::string;
using std::vector;
Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(-1), driverId(-1) {}
Order *Order::clone() const
{
   return new Order(*this);
}

int Order::getId() const
{
   return id;
}
int Order::getDistance() const
{
   return distance;
}
int Order::getCustomerId() const
{
   return customerId;
}

void Order::setStatus(OrderStatus status)
{
   this->status = status;
}

void Order::setCollectorId(int collectorId)
{
   this->collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
   this->driverId = driverId;
}

int Order::getCollectorId() const
{
   return collectorId;
}

int Order::getDriverId() const
{
   return driverId;
}

OrderStatus Order::getStatus() const
{
   return status;
}
const string Order::toString1(OrderStatus status) const
{
   if (status == OrderStatus::COMPLETED)
   {
      return "COMPLETED";
   }
   else if (status == OrderStatus::PENDING)
   {
      return "PENDING";
   }
   else if (status == OrderStatus::DELIVERING)
   {
      return "DELIVERING ";
   }
   else
      return "COLLECTING";
}
string Order::tostring2()
{
   string s1 = "OrderId: " + to_string(getId());
   string s2 = ", OrderStatus: " + toString1(getStatus());
   string s3 = ", CustomerId: " + to_string(getCustomerId());
   return s1 + s3 + s2;
}
const string Order::toString() const
{
   string s1 = "OrderId: " + to_string(getId());
   string s2 = "OrderStatus: " + toString1(getStatus());
   string s3 = "CustomerId: " + to_string(getCustomerId());
   string s4;
   if (getStatus() != OrderStatus::PENDING)
   {
      s4 = "Collector: " + to_string(getCollectorId());
   }
   else
   {
      s4 = "Collector: None";
   }
   string s5;
   if (!(getStatus() == OrderStatus::PENDING) & !(getStatus() == OrderStatus::COLLECTING))
   {
      s5 = "Driver: " + to_string(getDriverId());
   }
   else
   {
      s5 = "Driver: None";
   }
   string output = s1 + "\n" + s2 + "\n" + s3 + "\n" + s4 + "\n" + s5;
   return output;
}
