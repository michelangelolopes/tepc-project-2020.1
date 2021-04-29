#include "basic.h"

Config load_configs(char** argv){
    Config config;
    config.lane_length = atoi(argv[1]);
    config.lanes_count = atoi(argv[2]);
    config.vehicle_length = atoi(argv[3]);
    config.vehicles_count = atoi(argv[4]);
    config.time_steps = atoi(argv[5]);
    config.speed_max = atoi(argv[6]);
    config.stations_count = atoi(argv[7]);

    return config;
}

void print_configs(Config config){
    printf("%d %d %d %d %d %d %d\n", config.lane_length, config.lanes_count, config.vehicle_length, config.vehicles_count, config.time_steps, config.speed_max, config.stations_count);

    for(int i = 0; i < config.stations_count; i++)
    {
        printf("%d ", config.stations_pos[i]);
    }

    printf("\n");
}

int* set_stations(Config config){
    int* stations_pos = (int*) malloc(sizeof(int) * config.stations_count);

    int stations_dist = config.lane_length / (config.stations_count + 1);

    for(int i = 0; i < config.stations_count; i++){
        stations_pos[i] = stations_dist;

        if(i != 0){
            stations_pos[i] += stations_pos[i - 1];
        }
    }

    return stations_pos;
}

void fill_grid(Config config, int** grid){
    for(int i = 0; i < config.lanes_count; i++)
    {
        for(int j = 0; j < config.lane_length; j++)
        {
            grid[i][j] = EMPTY;
        }
    }
}

void markStations_grid(Config config, int** grid){
    for(int i = 0; i < config.stations_count; i++)
    {
        int count = config.stations_pos[i];

        while(count < config.stations_pos[i] + 10){
            if(grid[0][count] == EMPTY)
                grid[0][count] = STATION;
            count++;
        }
    }
}

int** create_grid(Config config){
    int** grid = (int**) malloc(sizeof(int*) * config.lanes_count);

    for(int i = 0; i < config.lanes_count; i++)
    {
        grid[i] = (int*) malloc(sizeof(int) * config.lane_length);
    }

    fill_grid(config, grid);
    markStations_grid(config, grid);

    return grid;
}

void print_grid(Config config, int** grid){
    for(int i = 0; i < config.lanes_count; i++)
    {
        printf("[%d] ", i);

        for(int j = 0; j < config.lane_length; j++)
        {
            if(grid[i][j] == EMPTY)
            {
                printf(". ");
            }
            else if (grid[i][j] == STATION)
            {
                printf("# ");
            }
            else
            {
                printf("%d ", grid[i][j]);
            }
        }

        printf("\n");
    }
}

void free_grid(Config config, int** grid){
    if(grid == NULL)
    {
        return;
    }

    for(int i = 0; i < config.lanes_count; i++)
    {
        free(grid[i]);
    }

    free(grid);
}

void fill_vehicles(Config config, Vehicle* vehicle){
    for(int i = 0; i < config.vehicles_count; i++){
        vehicle[i].speed = 0;
        vehicle[i].type = i % 2; //pares são expressos; ímpares são paradores
        vehicle[i].time_stop = 0;

        vehicle[i].lane = -1;
        vehicle[i].position = -1;
    }
}

void print_vehicles(Config config, Vehicle* vehicle){
    for(int i = 0; i < config.vehicles_count; i++)
    {
        printf("[%d] speed: %d | lane: %d | position: %d | type: %d | time_stop: %d\n", i, vehicle[i].speed, vehicle[i].lane, vehicle[i].position, vehicle[i].type, vehicle[i].time_stop);
    }
}

void free_vehicles(Vehicle* vehicle){
    free(vehicle);
}

int isStationEmpty(Config config, int** grid, int station_number){
    for(int i = station_number; i < station_number + config.vehicle_length; i++){
        if(grid[0][i] != -2){
            return 0;
        }
    }

    return 1;
}

int canChangeLane(Config config, int** grid_old, Vehicle* vehicle){
    int checkLane = vehicle->lane == 0 ? 1 : 0;

    for(int j = vehicle->position; j < vehicle->position + config.vehicle_length; j++){
        
        int pos = j;

        if(pos >= config.lane_length)
        {
            pos = pos - config.lane_length;
        }

        if(grid_old[checkLane][pos] != -1)
        {
            return 0;
        }
    }

    return 1;
}

void updateVehicles_grid(Config config, int** grid, Vehicle* vehicle){
    
    for(int i = 0; i < config.vehicles_count; i++)
    {
        if(vehicle[i].position == -1 && vehicle[i].lane == -1){
            continue;
        }

        for(int j = vehicle[i].position; j < vehicle[i].position + config.vehicle_length; j++)
        {
            int pos = j;

            if(pos >= config.lane_length)
            {
                pos = pos - config.lane_length;
            }

            grid[vehicle[i].lane][pos] = i;
        }
    }
}
