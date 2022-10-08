//
// Created by ning on 22-10-7.
//

#include "v_delivery_traverse_machine.h"

namespace job_shop { // 从涂装-PBS进车道取车任务id
enum delivery_task_id {
  pick_up_from_1_to_0, // 从1车道取车送至PBS-总装送车道0
  pick_up_from_2_to_0, // 从2车道取车送至PBS-总装送车道1
  pick_up_from_3_to_0, // 从3车道取车送至PBS-总装送车道2
  pick_up_from_4_to_0, // 从4车道取车送至PBS-总装送车道3
  pick_up_from_5_to_0, // 从5车道取车送至PBS-总装送车道4
  pick_up_from_6_to_0, // 从6车道取车送至PBS-总装送车道5

  pick_up_from_1_to_back_lane, // 从返回车道取车送至1车道6
  pick_up_from_2_to_back_lane, // 从返回车道取车送至2车道7
  pick_up_from_3_to_back_lane, // 从返回车道取车送至3车道8
  pick_up_from_4_to_back_lane, // 从返回车道取车送至4车道9
  pick_up_from_5_to_back_lane, // 从返回车道取车送至5车道10
  pick_up_from_6_to_back_lane, // 从返回车道取车送至6车道11
  no_task_,                    // 12
};

// 任务消耗时间
int delivery_task_id_time[] = {18, 12, 6, 0, 12, 18, 24, 18, 12, 6, 12, 18, 0};

// 判断任务状态,有任务返回1，没有任务返回0
bool v_delivery_traverse_machine::judge_delivery_task_phase(
    Lane &lane_in, std::vector<int> &car_type_in_) { // 车道信息lane_in,车身信息
  this->delivery_t_now++;

  // 当前任务为前6个，取某车道车身放入PBS-总装出车口
  if (this->delivery_task_id_now <= 5) {

    // 到达取车身的车道
    if (this->delivery_t_now ==
        delivery_task_id_time[this->delivery_task_id_now] / 2) {
      // 执行取车动作，更新车道信息，更新delivery_carry_car_id
      auto temp = lane_in.rightmost_car.front();
      this->delivery_carry_car_id = temp.first;
      lane_in.DeleteCar(temp.second.first, temp.second.second);
      std::cout << "到达" << this->delivery_task_id_now + 1 << "车道"
                << std::endl;
    } else if (
        this->delivery_t_now ==
        delivery_task_id_time
            [this->delivery_task_id_now]) { // 送车横移机回到4车道（起点）----空闲
      std::cout << "返回" << 4 << "车道" << std::endl;
      this->updata_result_subsection(delivery_carry_car_id, car_type_in_);
      this->delivered_car_index.push_back(delivery_carry_car_id);
      this->delivery_t_now = 0;
      this->delivery_t_task = 0;
      this->delivery_task_id_now = no_task_;
      this->delivery_carry_car_id = -1;
      return false;
    } else {
      std::cout << "正前往车道放车或者正返回起点途中。" << std::endl;
    }
  } else if (this->delivery_task_id_now > 5 &&
             this->delivery_task_id_now <=
                 9) {
    // 当前任务为7-10个，取某车道的车的车放入返回道
    // 送车横移机到达该取车车道
    if (this->delivery_t_now ==
        (delivery_task_id_time[delivery_task_id_now] - 6) / 2) {
      std::cout << "到达" << delivery_task_id_now - 5 << "车道" << std::endl;
      // 执行取返回道车身动作，更新车道信息，更新delivery_carry_car_id
      auto temp = lane_in.rightmost_car.front();
      this->delivery_carry_car_id = temp.first;
      lane_in.DeleteCar(temp.second.first, temp.second.second);
    } else if (this->delivery_t_now ==
               delivery_task_id_time[this->delivery_task_id_now] -
                   3) { // 送车横移机到达返回道
      std::cout << "到达返回车道" << std::endl;
      // 放置车身，更新车道信息
      lane_in.ReverseCar(this->delivery_carry_car_id);
      this->delivery_carry_car_id = -1;
    } else if (
        this->delivery_t_now ==
        delivery_task_id_time
            [this->delivery_task_id_now]) { // 送车横移机返回四车道（起点）
      std::cout << "到达" << 4 << "车道" << std::endl;
      this->delivery_t_now = 0;
      this->delivery_t_task = 0;
      this->delivery_task_id_now = no_task_;
      this->delivery_carry_car_id = -1;
      return false;

    } else {
      std::cout << "正前往车道放车或者正返回起点途中。" << std::endl;
    }
  } else if (this->delivery_task_id_now == 10 ||
             this->delivery_task_id_now == 11) {

    if (this->delivery_t_now == delivery_task_id_time[delivery_task_id_now] /
                                    2) { // 送车横移机到达5或者6车道,取车
      // 执行取车动作，更新车道信息,更新delivery_carry_car_id
      auto temp = lane_in.rightmost_car.front();
      this->delivery_carry_car_id = temp.first;

      lane_in.DeleteCar(temp.second.first, temp.second.second);

    } else if (this->delivery_t_now ==
               delivery_task_id_time[delivery_task_id_now] -
                   3) { // 送车横移机到达返回车道，放车

      // 执行放车动作，更新车道信息
      lane_in.ReverseCar(this->delivery_carry_car_id);
      this->delivery_carry_car_id = -1;

    } else if (
        this->delivery_t_now ==
        delivery_task_id_time
            [delivery_task_id_now]) { // 送车横移机回到4车道（起点）----空闲
      this->delivery_t_now = 0;
      this->delivery_t_task = 0;
      this->delivery_carry_car_id = -1;
      this->delivery_task_id_now = no_task_;
      return false;
    } else {
      std::cout << "正前往车道放车或者正返回起点途中。/" << std::endl;
    }
  } else { // 送车横移机空闲

    return false;
  }
  return true;
}

void v_delivery_traverse_machine::updata_result_subsection(
    int car_id_, std::vector<int> &car_type_) {
  // 混动二驱--1，混动四驱--2，非混动二驱--3，非混动四驱--4
  int car_type_in = car_type_[car_id_];
  // 4驱--1，2驱--0
  last_new_drive_mode.push_back(car_type_in % 2 ? 0 : 1);
  std::cout << "驱动类型：" << (car_type_in % 2 ? 0 : 1) << "  ";
  // 混动--1，非混--0
  last_new_hybrid.push_back((car_type_in - 1) / 2 ? 0 : 1);
  std::cout << "动力类型：" << ((car_type_in - 1) / 2 ? 0 : 1) << std::endl;

  // 更新驱动型号队列
  bool isturned = false;
  for (int i = 0; i < last_new_drive_mode.size() - 1; ++i) {
    if (last_new_drive_mode[i] != last_new_drive_mode[0]) {
      isturned = true;
      break;
    }
  }
  if (isturned) {
    if (last_new_drive_mode.back() == last_new_drive_mode.front()) {
      last_new_drive_mode.clear();
      last_new_drive_mode.push_back(car_type_in % 2 ? 0 : 1);
    }
  }

  // 更新混动与非混动
  // 上一时刻为空，初始化时,且当前加入了一个非混
  if (last_new_hybrid.size() == 1) {
    if (!((car_type_in - 1) / 2 ? 0 : 1)) {
      last_new_hybrid.clear();
    }
  }
  // 如果加入一个混动车
  if (((car_type_in - 1) / 2 ? 0 : 1)) {
    last_new_hybrid.clear();
    last_new_hybrid.push_back((car_type_in - 1) / 2 ? 0 : 1);
  }

  // 打印混动--1，非混--0
  for (int i = 0; i < last_new_hybrid.size(); ++i) {
    std::cout << last_new_hybrid[i] << " ";
  }
  std::cout << std::endl;
  // 打印4驱--1、2驱--0，
  for (int i = 0; i < last_new_drive_mode.size(); ++i) {
    std::cout << last_new_drive_mode[i] << " ";
  }
  std::cout << std::endl;
}

int job_shop::v_delivery_traverse_machine::assign_task(
    std::queue<std::pair<int, std::pair<int, int>>> &q_in,
    std::vector<int> &car_type_) {

  auto temp = q_in.front();
  // 车身索引
  int car_index = temp.first;
  auto location = temp.second;

  // 最先到达1车位车身所在车道
  int line_id = location.first;

  int car_type_in = car_type_
      [car_index]; // 混动二驱--1，混动四驱--2，非混动二驱--3，非混动四驱--4

  if (!last_new_drive_mode.empty()) {
    float score = 0;

    bool ishybrid = (car_type_in - 1) / 2 ? 0 : 1; // 混动--1，非混--0
    bool isFourwheel = car_type_in % 2 ? 0 : 1;    // 4驱--1，2驱--0

    // 判断驱动类型是否符合预期，并计算得分
    bool isone2one =
        last_new_drive_mode.size() % 2; // 1表示奇数不是1：1，0表示偶数，是1：1
    int x = 0;
    for (int i = 0; i < last_new_drive_mode.size(); ++i) {
      x += last_new_drive_mode[i];
    }
    if (x == last_new_drive_mode.size() / 2) {
      isone2one = true;
    }
    // 如果是1：1
    if (isone2one) {
      if (last_new_drive_mode.back() == isFourwheel) {
        score = score - 0.3;
      }
    }

    // 判断驱动方式是否符合预期，混动--1，非混--0
    if (!last_new_hybrid.empty()) {
      int unHybridNum = last_new_hybrid.size() - 1;
      if (unHybridNum <= 1) {
        if (ishybrid) {
          score = score - 0.4;
        }
      }
      if (unHybridNum == 2) {
        if (!ishybrid) {
          score = score - 0.4;
        }
      }
    }

    std::cout << "得分: " << score << std::endl;

    double xx = (double)rand() / RAND_MAX;
    double result = xx * (101 - 1) + 1;
    int r = floor(abs(result));

    std::cout << "给定随机数：" << r << std::endl;

    if (score == float(-0.7)) {
      if (r <= 90) {
        delivery_task_id_now = line_id + 5;
        std::cout << "分配任务：" << delivery_task_id_now << std::endl;
        back_road_used_time++;
        return delivery_task_id_now;
      } else {
        delivery_task_id_now = line_id - 1;
        std::cout << "分配任务：" << delivery_task_id_now << std::endl;
        return delivery_task_id_now;
      }
    } else if (score == float(-0.4)) {
      if (r <= 70) {
        delivery_task_id_now = line_id + 5;
        std::cout << "分配任务：" << delivery_task_id_now << std::endl;
        back_road_used_time++;
        return delivery_task_id_now;
      } else {
        delivery_task_id_now = line_id - 1;
        std::cout << "分配任务：" << delivery_task_id_now << std::endl;
        return delivery_task_id_now;
      }
    } else if (score == float(-0.3)) {
      if (r <= 50) {
        delivery_task_id_now = line_id + 5;
        std::cout << "分配任务：" << delivery_task_id_now << std::endl;
        back_road_used_time++;
        return delivery_task_id_now;
      } else {
        delivery_task_id_now = line_id - 1;
        std::cout << "分配任务：" << delivery_task_id_now << std::endl;
        return delivery_task_id_now;
      }
    } else {
      delivery_task_id_now = line_id - 1;
      std::cout << "分配任务：" << delivery_task_id_now << std::endl;
      return delivery_task_id_now;
    }
  } else {
    this->delivery_task_id_now = line_id - 1;
    std::cout << "分配任务：" << delivery_task_id_now << std::endl;
    return delivery_task_id_now;
  }
  return 0;
}
} // namespace job_shop