//
// Created by ning on 22-10-7.
//

#pragma once

#include "Eigen/Dense"
#include "Lane.h"
#include "iostream"
#include "vector"

namespace job_shop {

class v_delivery_traverse_machine {
public:
  // 当前正在执行的任务id,默认无任务，
  int delivery_task_id_now = 12;

  // 已经执行此任务的时间，默认为0
  int delivery_t_now = 0;

  // 执行当前任务所需要的时间，默认为0
  int delivery_t_task = 0;

  // 当前正在携带车的index,默认为-1，表示没有车
  int delivery_carry_car_id = -1;

  int last_delivered = -1;

  // 已经送走的车的队列
  std::vector<int> delivered_car_index;

  // 最新驱动型号
  std::vector<bool> last_new_drive_mode;

public:
  // 判断当前时刻处于什么任务阶段，并分配新的任务
  bool judge_delivery_task_phase(Lane &lane_in); // Lane &lane_in

  // 分配任务
  void assign_task(
      std::queue<std::pair<int, std::pair<int, int>>> &q_in, // 队列q
      std::vector<int> &
          car_type_); // 输入的车身类型：混动二驱--1，混动四驱--2，非混动二驱--3，非混动四驱--4

  // 给已经输出的队列分段，包括混动分段、驱动型号分段，返回两vector
  void result_subsection(std::vector<int> &car_type);
};

} // namespace job_shop
