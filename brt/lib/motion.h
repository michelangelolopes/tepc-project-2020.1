#ifndef MOTION_H
#define MOTION_H

#include "distance.h"

int queueStart_vehicles(Config config, int** grid_old, int** grid_cur, Vehicle* vehicle, int vehicle_count); //the vehicles start at the stations lane, one by one, after the time passes
void randomStart_vehicles(Config config, int** grid, Vehicle*); //the vehicles are distributed among the lanes, at different random positions

void moviment_vehicles(Config config, int** grid_old, int** grid_cur, Vehicle* vehicle); //calculating the new speed and the new position of each vehicle following the NaSch model's rules

void changeLanes_vehicles(Config config, int** grid, Vehicle* vehicle); //change the vehicles of express type, if necessary and possible

#endif