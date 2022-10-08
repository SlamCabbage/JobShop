//
// Created by 廖礼洲 on 2022/10/7.
//

#include "Lane.h"

namespace job_shop {
    // 更新一秒后的状态
    void Lane::UpdateLaneTime(bool revsere) {
        // 正向车道
        if (!revsere) {
            for (int i = 0; i < 6; ++i) {
                for (int j = 1; j <= 9; ++j) {
                    if (lane_time(i, j) != -1) {
                        if (lane_time(i, j) == 8 && lane_time(i, j - 1) == -1) {
                            // 时间
                            lane_time(i, j) = -1;
                            lane_time(i, j - 1) = 0;
                            // 车辆
                            lane_car(i, j - 1) = lane_car(i, j);
                            lane_car(i, j) = 0;
                            if (j - 1 == 0) {
                                // 如果新到的为1号车位，那么将其作为阻塞
                                lane_time(i, j - 1) = -2;
                                rightmost_car.push({lane_car(i, j - 1), {i, j - 1}});
                            }
                            if (j == 9) {
                                lane_occupancy(i) = 0;
                            }
                        } else if (lane_time(i, j) == -2 && lane_time(i, j - 1) == -1) {
                            // 占用解除了
                            lane_time(i, j) = 0;
                        } else if (lane_time(i, j) < 8 && lane_time(i, j) >= 0
                                   && lane_time(i, j - 1) < 8 && lane_time(i, j - 1) >= 0) {
                            // 前方有一个车，但是前方的车并未阻塞
                            lane_time(i, j)++;
                        } else if (lane_time(i, j) < 8 && lane_time(i, j) >= 0 && lane_time(i, j - 1) == -1) {
                            // 前方无车，当前时刻还不能跳格
                            lane_time(i, j)++;
                        } else if (lane_time(i, j) < 8 && lane_time(i, j) >= 0 && lane_time(i, j - 1) == -2) {
                            // 前方有车，前方阻塞
                            lane_time(i, j) = -2;
                        }
                    }
                }
            }
        } else {
            for (int j = 8; j >= 0; --j) {
                if (lane_time(6, j) != -1) {
                    if (lane_time(6, j) == 8 && lane_time(6, j + 1) == -1) {
                        // 时间
                        lane_time(6, j) = -1;
                        lane_time(6, j + 1) = 0;
                        // 车辆
                        lane_car(6, j + 1) = lane_car(6, j);
                        lane_car(6, j) = 0;
                        if (j + 1 == 9) {
                            lane_time(6, j + 1) = -2;
                            leftmost_car.push({lane_car(6, j + 1), {6, j + 1}});
                        }
                        if (j == 0) {
                            lane_occupancy(6) = 0;
                        }
                    } else if (lane_time(6, j) == -2 && lane_time(6, j + 1) == -1) {
                        // 占用解除
                        lane_time(6, j) = 0;
                    } else if (lane_time(6, j) < 8 && lane_time(6, j) >= 0 && lane_time(6, j + 1) == -2) {
                        lane_time(6, j) = -2;
                    } else if (lane_time(6, j) < 8 && lane_time(6, j) >= 0 && lane_time(6, j + 1) == -1) {
                        lane_time(6, j)++;
                    } else if (lane_time(6, j) < 8 && lane_time(6, j) >= 0 &&
                               lane_time(6, j + 1) < 8 && lane_time(6, j + 1) >= 0) {
                        lane_time(6, j)++;
                    }
                }
            }
        }

        // 反向车道
    }
    // 添加新的车身
    void Lane::AddCar(int index, int car_id) {
        if (lane_occupancy(index) != 0) {
            std::cerr << "占用的车道不能传入车辆" << std::endl;
            return;
        }
        lane_time(index, 9) = 0;
        lane_occupancy(index) = 1;
        lane_car(index, 9) = car_id;
    }
    // 删除车身
    void Lane::DeleteCar(int i, int j) {
        if (lane_car(i, j) == 0) {
            std::cerr << "该车道并没有车辆" << std::endl;
            return;
        } else if (j < 9) {
            std::cerr << "车道只能是第十个车道" << std::endl;
            return;
        }
        lane_car(i, j) = 0;
        lane_time(i, j) = -1;
    }
    // deliver翻转车身
    void Lane::ReverseCar(int car_type) {
        if (lane_occupancy(6) != 0) {
            std::cerr << "反向车道已经有车辆" << std::endl;
            return;
        }
        lane_time(6, 0) = 0;
        lane_occupancy(6) = 1;
        lane_car(6, 0) = car_type;
    }
    // recevier重新添加翻转车身
    void Lane::AddReverseCar() {
        if (lane_car(6, 9) == 0) {
            std::cerr << "反向车道并没有车辆" << std::endl;
            return;
        }
        lane_car(6, 9) = 0;
        lane_time(6, 9) = -1;
    }
}// namespace job_shop