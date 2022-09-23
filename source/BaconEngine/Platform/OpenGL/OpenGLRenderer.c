#include "SharedEngineCode/Internal/CppSupport.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "OpenGLRenderer.h"
#include "../../Rendering/SpecificRendererFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_Color_3U clearColor;
const char* vertexShaderCode = "#version 330 core\n"
                               "layout (location = 0) in vec3 position;\n"
                               "void main() {\n"
                               "    gl_Position = vec4(position.x, position.y, position.z, 1);\n"
                               "}";
const char* fragmentShaderCode = "#version 330 core\n"
                                 "out vec4 fragColor;\n"
                                 "void main() {\n"
                                 "  fragColor = vec4(1, 1, 1, 1);\n"
                                 "}";
unsigned shaderProgram;
unsigned vertexBufferObjects;
unsigned vertexArrayObjects;

void BE_OpenGLRenderer_CompileShaders(void) {
#if 0 // FIXME: This isn't finished.
    unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);

    SEC_LOGGER_DEBUG("Compiling shaders");
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    {
        int compiled;
        char error[512];

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
            glGetShaderInfoLog(vertexShader, sizeof(error), NULL, error);
            SEC_LOGGER_ERROR("Failed to compile vertex shader: %s\n", error);
        }
    }

    unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    {
        int compiled;
        char error[512];

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
            glGetShaderInfoLog(fragmentShader, sizeof(error), NULL, error);
            SEC_LOGGER_ERROR("Failed to compile fragment shader: %s\n", error);
        }
    }

    SEC_LOGGER_DEBUG("Creating shader program");

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        int linked;
        char error[512];

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);

        if (!linked) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, error);
            SEC_LOGGER_ERROR("Failed to link shaders: %s", error);
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glGenVertexArrays(1, &vertexArrayObjects);
    glGenBuffers(1, &vertexBufferObjects);
#endif
}

void BE_OpenGLRenderer_ClearScreen(void) {
    glClearColor((float) clearColor.r / 255, (float) clearColor.g / 255, (float)  clearColor.b / 255,  1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void BE_OpenGLRenderer_SetClearColor(BE_Color_3U color) {
    clearColor = color;
}

BE_Color_3U BE_OpenGLRenderer_GetClearColor(void) {
    return clearColor;
}

void BE_OpenGLRenderer_Render(void) {
    glfwSwapBuffers(BE_SpecificRendererFunctions_Get().windowFunctions.GetInternalWindow());
}

void BE_OpenGLRenderer_DrawFilledRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color) {
    (void) position;
    (void) size;
    (void) color;
    // TODO: Implementation.
}
SEC_CPP_SUPPORT_GUARD_END()
