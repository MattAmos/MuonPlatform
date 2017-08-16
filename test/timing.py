import matplotlib.pyplot as plt
import curses
import numpy as np

stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.keypad(1)

min_t = 553
max_t = 2450
mid_t = (max_t + min_t) / 2.0

time = mid_t

y = np.arange(0., 20000., 1.)
y[:] = 0
y[:time] = 1

plt.ion()
plt.plot(y)
plt.title("Interactive Servo Timing Chart")
plt.xlabel("Time (us)")
plt.axis([0, 20000.0, -0.1, 1.1])
plt.draw()

while True:
    c = stdscr.getch()
    if c == ord('a') or c == curses.KEY_LEFT:
        time -= 100
    elif c == ord('d') or c == curses.KEY_RIGHT:
        time += 100
    elif c == 27:
        break
    else:
        hangCycles += 1

    if time < min_t:
        time = min_t
    if time > max_t:
        time = max_t

    y[:] = 0
    y[:time] = 1

    plt.plot(y)
    plt.draw()
    plt.clf()
    plt.title("Interactive Servo Timing Chart")
    plt.xlabel("Time (us)")
    plt.axis([0, 20000.0, -0.1, 1.1])

    stdscr.addstr(0, 0, "          ")
    stdscr.addstr(0, 0, str(time))
    stdscr.addstr(1, 0, "-----------------------")
    stdscr.addstr(2, 0, "a : Decrease Duty Cycle")
    stdscr.addstr(3, 0, "d : Increase Duty Cycle")
    stdscr.addstr(4, 0, "ESC : Exit")

plt.close()

curses.nocbreak()
stdscr.keypad(0)
curses.echo()
curses.endwin()
