
#include "../include/Customer.h"
#include <algorithm>
#include <iostream>
using namespace std;

Customer::Customer(std::string c_name, int c_id):name{c_name},id{c_id}
{
}
//distructor
Customer::~Customer() {}

std::string Customer::getName() const{
    return name;
}

int Customer::getId() const{
    return id;
}


SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(name,id)
{

}
//distructor
SweatyCustomer::~SweatyCustomer(){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    //Get all the cardio seesions.
    std::vector<int> cardioWorkout;
    for (const Workout& i: workout_options){
        if(i.getType()==CARDIO){
            cardioWorkout.push_back(i.getId());
        }
    }
    return cardioWorkout;

}

SweatyCustomer* SweatyCustomer::clone() {
    return (new SweatyCustomer(this->getName(), this->getId()));
}

std::string SweatyCustomer::toString() const{
    return getName()+",swt";
}

CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id)
{
}
//distructor
CheapCustomer::~CheapCustomer(){}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options)
{
    //Get the chepest ssesion.
    std::vector<int> cheapstWorkout;
    int cheapst=0;
    for (size_t i=1; i<workout_options.size(); i++){
        if(workout_options[i].getPrice()<workout_options[cheapst].getPrice()){
            cheapst = i;
        }
    }
    cheapstWorkout.push_back(cheapst);
    return cheapstWorkout;
}

CheapCustomer* CheapCustomer::clone() {
    return (new CheapCustomer(this->getName(), this->getId()));
}

std::string CheapCustomer::toString() const{
    return getName()+",chp";
}


HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name,id)
{
       
}
//distructor
HeavyMuscleCustomer::~HeavyMuscleCustomer(){}


std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    //Get all the anaerobic seesions orderd by price.
    std::vector<pair<int,int>> anerobic_prices;
    for (Workout workout : workout_options){
        if (workout.getType()==ANAEROBIC) anerobic_prices.push_back(pair<int, int>(workout.getPrice(), workout.getId()));
    }
    //sorts in descending order by first element in pair == price
    std::sort(anerobic_prices.rbegin(), anerobic_prices.rend());
    std::vector<int> anaerobicWorkout;
    for (pair<int,int> p : anerobic_prices) {
        anaerobicWorkout.push_back(p.second);
    }
    return anaerobicWorkout;
}

HeavyMuscleCustomer* HeavyMuscleCustomer::clone() {
    return (new HeavyMuscleCustomer(this->getName(), this->getId()));
}

std::string HeavyMuscleCustomer::toString() const{
    return getName()+",mcl";
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name,id)
{
       
}
//distructor
FullBodyCustomer::~FullBodyCustomer(){}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> fdbWorkout;
    int cheapstCardio=0;
    for (size_t i=1; i<workout_options.size(); i++){
        if((workout_options[i].getPrice()<workout_options[cheapstCardio].getPrice() && workout_options[i].getType()==CARDIO)|| workout_options[cheapstCardio].getType()!=CARDIO ){
            cheapstCardio = i;
        }
    }
    if(workout_options[cheapstCardio].getType()==CARDIO){
        fdbWorkout.push_back(cheapstCardio);
    }
    

    int expensiveMix=0;
    for (size_t i=1; i<workout_options.size(); i++){
        if((workout_options[i].getPrice()>workout_options[expensiveMix].getPrice() && workout_options[i].getType()==MIXED)|| workout_options[expensiveMix].getType()!=MIXED ){
            expensiveMix = i;
        }
    }
    if(workout_options[expensiveMix].getType()==MIXED){
        fdbWorkout.push_back(expensiveMix);
    }
    
    int cheapstAnaerobic=0;
    for (size_t i=1; i<workout_options.size(); i++){
        if((workout_options[i].getPrice()<workout_options[cheapstAnaerobic].getPrice() && workout_options[i].getType()==ANAEROBIC)|| workout_options[cheapstAnaerobic].getType()!=ANAEROBIC ){
            cheapstAnaerobic = i;
        }
    }
    if(workout_options[cheapstAnaerobic].getType()==ANAEROBIC){
        fdbWorkout.push_back(cheapstAnaerobic);
    }
    return fdbWorkout;

}

FullBodyCustomer* FullBodyCustomer::clone() {
    return (new FullBodyCustomer(this->getName(), this->getId()));
}

std::string FullBodyCustomer::toString() const{
    return getName()+",fbd";
}



