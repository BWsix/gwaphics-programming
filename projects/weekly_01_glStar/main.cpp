#include <cassert>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const char* WINDOW_TITLE = "glStar";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float THICKNESS_OF_STAR = 0.25;
const float MAGNITUDE_OF_OCILLATION = 0.3;
const float OSCILLATION_PER_SECOND = 1;
const float ROTATION_PER_SECOND = 0.5;

float star_vertices[][3] {
    { 0.0,  0.7, 0.0}, // top
    {-0.2,  0.4, 0.0}, // top > left
    {-0.6,  0.4, 0.0}, // left
    {-0.3,  0.0, 0.0}, // left > bottom-left
    {-0.5, -0.4, 0.0}, // bottom-left
    { 0.0, -0.2, 0.0}, // bottom-left > bottom-right
    { 0.5, -0.4, 0.0}, // bottom-right
    { 0.3,  0.0, 0.0}, // bottom-right > right
    { 0.6,  0.4, 0.0}, // right
    { 0.2,  0.4, 0.0}, // right > top
    { 0.0,  0.7, 0.0}, // top
};

bool init() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    return true;
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();

    float time_elapsed = glfwGetTime();
    float s = sin(OSCILLATION_PER_SECOND * time_elapsed);
    float y = MAGNITUDE_OF_OCILLATION * s * s - MAGNITUDE_OF_OCILLATION / 2;

    float r = 360 * time_elapsed;
    float theta = ROTATION_PER_SECOND * r;

    glTranslatef(0, y, 0);
    glRotatef(theta, 0, 1, 0);

    // draw call for the front-half of the star
    glBegin(GL_TRIANGLE_FAN);
    {
        glColor3f(1, 1, 0);
        glVertex3f(0, 0, THICKNESS_OF_STAR);
        glColor3f(0.2, 0.2, 0);
        for (auto [x, y, z] : star_vertices) {
            glVertex3f(x, y, z);
        }
    }
    glEnd();

    // darw call for the back-half of the star
    glBegin(GL_TRIANGLE_FAN);
    {
        glColor3f(1, 1, 0);
        glVertex3f(0, 0, -THICKNESS_OF_STAR);
        glColor3f(0.2, 0.2, 0);
        for (auto [x, y, z] : star_vertices) {
            glVertex3f(x, y, z);
        }
    }
    glEnd();

    glPopMatrix();
}

void windowSizeCallback(GLFWwindow *, int width, int height) {
    if (height == 0) {
        height = 1;
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
    glMultMatrixf(glm::value_ptr(proj_mat));

    glTranslatef(0, 0, -4);
    glMatrixMode(GL_MODELVIEW);
}

int main() {
    assert(glfwInit() && "Failed to initialize GLFW ._.");

    glfwWindowHint(GL_DOUBLEBUFFER, GL_TRUE);
    auto *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    glfwMakeContextCurrent(window);

    assert(gladLoadGL(glfwGetProcAddress) && "Something went wrong with glad ._.");

    glfwSetWindowSizeCallback(window, windowSizeCallback);

    init();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        display();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
