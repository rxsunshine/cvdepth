#include <cstdio>

#include <opencv2/opencv.hpp>
#include <SDL.h>
#include <GL/glew.h>

#include "gl_shader.hpp"
#include "gl_texture.hpp"
#include "gl_prim.hpp"
#include "fileio.hpp"

int main(int argc, char** argv) {
    cv::VideoCapture cap(0, cv::CAP_DSHOW);
    if (!cap.isOpened()) {
        printf("failed to open device\n");
        return 1;
    }
    printf("successfully opened device\n");

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* wind = 
        SDL_CreateWindow(
            "opencv", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext glctx = SDL_GL_CreateContext(wind);
    SDL_GL_MakeCurrent(wind, glctx);
    glewInit();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint quadprog = GL_CreateProgram(
        ReadEntireFile("../shaders/quad.vert").c_str(),
        ReadEntireFile("../shaders/quad.frag").c_str());

    Primative quad = GL_CreateQuad();

    Image img = Image_Create(640, 480, 3);
    GLuint tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    GL_TextureFilter(tex, GL_NEAREST, GL_NEAREST);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTextureStorage2D(tex, 1, GL_RGB8UI, img.width, img.height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    bool running = true;
    while (running) {
        for (SDL_Event e; SDL_PollEvent(&e);) {
            if (e.type == SDL_QUIT) running = false;
        }

        cv::Mat3b out;
        cap >> out;

        uint8_t* bytes = (uint8_t*) img.data;
        for (int j = 0; j < img.height; ++j) {
            int y = (j * out.rows) / img.height;
            for (int i = 0; i < img.width; ++i) {
                int x = ((i * out.cols) / img.width) * 3;
                bytes[(j * img.width + i) * 3 + 0] = out.at<uint8_t>(y, x + 0);
                bytes[(j * img.width + i) * 3 + 1] = out.at<uint8_t>(y, x + 1);
                bytes[(j * img.width + i) * 3 + 2] = out.at<uint8_t>(y, x + 2);
            }
        }

        glTextureSubImage2D(tex, 0, 0, 0, img.width, img.height, GL_RGB, GL_UNSIGNED_INT, bytes);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(quadprog);

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, tex);
        GL_PassUniform(glGetUniformLocation(quadprog, "u_image"), 0);

        glBindVertexArray(quad.vao);
        quad.Draw();

        SDL_GL_SwapWindow(wind);
    }

    Image_Free(img);
    glDeleteTextures(1, &tex);
    glDeleteProgram(quadprog);
    GL_DestroyPrimative(quad);
    SDL_DestroyWindow(wind);
    SDL_Quit();

    return 0;
}