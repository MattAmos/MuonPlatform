import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches 
import curses
import numpy as np
import math

BOUNDS = 1000
TRAIL  = 100
CAR_W  = 190.0
CAR_H  = 380.0

def checkBounds(points):
    for pt in points:
        if abs(pt[0]) > BOUNDS or abs(pt[1]) > BOUNDS:
            return False
    return True

def checkThetaBounds(theta):
    if math.degrees(theta) > 20:
        theta = math.radians(20)
    if math.degrees(theta) < -20:
        theta = math.radians(-20)
    return theta

# Set up curses
# stdscr = curses.initscr()
# curses.noecho()
# curses.cbreak()
# stdscr.keypad(1)

# Initialise
p1 = [-CAR_H/2.0, 0.0]; p2 = [CAR_H/2.0, 0.0]; 
w1 = [-CAR_H/2.0, -CAR_W/2.0]; w2 = [-CAR_H/2.0, CAR_W/2.0];
w3 = [ CAR_H/2.0, -CAR_W/2.0];  w4 = [ CAR_H/2.0, CAR_W/2.0]
v = [p2[0] - p1[0], p2[1] - p1[1]]
norm  = math.sqrt(v[0]*v[0] + v[1]*v[1])
theta = 0.0; speed = 5.0
prevFront = [[],[]]; prevBack = [[],[]]

plt.ion()
x = [p1[0], p2[0], w1[0], w2[0], w3[0], w4[0]]
y = [p1[1], p2[1], w1[1], w2[1], w3[1], w4[1]] 
# Draw middle axel
plt.plot([p1[0], p2[0]], [p1[1], p2[1]], color=(0.0, 0.0, 1.0, 1.0))
# Draw back axel
plt.plot([w3[0], w4[0]], [w3[1], w4[1]], color=(0.0, 1.0, 0.0, 1.0))
# Draw front axel
plt.plot([w1[0], w2[0]], [w1[1], w2[1]], color=(1.0, 0.0, 0.0, 1.0))
plt.plot(x, y, 'ro')
plt.title("Muon Platform Pathing Diagram")
plt.xlabel("x")
plt.ylabel("y")
plt.axis([-BOUNDS, BOUNDS, -BOUNDS, BOUNDS])
plt.draw()

while True:
    # Update theta
    theta = math.radians(20)
    theta = checkThetaBounds(theta)
    
    # Update front and back points positions
    beta = math.atan2(v[1], v[0])
    print('{}, {}'.format(math.degrees(theta), math.degrees(beta)))
    cosTheta = math.cos(theta + beta); sinTheta = math.sin(theta + beta)
    phi1 = math.atan2(p2[1] - p1[1], p2[0] - p1[0])
    p2   = [p2[0] + speed * math.cos(theta + beta), p2[1] + speed * math.cos(theta + beta)]
    phi2 = math.atan2(p2[1] - p1[1], p2[0] - p1[0])
    phi  = phi2 - phi1
    
    cosPhi = math.cos(phi); sinPhi = math.sin(phi)

    p1 = [p2[0] - norm*math.cos(phi2), p2[1] - norm*math.sin(phi2)]
    
    prevBack[0]  += [p1[0]]; prevBack[1]  += [p1[1]]
    prevFront[0] += [p2[0]]; prevFront[1] += [p2[1]]
    if len(prevBack[0]) > TRAIL: 
        prevBack[0] = prevBack[0][1:]; prevBack[1] = prevBack[1][1:]
    if len(prevFront[0]) > TRAIL: 
        prevFront[0] = prevFront[0][1:]; prevFront[1] = prevFront[1][1:]

    v = [p2[0] - p1[0], p2[1] - p1[1]]
    n = [-v[1]/norm, v[0]/norm]

    alpha = math.atan2(n[1], n[0])

    # Back wheels
    w1 = [p1[0] + math.cos(alpha)*CAR_W/2.0, p1[1] + math.sin(alpha)*CAR_W/2.0]
    w2 = [p1[0] - math.cos(alpha)*CAR_W/2.0, p1[1] - math.sin(alpha)*CAR_W/2.0]
    # Front wheels
    w3 = [p2[0] + math.cos(alpha)*CAR_W/2.0, p2[1] + math.sin(alpha)*CAR_W/2.0]
    w4 = [p2[0] - math.cos(alpha)*CAR_W/2.0, p2[1] - math.sin(alpha)*CAR_W/2.0]
    
    # Check points are within boundary
    points = [p1, p2, w1, w2, w3, w4]
    if checkBounds(points) == False:
        break;

    # Process sensors


    # Plot model
    plt.plot(prevFront[0], prevFront[1], 'g.') 
    plt.plot(prevBack[0], prevBack[1], 'r.') 
    x = [p1[0], p2[0], w1[0], w2[0], w3[0], w4[0]]
    y = [p1[1], p2[1], w1[1], w2[1], w3[1], w4[1]]
    # Draw middle axel
    plt.plot([p1[0], p2[0]], [p1[1], p2[1]], color=(0.0, 0.0, 1.0, 1.0))
    # Draw back axel
    plt.plot([w3[0], w4[0]], [w3[1], w4[1]], color=(0.0, 1.0, 0.0, 1.0))
    # Draw front axel
    plt.plot([w1[0], w2[0]], [w1[1], w2[1]], color=(1.0, 0.0, 0.0, 1.0))
    plt.plot(x, y, 'ro')
    plt.draw()
    plt.clf()
    plt.title("Muon Platform Pathing Diagram")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.axis([-BOUNDS, BOUNDS, -BOUNDS, BOUNDS])

plt.close()

# curses.nocbreak()
# stdscr.keypad(0)
# curses.echo()
# curses.endwin()
