//
// Created by ning on 22-10-7.
//

#ifndef PBS_V_DELIVERY_TRAVERSE_MACHINE_H
#define PBS_V_DELIVERY_TRAVERSE_MACHINE_H
#include "iostream"

namespace job_shop {

    class v_delivery_traverse_machine {
    public:

        //当前正在执行的任务id,默认无任务，
        int delivery_task_id_now = 12;
        //已经执行此任务的时间，默认为0
        int delivery_t_now = 0;
        //执行当前任务所需要的时间，默认为0
        int delivery_t_task = 0;

    public:

        //判断当前时刻处于什么任务阶段，并分配新的任务
        void judge_delivery_task_phase();

    };


}


#endif //PBS_V_DELIVERY_TRAVERSE_MACHINE_H
