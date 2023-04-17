
#include "../include/Action.h"
#include "../include/Trainer.h"
#include "../include/Studio.h"
using namespace std;
#include <string>
#include <stdio.h>
#include <stdlib.h>

BaseAction::BaseAction(){}
//distructor
BaseAction::~BaseAction() {}

ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete(){
    status=COMPLETED;
}

void BaseAction::error(std::string errorMsg){
    status=ERROR;
    this->errorMsg=errorMsg;
    std::cout<<"Error: "<<errorMsg<<std::endl;
}

std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

OpenTrainer::OpenTrainer(int _id, std::vector<Customer *> &customersList) :BaseAction(),trainerId(_id),customers(customersList),
to_string("open "+std::to_string(_id))
{

}

void OpenTrainer::act(Studio &studio){
    //Open the trainers training session.
    Trainer* trainer=studio.getTrainer(trainerId);
    if(trainer==nullptr || trainer->isOpen()){
        error("Workout session does not exis or is already open.");
        for (Customer* c : customers){
            delete c;
        }
    } 
    else {
        trainer->openTrainer();
        for( Customer* customer : customers){
            if(trainer->getCapacity()>(static_cast<int>(trainer->getCustomers().size()))) {
                trainer->addCustomer(customer);
                to_string+=(" "+customer->toString());
            }
            else {
                delete customer;
            }
            
        }
        
        complete();

    }

}

std::string OpenTrainer::toString() const{
    string status=getStatus()==COMPLETED ? " Completed" : "Error: "+getErrorMsg();
    return to_string+status;
}

OpenTrainer* OpenTrainer::clone() {
    return new OpenTrainer(*this);
}

Order::Order(int id):trainerId(id){
}

void Order::act(Studio &studio){
    Trainer* trainer=studio.getTrainer(trainerId);
    if(trainer==nullptr || !(trainer->isOpen())){
        error("Trainer does not exist or is not open.");
    } else {
        for (Customer* customer : trainer->getCustomers()){
            trainer->order(customer->getId(),customer->order(studio.getWorkout_options()), studio.getWorkout_options());
            for (int i : customer->order(studio.getWorkout_options())) {
                std::cout<<customer->getName()<<" is Doing "<<studio.getWorkout_options()[i].getName()<<std::endl;
            }
        }
        trainer->orderTrainer();
        complete();
    }


}

std::string Order::toString() const{
    string status=getStatus()==COMPLETED ? "Completed" : "Error: "+getErrorMsg();
    return "order "+std::to_string(trainerId)+" "+status;
}

Order* Order::clone() {
    return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(src),dstTrainer(dst),id(customerId){}

void MoveCustomer::act(Studio &studio){
    Trainer* sTrainer=studio.getTrainer(srcTrainer);
    Trainer* dTrainer=studio.getTrainer(dstTrainer);
    if(sTrainer==nullptr || dTrainer==nullptr || !(dTrainer->isOpen())|| !(sTrainer->isOpen())|| sTrainer->getCustomer(id)==nullptr || dTrainer->getCapacity()<static_cast<int>(dTrainer->getCustomers().size()+1)){
        error("Cannot move customer");
    } else {
        Customer* customer=sTrainer->getCustomer(id);
        dTrainer->addCustomer(customer);
        dTrainer->order(id,customer->order(studio.getWorkout_options()), studio.getWorkout_options());
        sTrainer->removeForMoveCustomer(id);
        if (sTrainer->getCustomers().size() == 0) {
            sTrainer->closeTrainer();
        }
        complete();
    }
}

std::string MoveCustomer::toString() const{
    string status=getStatus()==COMPLETED ? "Completed" : "Error: "+getErrorMsg();
    return "move "+std::to_string(srcTrainer)+" "+std::to_string(dstTrainer)+" "+std::to_string(id)+" "+status;

}  


MoveCustomer* MoveCustomer::clone() {
    return new MoveCustomer(*this);
}

Close::Close(int id): trainerId(id){};

void Close::act(Studio &studio){
    Trainer* trainer=studio.getTrainer(trainerId);
    if(trainer==nullptr || !(trainer->isOpen())){
        error("Trainer does not exist or in not open");
    } else {
        vector<Customer*> temp= trainer->getCustomers();
        for(Customer* customer : temp){
            trainer->removeCustomer(customer->getId());
        }
        trainer->closeTrainer();
        cout<<"Trainer "+std::to_string(trainerId)+" closed. Salary "+std::to_string(trainer->getSalary())+"NIS"<<endl;
        
        complete();

    }
}

std::string Close::toString() const{
    string status=getStatus()==COMPLETED ? "Completed" : "Error: "+getErrorMsg();
    return "close "+std::to_string(trainerId)+" "+status;
}

Close* Close::clone() {
    return new Close(*this);
}

CloseAll::CloseAll(){}

void CloseAll::act(Studio &studio){
    for(size_t i=0; i<studio.getTrainers().size(); i++){
        if(studio.getTrainer(i)->isOpen()){
            std::cout<<"Trainer "<<i<<" closed. Salary "<<studio.getTrainer(i)->getSalary()<<"NIS"<<std::endl;
        }
       
    }
    complete();
}

std::string CloseAll::toString() const{
    string status=getStatus()==COMPLETED ? "Completed" : "Error: "+getErrorMsg();
    return "Closeall "+status;
}

CloseAll* CloseAll::clone() {
    return new CloseAll(*this);
}

PrintWorkoutOptions::PrintWorkoutOptions(){}

void PrintWorkoutOptions::act(Studio &studio){
   vector<Workout> workout_options =studio.getWorkout_options();
   for (Workout workout:workout_options){
       cout<<workout.getName()<<","<<workout.getType()<<","<<workout.getPrice()<<endl;
   }
   complete();
   
}

std::string PrintWorkoutOptions::toString() const{
    string status=getStatus()==COMPLETED ? "Completed" : "Error: "+getErrorMsg();
    return "workout_options "+status;
}


PrintWorkoutOptions* PrintWorkoutOptions::clone() {
    return new PrintWorkoutOptions(*this);
}

PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id){}

