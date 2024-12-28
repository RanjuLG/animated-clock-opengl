#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <windows.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void drawCircle(double radius, float red, float green, float blue);
void drawTriangle(float red, float green, float blue, float startLength, float endLength, float halfWidth, float rotationAngle);
void drawClockFace();
void drawbox(float coordinates[8], float red, float green, float blue);
void drawRotatedbox(float red, float green, float blue, float startLength, float endLength, float halfWidth, float rotationAngle);
void drawSecondHand(float rotationAngle);
void drawMinuteHand(float rotationAngle);
void drawHourHand(float rotationAngle);

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()) return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(700, 700, "Animated Clock - CSC 2411", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        // Set background color
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);

        // Get current time
        auto currentTime = system_clock::now();
        time_t currentTimeC = system_clock::to_time_t(currentTime);
        tm localTime;
        localtime_s(&localTime, &currentTimeC);

        int hours = localTime.tm_hour;
        int minutes = localTime.tm_min;
        int seconds = localTime.tm_sec;

        // Calculate angles for clock hands
        float secondHandAngle = 90 - (seconds * 6);
        float minuteHandAngle = 90 - (minutes * 6 + seconds * 0.1);
        float hourHandAngle = 90 - ((hours % 12) * 30 + minutes * 0.5);

        // Draw clock face and hands
        drawClockFace();
        drawHourHand(hourHandAngle);
        drawMinuteHand(minuteHandAngle);
        drawSecondHand(secondHandAngle);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Sleep for a short duration to limit the frame rate
        Sleep(16);
    }

    glfwTerminate();
    return 0;
}

void drawClockFace()
{
    // Outer circle
    drawCircle(1, 0.2, 0.2, 0.2);
    drawCircle(0.95, 0.9, 0.9, 0.9);

    // Hour indicators
    for (int i = 0; i < 12; i++) {
        drawRotatedbox(0.2, 0.3, 0.8, 0.8, 0.95, 0.02, 30 * i);
    }

    // Minute indicators
    for (int i = 0; i < 60; i++) {
        drawRotatedbox(0.2, 0.3, 0.8, 0.85, 0.95, 0.01, 6 * i);
    }
}

void drawCircle(double radius, float red, float green, float blue)
{
    glBegin(GL_POLYGON);
    glColor3f(red, green, blue);
    double originX = 0.0;
    double originY = 0.0;
    for (int i = 0; i <= 300; i++) {
        double angle = 2 * 3.14 * i / 300;
        double x = cos(angle) * radius;
        double y = sin(angle) * radius;
        glVertex2d(originX + x, originY + y);
    }
    glEnd();
}

void drawbox(float coordinates[8], float red, float green, float blue)
{
    glBegin(GL_QUADS);
    glColor3f(red, green, blue);
    glVertex2f(coordinates[0], coordinates[1]);
    glVertex2f(coordinates[2], coordinates[3]);
    glVertex2f(coordinates[4], coordinates[5]);
    glVertex2f(coordinates[6], coordinates[7]);
    glEnd();
}

void drawRotatedbox(float red, float green, float blue, float startLength, float endLength, float halfWidth, float rotationAngle)
{
    rotationAngle = rotationAngle * 3.14 / 180;
    float box[8];
    float diagonalStart = sqrt(startLength * startLength + halfWidth * halfWidth);
    float angleStart = atan(halfWidth / startLength);
    float angleEnd = atan(halfWidth / endLength);
    float diagonalEnd = sqrt(endLength * endLength + halfWidth * halfWidth);

    box[0] = diagonalStart * cos(rotationAngle - angleStart);
    box[1] = diagonalStart * sin(rotationAngle - angleStart);
    box[2] = diagonalEnd * cos(rotationAngle - angleEnd);
    box[3] = diagonalEnd * sin(rotationAngle - angleEnd);
    box[4] = diagonalEnd * cos(rotationAngle + angleEnd);
    box[5] = diagonalEnd * sin(rotationAngle + angleEnd);
    box[6] = diagonalStart * cos(rotationAngle + angleStart);
    box[7] = diagonalStart * sin(rotationAngle + angleStart);

    drawbox(box, red, green, blue);
}

void drawTriangle(float red, float green, float blue, float startLength, float endLength, float halfWidth, float rotationAngle)
{
    rotationAngle = rotationAngle * 3.14 / 180;
    float diagonalStart = sqrt(startLength * startLength + halfWidth * halfWidth);
    float angleOffset = atan(halfWidth / startLength);

    float x1 = diagonalStart * cos(rotationAngle - angleOffset);
    float y1 = diagonalStart * sin(rotationAngle - angleOffset);
    float x2 = endLength * cos(rotationAngle);
    float y2 = endLength * sin(rotationAngle);
    float x3 = diagonalStart * cos(rotationAngle + angleOffset);
    float y3 = diagonalStart * sin(rotationAngle + angleOffset);

    glBegin(GL_TRIANGLES);
    glColor3f(red, green, blue);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void drawSecondHand(float rotationAngle)
{
    drawRotatedbox(0.8, 0.1, 0.1, 0.0, 0.6, 0.01, rotationAngle);
    drawRotatedbox(0.8, 0.1, 0.1, 0.0, 0.15, 0.01, rotationAngle + 180);
    drawTriangle(1.0, 0.2, 0.2, 0.6, 0.69, 0.04, rotationAngle);
    drawCircle(0.05, 0.8, 0.1, 0.1);
}

void drawMinuteHand(float rotationAngle)
{
    drawRotatedbox(0.0, 0.5, 0.5, 0.0, 0.55, 0.02, rotationAngle);
    drawTriangle(0.0, 0.5, 0.5, 0.55, 0.6, 0.02, rotationAngle);
}

void drawHourHand(float rotationAngle)
{
    drawRotatedbox(0.8, 0.6, 0.2, 0.0, 0.4, 0.03, rotationAngle);
    drawTriangle(0.8, 0.6, 0.2, 0.4, 0.45, 0.03, rotationAngle);
}
