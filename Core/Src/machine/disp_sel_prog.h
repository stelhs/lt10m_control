/*
 * disp_sel_prog.h
 *
 *  Created on: Jun 9, 2025
 *      Author: stelhs
 */

#ifndef DISP_SEL_PROG_H_
#define DISP_SEL_PROG_H_

enum progs {
    PROG_FEED_LEFT,
    PROG_FEED_RIGHT,
    PROG_FEED_LEFT_UP,
    PROG_FEED_RIGHT_UP,
    PROG_FEED_LEFT_DOWN,
    PROG_FEED_RIGHT_DOWN,
    PROG_FEED_UP_LEFT,
    PROG_FEED_UP_RIGHT,
    PROG_FEED_DOWN_LEFT,
    PROG_FEED_DOWN_RIGHT,
    PROG_FEED_UP,
    PROG_FEED_DOWN,
    PROG_THREAD_LEFT,
    PROG_THREAD_RIGHT
};

int disp_sel_prog_run(void);
struct img *img_prog_by_num(enum progs prog);

#endif /* DISP_SEL_PROG_H_ */
