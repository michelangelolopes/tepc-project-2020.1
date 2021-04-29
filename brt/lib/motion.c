#include "motion.h"

int queueStart_vehicles(Config config, int** grid_old, int** grid_cur, Vehicle* vehicle, int vehicle_count){
    // printf("vehicle %d\n", vehicle_count);
    if(grid_old != NULL){
        for(int i = 0; i < config.vehicle_length; i++){
            int pos = i;

            if(pos >= config.lane_length)
            {
                pos = pos - config.lane_length;
            }

            if(grid_old[0][pos] != -1)
            {
                return 0;   
            }
        }
    }
    
    vehicle->speed = 0;
    vehicle->lane = 0;
    vehicle->position = 0;
    vehicle->type = vehicle_count % 2; //pares são expressos; ímpares são paradores

    for(int i = 0; i < config.vehicle_length; i++)
    {
        int pos = i;

        if(pos >= config.lane_length)
        {
            pos = pos - config.lane_length;
        }
        
        grid_cur[0][pos] = vehicle_count;
    }
    
    return 1;
}

void randomStart_vehicles(Config config, int** grid, Vehicle* vehicle){
    int rand_lane;
    int rand_startPos;
    int isFitting;

    for(int i = 0; i < config.vehicles_count; i++){
        do{
            isFitting = 1;
            rand_lane = rand() % config.lanes_count;
            rand_startPos = rand() % config.lane_length;

            for(int j = rand_startPos; j < rand_startPos + config.vehicle_length; j++)
            {
                int pos = j;

                if(pos >= config.lane_length)
                {
                    pos = pos - config.lane_length;
                }

                if(grid[rand_lane][pos] != -1)
                {
                    isFitting = 0;
                }
            }
        } while(grid[rand_lane][rand_startPos] != -1 || isFitting == 0);

        for(int j = rand_startPos; j < rand_startPos + config.vehicle_length; j++)
        {
            int pos = j;

            if(pos >= config.lane_length)
            {
                pos = pos - config.lane_length;
            }

            grid[rand_lane][pos] = i;
        }

        vehicle[i].lane = rand_lane;
        vehicle[i].position = rand_startPos;
    }

    //print_grid(config, grid);
    //return vehicle;
}

void moviment_vehicles(Config config, int** grid_old, int** grid_cur, Vehicle* vehicle){
    int new_speed;
    int new_position;
    int probability;
    int distance;

    #pragma omp parallel for
    for(int i = 0; i < config.vehicles_count; i++)
    {
        if(vehicle[i].position == -1 && vehicle[i].lane == -1){
            continue;
        }

        if(vehicle[i].time_stop > 0){
            vehicle[i].time_stop--;
            if(vehicle[i].time_stop == 0)
                vehicle[i].speed = 1;
            continue;
        }

        new_speed = vehicle[i].speed;
        probability = rand() % 100;

        //rule_1: new speed by acceleration
        new_speed = (new_speed + 1 < config.speed_max) ? new_speed + 1 : config.speed_max;

        //rule 2: new speed by probability
        if(probability < 9) //0 to 99, so 10%
            new_speed--;

        int distance_currentStation = distanceCurrentStation_vehicles(config, &(vehicle[i]));

        int distance_nextStation;
        int distance_stopStation;
        int distance_nextVehicle;
        int station_number;

        distance = new_speed;

        if(distance_currentStation == -1){ // não está dentro da estação
            // printf("[%d] fora da estação\n", i);
            distance_nextStation = distanceNextStation_vehicles(config, &(vehicle[i]));//distanceStopBeforeStation_vehicles(config, grid_old, &(vehicle[i]));
            // printf("[%d] distância até a próxima estação: %d\n", i, distance_nextStation);

            distance_nextVehicle = distanceNextVehicle_vehicles(config, grid_old, &(vehicle[i]));

            distance = distance_nextVehicle;
            // printf("[%d] distância até o próximo veículo: %d\n", i, distance);

            if(vehicle[i].lane != 0){ //não está na lane da estação
                if(distance_nextStation <= config.speed_max){ // está próximo da estação: 1 até speed_max
                    station_number = vehicle[i].position + config.vehicle_length + (distance_nextStation - 1);
                    // printf("[%d] a estação que está mais próxima: %d\n", i, station_number);

                    // printf("[%d] a estação está vazia: %s\n", i, isStationEmpty(config, grid_old, station_number) == 1 ? "sim" : "não");
                    if(isStationEmpty(config, grid_old, station_number) == 0){//estação não vazia
                        distance = distance_nextStation - 1; //não posso andar
                    }
                }
            }
            
            // printf("[%d] distância máxima a ser percorrida até a próximo estação: %d\n", i, distance);
        }
        else //está dentro da estação
        {
            // printf("[%d] distância na estação atual: %d\n", i, distance_currentStation);
            distance_stopStation = distanceStopStation_vehicles(config, grid_old, &(vehicle[i]));
            // printf("[%d] distância até ocupar a estação: %d\n", i, distance_stopStation);

            if(distance_stopStation != -1 && new_speed >= distance_stopStation){ //pode ocupar a estação completamente
                if(vehicle[i].type == PARADOR){
                    // printf("[%d] parador\n", i);
                    vehicle[i].time_stop = rand() % 2;
                    vehicle[i].time_stop += 4;
                    distance = distance_stopStation;
                }
                else{ //se for expresso
                    // printf("[%d] expresso\n", i);
                    station_number = vehicle[i].position + distance_stopStation;
                    // printf("[%d] a estação que está mais próxima: %d\n", i, station_number);

                    if(config.stations_pos[0] == station_number || config.stations_pos[config.stations_count - 1] == station_number){
                        vehicle[i].time_stop = rand() % 2;
                        vehicle[i].time_stop++;
                        distance = distance_stopStation;
                    }
                }
            }
            
            // else: não pode ocupar a estação completamente

            // printf("[%d] distância máxima a ser percorrida até parar na estação: %d\n", i, distance);
        }

        // printf("[%d] velocidade final: %d\n", i, new_speed);
        // printf("[%d] distância final: %d\n", i, distance);
        
        new_speed = (new_speed > distance) ? distance : new_speed;

        // printf("[%d] nova velocidade: %d\n\n", i, new_speed);

        //rule 4: new position by new speed
        new_position = vehicle[i].position + new_speed;

        if(new_position >= config.lane_length)
        {
            new_position = new_position - config.lane_length;
        }

        //update speed and position

        vehicle[i].speed = new_speed;
        vehicle[i].position = new_position;
    }
}

