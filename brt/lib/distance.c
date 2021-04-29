#include "distance.h"

int distanceNextStation_vehicles(Config config, Vehicle* vehicle){
    
    int distance_station;

    for(int j = 0; j < config.stations_count; j++){
        distance_station = config.stations_pos[j] - vehicle->position - (config.vehicle_length - 1);

        if(distance_station > 0){ //descobre a próxima estação
            return distance_station;
        }
    }

    //passou pela última estação, então verifica a distância até a primeira estação

    distance_station = config.stations_pos[0] + (config.lane_length - 1) - vehicle->position - (config.vehicle_length - 1);

    return distance_station;
}

int distanceCurrentStation_vehicles(Config config, Vehicle* vehicle){
    
    int distance_station;

    for(int j = 0; j < config.stations_count; j++){
        distance_station = config.stations_pos[j] - vehicle->position - (config.vehicle_length - 1);

        if(distance_station <= 0 && distance_station > (-1 * (config.vehicle_length - 1))){
            return -1 * distance_station;
        }
    }

    return -1;
}

int distanceStopStation_vehicles(Config config, int** grid_old, Vehicle* vehicle){
    int distance = -1;
    int front_position = vehicle->position + config.vehicle_length;

    //printf("front: %d | speed_max: %d\n", front_position, config.speed_max);

    for(int j = front_position; j < front_position + config.speed_max + 1; j++)
    {
        int pos = j;

        if(pos >= config.lane_length)
        {
            pos = pos - config.lane_length;
        }

        // printf("[0][%d] %d ", j, grid_old[0][pos]);
        if(grid_old[0][pos] != -2)
        {
            distance = j - front_position;
            break;
        }
    }

    // printf("\n");

    return distance;
}

int distanceVehicleLane_vehicles(Config config, int** grid_old, Vehicle* vehicle){
    int distance = -1;
    int front_position = vehicle->position + config.vehicle_length;
    int j;

    //printf("front: %d | speed_max: %d\n", front_position, config.speed_max);
    // printf("%d same lane ", vehicle->lane);

    for(j = front_position; j < front_position + (2 * config.vehicle_length); j++)//considerando que pode ver no máximo dois veículos à frente
    {
        int pos = j;

        if(pos >= config.lane_length)
        {
            pos = pos - config.lane_length;
        }
        // printf("%d %d ", pos, grid_old[vehicle->lane][pos]);

        if(grid_old[vehicle->lane][pos] > -1)
        {
            distance = j - front_position;
            // printf("\n");
            break;
        }
    }

    if(distance == -1)
        distance = (j - 1) - front_position;

    // printf("\n");

    // if(distance == -1) //sem veículos à vista
    // {
    //     distance = config.speed_max;
    // }

    return distance;
}

int distanceVehicleOtherLane_vehicles(Config config, int** grid_old, Vehicle* vehicle){
    int distance = -1;
    int front_position = vehicle->position + config.vehicle_length;
    int other_lane = vehicle->lane == 0 ? 1 : 0;
    int j;

    //printf("front: %d | speed_max: %d\n", front_position, config.speed_max);
    // printf("%d other lane ", other_lane);

    for(j = front_position; j < front_position + (2 * config.vehicle_length); j++)//considerando que pode ver no máximo dois veículos à frente
    {
        int pos = j;

        if(pos >= config.lane_length)
        {
            pos = pos - config.lane_length;

        }
        // printf("%d %d ", pos, grid_old[other_lane][pos]);

        if(grid_old[other_lane][pos] > -1)
        {
            distance = j - front_position;
            // printf("\n");
            break;
        }
    }

    if(distance == -1)
        distance = (j - 1) - front_position;
    // printf("\n");

    // if(distance == -1) //sem veículos à vista
    // {
    //     distance = config.speed_max;
    // }

    return distance;
}

int distanceNextVehicle_vehicles(Config config, int** grid_old, Vehicle* vehicle){
    int distance = -1;
    int front_position = vehicle->position + config.vehicle_length;

    //printf("front: %d | speed_max: %d\n", front_position, config.speed_max);

    for(int j = front_position; j < front_position + config.vehicle_length; j++)
    {
        int pos = j;

        if(pos >= config.lane_length)
        {
            pos = pos - config.lane_length;
        }

        if(grid_old[vehicle->lane][pos] > -1)
        {
            distance = j - front_position;
            break;
        }
    }

    if(distance == -1) //sem veículos à vista
    {
        distance = config.speed_max;
    }

    return distance;
}