void PrintTrainerStatus::act(Studio &studio){
    //If the trainer never was open - we dont print anything;
    //If the trainer is closed - we print only the trainer status;
    // else - print all the data.
    Trainer* trainer=studio.getTrainer(trainerId);
    if(trainer->getWasEverOpend()){
        string openOrClosed= trainer->isOpen() ? "open" : "closed";
        cout<<"Trainer "<<trainerId<<" status: "<<openOrClosed<<endl;
        if(trainer->isOrderd()){
            cout<<"Customers:"<<endl;
            for(Customer* customer: trainer->getCustomers()){
                cout<<customer->getId()<<" "<<customer->getName()<<endl;
            }
            cout<<"Orders:"<<endl;
            for(OrderPair orderPair: trainer->getOrders()){
                cout<<orderPair.second.getName()<<" "<<orderPair.second.getPrice()<<" "<<orderPair.first<<endl;
            }
            cout<<"Current Trainer's Salary: "<<std::to_string(trainer->getSalary())<<"NIS"<<endl;
        }
    }
    
    
    complete();

}

std::string PrintTrainerStatus::toString() const{
    string status=getStatus()==COMPLETED ? "Completed" : "Error: "+getErrorMsg();
    return "status "+std::to_string(trainerId)+" "+status;
}

PrintTrainerStatus* PrintTrainerStatus::clone() {
    return new PrintTrainerStatus(*this);
}

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Studio &studio)
{
    for(BaseAction* baseAction : studio.getActionsLog()){
        
        std::cout<<baseAction->toString()<<std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const{
    string status=getStatus()==COMPLETED ? "Completed" : "Error: "+getErrorMsg();
    return "log "+status;
}

PrintActionsLog* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

BackupStudio::BackupStudio(){}

void BackupStudio::act(Studio &studio){
    extern Studio* backup;
    //delete the old studio.
    if(backup!=nullptr){
        delete backup;
    }
    backup = new Studio(studio);
    complete();
}

BackupStudio* BackupStudio::clone() {
    return new BackupStudio(*this);
}

std::string BackupStudio::toString() const{
    string status = "Completed";
    return "backup "+status;
}

RestoreStudio::RestoreStudio(){}

void RestoreStudio::act(Studio &studio){
    extern Studio* backup;
    if(backup==nullptr){
        error("No backupavailable");
    } else {
        studio=*backup;
        complete();
    }

}
std::string RestoreStudio::toString() const{
    string status=getStatus()==COMPLETED ? "Completed" : "Error: "+getErrorMsg();
    return "restore "+status;

}

RestoreStudio* RestoreStudio::clone() {
    return new RestoreStudio(*this);
}
