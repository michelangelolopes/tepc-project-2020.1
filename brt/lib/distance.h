#ifndef DISTANCE_H
#define DISTANCE_H

#include "basic.h"

int distanceNextStation_vehicles(Config config, Vehicle* vehicle); //distance until reach the next station

int distanceCurrentStation_vehicles(Config config, Vehicle* vehicle); //distance that the vehicle moved inside the station
int distanceStopStation_vehicles(Config config, int** grid_old, Vehicle* vehicle); //distance until reach the end of the station, to stop motion and wait for the passengers

int distanceVehicleLane_vehicles(Config config, int** grid_old, Vehicle* vehicle); //distance until the next vehicle on the same lane, seeking for 2 vehicles away
int distanceVehicleOtherLane_vehicles(Config config, int** grid_old, Vehicle* vehicle); //distance until the next vehicle on the different lane, seeking for 2 vehicles away

int distanceNextVehicle_vehicles(Config config, int** grid_old, Vehicle* vehicle); //distance until the next vehicle on the same lane, seeking for at most by the speed max choosed

#endif