#include "Action.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using std::string;
using std::vector;
using namespace std;
extern WareHouse *backup;
// constructor
BaseAction::BaseAction() : errorMsg(""), status() {}
BaseAction::~BaseAction()
{
}
ActionStatus BaseAction::getStatus() const
{
   return status;
}
void BaseAction::setActionStatus(ActionStatus newStatus)
{
   status = newStatus;
}

void BaseAction::complete()
{
   setActionStatus(ActionStatus::COMPLETED);
}
void BaseAction::error(string errorMsg)
{
   setActionStatus(ActionStatus::ERROR);
   std::cout << "Error:" << errorMsg << std::endl;
}
string BaseAction::getErrorMsg() const
{
   return errorMsg;
}
/*SimulateStep*/
SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}
int SimulateStep::getNumOfSteps() const
{
   return numOfSteps;
}

std::string SimulateStep::toString() const
{
   string str = "simulateStep " + to_string(numOfSteps); //+ status to string
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}
SimulateStep *SimulateStep::clone() const
{
   return new SimulateStep(*this);
}
void SimulateStep::act(WareHouse &wareHouse)
{
   for (int i = 0; i < getNumOfSteps(); i++)
   {
      stage1(wareHouse);
      // cout << "stage 1 Done" << endl;
      stage2(wareHouse);
      // cout << "stage 2 Done" << endl;
      stage3(wareHouse);
      // cout << "stage 3 Done" << endl;
      stage4(wareHouse);
      // cout << "stage 4 Done" << endl;
      complete();
   }
}

void SimulateStep::stage4(WareHouse &wareHouse)
{
   auto volunteers = wareHouse.getVolunteers();
   auto it = volunteers.begin();

   while (it != volunteers.end())
   {
      if (!(*it)->hasOrdersLeft())
      {
         delete *it;                // Consider if you really need to delete the volunteer here
         it = volunteers.erase(it); // Use the returned iterator after erasing
      }
      else
      {
         ++it;
      }
   }
}

void SimulateStep::stage3(WareHouse &wareHouse)
{
   for (Volunteer *volunteer : wareHouse.getVolunteers())
   {
      if (volunteer->getActiveOrderId() == -1 && volunteer->getCompletedOrderId() != -1 && volunteer->getCompletedOrderId() != -1)
      {

         Order &curr = wareHouse.getOrder(volunteer->getCompletedOrderId());
         // printf("completed: %d\n", curr.getId());
         if (wareHouse.getOrder(volunteer->getCompletedOrderId()).getStatus() == OrderStatus::COLLECTING)
         {
            // printf("test1\n");
            curr.setStatus(OrderStatus::COLLECTING);
            wareHouse.addPendingOrder(&curr);
            auto &completed = wareHouse.getOrder(volunteer->getCompletedOrderId());
            for (auto it = wareHouse.getInProcessOrders().begin(); it != wareHouse.getInProcessOrders().end(); ++it)
            {
               if ((*it)->getId() == completed.getId())
               {

                  // cout << "Before erase1 " << wareHouse.getInProcessOrders().size() << endl;
                  wareHouse.getInProcessOrders().erase(it);
                  // cout << "After erase1 " << wareHouse.getInProcessOrders().size() << endl;
                  break;
               }
            }
         }
         else if (wareHouse.getOrder(volunteer->getCompletedOrderId()).getStatus() == OrderStatus::DELIVERING)
         {
            // printf("test2\n");
            curr.setStatus(OrderStatus::COMPLETED);
            wareHouse.addCompletedOrder(&curr);
            auto &completed = wareHouse.getOrder(volunteer->getCompletedOrderId());
            // printf("completed %d\n", completed.getId());
            for (auto it = wareHouse.getInProcessOrders().begin(); it != wareHouse.getInProcessOrders().end(); ++it)
            {
               if ((*it)->getId() == completed.getId())
               {
                  // cout << "Before erase2 " << wareHouse.getInProcessOrders().size() << endl;
                  wareHouse.getInProcessOrders().erase(it);
                  // cout << "After erase2 " << wareHouse.getInProcessOrders().size() << endl;
                  break;
               }
            }
         }
         else
         {
            printf("unknown status\n");
         }
      }
   }
}

