#include "Rendering\System\TiledRenderer.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include"Core/Log/Log.h"
#include<sstream>

TiledRenderer::TiledRenderer(std::string shaderLocation)
{
	progID = 0;
	outTex = 0;
    compileComputeShader(shaderLocation);
    ParseUniforms();
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

void TiledRenderer::set4x4Matrixfv(std::string name, glm::mat4 value)
{
    glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void TiledRenderer::set4x4MatrixfvArray(std::string name,unsigned int index, glm::mat4 value)
{
    glUniformMatrix4fv(GetUniformLocation(name.c_str())+index, 1, GL_FALSE, glm::value_ptr(value));
}

void TiledRenderer::setVec3(std::string name, glm::vec3 value)
{
    glUniform3fv(GetUniformLocation(name.c_str()),1,glm::value_ptr(value));
}

void TiledRenderer::setFloat(std::string name, float value)
{
    glUniform1f(GetUniformLocation(name.c_str()), value);
}

void TiledRenderer::setInt(std::string name, int value)
{
    glUniform1i(GetUniformLocation(name.c_str()), value);
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

void TiledRenderer::unBindFrameBuffer(unsigned int frameBuffer)
{
	drfb.unBindFrameBuffer(frameBuffer);
}

void TiledRenderer::bindTextures()
{
   
    glBindImageTexture(0, drfb.AlbedoRough, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(1, drfb.Normal, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(2, drfb.Position, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,drfb.depthBuffer);
    glBindImageTexture(4, outTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
    glBindImageTexture(7, drfb.PBR, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
 

}

void TiledRenderer::ParseUniforms()
{
    //max allowed length of a uniform variable name
    int max = 0;
    glGetProgramiv(progID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max);
    char* name = (char*)malloc((max + (unsigned long long)1) * sizeof(char));
    if (progID)
    {

        int uniformCount = 0;

        glGetProgramiv(progID, GL_ACTIVE_UNIFORMS, &uniformCount);
        for (int i = 0; i < uniformCount; i++)
        {
            int nameLength = 0;
            int size = 0;
            GLenum type;
            glGetActiveUniform(progID, i, max + 1, &nameLength, &size, &type, name);
            if (name)
            {

                std::string st = name;
                uniformNameToLoc[st] = glGetUniformLocation(progID, st.c_str());
                //ENGINE_CORE_CRITICAL(st + ": {0:d}", uniformNameToLoc[st]);
            }
        }
    }
    free(name);
}

unsigned int TiledRenderer::GetUniformLocation(std::string varName) { return uniformNameToLoc[varName]; }


void TiledRenderer::setUpShader(Camera& cam, std::shared_ptr<LightSystem> lsys)
{
    glUseProgram(progID);

    //bind the G-Buffers
    bindTextures();

    //bind the uniform buffer storing list of point lights
    lsys->updatePointLightContents();
    lsys->bindPointLightBuffer(5);

    glUniform1i(GetUniformLocation("height"), height);
    glUniform1i(GetUniformLocation("width"), width);
    glUniform1i(GetUniformLocation("numOfFrustum"), lsys->dirLightSpace.size());
    glUniform3fv(GetUniformLocation("viewPos"), 1, glm::value_ptr(cam.transform.GetGlobalPosition()));
    glUniform1i(GetUniformLocation("NUM_POINT_LIGHT"), int(lsys->ptVector.size()));
    glUniformMatrix4fv(GetUniformLocation("projInv"), 1, GL_FALSE, glm::value_ptr(glm::inverse(cam.getProjectionMatrix())));
    glUniformMatrix4fv(GetUniformLocation("viewMat"), 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
  
    glUniform1i(GetUniformLocation("NUM_DIR_LIGHT"), int(lsys->drVector.size()));
  
    int ith_dirLight = 0;
    for (auto& l : lsys->drVector)
    {
        char ith ='0'+ith_dirLight;
        std::string varBase= std::string("DIR_L[0].");
        varBase[6] = ith;

        std::string var = varBase+"lightDir";
        glUniform3fv(GetUniformLocation(var.c_str()), 1, glm::value_ptr(l.lightDir));
        var = varBase +"lightColor";
        glUniform3fv(GetUniformLocation(var.c_str()), 1, glm::value_ptr(l.lightColor));
        var = varBase + "ambient";
        glUniform3fv(GetUniformLocation(var.c_str()), 1, glm::value_ptr(l.ambient));
        var = varBase + "intensity";
        glUniform1f(GetUniformLocation(var.c_str()), l.intensity);
        ith_dirLight++;
    }

}

void TiledRenderer::outPutToQaud()
{
   
    int x = glm::ceil((float)width / 32.0f), y = glm::ceil((float)height / 32.0f);
  
    
    glDispatchCompute(x,y, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);


    std::shared_ptr<Shader> quadRend = ShaderManager::GetShader("SCREENSHADER");
    quadRend->useShaderProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,outTex);
    glBindVertexArray(Mesh::getVAO());
    drfb.quad.renderMesh();
    glBindVertexArray(0);
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

char** TiledRenderer::readShaderFile(std::string currPath, int& len)
{
    /*read shader file content from the given location
    and then store the content in an array of strings
    with each string containing a part of the code*/

    const GLuint CODE_BLOCK_SIZE = 512;
    std::fstream file(currPath, std::ios::in);
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
        ENGINE_CORE_ERROR("SHADER_CLASS::can't open File: " + currPath);
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

void TiledRenderer::compileComputeShader(std::string currPath)
{
    progID=glCreateProgram();

    unsigned int compShader = glCreateShader(GL_COMPUTE_SHADER);
    int l;
    char** code = readShaderFile(currPath,l);
    
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

