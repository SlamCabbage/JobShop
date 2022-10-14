//
// Created by ning on 22-10-7.
//

#ifndef PBS_V_DELIVERY_TRAVERSE_MACHINE_H
#define PBS_V_DELIVERY_TRAVERSE_MACHINE_H

#include "iostream"
#include "Eigen/Dense"
#include "Lane.h"
#include "vector"

namespace job_shop {

    class v_delivery_traverse_machine {
    public:

        //当前正在执行的任务id,默认无任务，
        int delivery_task_id_now = 12;

        //已经执行此任务的时间，默认为0
        int delivery_t_now = -1;

        //当前正在携带车的index,默认为-1，表示没有车
        int delivery_carry_car_id = -1;

        int last_delivered = -1;

        //已经送走的车的队列
        std::vector<int> delivered_car_index;

        //按照四驱、二驱型号分段计算分数的队列中最新的一段 4驱--1，2驱--0
        std::vector<bool> last_new_drive_mode;

        //按照混动、非混划分的最新的一段，混--1，非混--0
        std::vector<bool> last_new_hybrid;

        //返回道使用次数
        int back_road_used_time = 0;

    public:

        //判断当前时刻处于什么任务阶段，并分配新的任务
        bool judge_delivery_task_phase(Lane &lane_in, std::vector<int> &car_type_in_,bool &isbackupdat,int t_now);

        //更新队列分段，包括混动分段、驱动型号分段，
        void updata_result_subsection(int car_id_,
                                      std::vector<int> &car_type_);


        //分配任务
        int assign_task(std::queue<std::pair<int, std::pair<int, int>>> &q_in, //队列q
                        std::vector<int> &car_type_,Lane &lane_in);//输入的车身类型：混动二驱--1，混动四驱--2，非混动二驱--3，非混动四驱--4
    };


}


#endif //PBS_V_DELIVERY_TRAVERSE_MACHINE_H
