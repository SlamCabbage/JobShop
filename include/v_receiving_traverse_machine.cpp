//
// Created by ning on 22-10-7.
//

#include "v_receiving_traverse_machine.h"

namespace job_shop {
// 从涂装-PBS进车道取车任务id
enum receiving_task_id {
  pick_up_from_0_to_1, // 从涂装-PBS进车道取车送至1车道
  pick_up_from_0_to_2, // 从涂装-PBS进车道取车送至2车道
  pick_up_from_0_to_3, // 从涂装-PBS进车道取车送至3车道
  pick_up_from_0_to_4, // 从涂装-PBS进车道取车送至4车道
  pick_up_from_0_to_5, // 从涂装-PBS进车道取车送至5车道
  pick_up_from_0_to_6, // 从涂装-PBS进车道取车送至6车道

  pick_up_from_back_lane_to_1, // 从返回车道取车送至1车道
  pick_up_from_back_lane_to_2, // 从返回车道取车送至2车道
  pick_up_from_back_lane_to_3, // 从返回车道取车送至3车道
  pick_up_from_back_lane_to_4, // 从返回车道取车送至4车道
  pick_up_from_back_lane_to_5, // 从返回车道取车送至5车道
  pick_up_from_back_lane_to_6, // 从返回车道取车送至6车道
  no_task,
};

// 任务消耗时间
int receiving_task_id_time[] = {18, 12, 6, 0, 12, 18, 24, 18, 12, 6, 12, 18, 0};

bool job_shop::v_receiving_traverse_machine::judge_receiver_task_phase(
    Lane &lane_in) {
  this->receiving_t_now++;

  // 当前任务为前6个，取涂装-PBS进车口的车放入某车道
  if (this->receiving_task_id_now <= 5) {

    // 到达该放置车身的车道
    if (this->receiving_t_now ==
        receiving_task_id_time[this->receiving_task_id_now] / 2) {

      // TODO 执行放车动作，更新车道信息
      this->carry_car_id = -1;

      std::cout << "到达" << this->receiving_task_id_now + 1 << "车道"
                << std::endl;
    } else if (
        this->receiving_t_now ==
        receiving_task_id_time
            [this->receiving_task_id_now]) { // 接车横移机回到4车道（起点）----空闲
      std::cout << "返回" << 4 << "车道" << std::endl;

      this->receiving_t_now = 0;
      this->receiving_t_task = 0;
      this->receiving_task_id_now = no_task;

      // TODO 根据车道信息，决定是否处理返回道
    } else {
      std::cout << "正前往车道放车或者正返回起点途中。" << std::endl;
    }
  }

  // 当前任务为7-12个，取返回道的车的车放入某车道
  else if (this->receiving_task_id_now > 5 &&
           this->receiving_task_id_now <= 11) {

    // 接车横移机到达返回道
    if (this->receiving_t_now == 3) {

      std::cout << "到达返回车道" << std::endl;

      // TODO 执行取返回道车身动作，更新车道信息
    } else if (this->receiving_t_now ==
               receiving_task_id_time[this->receiving_task_id_now] /
                   2) { // 接车横移机到达要放置车身车道
      std::cout << "到达" << this->receiving_task_id_now + 1 << "车道"
                << std::endl;
      // TODO 放置车身，更新车道信息

    } else if (
        this->receiving_t_now ==
        receiving_task_id_time
            [this->receiving_task_id_now]) { // 接车横移机返回四车道（起点）

      std::cout << "到达" << 4 << "车道" << std::endl;

      this->receiving_t_now = 0;
      this->receiving_t_task = 0;
      this->receiving_task_id_now = no_task;
      // TODO 根据车道信息，决定是否处理返回道
    }

  } else if (this->receiving_task_id_now == 12) {
    this->receiving_t_now = 0;

    // TODO 根据车道信息分配任务
  }
}

// 生成一个范围在【a,b）内的随机整数，注意：：需要在main函数中写随机数种子：：srand((int)time(0));
int job_shop::v_receiving_traverse_machine::get_rand_number(int a, int b) {

  double x = (double)rand() / RAND_MAX;
  double result = x * (b - a) + a;
  double r = floor(abs(result));

  return r;
}
} // namespace job_shop