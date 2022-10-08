//
// Created by ning on 22-10-7.
//

#pragma once

#include "iostream"
#include "Eigen/Dense"
#include "Lane.h"
#include<stdlib.h>
#include<time.h>
#include<iomanip>

namespace job_shop {

    class v_receiving_traverse_machine {
    public:

        //当前正在执行的任务id,默认无任务，
        int receiving_task_id_now = 12;
        //已经执行此任务的时间，默认为0
        int receiving_t_now = 0;
        //执行当前任务所需要的时间，默认为0
        int receiving_t_task = 0;
        //当前正携带车辆id（在车辆队列中的位置）,默认-1表示没有携带车
        int carry_car_id = -1;

    public:

        //判断当前时刻处于什么任务阶段，并分配新的任务
        bool judge_receiver_task_phase();

        //生成一个范围在【a,b）内的整数，包含a
        int get_rand_number(int a,int b);

    };


}