void SimulateStep::stage2(WareHouse &wareHouse)
{
   auto volunteers = wareHouse.getVolunteers();
   for (auto it = volunteers.begin(); it != volunteers.end(); ++it)
   {
      if ((*it)->isBusy())
      {
         (*it)->step(); // Consider if you really need to delete the volunteer here
      }
   }
}
void SimulateStep::stage1(WareHouse &wareHouse)
{
   auto &pendingOrders = wareHouse.getPendingOrders();
   auto it = pendingOrders.begin();

   while (it != pendingOrders.end() && pendingOrders.size() != 0)
   {
      // printf("Searching for volutneer for order %d\n", (*it)->getId());
      if ((*it)->getStatus() == OrderStatus::PENDING && wareHouse.getVolCounter() != 0)
      {
         for (unsigned int j = 0; j < wareHouse.getVolunteers().size(); j++)
         {
            if (wareHouse.getVolunteers()[j]->canTakeOrder(*(*it)))
            {
               // cout << wareHouse.getVolunteers()[j]->getName() << endl;
               // cout << std::to_string(wareHouse.getVolunteers()[j]->getId()) << endl;
               wareHouse.getVolunteers()[j]->acceptOrder(*(*it));
               (*it)->setStatus(OrderStatus::COLLECTING);
               (*it)->setCollectorId(wareHouse.getVolunteers()[j]->getId());
               // cout << "a" << endl;
               wareHouse.getInProcessOrders().push_back(*it);
               // cout << "b" << endl;
               // cout << "Before erase " << pendingOrders.size() << endl;
               it = pendingOrders.erase(it); // Use the returned iterator after erasing
               // cout << "After erase " << pendingOrders.size() << endl;
               wareHouse.decreaseVolCounter();
               break;
            }
         }
      }
      else if ((*it)->getStatus() == OrderStatus::COLLECTING && wareHouse.getVolCounter() != 0)
      {
         for (unsigned int j = 0; j < wareHouse.getVolunteers().size(); j++)
         {
            if (wareHouse.getVolunteers()[j]->canTakeOrder(*(*it)))
            {
               (*it)->setStatus(OrderStatus::DELIVERING);
               // cout << wareHouse.getVolunteers()[j]->getName() << endl;
               // cout << std::to_string(wareHouse.getVolunteers()[j]->getId()) << endl;
               wareHouse.getVolunteers()[j]->acceptOrder(*(*it));
               // cout << std::to_string(wareHouse.getVolunteers()[j]->getId()) + " accepted the order : " + std::to_string((*it)->getId()) << endl;
               (*it)->setDriverId(wareHouse.getVolunteers()[j]->getId());

               // cout << "A" << endl;
               wareHouse.getInProcessOrders().push_back(*it);
               // cout << "Before erase " << pendingOrders.size() << endl;
               it = pendingOrders.erase(it); // Use the returned iterator after erasing
               // cout << "After erase " << pendingOrders.size() << endl;
               //  cout << "C" << endl;
               wareHouse.decreaseVolCounter();

               break;
            }
         }
      }
      else
      {
         ++it;
      }
   }
}

/*Add Order*/
AddOrder::AddOrder(int id) : BaseAction(), customerId(id)
{
}

AddOrder *AddOrder::clone() const
{
   return new AddOrder(*this);
}

void AddOrder::act(WareHouse &wareHouse)
{

   if (wareHouse.customerExists(customerId))
   {
      if (wareHouse.getCustomer(customerId).canMakeOrder())
      {
         wareHouse.getCustomer(customerId).addOrder(wareHouse.getOrderCounter());
         wareHouse.addOrder(new Order(wareHouse.getOrderCounter(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance()));
         // wareHouse.incOrderCounter();
         complete();
      }
      else
      {
         error("Cannot place this order");
      }
   }
   else
   {
      error("Cannot place this order");
   }
}
string AddOrder::toString() const
{
   string str = "order " + to_string(customerId);
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}
int AddOrder::getCustomerId() const
{
   return customerId;
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : BaseAction(), customerName(customerName), customerType(customerType == "civilian" ? CustomerType::Civilian : CustomerType::Soldier), distance(distance), maxOrders(maxOrders) {}

string AddCustomer::getCustomerName() const
{
   return customerName;
}
int AddCustomer::getDistance() const
{
   return distance;
}
CustomerType AddCustomer::getCustomerType() const
{
   return customerType;
}
int AddCustomer::getMaxOrders() const
{
   return maxOrders;
}

void AddCustomer::act(WareHouse &warehouse)
{
   if (getCustomerType() == CustomerType::Soldier)
   {
      Customer *c = new SoldierCustomer(warehouse.getCustomerCounter() + 1, getCustomerName(), getDistance(), getMaxOrders());
      warehouse.addCustomer(c);
      complete();
   }
   else if (getCustomerType() == CustomerType::Civilian)
   {
      Customer *c = new CivilianCustomer(warehouse.getCustomerCounter() + 1, getCustomerName(), getDistance(), getMaxOrders());
      warehouse.addCustomer(c);
      complete();
   }
}

AddCustomer *AddCustomer::clone() const
{
   return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
   string output = "customer " + customerName + " " + toString1(customerType) + " " + to_string(distance) + " " + to_string(maxOrders);
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      output += "COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      output += "ERROR";
   }
   return output;
}
string AddCustomer::toString1(CustomerType type) const
{
   string str = "";
   if (type == CustomerType::Civilian)
   {
      str = "Civilian";
   }
   else
      str = "Soldier";
   return str;
}
PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id)
{
}

