#ifndef SRC_IMAGES_H_
#define SRC_IMAGES_H_

#include "disp_mipi_dcs.h"

struct img *img_feed_left_down(void);
struct img *img_feed_left(void);
struct img *img_feed_right_up(void);
struct img *img_feed_up(void);
struct img *img_feed_down_right(void);
struct img *img_feed_left_up(void);
struct img *img_feed_down(void);
struct img *img_feed_right_down(void);
struct img *img_feed_up_left(void);
struct img *img_feed_up_right(void);
struct img *img_feed_down_left(void);
struct img *img_thread_right(void);
struct img *img_feed_right(void);
struct img *img_thread_left(void);

#endif /* SRC_IMAGES_H_ */
