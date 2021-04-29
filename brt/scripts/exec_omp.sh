#!/bin/bash


#time ./brt lane_length lanes_count vehicle_length vehicles_count time_steps speed_max stations_count
#               |            |             |               |          |           |           7|-----> quantidade de estações
#               |            |             |               |          |          6|------------------> velocidade máxima em células por segundos
#               |            |             |               |         5|------------------------------> duração em segundos da simulação
#               |            |             |              4|-----------------------------------------> quantidade de veículos na simulação
#               |            |            3|---------------------------------------------------------> comprimento dos veículos em células
#               |           2|-----------------------------------------------------------------------> quantidade de pistas na simulação
#              1|------------------------------------------------------------------------------------> comprimento das pistas em células

time  bin/brt_omp 500 2 10 10 10800 5 10