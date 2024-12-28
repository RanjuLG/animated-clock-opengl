#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <windows.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

const float DEG_TO_RAD = 3.14159 / 180;

void drawCircle(float radius, float red, float green, float blue);
void drawBox(float coordinates[8], float red, float green, float blue);
void drawBoxWithAngle(float startL, float endL, float halfW, float rotAngle, float red, float green, float blue);
void drawClock();
void drawClockMarks(int count, float startL, float endL, float halfW, float red, float green, float blue);
void drawHand(float L, float W, float rotAngle, float red, float green, float blue);

int main() {
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit()) return -1;

    // Create a window
    window = glfwCreateWindow(700, 700, "Animated Clock - CSC 2411", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);

        // Get the current time
        auto currentTime = system_clock::now();
        time_t currentTimeC = system_clock::to_time_t(currentTime);
        tm localTime;
        localtime_s(&localTime, &currentTimeC);

        int hours = localTime.tm_hour % 12;
        int minutes = localTime.tm_min;
        int seconds = localTime.tm_sec;

        float secondHandAngle = 90 - (seconds * 6);
        float minuteHandAngle = 90 - (minutes * 6 + seconds * 0.1);
        float hourHandAngle = 90 - (hours * 30 + minutes * 0.5);

        drawClock();
        drawHand(0.75, 0.01, secondHandAngle, 0.8, 0.1, 0.1); //Second hand
        drawHand(0.68, 0.02, minuteHandAngle, 0.0, 0.5, 0.5);  //Minute hand
        drawHand(0.55, 0.03, hourHandAngle, 0.8, 0.6, 0.2);  //Hour hand

        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(16);
    }

    glfwTerminate();
    return 0;
}

void drawClock() {
    drawCircle(1, 0.2, 0.2, 0.2);  // Outer circle
    drawCircle(0.95, 0.9, 0.9, 0.9);  // Inner circle
    drawClockMarks(12, 0.8, 0.95, 0.02, 0.2, 0.3, 0.8);  // Hour marks
    drawClockMarks(60, 0.85, 0.95, 0.01, 0.2, 0.3, 0.8);  // Minute marks
}

void drawClockMarks(int count, float startL, float endL, float halfW, float red, float green, float blue) {
    for (int i = 0; i < count; ++i) {
        float angle = i * (360.0f / count);
        drawBoxWithAngle(startL, endL, halfW, angle, red, green, blue);
    }
}

void drawCircle(float radius, float red, float green, float blue) {
    glBegin(GL_POLYGON);
    glColor3f(red, green, blue);
    for (int i = 0; i <= 300; ++i) {
        float angle = 2 * 3.14159 * i / 300;
        glVertex2f(cos(angle) * radius, sin(angle) * radius);
    }
    glEnd();
}

void drawBox(float coordinates[8], float red, float green, float blue) {
    glBegin(GL_QUADS);
    glColor3f(red, green, blue);
    for (int i = 0; i < 8; i += 2) {
        glVertex2f(coordinates[i], coordinates[i + 1]);
    }
    glEnd();
}

void drawBoxWithAngle(float startL, float endL, float halfW, float rotAngle, float red, float green, float blue) {
    float angle = rotAngle * DEG_TO_RAD;
    float coordinates[8] = {
        startL * cos(angle) - halfW * sin(angle), startL * sin(angle) + halfW * cos(angle), // Bottom-left
        endL * cos(angle) - halfW * sin(angle),   endL * sin(angle) + halfW * cos(angle),   // Bottom-right
        endL * cos(angle) + halfW * sin(angle),   endL * sin(angle) - halfW * cos(angle),   // Top-right
        startL * cos(angle) + halfW * sin(angle), startL * sin(angle) - halfW * cos(angle)  // Top-left
    };

    drawBox(coordinates, red, green, blue);
}

void drawHand(float L, float W, float rotAngle, float red, float green, float blue) {
    drawBoxWithAngle(0, L, W, rotAngle, red, green, blue);
}
