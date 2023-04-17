#include "../include/Trainer.h"
#include <iostream>

//constructor
Trainer::Trainer(int t_capacity):capacity{t_capacity},open{false},orderd{false},salary{0},wasEverOpend{false}
{
};

// Copy Constructor
Trainer::Trainer(const Trainer& t): capacity(t.capacity), open(t.open),orderd(t.orderd),customersList(),orderList(t.orderList), salary(t.salary),wasEverOpend(t.wasEverOpend)
    {
        for (Customer* customer : t.customersList)
        {
            customersList.push_back(customer->clone());
        }
    }

//Copy Assignment
Trainer& Trainer::operator=(const Trainer &t){
    if (this != &t) {
        open = t.open;
        orderd=t.orderd;
        wasEverOpend=t.wasEverOpend;
        capacity = t.capacity;
        salary = t.salary;
        for(Customer* customer : customersList){
            delete customer;
        }
        customersList.clear();
        orderList.clear();
        for (OrderPair pair : t.orderList) {
            orderList.push_back(OrderPair(pair.first, Workout(pair.second.getId(), pair.second.getName(), pair.second.getPrice(), pair.second.getType())));
        }
        for (Customer* customer : t.customersList)
        {
            customersList.push_back(customer->clone());
        }
    }
    return *this;
}
//Move Constructor
Trainer::Trainer(Trainer &&t): capacity(t.capacity), open(t.open),orderd(t.orderd),customersList(),orderList(), salary(t.salary), wasEverOpend(t.wasEverOpend) {
    for (Customer* customer : t.customersList)
    {
        customersList.push_back(customer);
    }
    for (OrderPair pair : t.orderList){
        orderList.push_back(pair);
    }
    t.orderList.clear();
    t.customersList.clear();
}

//Move Assignment
Trainer& Trainer::operator=(Trainer &&t){
    if (this != &t) {
        open = t.open;
        orderd=t.orderd;
        wasEverOpend=t.wasEverOpend;
        capacity = t.capacity;
        salary = t.salary;
        for(Customer* customer : customersList){
            delete customer;
        }
        customersList.clear();
        orderList.clear();
        for (OrderPair pair : t.orderList){
            orderList.push_back(pair);
        }
        for (Customer* customer : t.customersList)
        {
            customersList.push_back(customer);
        }
        t.orderList.clear();
        t.customersList.clear();
    }
    return *this;
}

//Distractor
Trainer::~Trainer(){
    for(Customer* customer : customersList){
        delete customer;
    }
    customersList.clear();
    orderList.clear();
    open = NULL;
    orderd=NULL;
    wasEverOpend=NULL;
    capacity=0;
    salary=0;
}

void Trainer::removeForMoveCustomer(int id){
    int index=0;
    for(Customer* customer : customersList){
        if(customer->getId()==id){
            customersList.erase(customersList.begin()+index);
        }
        index++;
    }

    std::vector<OrderPair> orderListTemp; 
    for (OrderPair order : orderList) {
        if (order.first != id) {
            orderListTemp.push_back(order);
        }
        else {
            salary -= order.second.getPrice();
        }
    }
    orderList.clear();
    for(OrderPair order : orderListTemp){
        orderList.push_back(order);
    }

}

int Trainer::getCapacity() const{
    return capacity;
}

void Trainer::addCustomer(Customer* customer){
    customersList.push_back(customer);
}
void Trainer::removeCustomer(int id){
    int index=0;
    for(Customer* customer : customersList){
        if(customer->getId()==id){
            customersList.erase(customersList.begin()+index);
            delete customer;
        }
        index++;
    }

    std::vector<OrderPair> orderListTemp; 
    for (OrderPair order : orderList) {
        if (order.first != id) {
            orderListTemp.push_back(order);
        }
    }
    orderList.clear();
    for(OrderPair order : orderListTemp){
        orderList.push_back(order);
    }
}

Customer* Trainer::getCustomer(int id){
    for(Customer*& customer : customersList){
        if(customer->getId()==id){
            return customer;
        }
    }
    return nullptr;
}

std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    
    for(const int& i : workout_ids){
        OrderPair temp(customer_id,workout_options[i]);
        orderList.push_back(temp);
        
    }
    for(OrderPair& order : orderList){
        if(order.first==customer_id){
            salary+=order.second.getPrice();
            
        }
        
        
    }
}

void Trainer::openTrainer(){
    open=true;
    if(!wasEverOpend){
        wasEverOpend=true;
    }
}

void Trainer::orderTrainer(){
    orderd=true;
}

void Trainer::closeTrainer(){
    open=false;
    orderd=false;
}

int Trainer::getSalary() const {
    return salary;
}


bool Trainer::isOpen(){
    return open;
}

bool Trainer::isOrderd(){
    return orderd;
}

bool Trainer::getWasEverOpend(){
    return wasEverOpend;
}

