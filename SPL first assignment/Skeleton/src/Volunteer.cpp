#include "Volunteer.h"
using namespace std;
#include <string>
Volunteer::Volunteer(int id, const string &name) : completedOrderId(-1), activeOrderId(-1), id(id), name(name)
{
}
Volunteer::~Volunteer() {}
int Volunteer::getId() const
{
   return id;
}

const string &Volunteer::getName() const
{
   return name;
}

int Volunteer::getActiveOrderId() const
{
   return activeOrderId;
}

int Volunteer::getCompletedOrderId() const
{
   return completedOrderId;
}

void Volunteer::setCompletedOrderId(int newId)
{
   completedOrderId = newId;
}
void Volunteer::setActiveOrderId(int newId)
{
   activeOrderId = newId;
}
bool Volunteer::isBusy() const
{
   return activeOrderId != -1;
}
// Signal whether the volunteer is currently processing an order

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown)
{
}
// CollectorVolunteer::~CollectorVolunteer() {}
CollectorVolunteer *CollectorVolunteer::clone() const
{
   return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step()
{
   if (getTimeLeft() > 0)
   {
      this->timeLeft = this->timeLeft - 1;
   }
   if (getTimeLeft() == 0)
   {
      completedOrderId = activeOrderId;
      activeOrderId = NO_ORDER;
   }
}

int CollectorVolunteer::getCoolDown() const
{
   return coolDown;
}

int CollectorVolunteer::getTimeLeft() const
{
   return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown()
{
   timeLeft--;
   if (timeLeft == 0)
      return true;
   return false;
}
// Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
bool CollectorVolunteer::hasOrdersLeft() const
{
   return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
   return !isBusy() && order.getStatus() == OrderStatus::PENDING;
}

void CollectorVolunteer::acceptOrder(const Order &order)
{
   activeOrderId = order.getId();
   timeLeft = getCoolDown();
}

string CollectorVolunteer::toString() const
{
   string str = "";
   if (isBusy())
      str = "True";
   else
      str = "False";
   string s1 = "VolunteerID: " + to_string(getId());
   string s2 = "isBusy: " + str;
   string s3;
   if (isBusy())
      s3 = "OrderId: " + to_string(getActiveOrderId());
   else
      s3 = "None";
   string s4 = "TimeLeft: ";
   if (timeLeft == 0)
      s4 += "None";
   else
      s4 += to_string(getTimeLeft());
   string s5 = "OrdersLeft: No limit";

   string output = s1 + "\n" + s2 + "\n" + s3 + "\n" + s4 + "\n" + s5;
   return output;
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders)
{
}
// LimitedCollectorVolunteer::~LimitedCollectorVolunteer() {}
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
   return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
   // return hasOrdersLeft();
   return getNumOrdersLeft() > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
   return !(isBusy()) && order.getStatus() == OrderStatus::PENDING && hasOrdersLeft();
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
   CollectorVolunteer::acceptOrder(order);

   ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const
{
   return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
   return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const
{
   string str = "";
   if (isBusy())
      str = "True";
   else
      str = "False";
   string s1 = "VolunteerID: " + to_string(getId());
   string s2 = "isBusy: " + str;
   string s3;
   if (isBusy())
      s3 = "OrderId: " + to_string(getActiveOrderId());
   else
      s3 = "None";
   string s4 = "TimeLeft: ";
   if (getTimeLeft() == 0)
      s4 += "None";
   else
      s4 += to_string(getTimeLeft());
   string s5 = "OrdersLeft: " + to_string(getNumOrdersLeft());

   string output = s1 + "\n" + s2 + "\n" + s3 + "\n" + s4 + "\n" + s5;
   return output;
}

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0)
{
}
// DriverVolunteer::~DriverVolunteer() {}

DriverVolunteer *DriverVolunteer::clone() const
{
   return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const
{
   return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const
{
   return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const
{
   return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft()
{
   distanceLeft -= distancePerStep;
   if (getDistanceLeft() >= 0)
      return true;
   return false;
}
// Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
bool DriverVolunteer::hasOrdersLeft() const
{
   return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const
{
   return (!isBusy() && order.getStatus() == OrderStatus::COLLECTING && order.getDistance() <= getMaxDistance());
}
// Signal if the volunteer is not busy and the order is within the maxDistance
void DriverVolunteer::acceptOrder(const Order &order)
{
   activeOrderId = order.getId();
   distanceLeft = order.getDistance();
}
// Assign distanceLeft to order's distance
void DriverVolunteer::step()
{
   if (decreaseDistanceLeft())
   {
      completedOrderId = activeOrderId;
      activeOrderId = NO_ORDER;
   }
}
// Decrease distanceLeft by distancePerStep
string DriverVolunteer::toString() const
{
   string str = "";
   if (isBusy())
      str = "True";
   else
      str = "False";
   string s1 = "DriverID: " + to_string(getId());
   string s2 = "isBusy: " + str;
   string s3;
   if (isBusy())
      s3 = "OrderId: " + to_string(getActiveOrderId());
   else
      s3 = "None";
   string s4 = "TimeLeft: ";
   if (getDistanceLeft() == 0)
      s4 += "None";
   else
      s4 += to_string(getDistanceLeft());
   string s5 = "OrdersLeft: No limit";
   string output = s1 + "\n" + s2 + "\n" + s3 + "\n" + s4 + "\n" + s5;
   return output;
}

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}
// LimitedDriverVolunteer::~LimitedDriverVolunteer() {}
LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
   return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const
{
   return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const
{
   return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const
{
   return getNumOrdersLeft() > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
   return (!(isBusy()) && hasOrdersLeft() && order.getStatus() == OrderStatus::COLLECTING && order.getDistance() <= getMaxDistance());
}

// Signal if the volunteer is not busy, the order is within the maxDistance.
void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
   activeOrderId = order.getId();
   ordersLeft--;
}
// Assign distanceLeft to order's distance and decrease ordersLeft
string LimitedDriverVolunteer::toString() const
{
   string str = "";
   if (isBusy())
      str = "True";
   else
      str = "False";
   string s1 = "DriverID: " + to_string(getId());
   string s2 = "isBusy: " + str;
   string s3;
   if (isBusy())
      s3 = "OrderId: " + to_string(getActiveOrderId());
   else
      s3 = "None";
   string s4 = "TimeLeft: ";
   if (getDistanceLeft() == 0)
      s4 += "None";
   else
      s4 += to_string(getDistanceLeft());
   string s5 = "OrdersLeft: " + to_string(getNumOrdersLeft());
   string output = s1 + "\n" + s2 + "\n" + s3 + "\n" + s4 + "\n" + s5;
   return output;
}