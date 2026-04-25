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
#ifndef STARLIGHT_TESTS_SHADERPROGRAM_H_G
#define STARLIGHT_TESTS_SHADERPROGRAM_H_G

#include <GL/glew.h>

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "Types/StarlightString.h"
#include "Types/DynamicList.h"

typedef struct
{
    GLuint ID;
    STARLIGHT_String Name;
} STARLIGHT_TESTS_ShaderProgram;

STARLIGHT_TESTS_ShaderProgram
    STESTS_ShaderProgramCreate
    (
        const char* fp_ShaderName,
        const char* fp_VertexSourceFilePath,
        const char* fp_FragmentSourceFilePath
    )
{
    STARLIGHT_TESTS_ShaderProgram f_ShaderProgram;

    f_ShaderProgram.Name = STARLIGHT_StringCreate(size_t fp_InitialCapacity);
    f_ShaderProgram.ID = glCreateProgram();

    char* f_VertexSourceCode, f_FragmentSourceCode;

    ///vertex shader
    if (not ReadFileIntoString(fp_VertexSourceFilePath, f_VertexSourceCode, logger))
    {
        logger->Error("Unable to read vertex shader code into a string", "OpenGL::ShaderProgram: " + pm_ProgramName);
    }
    ///fragment shader
    if (not ReadFileIntoString(fp_FragmentSourceFilePath, f_FragmentSourceCode, logger))
    {
        logger->Error("Unable to read fragment shader code into a string", "OpenGL::ShaderProgram: " + pm_ProgramName);
    }

    GLint f_VertexShaderID = CreateShader(f_VertexSourceCode, GL_VERTEX_SHADER, logger);
    GLint f_FragmentShaderID = CreateShader(f_FragmentSourceCode, GL_FRAGMENT_SHADER, logger);


    if (f_VertexShaderID and f_FragmentShaderID) //CreateShader returns 0 if it fails so this will work fine
    {
        Link({f_VertexShaderID, f_FragmentShaderID}, logger);
    }
    else
    {
        logger->Error("Shader failed to link due to invalid shader(s)", "OpenGL::ShaderProgram: " + pm_ProgramName);
    }
}

//////////////////////////////////////////////
// Fog Uniform Creator Setters
//////////////////////////////////////////////

// void
//     SetUniform
//     (
//         const string& fp_UniformName,
//         const Fog3D& fp_Fog
//     )
// {
//     SetUniform(fp_UniformName + ".activeFog", fp_Fog.IsActive() ? 1 : 0);
//     SetUniform(fp_UniformName + ".colour", fp_Fog.GetColour());
//     SetUniform(fp_UniformName + ".density", fp_Fog.GetDensity());
// }

//////////////////////////////////////////////
// Material Uniform Setters
//////////////////////////////////////////////

// void
//     SetMaterialUniforms
//     (
//         const string& fp_UniformName,
//         const PeachMaterial& fp_Material
//     )
// {
//     SetUniform(fp_UniformName + ".ambient", fp_Material.GetAmbientColour());
//     SetUniform(fp_UniformName + ".diffuse", fp_Material.GetDiffuseColour());
//     SetUniform(fp_UniformName + ".specular", fp_Material.GetSpecularColour());
//     SetUniform(fp_UniformName + ".hasTexture", fp_Material.IsTextured() ? 1 : 0);
//     SetUniform(fp_UniformName + ".reflectance", fp_Material.GetReflectance());
// }

//////////////////////////////////////////////
// Texture Uniform Setter
//////////////////////////////////////////////

// void
//     SetTexture
//     (
//         const string& fp_UniformName,
//         int fp_TextureID,
//         int fp_TextureUnit
//     )
// {

// }

//////////////////////////////////////////////
// Shader Linker 
//////////////////////////////////////////////

