/*
 * touch_xpt2046.h
 *
 *  Created on: May 24, 2025
 *      Author: stelhs
 */

#ifndef SRC_MACHINE_TOUCH_XPT2046_H_
#define SRC_MACHINE_TOUCH_XPT2046_H_

#include "spi_dev.h"

struct touch_xpt2046 {
    struct spi_dev *spi;
};

#endif /* SRC_MACHINE_TOUCH_XPT2046_H_ */
