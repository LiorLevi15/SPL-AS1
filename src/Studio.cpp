#include "../include/Studio.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "../include/Action.h"

using namespace std;


Studio::Studio(const string &configFilePath) {
    ifstream file(configFilePath);
    string line;
    int paragraph=0;
    int workoutId=0;
    nextCustomerId=0;
    while (getline(file,line)){
        if(line[0] != '\0'){
            if(line[0]=='#') {
                 paragraph=paragraph+1;
            }
            else if (paragraph==1) {
                continue;
            }
            else if (paragraph==2){

                string temp ="";
                for(size_t i=0; i<line.size(); i++){
                    if (line[i]!=',') temp.push_back(line[i]);
                    else {
                        trainers.push_back(new Trainer(stoi(temp)));
                        temp="";
                    }
                }
                trainers.push_back(new Trainer(stoi(temp)));
                
                
            } else {
                
                string workoutName ="";
                WorkoutType workoutType;
                string workoutPriceSring="";
                int workouPrice;
                size_t i=0;
                while(line[i]!=','){
                    workoutName.push_back(line[i]);
                    i++;
                }
                i++;
                while(line[i]==' '){
                    i++;
                };
                if(line[i]=='A') workoutType=ANAEROBIC;
                else if (line[i]=='M') workoutType=MIXED;
                else workoutType=CARDIO;
                while(line[i]!=','){
                    i++;
                }
                i++;
                while(i<line.size()){
                    workoutPriceSring.push_back(line[i]);
                    i++;
                }
                workouPrice=stoi(workoutPriceSring);
                workout_options.push_back(Workout(workoutId,workoutName,workouPrice,workoutType));
                workoutId++;

            }

        }
    }
    file.close();
}

//Copy Constructor
Studio::Studio(const Studio &studio1): open(studio1.open), trainers(), actionsLog(),nextCustomerId(studio1.nextCustomerId){
    for (Trainer* trainer: studio1.trainers){
        trainers.push_back(new Trainer(*trainer));
    }
    
    for(Workout workout : studio1.workout_options){
        workout_options.push_back(Workout(workout.getId(),workout.getName(),workout.getPrice(),workout.getType()));
    }
    for(BaseAction* baseAction : studio1.actionsLog){
        actionsLog.push_back(baseAction->clone());
    }

    
    
}
//Destructor
Studio::~Studio() {
    for(Trainer* trainer : trainers){
        delete trainer;
    }
    trainers.clear();
    for(BaseAction* baseAction : actionsLog){
        delete baseAction;
    }
    actionsLog.clear();
    workout_options.clear();
}

//Copy Assignment
Studio& Studio::operator=(const Studio &studio1){
    if(this != &studio1){
        for(Trainer* trainer : trainers){
            delete trainer;
        }
        trainers.clear();
        for (Trainer* trainer: studio1.trainers){
            trainers.push_back(new Trainer(*trainer));
        }
        workout_options.clear();
        for(Workout workout : studio1.workout_options){
            workout_options.push_back(Workout(workout.getId(),workout.getName(),workout.getPrice(),workout.getType()));
        }
        for(BaseAction* baseAction : actionsLog){
            delete baseAction;
        }
        actionsLog.clear();
        for(BaseAction* baseAction : studio1.actionsLog){
            BaseAction* temp=baseAction->clone();
            actionsLog.push_back(temp);
        }
        nextCustomerId=studio1.nextCustomerId;
        
    }
    return *this;
}
//Move Constructor
Studio::Studio(Studio &&studio): open(studio.open),trainers(), workout_options(),  actionsLog(),nextCustomerId(studio.nextCustomerId){
    for (Trainer* trainer : studio.trainers) {
        trainers.push_back(trainer);
    }
    for(Workout workout : studio.workout_options){
        workout_options.push_back(workout);
    }
    for(BaseAction* baseAction : studio.actionsLog){
        actionsLog.push_back(baseAction);
    }
    studio.actionsLog.clear();
    studio.trainers.clear();
    studio.workout_options.clear();
}
//Move Assignment
Studio& Studio::operator=(Studio &&studio){
    if(this != &studio){
        for(Trainer* trainer : trainers){
            delete trainer;
        }
        trainers.clear();
        for (Trainer* trainer : studio.trainers) {
            trainers.push_back(trainer);
        }
        workout_options.clear(); 
        for(Workout workout : studio.workout_options){
            workout_options.push_back(workout);
        }
        for(BaseAction* baseAction : studio.actionsLog){
            delete baseAction;
        }
        actionsLog.clear();
        for(BaseAction* baseAction : studio.actionsLog){
            actionsLog.push_back(baseAction);
        }
        nextCustomerId=studio.nextCustomerId;
        open = studio.open;
        studio.trainers.clear();
        studio.actionsLog.clear();
        studio.workout_options.clear();
    
    } 
    return *this;
    
}        
    
