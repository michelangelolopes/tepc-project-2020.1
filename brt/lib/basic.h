#ifndef BASIC_H
#define BASIC_H

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define STATION -2
#define EMPTY -1
#define EXPRESSO 0
#define PARADOR 1

struct Vehicle_{
    int speed;
    int lane;
    int position;
    int type;
    int time_stop;
};

struct Config_{
    int lane_length;
    int lanes_count;
    int vehicle_length;
    int vehicles_count;
    int time_steps;
    int speed_max;
    int stations_count;
    int* stations_pos;
};

typedef struct Config_ Config;
typedef struct Vehicle_ Vehicle;

Config load_configs(char** argv); //loads the configurations
void print_configs(Config config); //prints the configurations being used
int* set_stations(Config config); //based on the length of the lane, set well-divided stations

void fill_grid(Config config, int** grid); //at the beginning, all cells in lanes are EMPTY
void markStations_grid(Config config, int** grid); //mark the stations positions in the grid
int** create_grid(Config config); //create a grid with all cells empty, except the stations cells
void print_grid(Config, int**); //print grid cells
void free_grid(Config config, int** grid); //dealocate the memory used by the grid

void fill_vehicles(Config config, Vehicle* vehicle); //set the vehicles attributes before initial distribution
void print_vehicles(Config config, Vehicle* vehicle); //print the vehicles attributes
void free_vehicles(Vehicle*); //dealocate the memory used by the vehicles data structure

int isStationEmpty(Config config, int** grid, int station_number); //check if the station is completely empty or not
int canChangeLane(Config config, int** grid_old, Vehicle* vehicle); //check if at the other lane has some vehicle, preventing the changing lane action

void updateVehicles_grid(Config config, int** grid, Vehicle* vehicle); //update the grid with the current attributes of the vehicles

#endif