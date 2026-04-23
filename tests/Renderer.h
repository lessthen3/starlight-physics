/*******************************************************************
 *                  Starlight Physics v0.0.1
 *        Created by Ranyodh Singh Mandur - 👾 2026-2026
 *
 *             Licensed under the MIT License (MIT).
 *        For more details, see the LICENSE file or visit:
 *              https://opensource.org/licenses/MIT
 *
 *      Starlight Physics is a free open source physics engine
********************************************************************/
#ifndef STARLIGHT_TESTS_RENDERER_H_G
#define STARLIGHT_TESTS_RENDERER_H_G

#include "ShaderProgram.h"

#include "Macros.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <iso646.h>
#include <stdio.h>

typedef struct 
{
    bool hi;

} STESTS_OpenGLctx;

typedef enum 
{
    TEXTURE_FILTER_Nearest,
    TEXTURE_FILTER_Linear,
    TEXTURE_FILTER_MipMapNearestNearest,
    TEXTURE_FILTER_MipMapLinearNearest,
    TEXTURE_FILTER_MipMapNearestLinear,
    TEXTURE_FILTER_MipMapLinearLinear
} TextureFiltering;

 STARLIGHT_NODISCARD bool
    InitializeOpenGL //peach renderer is never supposed to create an sdl window, it only manages closing it
    (
        SDL_Window* fp_CurrentWindow
    )
    {
        ////Set Core Profile for OpenGL Context whatever the fuck that means
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        //// Set OpenGL version
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        // Create an OpenGL context associated with the window
        struct SDL_GLContextState* ctx = SDL_GL_CreateContext(fp_CurrentWindow);

        SDL_GL_MakeCurrent(fp_CurrentWindow, ctx);

        if (not ctx)
        {
            printf("Failed to create OpenGL context: %s", SDL_GetError());
            SDL_DestroyWindow(fp_CurrentWindow);
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        return true;
    }

    //bool
    //    DeleteShaderProgram
    //    (
    //        const string& fp_ShaderProgramName
    //    )
    //{
    //    try //idk lazy way of dealing with repeated deletes of a shader program
    //    {
    //        glDeleteProgram(pm_ShaderPrograms.at(fp_ShaderProgramName).GetProgramID());
    //        pm_ShaderPrograms.erase(fp_ShaderProgramName);
    //        return true;
    //    }
    //    catch (const exception& ex)
    //    {
    //        rendering_logger->Warning(fmt::format("An error occurred: {}", ex.what()), "OpenGL::Renderer"); //this might not work LOL
    //        return false;
    //    }
    //}

    // Example usage: suppose you want to pass a model matrix per instance
    // You would call setupInstancedArray four times, one for each row of the matrix, because glVertexAttribPointer can only handle up to 4 float components at a time.

    void
        GenerateUniformBufferObject() //used for uniforms that are shared across multiple shaders
    {
        69;
        return;
    }

    static void
        SetTextureFiltering
        (
            GLuint fp_TextureID,
            TextureFiltering fp_Filter
        )
    {
        glBindTexture(GL_TEXTURE_2D, fp_TextureID);

        switch (fp_Filter)
        {
        case TEXTURE_FILTER_Nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TEXTURE_FILTER_Linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TEXTURE_FILTER_MipMapNearestNearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case TEXTURE_FILTER_MipMapLinearNearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case TEXTURE_FILTER_MipMapNearestLinear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        case TEXTURE_FILTER_MipMapLinearLinear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }


    GLuint
        RegisterTexture
        (
            unsigned char* fp_Data,
            const unsigned int fp_Width,
            const unsigned int fp_Height,
            const unsigned int fp_Channels
        )
    {
        GLuint f_Texture;

        glGenTextures(1, &f_Texture);
        glBindTexture(GL_TEXTURE_2D, f_Texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum f_ColourFormat = GL_RGBA;

        if (fp_Channels == 3)
        {
            f_ColourFormat = GL_RGB;
        }
        else if (fp_Channels == 1)
        {
            f_ColourFormat = GL_RED;
        }

        if (fp_Data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, f_ColourFormat, fp_Width, fp_Height, 0, f_ColourFormat, GL_UNSIGNED_BYTE, fp_Data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("[%s]: Failed to Register Texture", "OpenGL::Renderer");
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        return f_Texture;
    }

    void
        DrawTexture
        (
            const ShaderProgram* fp_Shader,
            GLuint fp_VAO,
            GLuint fp_Texture
        )
    {
        glUseProgram(fp_Shader->ID);
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, fp_Texture);

        // tell the sampler uniform it lives on texture unit 0
        //GLint f_SamplerLocation = glGetUniformLocation(fp_Shader.GetProgramID(), "u_Texture");

        //if (f_SamplerLocation != -1)
        //{
        //    glUniform1i(f_SamplerLocation, 0);
        //}

        glBindVertexArray(fp_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void
        DrawShapePrimitive
        (
            const ShaderProgram* fp_Shader,
            const GLuint fp_VAO
        )
    {
        glBindVertexArray(fp_VAO);

        glUseProgram(fp_Shader->ID);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
    }

    GLuint //returns the vao id
        Generate2DBuffers
        (
            const float* fp_Vertices,
            const size_t fp_VertexCount,
            const uint32_t* fp_Indices,
            const size_t fp_IndexCount

        )
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);

        GLuint vao;
        glGenVertexArrays(1, &vao);

        GLuint ebo;
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * fp_IndexCount, fp_Indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fp_VertexCount, fp_Vertices, GL_STATIC_DRAW);

        // position coord attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0); //IMPORTANT: REMEMBER TO ALWAYS UNBIND VERTEX ARRAY FIRST SINCE UNBINDING ANYTHING INSIDE OF IT BEFOREHAND WILL DE CONFIGURE IT
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return vao;
    }

    //WIP
    GLuint //returns the vao id
        Generate3DBuffers
        (
            const float* fp_Vertices,
            const size_t fp_VertexCount,
            const uint32_t* fp_Indices,
            const size_t fp_IndexCount
        )
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);

        GLuint vao;
        glGenVertexArrays(1, &vao);

        GLuint ebo;
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * fp_IndexCount, fp_Indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fp_VertexCount, fp_Vertices, GL_STATIC_DRAW);

        // position coord attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 0th point start stride by 8, eg 0-3, 8-11, 16-19, . . .
        glEnableVertexAttribArray(0);

        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //3rd point start, stride by 8, eg. 3-5, 11-13, 19-21, . . .
        glEnableVertexAttribArray(1);

        // normal coord attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); //5th point start, stride by 8, eg. 5-8, 13-16, 21-24, . . .
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); //IMPORTANT: REMEMBER TO ALWAYS UNBIND VERTEX ARRAY FIRST SINCE UNBINDING ANYTHING INSIDE OF IT BEFOREHAND WILL DE CONFIGURE IT
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return vao;
    }

    bool
        RenderFrame()
    {

        return true;
    }



#endif /*STARLIGHT_TESTS_RENDERER_H_G*/

