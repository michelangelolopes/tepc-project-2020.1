#include <time.h>
//#include "lib/basic.h"
#include "lib/motion.h"

int main(int argc, char** argv){
    srand(time(0));
    Config config = load_configs(argv);
    config.stations_pos = set_stations(config);

    // print_configs(config);
//
    int** grid_old = NULL;
    int** grid_cur = create_grid(config);
    int speed_matrix[config.time_steps][config.vehicles_count];
//
    Vehicle* vehicle = (Vehicle*) malloc(sizeof(Vehicle) * config.vehicles_count);
    fill_vehicles(config, vehicle);
    //randomStart_vehicles(config, grid_cur, vehicle);
//
    //print_vehicles(config, vehicle);
    //print_grid(config, grid_cur);
    int vehicle_count = 0;
    
    for(int i = 0; i < config.time_steps; i++)//config.time_steps; i++)
    {
        if(vehicle_count + 1 - config.vehicles_count <= 0)
        {
            vehicle_count += queueStart_vehicles(config, grid_old, grid_cur, &(vehicle[vehicle_count]), vehicle_count);
        }

        if(grid_old == NULL){
            grid_old = grid_cur;
            grid_cur = create_grid(config);
        }

        changeLanes_vehicles(config, grid_old, vehicle);
        #pragma omp barrier
        updateVehicles_grid(config, grid_cur, vehicle);

        // print_vehicles(config, vehicle);
        // print_grid(config, grid_cur);

        free_grid(config, grid_old);
        grid_old = grid_cur;
        grid_cur = create_grid(config);

        //printf("TESTE1\n");
        //printf("TESTE2\n");
        //printf("TESTE3\n");
        moviment_vehicles(config, grid_old, grid_cur, vehicle);
        #pragma omp barrier
        updateVehicles_grid(config, grid_cur, vehicle);

        //print_grid(config, grid_old);

        // print_vehicles(config, vehicle);
        // print_grid(config, grid_cur);

        free_grid(config, grid_old);
        grid_old = grid_cur;
        grid_cur = create_grid(config);

        for(int j = 0; j < config.vehicles_count; j++)
            speed_matrix[i][j] = vehicle[j].speed;
    }

    double mean;
    for(int i = 0; i < config.time_steps; i++)
    {
        mean = 0;
        // printf("passo %d\n", i);
        for(int j = 0; j < config.vehicles_count; j++)
        {
            mean += speed_matrix[i][j];
            //printf("[%d] speed = %d\n", j, speed_matrix[i][j]);
        }

        mean /= (double) config.vehicles_count;
        printf("%d, %.2lf\n", i + 1, mean);
        // printf("\n");
    }
    free_vehicles(vehicle);
    free_grid(config, grid_cur);
    free_grid(config, grid_old);

    return 0;
}
