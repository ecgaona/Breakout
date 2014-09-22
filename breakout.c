//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

//height and width of paddle (Eduardo Carr-Gaona)
#define PADDLE_HEIGHT 10
#define PADDLE_WIDTH 60

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);


int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // wait for click to start game (Eduardo Carr-Gaona)
    waitForClick();

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // Eduardo Carr-Gaona's game play algorithm
        
        double xVelocity = drand48() + 2;
        double yVelocity = 2.0;
        
        while(true)
        {
            GEvent movePaddle = getNextEvent(MOUSE_EVENT); /* paddle follows mouse movement */
            if (movePaddle != NULL)
            {
                if (getEventType(movePaddle) == MOUSE_MOVED)
                {
                    double x = getX(movePaddle) - PADDLE_WIDTH / 2;
                    setLocation(paddle, x, 540);
                }
            }
             
            move(ball, xVelocity, yVelocity); /* bounces ball off window's edge at an angle */
            if (getX(ball) + getWidth(ball) >= getWidth(window))
            {
                xVelocity = -xVelocity;
            }
            else if (getX(ball) <= 0)
            {
                xVelocity = -xVelocity;
            }
            if (getY(ball) + getHeight(ball) >= getHeight(window))
            {
                yVelocity = -yVelocity;
            }
            else if (getY(ball) <= 0)
            {
                yVelocity = -yVelocity;
            }
            pause(10);
            
            GObject object = detectCollision(window, ball); /* if ball collides with paddle, reverses trajectory */
            if (object == paddle)
            {
                xVelocity = -xVelocity;
                yVelocity = -yVelocity;
            }
           if (object != NULL && object != paddle && object != label && object != window) /* if ball collides with brick, ball reverses */
            {                                                                             /* trajectory and brick disappears */
                xVelocity = -xVelocity;
                yVelocity = -yVelocity;
                removeGWindow(window, object);
                bricks--;
                points = (COLS * ROWS) - bricks;
                updateScoreboard(window, label, points);              
            }
            
            if (getY(ball) + getHeight(ball) >= HEIGHT) /* if player failes to catch ball with paddle and ball touches */
            {                                           /* bottom of window, player looses life and ball returns to */
                lives--;                                /* starting position awaiting mouse click to resume play */
                if (lives != 0)
                {  
                    setBounds(ball, ((WIDTH/2) - RADIUS), ((HEIGHT/2) - RADIUS), (2 * RADIUS), (2 * RADIUS));
                    waitForClick();
                }
                
            }
            if (lives == 0 || bricks == 0) 
            {
                break;
            }
        }
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // Eduardo Carr-Gaona's initBricks algorithm
    for (int i = 2; i < (WIDTH - 2); i = i + 40)
    {
        GRect brick = newGRect (i, 50, 36, 12);
        setFilled(brick, true);
        setColor(brick, "BLUE");
        add(window, brick);
    }
    for (int i = 2; i < (WIDTH - 2); i = i + 40)
    {
        GRect brick = newGRect (i, 66, 36, 12);
        setFilled(brick, true);
        setColor(brick, "CYAN");
        add(window, brick);
    }
    for (int i = 2; i < (WIDTH - 2); i = i + 40)
    {
        GRect brick = newGRect (i, 82, 36, 12);
        setFilled(brick, true);
        setColor(brick, "GREEN");
        add(window, brick);
    }
    for (int i = 2; i < (WIDTH - 2); i = i + 40)
    {
        GRect brick = newGRect (i, 98, 36, 12);
        setFilled(brick, true);
        setColor(brick, "MAGENTA");
        add(window, brick);
    }
    for (int i = 2; i < (WIDTH - 2); i = i + 40)
    {
        GRect brick = newGRect (i, 114, 36, 12);
        setFilled(brick, true);
        setColor(brick, "ORANGE");
        add(window, brick);
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // Eduardo Carr-Gaona's initBall algorithm
    GOval ball = newGOval(((WIDTH/2) - RADIUS), ((HEIGHT/2) - RADIUS), (2 * RADIUS), (2 * RADIUS));
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // Eduardo Carr-Gaona's initPaddle algorithm
    GRect paddle = newGRect(170, 540, PADDLE_WIDTH, PADDLE_HEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "RED");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // Eduardo Carr-Gaona's initScoreboard algorithm
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-36");
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    setColor(label, "LIGHT_GRAY");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
