#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <Windows.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float CLOCK_RADIUS = 0.4;
const int NUM_HOUR_LINES = 12;
const float HOUR_LINE_LENGTH = 0.08;
const float HOUR_LINE_WIDTH = 0.01;
const float HOUR_HAND_WIDTH = 0.03;
const float MINUTE_HAND_WIDTH = 0.02;
const float SECOND_HAND_WIDTH = 0.01;
const float HOUR_HAND_LENGTH = 0.20;
const float MINUTE_HAND_LENGTH = 0.30;
const float SECOND_HAND_LENGTH = 0.35;
const float PENDULUM_LENGTH = 0.45;
const float PENDULUM_WIDTH = 0.02;
const float GRAVITY = 9.8;
const float PENDULUM_PERIOD = 1.0;
float pendulumAngle = 0.0;

void drawClock()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Calculate the rectangle coordinates with increased size
    float rectLeft = -CLOCK_RADIUS - 0.07;
    float rectRight = CLOCK_RADIUS + 0.07;
    float rectBottom = -CLOCK_RADIUS - 0.07 - PENDULUM_LENGTH - PENDULUM_WIDTH;
    float rectTop = CLOCK_RADIUS + 0.07;

    // Draw rectangle around the clock and pendulum
    glBegin(GL_LINE_LOOP);
    glVertex2f(rectLeft, rectBottom);
    glVertex2f(rectRight, rectBottom);
    glVertex2f(rectRight, rectTop);
    glVertex2f(rectLeft, rectTop);
    glEnd();

    SYSTEMTIME currentTime;
    GetLocalTime(&currentTime);

    float hour = currentTime.wHour;
    float minute = currentTime.wMinute;
    float second = currentTime.wSecond;

    glLoadIdentity();

    // Draw outer circle
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++)
    {
        float theta = 2.0 * 3.1415926 * i / 360.0;
        float x = CLOCK_RADIUS * std::cos(theta);
        float y = CLOCK_RADIUS * std::sin(theta);

        // Interpolate colors based on y-coordinate
        float r = (y + 1.0) / 2.0;  // Red component
        float g = 0.0;              // Green component
        float b = (1.0 - y) / 2.0;  // Blue component
        glColor3f(r, g, b);

        glVertex2f(x, y);
    }
    glEnd();

    // Draw second outer circle
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++)
    {
        float theta = 2.0 * 3.1415926 * i / 360.0;
        float x = (CLOCK_RADIUS + 0.02) * std::cos(theta);  // 0.02 is the distance between the two circles
        float y = (CLOCK_RADIUS + 0.02) * std::sin(theta);

        // Interpolate colors based on y-coordinate
        float r = (y + 1.0) / 2.0;  // Red component
        float g = 0.0;              // Green component
        float b = (1.0 - y) / 2.0;  // Blue component
        glColor3f(r, g, b);

        glVertex2f(x, y);
    }
    glEnd();

    // Draw hour lines
    glLineWidth(HOUR_LINE_WIDTH);
    glColor3f(1.0, 1.0, 1.0); // White color
    for (int i = 0; i < NUM_HOUR_LINES; i++)
    {
        float theta = 2.0 * 3.1415926 * i / NUM_HOUR_LINES;
        float x1 = (CLOCK_RADIUS - HOUR_LINE_LENGTH) * std::cos(theta);
        float y1 = (CLOCK_RADIUS - HOUR_LINE_LENGTH) * std::sin(theta);
        float x2 = (CLOCK_RADIUS - HOUR_LINE_WIDTH) * std::cos(theta);
        float y2 = (CLOCK_RADIUS - HOUR_LINE_WIDTH) * std::sin(theta);
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }

    // Draw hour hand
    glPushMatrix();
    glRotatef(-30.0 * (hour + minute / 60.0), 0.0, 0.0, 1.0);
    glLineWidth(HOUR_HAND_WIDTH);
    glColor3f(0, 0, 1.0); // White color
    glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 1.0); // Dark blue color at the base
	glVertex2f(-HOUR_HAND_WIDTH / 2.0, 0.0);
	glColor3f(0.0, 0.5, 1.0); // Light blue color at the tip
	glVertex2f(HOUR_HAND_WIDTH / 2.0, 0.0);
	glColor3f(1.0, 1.0, 1.0); // White color
	glVertex2f(0.0, HOUR_HAND_LENGTH);
	glEnd();
    glPopMatrix();

    // Draw minute hand
    glPushMatrix();
    glRotatef(-6.0 * (minute + second / 60.0), 0.0, 0.0, 1.0);
    glLineWidth(MINUTE_HAND_WIDTH);
    glColor3f(1.0, 1.0, 1.0); // White color
    glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 1.0); // Dark blue color at the base
	glVertex2f(-MINUTE_HAND_WIDTH / 2.0, 0.0);
	glColor3f(0.0, 0.5, 1.0); // Light blue color at the tip
	glVertex2f(MINUTE_HAND_WIDTH / 2.0, 0.0);
	glColor3f(1.0, 1.0, 1.0); // White color
	glVertex2f(0.0, MINUTE_HAND_LENGTH);
	glEnd();
    glPopMatrix();

    // Draw second hand
    glPushMatrix();
    glRotatef(-6.0 * second, 0.0, 0.0, 1.0);
    glLineWidth(SECOND_HAND_WIDTH);
    glColor3f(1.0, 1.0, 1.0); // White color
    glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 1.0); // Dark blue color at the base
	glVertex2f(-SECOND_HAND_WIDTH / 2.0, 0.0);
	glColor3f(0.0, 0.5, 1.0); // Light blue color at the tip
	glVertex2f(SECOND_HAND_WIDTH / 2.0, 0.0);
	glColor3f(1.0, 1.0, 1.0); // White color
	glVertex2f(0.0, SECOND_HAND_LENGTH);
	glEnd();
    glPopMatrix();

    // Draw pendulum
    glPushMatrix();
    glTranslatef(0.0, -CLOCK_RADIUS - 0.02, 0.0); // Translate to the bottom center of the second outer circle
    glRotatef(pendulumAngle, 0.0, 0.0, 1.0); // Rotate the pendulum
    glLineWidth(PENDULUM_WIDTH);
    glColor3f(1.0, 1.0, 1.0); // White color

    // Draw circle at the bottom of the pendulum
    float bottomCircleRadius = PENDULUM_WIDTH * 2;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, -PENDULUM_LENGTH);
    for (int i = 0; i <= 360; i++)
    {
        float theta = 2.0 * 3.1415926 * i / 360.0;
        float x = bottomCircleRadius * std::cos(theta);
        float y = -PENDULUM_LENGTH - PENDULUM_WIDTH + bottomCircleRadius * std::sin(theta);
        glVertex2f(x, y);
    }
    glEnd();

    // Draw circle at the top of the pendulum where it is hanging
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, 0.0);
    for (int i = 0; i <= 360; i++)
    {
        float theta = 2.0 * 3.1415926 * i / 360.0;
        float x = PENDULUM_WIDTH * std::cos(theta);
        float y = PENDULUM_WIDTH * std::sin(theta);
        glVertex2f(x, y);
    }
    glEnd();

    // Draw the pendulum line
    glLineWidth(5.0);  // Adjust this value to change the line thickness
    glBegin(GL_LINES);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.0, -PENDULUM_LENGTH);
    glEnd();
    glPopMatrix();

	// Draw small filled circle at the center of the clock
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, 0.0);
    for (int i = 0; i <= 360; i++)
    {
        float theta = 2.0 * 3.1415926 * i / 360.0;
        float x = .015 * std::cos(theta);
        float y = .015 * std::sin(theta);

        // Interpolate colors based on y-coordinate
        float r = (y + 1.0) / 2.0;  // Red component
        float g = 0.0;              // Green component
        float b = (1.0 - y) / 2.0;  // Blue component
        glColor3f(0, 0, 1);

        glVertex2f(x, y);
    }
    glEnd();

    glFlush();
}


void update(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, update, 0);

    static float pendulumDirection = 1.0;  // Direction of pendulum swing
    static float pendulumSpeed = 45.0 * 2.0 / PENDULUM_PERIOD;  // Maximum angle change per second

    // Update pendulum angle
    pendulumAngle += pendulumDirection * pendulumSpeed / 60.0;  // Increment angle per frame

    // Check if pendulum reaches the maximum angle, reverse direction
    if (pendulumAngle >= 45.0 || pendulumAngle <= -45.0)
        pendulumDirection *= -1.0;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Clock with Pendulum");
    glutDisplayFunc(drawClock);
    glutTimerFunc(0, update, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background

    glutMainLoop();

    return 0;
}