void changeLanes_vehicles(Config config, int** grid_old, Vehicle* vehicle){
    int distance_nextStation;
    int station_number;
    int need_break;
    
    #pragma omp parallel for
    for(int i = 0; i < config.vehicles_count; i++){
        
        need_break = 0;
        if(vehicle[i].position == -1)
            continue;

        if(vehicle[i].time_stop > 0)
            continue;

        if(vehicle[i].type == EXPRESSO && canChangeLane(config, grid_old, &(vehicle[i]))){
            // printf("[%d] change lane : expresso\n", i);
            distance_nextStation = distanceNextStation_vehicles(config, &(vehicle[i]));
            station_number = vehicle[i].position + config.vehicle_length + (distance_nextStation - 1);
            // printf("[%d] distância para próxima estação: %d\n", i, distance_nextStation);
            // printf("[%d] próxima estação: %d\n", i, station_number);

            if(station_number == config.stations_pos[0] || station_number == config.stations_pos[config.stations_count - 1]){ // precisa parar na estação
                // printf("[%d] precisa parar na estação\n", i);
                if(vehicle[i].lane == 1){
                    vehicle[i].lane = 0;
                }
                //else: lane 0 não troca, porque vai ter que parar
            }
            else{ //não preciso parar na estação

                // printf("[%d] não precisa parar na estação\n", i);
                int distance_vehicleCurrentStation = distanceCurrentStation_vehicles(config, &(vehicle[i]));
                // printf("[%d] distância dentro da estação: %d\n", i, distance_vehicleCurrentStation);

                if(distance_vehicleCurrentStation > -1)
                    need_break = 1;
                
                if(need_break)
                    continue;

                int distance_vehicleSameLane = distanceVehicleLane_vehicles(config, grid_old, &(vehicle[i]));
                int distance_vehicleOtherLane = distanceVehicleOtherLane_vehicles(config, grid_old, &(vehicle[i]));

                // printf("[%d] distância para veículo na mesma faixa: %d\n", i, distance_vehicleSameLane);
                // printf("[%d] distância para veículo na outra faixa: %d\n", i, distance_vehicleOtherLane);

                if(distance_vehicleOtherLane > distance_vehicleSameLane){//distance_vehicleSameLane < vehicle[i].speed && 
                    if(vehicle[i].lane == 1){
                        // printf("[%d] estação vazia: %s\n", i, isStationEmpty(config, grid_old, station_number) ? "sim" : "não");
                        if(isStationEmpty(config, grid_old, station_number)){
                            vehicle[i].lane = 0;
                            // printf("[%d] estação vazia\n", i);
                        }
                        else{ //estação não vazia
                            vehicle[i].lane = 1;
                            // printf("[%d] estação vazia\n", i);
                        }
                    }
                    else //lane == 0
                        vehicle[i].lane = 1;
                }
            }
        }
    }   
}