string PrintOrderStatus::toString() const
{
   string str = "orderStatus " + to_string(orderId);
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
   if (wareHouse.orderExists(orderId))
   {

      cout << wareHouse.getOrder(orderId).toString() << endl;
      complete();
   }
   else
      error("Order doesn't exist");
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
   return new PrintOrderStatus(*this);
}
PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId)
{
}
PrintCustomerStatus *PrintCustomerStatus::clone() const
{
   return new PrintCustomerStatus(*this);
}
string PrintCustomerStatus::toString() const
{
   string str = "customerStatus " + to_string(customerId); // status to string
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}
void PrintCustomerStatus::act(WareHouse &wareHouse)
{ ////////////////////////
   string output = "";
   if (wareHouse.customerExists(customerId))
   {
      string name = wareHouse.getCustomer(customerId).getName();
      output = "CustomerID: " + to_string(customerId) + "\n";
      for (int orderId : wareHouse.getCustomer(customerId).getOrdersIds())
      {
         output += "OrderID: " + to_string(orderId) + "\n";
         output += "OrderStatus: " + wareHouse.getOrder(orderId).toString1(wareHouse.getOrder(orderId).getStatus()) + "\n";
      }
      int ordersLeft = wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders();
      output += "numOrdersLeft:" + to_string(ordersLeft);
      cout << output << endl;
      complete();
   }

   else
      error("Customer doesn't exist");
}
PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id)
{
}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
   return new PrintVolunteerStatus(*this);
}
void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
   if (wareHouse.volunteerExists(volunteerId))
   {
      cout << wareHouse.getVolunteer(volunteerId).toString() << endl;
      complete();
   }
   else
      error("Volunteer doesn't exist");
}
string PrintVolunteerStatus::toString() const
{
   string str = "volunteerStatus " + to_string(volunteerId); // status to string
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}

PrintActionsLog::PrintActionsLog() : BaseAction() {}
PrintActionsLog *PrintActionsLog::clone() const
{
   return new PrintActionsLog(*this);
}
void PrintActionsLog::act(WareHouse &wareHouse)
{
   for (BaseAction *action : wareHouse.getActions())
   {
      cout << action->toString() << endl;
      complete();
   }
}
string PrintActionsLog::toString() const
{
   string str = "log"; // status to string
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}

Close::Close() : BaseAction() {}

Close *Close::clone() const
{
   return new Close(*this);
}
string Close::toString() const
{
   string str = "close"; // status to string
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}
void Close::act(WareHouse &warehouse)
{
   for (Order *o : warehouse.getPendingOrders())
   {
      std::cout << o->tostring2() << std::endl;
   }
   for (Order *o : warehouse.getInProcessOrders())
   {
      std::cout << o->tostring2() << std::endl;
   }
   for (Order *o : warehouse.getCompletedOrders())
   {
      std::cout << o->tostring2() << std::endl;
   }
   warehouse.close();
   complete();
}

RestoreWareHouse::RestoreWareHouse() : BaseAction() {}

void RestoreWareHouse::act(WareHouse &wareHouse)
{

   if (backup == nullptr)
   {
      error("No backup available\n");
   }
   else
   {
      wareHouse = *backup;
      complete();
   }
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
   return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
   string str = "restore"; // status to string
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}

BackupWareHouse::BackupWareHouse() : BaseAction()
{
}

void BackupWareHouse::act(WareHouse &wareHouse)
{
   if (backup != nullptr)
   {
      delete backup;
   }
   backup = new WareHouse(wareHouse);
   complete();
}
BackupWareHouse *BackupWareHouse::clone() const
{
   return new BackupWareHouse(*this);
}
string BackupWareHouse::toString() const
{
   string str = "backup"; // status to string
   if (this->getStatus() == ActionStatus::COMPLETED)
   {
      str += " COMPLETED";
   }
   else if (this->getStatus() == ActionStatus::ERROR)
   {
      str += " ERROR";
   }
   return str;
}