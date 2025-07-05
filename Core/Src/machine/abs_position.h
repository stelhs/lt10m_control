/*
 * abs_position.h
 *
 *  Created on: Jun 17, 2025
 *      Author: stelhs
 */

#ifndef SRC_MACHINE_ABS_POSITION_H_
#define SRC_MACHINE_ABS_POSITION_H_

#include "spi_dev.h"

#define LINEAR_CROSS_RESOLUTION 5
#define LINEAR_LONGITUDAL_RESOLUTION 5

struct xy {
    int longitudal;
    int cross;
};

struct abs_position {
    struct spi_dev *dev;
    bool is_longitudal_inc_left;
    bool is_cross_inc_down;
    struct xy raw;
    struct xy offset_tools[4];
    int curr_tool_num;
    int longitudal_speed;
    int cross_speed;
};
struct abs_position *
abs_position_dev_register(char *name, SPI_HandleTypeDef *hspi,
                          struct gpio *cs);
void abs_position_update(struct abs_position *ap);
int abs_longitudal(struct abs_position *ap, int tool_num);
int abs_longitudal_curr_tool(struct abs_position *ap);
int abs_cross(struct abs_position *ap, int tool_num);
int abs_cross_curr_tool(struct abs_position *ap);
void abs_cross_set(struct abs_position *ap, int tool_num, int val);
void abs_longitudal_set(struct abs_position *ap, int tool_num, int val);
void abs_pos_set_tool(struct abs_position *ap, int tool_num);
int abs_pos_tool(struct abs_position *ap);
bool is_longitudal_target_position_left(struct abs_position *ap,
                                     int curr_position, int target_position);
bool is_cross_target_position_down(struct abs_position *ap,
                                   int curr_position, int target_position);
#endif /* SRC_MACHINE_ABS_POSITION_H_ */
