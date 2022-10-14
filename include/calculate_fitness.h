//
// Created by ning on 22-10-8.
//

#ifndef JOBSHOP_CALCULATE_FITNESS_H
#define JOBSHOP_CALCULATE_FITNESS_H
#include "vector"
#include "iostream"

namespace job_shop{
    class calculate_fitness {
    public:
        double fitness(std::vector<int> &result_out_,std::vector<int> car_type_,int time_cost,int back_road_used_time_);

    };



}


#endif //JOBSHOP_CALCULATE_FITNESS_H