void Studio::start(){
//   int i = 0;
//   for (Trainer* trainer : trainers){
//       cout << "trainr " << i << ": " << trainer->getCapacity() << endl;
//       i++;
//   }
//   for (Workout workout : workout_options) {
//       cout << "workout " << workout.getId() << ": " << workout.getName() << ", " << workout.getType() << ", " << workout.getPrice() << endl;
//   }
  std::cout << "The Studio is now open!" << std::endl;
//   std::cout<< "!!" << std::endl;
  bool open=true;
  std::string input;
  while (open)
  {
      getline(std::cin, input);
      std::stringstream inputStream(input);
      std::string actionName;
      //inputSream - get evey time a new word.
      inputStream >> actionName;
      if(actionName=="open"){
          std:: string id;
          inputStream >> id;
          vector <Customer*> customersList;
          
          
          while (!inputStream.eof()) {
            std:: string nextString;
            inputStream >> nextString;
            string name="";
            for(size_t i=0; i<nextString.size(); i++){
                if (nextString[i]!=',') name.push_back(nextString[i]);
                    else {
                        nextString=nextString.substr(i+1);
                        break;
                    }
                }
              if (nextString == "swt") {
                customersList.push_back(new SweatyCustomer(name, nextCustomerId));
              }
              else if (nextString == "chp") {
                customersList.push_back(new CheapCustomer(name, nextCustomerId));
              }
              else if (nextString == "mcl") {
                customersList.push_back(new HeavyMuscleCustomer(name, nextCustomerId));
              }
              else {
                customersList.push_back(new FullBodyCustomer(name, nextCustomerId));
              }
               nextCustomerId++;
          }
          
          OpenTrainer *action=new OpenTrainer(std::stoi(id), customersList);
          action->act(*this);
          actionsLog.push_back(action);
          
      } else if(actionName=="order") {
          std::string id;
          inputStream >> id;
          Order *action=new Order(std::stoi(id));
          action->act(*this);
          actionsLog.push_back(action);

      } else if(actionName=="move") {
          std::string sTrainerID;
          inputStream >> sTrainerID;
          std::string dTrainerID;
          inputStream >> dTrainerID;
          std::string customerId;
          inputStream >> customerId;
          MoveCustomer *action=new MoveCustomer(std::stoi(sTrainerID),std::stoi(dTrainerID),std::stoi(customerId));
          action->act(*this);
          actionsLog.push_back(action);

      } else if(actionName=="close") {
          std::string id;
          inputStream >> id;
          Close *action=new Close(std::stoi(id));
          action->act(*this);
          actionsLog.push_back(action);


      } else if(actionName=="closeall") {
          CloseAll *action=new CloseAll();
          action->act(*this);
          actionsLog.push_back(action);
          open=false;

      } else if(actionName=="workout_options") {
          PrintWorkoutOptions *action=new PrintWorkoutOptions();
          action->act(*this);
          actionsLog.push_back(action);

      } else if(actionName=="status") {
          std::string id;
          inputStream >> id;
          PrintTrainerStatus *action=new PrintTrainerStatus(std::stoi(id));
          action->act(*this);
          actionsLog.push_back(action);

      } else if(actionName=="log") {
          PrintActionsLog *action=new PrintActionsLog();
          action->act(*this);
          actionsLog.push_back(action);

      } else if(actionName=="backup") {
          BackupStudio *action=new BackupStudio();
          actionsLog.push_back(action);
          action->act(*this);
          
          

      } else if(actionName=="restore") {
          RestoreStudio *action=new RestoreStudio();
          action->act(*this);
          actionsLog.push_back(action);
          
      } else{
          continue;
      }

  }
  

}

int Studio::getNumOfTrainers() const{
    return trainers.size();
}

std::vector<Workout>  Studio::getWorkout_options() const{
    return workout_options;
}
Trainer* Studio::getTrainer(int tid){
    return trainers[tid];
}

std::vector<Trainer*>  Studio::getTrainers() const{
    return trainers;
}

const std::vector<BaseAction*>& Studio::getActionsLog() const{
    return actionsLog;
}