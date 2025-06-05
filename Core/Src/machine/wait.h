/*
 * wait.h
 */

#ifndef WAIT_H_
#define WAIT_H_

#define EV_BTN_UP_PRESSED (1 << 0)
#define EV_BTN_UP_HOLD (1 << 1)
#define EV_BTN_UP_RELEASE (1 << 2)

#define EV_BTN_DOWN_PRESSED (1 << 3)
#define EV_BTN_DOWN_HOLD (1 << 4)
#define EV_BTN_DOWN_RELEASE (1 << 5)

#define EV_BTN_LEFT_PRESSED  (1 << 6)
#define EV_BTN_LEFT_HOLD  (1 << 7)
#define EV_BTN_LEFT_RELEASE  (1 << 8)

#define EV_BTN_RIGHT_PRESSED (1 << 9)
#define EV_BTN_RIGHT_HOLD (1 << 10)
#define EV_BTN_RIGHT_RELEASE (1 << 11)

#define EV_BTN_ENC (1 << 12)

int wait(u32 flags, uint timeout_ms);
void buttons_reset(void);

#endif /* WAIT_H_ */
