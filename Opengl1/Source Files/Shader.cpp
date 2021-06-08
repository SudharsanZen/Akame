#include "Shader.h"
#include<fstream>
#include<math.h>


Shader::Shader(std::string vertexShaderDir, std::string fragmentShaderDir)
{
    //contructor
    //callAfterUseProgram = [](GLuint progID) {};
    vertDir = vertexShaderDir;
    fragDir = fragmentShaderDir;
    programID = 0;
    uboMatricesIndex = -1;
    compileShader();
}

Shader::~Shader()
{
    deleteProgram();

}
void Shader::deleteProgram() 
{
    if (programID)
    {

        glDeleteProgram(programID);
        programID = 0;
    }
}
void Shader::useShaderProgram()
{
    //use created program
    if (programID)
    {
        if (uboMatricesIndex == -1)
        {
            uboMatricesIndex = glGetUniformBlockIndex(programID, "Matrices");
        }
        glUniformBlockBinding(programID, uboMatricesIndex, 0);
        glUseProgram(programID);
        //callAfterUseProgram(programID);
    }
}


char** Shader::readShaderFile(std::string fileName,unsigned int &len)
{
    /*read shader file content from the given location
    and then store the content in an array of strings
    with each string containing a part of the code*/

    const GLuint CODE_BLOCK_SIZE = 512;
    std::fstream file(fileName, std::ios::in);
    std::string content;
    char cnt[512];

    if (file)
    {
        while (!file.eof())
        {

            file.getline(cnt, CODE_BLOCK_SIZE, '\n');

            content.append(cnt);
            if (!file.eof())
                content += "\n";
        }
    }
    else
    {
        std::cout << "ERROR::SHADER_CLASS::can't open File: " << fileName << "\"";
    }
    size_t contentLength = content.length();
    unsigned int numOfChunks = ceil((float)contentLength / (float)(CODE_BLOCK_SIZE - 1));

    char** code = new char* [numOfChunks];
    size_t pos = 0, i = 0;


    //seperate the read content into code blocks and store each blocks in char** pointer
    for (i = 0; i < numOfChunks; i++)
    {
        code[i] = new char[CODE_BLOCK_SIZE];
        std::string sub = content.substr(i * (CODE_BLOCK_SIZE - 1), CODE_BLOCK_SIZE - 1);
        sub.copy(code[i], CODE_BLOCK_SIZE - 1, pos);
        code[i][sub.length()] = '\0';
        //std::cout << code[i];

    }

    len = numOfChunks;
    return code;

}


bool Shader::compileShader()
{
    /*
    * compile the shader and set programID
    */
    deleteProgram();
    GLuint v_ShaderID, f_ShaderID;
    char** vCode,**fCode;
    unsigned int vCount, fCount;
    GLint success;

    //read shaderFiles and get a char** pointer to it's content string
    vCode = readShaderFile(vertDir, vCount);
    fCode = readShaderFile(fragDir, fCount);   

    //Generate Shader objects
    v_ShaderID = glCreateShader(GL_VERTEX_SHADER);
    f_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    

    //compile and add the shader
    addShader(vCode,v_ShaderID,vCount,GL_VERTEX_SHADER);
    addShader(fCode,f_ShaderID,fCount,GL_FRAGMENT_SHADER);

    programID = glCreateProgram();

    //attach the compiled shader to shader program and link
    glAttachShader(programID,v_ShaderID);
    glAttachShader(programID, f_ShaderID);
    glLinkProgram(programID);

    //check if linking is sucessful
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar log[1024];
        glGetProgramInfoLog(programID, 1024, NULL, log);
        std::cout << "\nERROR::LINKING_PROGRAM: " << log;
        return false;
    }

    //deleting vertex and fragment shader object since it has been compiled
    glDeleteShader(v_ShaderID);
    glDeleteShader(f_ShaderID);

    //de allocating the char** pointer to the shaderFileContent 
    freeCodePointer(vCode,vCount);
    freeCodePointer(fCode,fCount);

    //getIndex of unfirom buffer for view and projection matrix
  

    return true;
}

void Shader::addShader(char** code,GLuint shaderID,GLuint codeCount,GLenum shaderType)
{
    /*
    * compile shader code into the given shaderObject
    */
    int success;
    glShaderSource(shaderID,codeCount,code,nullptr);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID,GL_COMPILE_STATUS,&success);
    
    if (!success)
    {
        GLint errorCodeLen;
        char log[1024];
        std::cout << "ERROR::SHADER_TYPE("<<shaderType<<")::FAILED_TO_COMPILE:" << std::endl;
        glGetShaderInfoLog(shaderID, 1024, &errorCodeLen, log);
        std::cout << log;
    }
}

void Shader::freeCodePointer(GLchar** code,unsigned int len)
{
    /*
    * freeing the memoryused by the code pointer.
    * call this after compiling the code for shaders.
    */
    for (int i = 0; i < len; i++)
        free(code[i]);

    free(code);
}

