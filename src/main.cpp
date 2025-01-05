#include <GLFW/glfw3.h>  // 引入 GLFW 头文件

int main() {
    // 初始化 GLFW
    if (!glfwInit()) {
        return -1; // 如果初始化失败，返回错误码
    }

    // 创建一个窗口ed模式的窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello GLFW", nullptr, nullptr);
    if (!window) {
        glfwTerminate();  // 创建窗口失败，终止 GLFW
        return -1;
    }

    // 将窗口的上下文设置为当前的 OpenGL 上下文
    glfwMakeContextCurrent(window);

    // 主循环，直到窗口被关闭
    while (!glfwWindowShouldClose(window)) {
        // 渲染背景色为黑色
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 交换缓冲区，显示渲染内容
        glfwSwapBuffers(window);

        // 处理窗口事件
        glfwPollEvents();
    }

    // 终止 GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}