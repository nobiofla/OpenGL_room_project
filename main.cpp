#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/Mesh.h"
#include "Libs/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

float yaw = 0.0f, pitch = 0.0f;


//Number of Sqaures
const GLbyte squareNum = 55;

//Array for store pre-loaded textures
unsigned int squareTextures[squareNum];


//Vertex Shader
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";


void CreateTriangle()
{
    GLfloat vertices[] =
    {
        //pos                   //TexCoord
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        0.0, -1.0f, 1.0f,       0.5f, 0.0f,
        1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.0f
    };

    unsigned int indices[] = 
    {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2,
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 20, 12);
    
    // for(int i = 0; i < 2; i++) {
    //     meshList.push_back(obj1);
    // }
}

void CreateSquare()
{
    GLfloat SQvertices[] =
    {
        // Side                 //TexCoord
        // up
        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,          
        1.0f, 1.0f, -1.0f,      0.0f, 1.0f,    
        -1.0f, 1.0f, -1.0f,     1.0f, 1.0f,     
        -1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        
        // down
        1.0f, -1.0f, 1.0f,      0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,     0.0f, 1.0f,      
        -1.0f, -1.0f, -1.0f,    1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,     1.0f, 0.0f,

        // left
        -1.0f, 1.0f, 1.0f,      0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,     0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,    1.0f, 0.0f,

        // right
        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,      0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,     1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,      1.0f, 0.0f,

        // front
        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,      0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,     1.0f, 1.0f,  
        -1.0f, 1.0f, 1.0f,      1.0f, 0.0f,

        // back
        1.0f, 1.0f, -1.0f,      0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,     0.0f, 1.0f, 
        -1.0f, -1.0f, -1.0f,    1.0f, 1.0f,     
        -1.0f, 1.0f, -1.0f,     1.0f, 0.0f,
    };

    unsigned int SQindices[] = 
    {

        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,
        
        20, 21, 22,
        22, 23, 20,

    };

    Mesh *obj2 = new Mesh();
    obj2 -> CreateMesh(SQvertices, SQindices, 5*24, 3*12);
    for(int i = 0; i < squareNum; i++) {
        meshList.push_back(obj2);
    }
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    static float lastX = mainWindow.getBufferWidth() / 2.0f;
    static float lastY = mainWindow.getBufferHeight() / 2.0f;

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f)   pitch = 89.0f;
    if(pitch < -89.0f)  pitch = -89.0f;
}

//method loadTexture 
unsigned int loadTexture(const char *textureFile){

    //texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        //bind image with data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load texture"<<std::endl;
    }

    stbi_image_free(data);
    return texture;
}