void
    Link
    (
        const vector<GLint>& fp_ShaderIDs,
        Logger* logger
    )
{
    for (auto& lv_ShaderID : fp_ShaderIDs)
    {
        glAttachShader(pm_ProgramID, lv_ShaderID);
    }

    glLinkProgram(pm_ProgramID);

    logger->Debug
    (
        "Successfully Linked!", 
        fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
    );

    GLint f_Success;
    GLchar f_InfoLog[512];

    glGetProgramiv(pm_ProgramID, GL_LINK_STATUS, &f_Success);
    if (not f_Success)
    {
        glGetProgramInfoLog(pm_ProgramID, 512, NULL, f_InfoLog);
        logger->Error
        (
            fmt::format("ERROR::SHADER::PROGRAM::LINKING_FAILED: {}", f_InfoLog),
            fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
        );
    }

    for (auto& lv_ShaderID : fp_ShaderIDs)
    {
        glDetachShader(pm_ProgramID, lv_ShaderID);
        glDeleteShader(lv_ShaderID);  // Delete the shader as it's no longer needed
    }

    glValidateProgram(pm_ProgramID);

    glGetProgramiv(pm_ProgramID, GL_VALIDATE_STATUS, &f_Success);
    if (not f_Success)
    {
        glGetProgramInfoLog(pm_ProgramID, 512, NULL, f_InfoLog);
        logger->Error
        (
            fmt::format("Shader validation error: {}", f_InfoLog), 
            fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
        );
    }

    AutoCaptureActiveUniforms(logger);
}

void
    AutoCaptureActiveUniforms //gets uniforms detected by current glContext, then puts them into a map of the form <uniform-name, uniformLocation>
    (
        Logger* fp_RenderingLogger
    ) 
{
    int f_Total = -1;
    glGetProgramiv(pm_ProgramID, GL_ACTIVE_UNIFORMS, &f_Total);

    for (int lv_Index = 0; lv_Index < f_Total; ++lv_Index)
    {
        int name_len = -1, num = -1;
        GLenum type = GL_ZERO;
        char name[100];

        glGetActiveUniform(pm_ProgramID, GLuint(lv_Index), sizeof(name) - 1, &name_len, &num, &type, name);

        name[name_len] = 0;
        GLint f_UniformLocation = glGetUniformLocation(pm_ProgramID, name);

        const char* f_temp = name;

        pm_Uniforms.insert({ f_temp, f_UniformLocation });

        fp_RenderingLogger->Debug
        (
            fmt::format("Uniform #: {}, Type(GLenum): {}, Name: {}, Location(GLint): {}", lv_Index, type, f_temp, f_UniformLocation),
            fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
        );
    }
}

///////////////////////////////////////////////
// Create Shaders
///////////////////////////////////////////////

int
    CreateShader //creates, compiles and attaches desired shader type to current shaderprogram
    (
        const string& fp_ShaderSourceCode,
        GLuint fp_ShaderType,
        Logger* fp_RenderingLogger
    ) 
{
    int f_ShaderID = glCreateShader(fp_ShaderType);

    if (f_ShaderID == 0)
    {
        return 0;
    }

    const char* f_Cstr = fp_ShaderSourceCode.c_str(); //idk why cpp makes me do this in two lines but whatever

    glShaderSource(f_ShaderID, 1, &f_Cstr, NULL);
    glCompileShader(f_ShaderID);

    int success;
    GLchar infoLog[512];

    // After glCompileShader(f_ShaderID);
    glGetShaderiv(f_ShaderID, GL_COMPILE_STATUS, &success);

    if (not success)
    {
        glGetShaderInfoLog(f_ShaderID, 512, NULL, infoLog);
        fp_RenderingLogger->Error
        (
            fmt::format("Shader compilation error: {}", infoLog), 
            fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
        );

        return 0; // Or handle the error appropriately
    }

    return f_ShaderID;
}

//////////////////////////////////////////////
// Utility Functions
//////////////////////////////////////////////

