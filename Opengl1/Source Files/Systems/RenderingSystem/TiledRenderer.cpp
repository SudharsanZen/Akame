#include "Systems\RenderingSystem\TiledRenderer.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include"Log/Log.h"
#include<sstream>

TiledRenderer::TiledRenderer(std::string shaderLocation)
{
	progID = 0;
	outTex = 0;
    compileComputeShader(shaderLocation);
    height = 800;
    width = 600;
    glGenTextures(1,&outTex);
    glBindTexture(GL_TEXTURE_2D,outTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,800,600,0,GL_RGBA,GL_FLOAT,NULL);

}

void TiledRenderer::updateBufferSize(int height, int width)
{
    this->height = height;
    this->width = width;
	drfb.updateBufferSize(height,width);
    glBindTexture(GL_TEXTURE_2D,outTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width,height, 0, GL_RGBA, GL_FLOAT, NULL);
}

void TiledRenderer::bindFrameBuffer()
{
	drfb.bindFrameBuffer();
}

void TiledRenderer::unBindFrameBuffer()
{
	drfb.unBindFrameBuffer();
}

void TiledRenderer::bindTextures()
{
   
    glBindImageTexture(0, drfb.AlbedoSpec, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(1, drfb.Normal, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(2, drfb.Position, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,drfb.depthBuffer);
    glBindImageTexture(4, outTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
 

}


void TiledRenderer::outPutToQaud(Camera& cam, std::shared_ptr<LightSystem> lsys)
{
   
    glUseProgram(progID);
 
    //bind the G-Buffers
    bindTextures();

    //bind the uniform buffer storing list of point lights
    lsys->updatePointLightContents();
    lsys->bindPointLightBuffer(5);

    glUniform1i(glGetUniformLocation(progID,"height"),height);
    glUniform1i(glGetUniformLocation(progID,"width"),width);
    glUniform3fv(glGetUniformLocation(progID, "viewPos"),1,glm::value_ptr(cam.transform.position));
    glUniform1i(glGetUniformLocation(progID, "NUM_POINT_LIGHT"), int(lsys->ptVector.size()));
    glUniformMatrix4fv(glGetUniformLocation(progID,"projInv"), 1, GL_FALSE, glm::value_ptr(glm::inverse(cam.getProjectionMatrix())));
    glUniformMatrix4fv(glGetUniformLocation(progID, "viewMat"), 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
    glDispatchCompute(width/32,(height/32+1),1);
    glUniform1i(glGetUniformLocation(progID, "NUM_DIR_LIGHT"), int(lsys->drVector.size()));
    auto dirVarName = [](std::string structVar, int index)
    {
        std::stringstream str;
        str << "DIR_L[" << index << "]." + structVar;
        return str.str();
    };
    int ith_dirLight=0;
    for (auto& l : lsys->drVector)
    {
       
        std::string var=dirVarName("lightDir", ith_dirLight);
        glUniform3fv(glGetUniformLocation(progID,var.c_str()), 1, glm::value_ptr(l.lightDir));
        var = dirVarName("lightColor", ith_dirLight);
        glUniform3fv(glGetUniformLocation(progID, var.c_str()), 1, glm::value_ptr(l.lightColor));
        var = dirVarName("ambient", ith_dirLight);
        glUniform3fv(glGetUniformLocation(progID, var.c_str()), 1, glm::value_ptr(l.ambient));
        var = dirVarName("intensity", ith_dirLight);
        glUniform1f(glGetUniformLocation(progID, var.c_str()),l.intensity);
        ith_dirLight++;
    }
    

    glMemoryBarrier(GL_ALL_BARRIER_BITS);


    std::shared_ptr<Shader> quadRend = ShaderManager::GetShader("SCREENSHADER");
    quadRend->useShaderProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,outTex);

    drfb.quad.renderMesh();

}

void TiledRenderer::drawPositionBuffer()
{
	drfb.drawPositionBuffer();
}

void TiledRenderer::drawNormalBuffer()
{
	drfb.drawNormalBuffer();
}

void TiledRenderer::drawAlbedoBuffer()
{
	drfb.drawAlbedoBuffer();
}

TiledRenderer::~TiledRenderer()
{
    if (outTex)
    {
        glDeleteTextures(1,&outTex);
        outTex = 0;

    }
    if (progID)
    {
        glDeleteProgram(progID);
        progID = 0;
    }
}

char** TiledRenderer::readShaderFile(std::string fileName, int& len)
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
        ENGINE_CORE_ERROR("SHADER_CLASS::can't open File: " + fileName);
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

void TiledRenderer::compileComputeShader(std::string fileName)
{
    progID=glCreateProgram();

    unsigned int compShader = glCreateShader(GL_COMPUTE_SHADER);
    int l;
    char** code = readShaderFile(fileName,l);
    
    ENGINE_CORE_TRACE("COMPUTECODE:");
    /*for (int i = 0; i < l; i++)
    {
        std::cout << code[i];
    }*/
    glShaderSource(compShader,l,code,nullptr);
    glCompileShader(compShader);
    int success;
    glGetShaderiv(compShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint errorCodeLen;
        char log[1024];
        ENGINE_CORE_ERROR("SHADER_TYPE(COMPUTE)::FAILED_TO_COMPILE!");
        glGetShaderInfoLog(compShader, 1024, &errorCodeLen, log);
        ENGINE_CORE_INFO(log);
    }
   

    glAttachShader(progID,compShader);

    glLinkProgram(progID);

    // Check if there were some issues when linking the shader.

    glGetProgramiv(progID, GL_LINK_STATUS, &success);

    if (!success)

    {
        GLint errorCodeLen;
        char log[1024];
        glGetProgramInfoLog(progID, 1024, &errorCodeLen, log);

        printf("Error: Linker log:\n%s\n", log);
        ENGINE_CORE_INFO(log);
    }
    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    ENGINE_CORE_TRACE("max global (total) work group counts x:{0:d} y:{1:d} z:{2:d}\n",
        work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
    int work_grp_size[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

    ENGINE_CORE_TRACE("max local (in one shader) work group sizes x:{0:d} y:{1:d} z:{2:d}\n",
        work_grp_size[0], work_grp_size[1], work_grp_size[2]);

    int work_grp_inv;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    ENGINE_CORE_TRACE("max local work group invocations {0:d}\n", work_grp_inv);

    freeCodePointer(code, l);
    code = nullptr;

}

void TiledRenderer::freeCodePointer(char** code, int len)
{
    /*
    * freeing the memoryused by the code pointer.
    * call this after compiling the code for shaders.
    */
    for (int i = 0; i < len; i++)
        free(code[i]);

    free(code);
}

