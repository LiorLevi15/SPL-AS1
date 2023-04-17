#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    ~Studio();
    Studio(const Studio& StudioOther); //copy constructor
    Studio& operator=(const Studio& StudioOther); // copy assignment
    Studio(Studio&& StudioOther); // move constructor
    Studio& operator=(Studio&& StudioOther); // move assignment
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    std::vector<Workout>  getWorkout_options() const;
    std::vector<Trainer*>  getTrainers() const;
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions


private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int nextCustomerId;
};

#endif