bool
    ReadFileIntoString
    (
        const string& fp_ScriptFilePath,
        string& fp_SourceCode,
        Logger* fp_RenderingLogger
    )
{
    size_t f_LastDotIndex = fp_ScriptFilePath.rfind('.');  // Extract file extension assuming fmt::format "filename.ext"

    if (f_LastDotIndex == string::npos)
    {
        fp_RenderingLogger->Error
        (
            fmt::format("No file extension found for GLSL Shader at specified filepath: {}", fp_ScriptFilePath),
            fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
        );
        return false;
    }

    string f_FileExtension = fp_ScriptFilePath.substr(f_LastDotIndex);

    if (f_FileExtension != ".fs" and f_FileExtension != ".vs" and f_FileExtension != ".glsl" and f_FileExtension != ".frag" and f_FileExtension != ".vert")
    {
        fp_RenderingLogger->Error
        (
            fmt::format("Invalid file extension found when GLSL Shader was expected at specified filepath: {}", fp_ScriptFilePath), 
            fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
        );
        return false;
    }

    ifstream f_FileStream(fp_ScriptFilePath);

    if (not f_FileStream)
    {
        fp_RenderingLogger->Error
        (
            fmt::format("Shader failed to load at file path: {}", fp_ScriptFilePath),
            fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
        );
        return false;
    }

    stringstream f_Buffer;
    f_Buffer << f_FileStream.rdbuf();
    fp_SourceCode = f_Buffer.str();

    fp_RenderingLogger->Debug
    (
        fmt::format("Shader successfully loaded at file path: {}", fp_ScriptFilePath), 
        fmt::format("OpenGL::ShaderProgram: {}:{}", pm_ProgramName, pm_ProgramID)
    );

    return true;
}

    ///////////////////////////////////////////////
// Generic Uniform Setters 
///////////////////////////////////////////////

/// Matrices 

// void 
//     SetUniform(const string& fp_UniformName, const mat4s& fp_Matrix) 
// {
//     glUniformMatrix4fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// void
//     SetUniform(const string& fp_UniformName, const mat3s& fp_Matrix)
// {
//     glUniformMatrix3fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// void
//     SetUniform(const string& fp_UniformName, const mat2s& fp_Matrix)
// {
//     glUniformMatrix2fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// void
//     SetUniform(const string& fp_UniformName, const mat2x3s& fp_Matrix)
// {
//     glUniformMatrix2x3fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// void
//     SetUniform(const string& fp_UniformName, const mat3x2s& fp_Matrix)
// {
//     glUniformMatrix3x2fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// void
//     SetUniform(const string& fp_UniformName, const mat2x4s& fp_Matrix)
// {
//     glUniformMatrix2x4fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// void
//     SetUniform(const string& fp_UniformName, const mat4x2s& fp_Matrix)
// {
//     glUniformMatrix4x2fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// void
//     SetUniform(const string& fp_UniformName, const mat3x4s& fp_Matrix)
// {
//     glUniformMatrix3x4fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// void
//     SetUniform(const string& fp_UniformName, const mat4x3s& fp_Matrix)
// {
//     glUniformMatrix4x3fv(pm_Uniforms.at(fp_UniformName), 1, GL_FALSE, fp_Matrix.raw[0]);
// }

// /// int vecs 

// void 
//     SetUniform(const string& fp_UniformName, const int fp_Value)
// {
//     glUniform1i(pm_Uniforms.at(fp_UniformName), fp_Value);
// }

// void
//     SetUniform(const string& fp_UniformName, const ivec2s& fp_Value)
// {
//     glUniform2i(pm_Uniforms.at(fp_UniformName), fp_Value.x, fp_Value.y);
// }

// void
//     SetUniform(const string& fp_UniformName, const ivec3s& fp_Value)
// {
//     glUniform3i(pm_Uniforms.at(fp_UniformName), fp_Value.x, fp_Value.y, fp_Value.z);
// }

// void
//     SetUniform(const string& fp_UniformName, const ivec4s& fp_Value)
// {
//     glUniform4i(pm_Uniforms.at(fp_UniformName), fp_Value.x, fp_Value.y, fp_Value.z, fp_Value.w);
// }

// /// float vecs 

// void 
//     SetUniform(const string& fp_UniformName, const float fp_Value) 
// {
//     glUniform1f(pm_Uniforms.at(fp_UniformName), fp_Value);
// }

// void
//     SetUniform(const string& fp_UniformName, const vec2s& fp_Value)
// {
//     glUniform2f(pm_Uniforms.at(fp_UniformName), fp_Value.x, fp_Value.y);
// }

// void 
//     SetUniform(const string& fp_UniformName, const vec3s& fp_Value) 
// {
//     glUniform3f(pm_Uniforms.at(fp_UniformName), fp_Value.x, fp_Value.y, fp_Value.z);
// }

// void 
//     SetUniform(const string& fp_UniformName, const vec4s& fp_Value) 
// {
//     glUniform4f(pm_Uniforms.at(fp_UniformName), fp_Value.x, fp_Value.y, fp_Value.z, fp_Value.w);
// }

#endif /*STARLIGHT_TESTS_SHADERPROGRAM_H_G*/

