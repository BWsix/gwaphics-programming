#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

float g_rotation_speed = 0.1f;
float g_triangle_rotation = 0;
float g_offset_x = 0;
float g_offset_y = 0;

vec3 star_verticies[] = {
    vec3( 0.0,  0.0, 0.0), // center
    vec3( 0.0,  0.7, 0.0), // top
    vec3(-0.2,  0.4, 0.0), // top > left
    vec3(-0.6,  0.4, 0.0), // left
    vec3(-0.3,  0.0, 0.0), // left > bottom-left
    vec3(-0.5, -0.4, 0.0), // bottom-left
    vec3( 0.0, -0.2, 0.0), // bottom-left > bottom-right
    vec3( 0.5, -0.4, 0.0), // bottom-right
    vec3( 0.3,  0.0, 0.0), // bottom-right > right
    vec3( 0.6,  0.4, 0.0), // right
    vec3( 0.2,  0.4, 0.0), // right > top
    vec3( 0.0,  0.7, 0.0), // top
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

    glTranslatef(g_offset_x, g_offset_y, 0.0);
    glRotatef(g_triangle_rotation, 0.0, 0.0, 1.0);

    glBegin(GL_TRIANGLE_FAN);
    for (const vec3 &vertex : star_verticies) {
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
    glPopMatrix();

    g_triangle_rotation += g_rotation_speed;
}

void windowSizeCallback(GLFWwindow *, int width, int height) {
    if (height == 0) {
        height = 1;
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMultMatrixf(glm::value_ptr(projMat));

    glTranslatef(0, 0, -4.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyCallback(GLFWwindow *, int key, int, int, int) {
    switch (key) {
        case GLFW_KEY_KP_ADD:
            g_rotation_speed += 0.01;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            g_rotation_speed -= 0.01;
            break;
        case GLFW_KEY_UP:
            g_offset_y += 0.05;
            break;
        case GLFW_KEY_DOWN:
            g_offset_y -= 0.05;
            break;
        case GLFW_KEY_LEFT:
            g_offset_x -= 0.05;
            break;
        case GLFW_KEY_RIGHT:
            g_offset_x += 0.05;
            break;
    }
}

int main(int, char **argv) {
    if (!glfwInit()) {
        exit(1);
    }

    glfwWindowHint(GL_DOUBLEBUFFER, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, argv[0], NULL, NULL);
    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        exit(1);
    }

    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    init();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        display();
        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return 0;
}
