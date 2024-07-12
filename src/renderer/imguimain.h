#ifndef IMGUIMAIN_H
#define IMGUIMAIN_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

namespace MainWindow {
    static void glfw_error_callback(int error, const char* description);
    int getPrimaryMonitorWidth();
    int getPrimaryMonitorHeight();
    void toggleWindow();
    int makeChtWindow();
}

#endif // IMGUIMAIN_H