int main()
{
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    CreateSquare();

    CreateShaders();

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);
    
    // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraPos = glm::vec3(1.0f, 0.5f, 2.0f);
    // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, -0.3f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);

    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, -cameraRight));

    glfwSetCursorPosCallback(mainWindow.getWindow(), mouse_callback);
    glfwSetInputMode(mainWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    //Pre-Loaded Texture 
    unsigned int textureContainer = loadTexture("Textures/container.jpg");
    unsigned int textureCloth = loadTexture("Textures/cloth.jpg");

    unsigned int textureWall = loadTexture("Textures/wall.jpg");
    unsigned int textureFloor = loadTexture("Textures/floor.jpg");
    
    unsigned int textureWoodPlate1 = loadTexture("Textures/woodPlate1.jpg");
    unsigned int textureWoodPlate2 = loadTexture("Textures/woodPlate2.jpg");
    unsigned int textureWoodPlateReal = loadTexture("Textures/woodPlateReal.jpg");
    unsigned int textureWoodPlateReal2 = loadTexture("Textures/woodPlateReal2.jpg");

    unsigned int textureBackground = loadTexture("Textures/background.jpg");

    unsigned int textureTableReal1 = loadTexture("Textures/tableReal.jpg");
    unsigned int textureTableReal2 = loadTexture("Textures/tableReal2.jpg");
    unsigned int textureTable1 = loadTexture("Textures/table1.jpg");
    unsigned int textureTable4 = loadTexture("Textures/table4.jpg");

    unsigned int textureBin = loadTexture("Textures/bin.jpg");
    unsigned int textureBin2 = loadTexture("Textures/bin2.jpg");
    
    unsigned int texturePillow = loadTexture("Textures/pillow.jpg");

    unsigned int textureWindowWhite = loadTexture("Textures/windowWhite.jpg");
    unsigned int textureCurtainSolid = loadTexture("Textures/curtainSolid.jpg");
    unsigned int textureCurtainCloth = loadTexture("Textures/curtainCloth.jpg");

    unsigned int textureDoor = loadTexture("Textures/door.jpg");
    unsigned int textureDoorEdge = loadTexture("Textures/doorEdge.jpg");
    unsigned int textureDoorPlate = loadTexture("Textures/doorPlate.jpg");
    unsigned int textureDoorKnob = loadTexture("Textures/doorKnob.jpg");

    unsigned int textureCalendar = loadTexture("Textures/calendar.jpg");

    unsigned int textureLampWhite = loadTexture("Textures/tableLampWhite.jpg");
    unsigned int textureLampBlue = loadTexture("Textures/tableLampBlue.jpg");
    
    unsigned int textureChairBack = loadTexture("Textures/chairBack.jpg");
    unsigned int textureChairCushion = loadTexture("Textures/chairCushion.jpg");
    unsigned int textureChairLeg = loadTexture("Textures/chairLeg.jpg");
    unsigned int textureChairLumbar = loadTexture("Textures/chairLumbar.jpg");
    unsigned int textureChairWheels = loadTexture("Textures/chairWheels.jpg");

    unsigned int textureClock = loadTexture("Textures/clock.jpg");


    //Loop until window closed
    while (!mainWindow.getShouldClose())
    {
        //Get + Handle user input events
        glfwPollEvents();

        glm::vec3 direction;
        direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        direction.y = sin(glm::radians(pitch));
        direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraDirection = glm::normalize(direction);

        cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        cameraUp = glm::normalize(glm::cross(cameraDirection, -cameraRight));

        if(glfwGetKey(mainWindow.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraDirection * 0.01f;
        if(glfwGetKey(mainWindow.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraDirection * 0.01f;
        if(glfwGetKey(mainWindow.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= cameraRight * 0.01f;
        if(glfwGetKey(mainWindow.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += cameraRight * 0.01f;

        //Clear window
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(186.0f/255.0f, 228.0f/255.0f, 228.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw here
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetUniformLocation("model");
        uniformView = shaderList[0].GetUniformLocation("view");
        uniformProjection = shaderList[0].GetUniformLocation("projection");


        //square's position to draw
        glm::vec3 squarePositions[] =
        {
            //         Positions                // i    description
            // wall
            glm::vec3(1.0f, 1.0f, 0.0f),        // 0    wall 1
            glm::vec3(1.0f, 0.0f, 1.0f),        // 1    floor
            glm::vec3(0.0f, 1.0f, 1.0f),        // 2    wall 2

            // top, bottom plate wall 2
            glm::vec3(3.0f, 79.0f, 1.0f),       // 3    top
            glm::vec3(3.0f, 2.0f, 1.0f),        // 4    bottom

            // top, bottom plate wall 1
            glm::vec3(1.0f, 79.0f, 3.0f),       // 5    top
            glm::vec3(1.0f, 2.0f, 3.0f),        // 6    bottom

            // middle plate wall 2
            glm::vec3(3.0f, 19.0f, 1.0f),       // 7    middle-top
            glm::vec3(3.0f, 7.0f, 1.0f),        // 8    middle-bottom

            // middle plate wall 1
            glm::vec3(1.0f, 19.0f, 3.0f),       // 9    middle-top

            //vertical plate wall 2
            glm::vec3(3.0f, 1.0f, 23.0f),       // 10   vertical-left
            glm::vec3(3.0f, 1.0f, 9.0f),        // 11   vertical-right

            //vertical plate wall 1
            glm::vec3(9.0f, 1.0f, 3.0f),        // 12   vertical-left

            //corner plate
            glm::vec3(2.0f, 1.0f, 2.0f),        // 13   corner
            
            // table
            glm::vec3(1.3f, 24.0f, 1.67f),    // 14   top
            glm::vec3(1.3f, 0.94f, 2.9f),     // 15   right big chest
            glm::vec3(3.3f, 0.94f, 46.0f),    // 16   left leg 1 (close to wall 2)
            glm::vec3(17.5f, 0.94f, 46.0f),   // 17   left leg 2 (far from wall 2)
            glm::vec3(1.4f, 8.5f, 3.8f),      // 18   left chest
            glm::vec3(55.0f, 35.5f, 2.9f),    // 19   right line 1
            glm::vec3(55.0f, 25.5f, 2.9f),    // 20   right line 2
            glm::vec3(55.0f, 14.5f, 2.9f),    // 21   right line 3
            
            //bin
            glm::vec3(3.2f, 15.40f, 1.5f),      // 22   bin - top
            glm::vec3(3.2f, 0.94f, 1.5f),       // 23   bin - below

            // door
            glm::vec3(2.25f, 1.125f, 6.0f),     // 24   door 
            glm::vec3(2.25f, 4.0f, 4.0f),       // 25   plate
            glm::vec3(2.25f, 7.0f, 3.0f),       // 26   bottom edge
            glm::vec3(2.25f, 101.0f, 3.0f),     // 27   top edge
            glm::vec3(61.0f, 1.125f, 3.0f),     // 28   left edge
            glm::vec3(108.0f, 1.125f, 3.0f),    // 29   middle edge
            glm::vec3(155.0f, 1.125f, 3.0f),    // 30   right edge
            glm::vec3(17.0f, 16.0f, 3.0f),      // 31   left knob
            glm::vec3(37.0f, 16.0f, 3.0f),      // 32   right knob

            // pillow
            glm::vec3(7.5f, 0.94f, 3.5f),       // 33   pillow
            
            // calendar
            glm::vec3(1.5f, 4.5f, 3.0f),        // 34   calendar

            // window
            glm::vec3(3.0f, 2.7f, 2.7f),        // 35   white Window
            glm::vec3(2.6f, 19.0f, 2.7f),       // 36   curtain (close to wood from wall 2)
            glm::vec3(7.6f, 2.65f, 7.2f),       // 37   Right curtain (far from wall 2)
            glm::vec3(7.6f, 2.65f, 8.75f),      // 38   Left curtain (far from wall 2)

            // lamp
            glm::vec3(2.9f, 25.9f, 16.2f),      // 39   lamp base
            glm::vec3(7.8f, 4.7f, 43.2f),       // 40   lamp neck 1 (from base)
            glm::vec3(22.9f, 5.8f, 129.7f),     // 41   lamp neck 2
            glm::vec3(4.8f, 48.8f, 6.4f),       // 42   lamp head
            glm::vec3(27.0f, 94.4f, 6.4f),      // 43   lamp light
            // chair
            glm::vec3(60.0f, 1.150f, 84.0f),    // 44   leg core
            glm::vec3(60.0f, 2.5f, 7.0f),       // 45   leg 1 (z)
            glm::vec3(5.0f, 2.5f, 84.0f),       // 46   leg 2 (x)
            glm::vec3(59.5f, 5.0f, 70.0f),      // 47   wheel 1
            glm::vec3(40.5f, 5.0f, 70.0f),      // 48   wheel 2
            glm::vec3(50.0f, 5.0f, 79.5f),      // 49   wheel 3
            glm::vec3(50.0f, 5.0f, 60.5f),      // 50   wheel 4
            glm::vec3(6.5f, 9.5f, 10.5f),       // 51   cushion
            glm::vec3(110.0f, 8.0f, 35.0f),     // 52   lumbar core
            glm::vec3(55.0f, 5.5f, 9.35f),      // 53   back

            // clock
            glm::vec3(13.5f, 17.25f, 3.0f),       // 54   clock
            
        };
        

        glm::mat4 view (1.0f);

        glm::mat4 cameraPosMat (1.0f);
        cameraPosMat[0][3] = -cameraPos.x;
        cameraPosMat[1][3] = -cameraPos.y;
        cameraPosMat[2][3] = -cameraPos.z;

        glm::mat4 cameraRotateMat(1.0f);
        cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, -cameraDirection.x, 0.0f);
        cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, -cameraDirection.y, 0.0f);
        cameraRotateMat[2] = glm::vec4(cameraRight.z, cameraUp.z, -cameraDirection.z, 0.0f);
        
        // view = cameraRotateMat * glm::transpose(cameraPosMat);
        view = glm::lookAt(cameraPos, cameraPos + cameraDirection, up);

        //Object
        glm::mat4 model(1.0f);

        for (int i = 0; i < squareNum; i++)
        {
            glm::mat4 model (1.0f);

            // wall
            if (i == 0)  // wall 1
            {
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.02f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWall;
            }
            else if (i == 1) // floor
            {
                model = glm::scale(model, glm::vec3(1.0f, 0.02f, 1.0f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureFloor;
            }
            else if (i == 2) // wall 2
            {
                model = glm::scale(model, glm::vec3(0.02f, 1.0f, 1.0f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWall;
            }
            
            // wood plate
            else if (i <= 4) // (top/bottom) plate wall 2
            { 
                model = glm::scale(model, glm::vec3(0.01f, 0.025f, 1.0f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlateReal2;
            }
            else if (i <= 6) // (top/bottom) plate wall 1
            { 
                model = glm::scale(model, glm::vec3(1.0f, 0.025f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlateReal2;
            }
            else if (i <= 8) // middle plate wall 2
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.075f, 1.0f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlateReal2;
            }
            else if (i == 9) // middle-top plate wall 1
            {
                model = glm::scale(model, glm::vec3(1.0f, 0.075f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlateReal2;
            }
            else if (i <= 11) // vertical plate wall 2
            {
                model = glm::scale(model, glm::vec3(0.015f, 1.0f, 0.075f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlateReal;
                // squareTextures[i] = textureWoodPlate1;
            }
            else if (i == 12) // vertical plate wall 1
            {
                model = glm::scale(model, glm::vec3(0.075f, 1.0f, 0.015f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlateReal;
                // squareTextures[i] = textureWoodPlate1;
            }
            else if (i == 13) // corner plate
            {
                model = glm::scale(model, glm::vec3(0.025f, 1.0f, 0.025f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlateReal;
                // squareTextures[i] = textureWoodPlate1;
            }

            // table
            else if (i == 14) //table top
            {
                model = glm::scale(model, glm::vec3(0.25f, 0.02f, 0.55f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureTableReal1;
            }
            else if (i == 15) //table right big chest
            {
                model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.21f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureTableReal2;
            }
            else if (i >= 16 && i <= 17) //left leg 1
            {
                model = glm::scale(model, glm::vec3(0.03f, 0.25f, 0.03f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureTableReal2;
            }
            else if (i == 18) // left chest
            {
                model = glm::scale(model, glm::vec3(0.23f, 0.05f, 0.29f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureTableReal2;
            }
            else if (i > 18 && i <= 21) // right line 1 2 3
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.21f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureTable4;
            }

            // bin
            else if (i == 22) // bin top
            {
                model = glm::scale(model, glm::vec3(0.13f, 0.02f, 0.13f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureBin2;
            }
            else if (i == 23) // bin below
            {
                model = glm::scale(model, glm::vec3(0.13f, 0.15f, 0.13f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureBin;
            }

            // door
            else if (i == 24) // door
            {
                model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.005f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureDoor;
            }
            else if (i == 25) // plate
            {
                model = glm::scale(model, glm::vec3(0.6f, 0.2f, 0.0075f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureDoorPlate;
            }
            else if (i <= 27) // (bottom-top) edge
            {
                model = glm::scale(model, glm::vec3(0.6f, 0.0125f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureDoorEdge;
            }
            else if (i <= 30) // (left-middle-right) edge
            {
                model = glm::scale(model, glm::vec3(0.0125f, 0.6f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureDoorEdge;
            }
            else if (i <= 32) // left-right knob
            {
                model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureDoorKnob;
            }
            
            // pillow
            else if (i == 33) 
            {
                model = glm::scale(model, glm::vec3(0.20f, 0.03f, 0.20f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = texturePillow;
            }

            // calendar
            else if (i == 34)
            {
                model = glm::scale(model, glm::vec3(0.225f, 0.225f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureCalendar;
            }

            // window
            else if(i == 35) // white window
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.36f, 0.45f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWindowWhite;
            }
            else if(i == 36) // curtain (close to wood from wall 2)
            {
                model = glm::scale(model, glm::vec3(0.04f, 0.075f, 0.45f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureCurtainSolid;
            }
            else if(i == 37) // Right curtain (far from wall 2)
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.37f, 0.20f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureCurtainCloth;
            }
            else if(i == 38) // Left curtain (far from wall 2)
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.37f, 0.10f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureCurtainCloth;
            }

            // lamp
            else if(i == 39) // lamp base
            {
                model = glm::scale(model, glm::vec3(0.08f, 0.02f, 0.08f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureLampBlue;
            }
            else if(i == 40) // lamp neck 1
            {
                model = glm::scale(model, glm::vec3(0.03f, 0.14f, 0.03f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureLampBlue;
            }
            else if(i == 41) // lamp neck 2
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.14f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureLampWhite;
            }
            else if(i == 42) // lamp head
            {
                model = glm::scale(model, glm::vec3(0.05f, 0.02f, 0.2f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureLampBlue;
            }
            else if(i == 43) // lamp light
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.2f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureLampWhite;
            }

            // chair
            else if (i == 44) // leg core
            {
                model = glm::scale(model, glm::vec3(0.0125f, 0.1f, 0.0125f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureChairLeg;
            } 
            else if (i == 45) // leg 1 (z)
            {
                model = glm::scale(model, glm::vec3(0.0125f, 0.0125f, 0.15f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureChairLeg;
            }
            else if (i == 46) // leg 2 (x)
            {
                model = glm::scale(model, glm::vec3(0.15f, 0.0125f, 0.0125f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureChairLeg;
            }
            else if (i <= 50) // wheels (1-4)
            {
                model = glm::scale(model, glm::vec3(0.0150f, 0.005f, 0.0150f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureChairWheels;
            }
            else if (i == 51) // cushion
            {
                model = glm::scale(model, glm::vec3(0.1125f, 0.025f, 0.1f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureChairCushion;
            }
            else if (i == 52) // lumbar core
            {
                model = glm::scale(model, glm::vec3(0.0075f, 0.0375f, 0.03f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureChairLumbar;
            }
            else if (i == 53) // back
            {
                model = glm::scale(model, glm::vec3(0.015f, 0.075f, 0.1125f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureChairBack;
            }

            // clock
            else if (i == 54)
            {
                model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureClock;
            }


            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, squareTextures[i]);
            meshList[i]->RenderMesh();
        }

        glUseProgram(0);
        //end draw

        mainWindow.swapBuffers();
    }

    return 0;
}
