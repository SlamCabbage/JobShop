//
// Created by ning on 22-10-7.
//

#pragma once

#include "Eigen/Dense"
#include "Lane.h"
#include "iostream"
#include <cstdlib>
#include <ctime>
#include <iomanip>

namespace job_shop {

class v_receiving_traverse_machine {
public:
  // 当前正在执行的任务id,默认无任务，
  int receiving_task_id_now = 12;
  // 已经执行此任务的时间，默认为0
  int receiving_t_now = -1;
  // 当前正携带车辆id（在车辆队列中的位置）,默认-1表示没有携带车
  int carry_car_id = -1;
  // 记录送车半程加上等待的时间
  int time_add_wait = -1;

public:
  // 判断当前时刻处于什么任务阶段
  bool judge_receiver_task_phase(Lane &lane_in,std::vector<int> &car_type_in_,std::queue<int> &q_in, int t);
  // 分配任务
  int assign_task(Lane &lane_in, std::vector<int> &car_type_);
  int assign_task(std::queue<int> &q_in, std::vector<int> &car_type_,
                  Lane &lane_in);
  // 生成一个范围在【a,b）内的整数，包含a
  int get_rand_number(int a, int b);
};

} // namespace job_shop
