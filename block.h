#pragma once

#include <vector>
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define pi 3.14159265
const int totNo=7;

GLuint screenWidth = 600, screenHeight = 480;
bool fall = false;
/**************************************************************************************************************************************
ENVIRONMENT MODEL

                                                    top : four
                                            /---------------------------\
                                           /                             \
                                          /                               \
                        slantUpL : five  /                                 \
                                        /                                   \ slantUpR : three
                                       /                                     \
                                      /                                       \
                                     /                                         \
                                    |                                           |
                                    |                                           |
                                    |                                           |
                        sideL : six |                                           | sideR : two
                                    |                                           |
                                    |                                           |
                                    |                                           |
                                    |                                           |
                                     \                                         /
                                      \                                       /
                                       \                                     /
                                        \                                   /
                     slantDownL : seven  \                                 / slantDownR : one
                                          \                               /
                                           \       bottom : zero         /
                                            \---------------------------/

***************************************************************************************************************************************/
enum
{
    bottom, slantDownR, sideR, slantUpR, top, slantUpL, sideL, slantDownL
};

GLfloat radC = 0.19;
glm::vec2 cameraPos[] =
{
    glm::vec2( radC * sin(270.0 * 2 * pi / 180),  radC * cos(270.0 * 2 * pi / 180) ), // 270 degrees
    glm::vec2( radC * sin(247.5 * 2 * pi / 180),  radC * cos(247.5 * 2 * pi / 180) ), // 315 degrees
    glm::vec2( radC * sin(225.0 * 2 * pi / 180),  radC * cos(225.0 * 2 * pi / 180) ), //   0 degrees
    glm::vec2( radC * sin(202.5 * 2 * pi / 180),  radC * cos(202.5 * 2 * pi / 180) ), //  45 degrees
    glm::vec2( radC * sin(180.0 * 2 * pi / 180),  radC * cos(180.0 * 2 * pi / 180) ), //  90 degrees
    glm::vec2( radC * sin(157.5 * 2 * pi / 180),  radC * cos(157.5 * 2 * pi / 180) ), // 135 degrees
    glm::vec2( radC * sin(135.0 * 2 * pi / 180),  radC * cos(135.0 * 2 * pi / 180) ), // 180 degrees
    glm::vec2( radC * sin(112.5 * 2 * pi / 180),  radC * cos(112.5 * 2 * pi / 180) )  // 225 degrees
};
glm::vec3 cameraWorldUp[] =
{
    glm::vec3( 0,  1,  0),
    glm::vec3(-1,  1,  0),
    glm::vec3(-1,  0,  0),
    glm::vec3(-1, -1,  0),
    glm::vec3( 0, -1,  0),
    glm::vec3( 1, -1,  0),
    glm::vec3( 1,  0,  0),
    glm::vec3( 1,  1,  0)
};

class Collision
{
public:
    std::vector<glm::vec2> collision[8];
    bool checkCollision(int activeLane, glm::vec3 camZee)
    {
        for(int i = 0; i <=collision[activeLane].size(); i++){
            if((((camZee.z -0.2f) < collision[activeLane][i].x) && ((camZee.z-0.2f) > collision[activeLane][i].y))){
            cout<<"hell yeah";

                return true ;
            }
        }
        return false;
    }
};

class CubeInfo{
public:
    float scale;
    glm::vec3 Transition;
    glm::vec2 Regionz;
    glm::vec2 Regionx;
    float scalez;
    int lane;
};

class Environment
{
    Shader shader;
    Collision collisionData;

    CubeInfo C[totNo];
    GLuint VBO, VAO;
    GLushort modelLoc2, viewLoc2, projLoc2;

public:
    static Camera camera;
    static GLushort activeLane;
    Environment(GLfloat Z)
    {
        //activeLane = 0;
        shader = Shader("shaders/tileShader.vs", "shaders/tileShader.frag");
        float a = 0.1f, p = 1.0f, w = 0.005f;
        GLfloat vertices[] = {
            -a, -w, -p,  0.0f, 0.0f,
             a, -w, -p,  1.0f, 0.0f,
             a,  w, -p,  1.0f, 1.0f,
             a,  w, -p,  1.0f, 1.0f,
            -a,  w, -p,  0.0f, 1.0f,
            -a, -w, -p,  0.0f, 0.0f,

            -a, -w,  p,  0.0f, 0.0f,
             a, -w,  p,  1.0f, 0.0f,
             a,  w,  p,  1.0f, 1.0f,
             a,  w,  p,  1.0f, 1.0f,
            -a,  w,  p,  0.0f, 1.0f,
            -a, -w,  p,  0.0f, 0.0f,

            -a,  w,  p,  1.0f, 0.0f,
            -a,  w, -p,  1.0f, 1.0f,
            -a, -w, -p,  0.0f, 1.0f,
            -a, -w, -p,  0.0f, 1.0f,
            -a, -w,  p,  0.0f, 0.0f,
            -a,  w,  p,  1.0f, 0.0f,

             a,  w,  p,  1.0f, 0.0f,
             a,  w, -p,  1.0f, 1.0f,
             a, -w, -p,  0.0f, 1.0f,
             a, -w, -p,  0.0f, 1.0f,
             a, -w,  p,  0.0f, 0.0f,
             a,  w,  p,  1.0f, 0.0f,

            -a, -w, -p,  0.0f, 1.0f,
             a, -w, -p,  1.0f, 1.0f,
             a, -w,  p,  1.0f, 0.0f,
             a, -w,  p,  1.0f, 0.0f,
            -a, -w,  p,  0.0f, 0.0f,
            -a, -w, -p,  0.0f, 1.0f,

            -a,  w, -p,  0.0f, 1.0f,
             a,  w, -p,  1.0f, 1.0f,
             a,  w,  p,  1.0f, 0.0f,
             a,  w,  p,  1.0f, 0.0f,
            -a,  w,  p,  0.0f, 0.0f,
            -a,  w, -p,  0.0f, 1.0f
        };



        GLfloat rad = 0.247f;
        glm::vec2 cubePosition[] =
        {
            glm::vec2( rad * sin(270.0 * 2 * pi / 180),  rad * cos(270.0 * 2 * pi / 180) ), // 270 degrees
            glm::vec2( rad * sin(247.5 * 2 * pi / 180),  rad * cos(247.5 * 2 * pi / 180) ), // 315 degrees
            glm::vec2( rad * sin(225.0 * 2 * pi / 180),  rad * cos(225.0 * 2 * pi / 180) ), //   0 degrees
            glm::vec2( rad * sin(202.5 * 2 * pi / 180),  rad * cos(202.5 * 2 * pi / 180) ), //  45 degrees
            glm::vec2( rad * sin(180.0 * 2 * pi / 180),  rad * cos(180.0 * 2 * pi / 180) ), //  90 degrees
            glm::vec2( rad * sin(157.5 * 2 * pi / 180),  rad * cos(157.5 * 2 * pi / 180) ), // 135 degrees
            glm::vec2( rad * sin(135.0 * 2 * pi / 180),  rad * cos(135.0 * 2 * pi / 180) ), // 180 degrees
            glm::vec2( rad * sin(112.5 * 2 * pi / 180),  rad * cos(112.5 * 2 * pi / 180) )  // 225 degrees
        };


        C[0].lane=bottom;       C[ 0].Transition = setCubePosition(cubePosition[bottom],       Z - 0.0f); C[ 0].scalez = 2.0f; collisionData.collision[bottom      ].push_back (glm::vec2(C[ 0].Transition.z + (C[ 0].scalez ) , C[ 0].Transition.z - (C[ 0].scalez )));
        C[1].lane=slantDownR;   C[ 1].Transition = setCubePosition(cubePosition[slantDownR ],  Z - 4.1f); C[ 1].scalez = 1.0f; collisionData.collision[slantDownR  ].push_back (glm::vec2(C[ 1].Transition.z + (C[ 1].scalez ) , C[ 1].Transition.z - (C[ 1].scalez )));
        C[2].lane=sideR;        C[ 2].Transition = setCubePosition(cubePosition[sideR ],       Z - 6.4f); C[ 2].scalez = 1.0f; collisionData.collision[sideR  ].push_back (glm::vec2(C[ 2].Transition.z + (C[ 2].scalez ) , C[ 2].Transition.z - (C[ 2].scalez )));
        C[3].lane=slantUpR;     C[ 3].Transition = setCubePosition(cubePosition[slantUpR   ],  Z - 8.4f); C[ 3].scalez = 0.4f; collisionData.collision[slantUpR      ].push_back (glm::vec2(C[ 3].Transition.z + (C[ 3].scalez ) , C[ 3].Transition.z - (C[ 3].scalez )));
        C[4].lane=top;          C[ 4].Transition = setCubePosition(cubePosition[top],          Z - 8.4f); C[ 4].scalez = 0.4f; collisionData.collision[top   ].push_back (glm::vec2(C[ 4].Transition.z + (C[ 4].scalez ) , C[ 4].Transition.z - (C[ 4].scalez )));
        C[5].lane=slantUpL;     C[ 5].Transition = setCubePosition(cubePosition[slantUpL   ],  Z - 4.1f); C[ 5].scalez = 0.4f; collisionData.collision[slantUpL     ].push_back (glm::vec2(C[ 5].Transition.z + (C[ 5].scalez ) , C[ 5].Transition.z - (C[ 5].scalez )));
        C[6].lane= sideL;       C[ 6].Transition = setCubePosition(cubePosition[sideL   ],     Z - 6.4f); C[ 6].scalez = 0.4f; collisionData.collision[sideL   ].push_back (glm::vec2(C[ 6].Transition.z + (C[ 6].scalez ) , C[ 6].Transition.z - (C[ 6].scalez )));
        /*C[7].lane=slantDownL;   C[ 7].Transition = setCubePosition(cubePosition[slantDownL ],  Z - 4.4f); C[ 7].scalez = 0.5f; collisionData.collision[sideR       ].push_back (glm::vec2(C[ 7].Transition.z + (C[ 7].scalez ) , C[ 7].Transition.z - (C[ 7].scalez )));
         collisionData.collision[slantUpR    ].push_back (glm::vec2(C[ 8].Transition.z + (C[ 8].scalez ) , C[ 8].Transition.z - (C[ 8].scalez )));
        /*C[8].lane=bottom;       C[ 8].Transition = setCubePosition(cubePosition[bottom    ],   Z - 6.4f); C[ 8].scalez = 0.7f;C[ 9].Transition = setCubePosition(cubePosition[sideL       ], Z - 6.4f); C[ 9].scalez = 0.2f; collisionData.collision[sideL       ].push_back (glm::vec2(C[ 9].Transition.z + (C[ 9].scalez ) , C[ 9].Transition.z - (C[ 9].scalez )));
        C[10].Transition = setCubePosition(cubePosition[slantUpL    ], Z - 6.4f); C[10].scalez = 0.2f; collisionData.collision[slantUpL    ].push_back (glm::vec2(C[10].Transition.z + (C[10].scalez ) , C[10].Transition.z - (C[10].scalez )));
        C[11].Transition = setCubePosition(cubePosition[sideL       ], Z - 7.4f); C[11].scalez = 0.2f; collisionData.collision[sideL       ].push_back (glm::vec2(C[11].Transition.z + (C[11].scalez ) , C[11].Transition.z - (C[11].scalez )));
        C[12].Transition = setCubePosition(cubePosition[slantUpL    ], Z - 7.4f); C[12].scalez = 0.2f; collisionData.collision[slantUpL    ].push_back (glm::vec2(C[12].Transition.z + (C[12].scalez ) , C[12].Transition.z - (C[12].scalez )));
        C[13].Transition = setCubePosition(cubePosition[sideL       ], Z - 8.4f); C[13].scalez = 0.2f; collisionData.collision[sideL       ].push_back (glm::vec2(C[13].Transition.z + (C[13].scalez ) , C[13].Transition.z - (C[13].scalez )));
        C[14].Transition = setCubePosition(cubePosition[slantUpL    ], Z - 8.4f); C[14].scalez = 0.2f; collisionData.collision[slantUpL    ].push_back (glm::vec2(C[14].Transition.z + (C[14].scalez ) , C[14].Transition.z - (C[14].scalez )));
       /* C[15].Transition = setCubePosition(cubePosition[            ], Z + f); C[15].scalez = 0.2f;
        C[16].Transition = setCubePosition(cubePosition[], ); C[].scalez =
        C[17].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[18].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[19].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[20].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[21].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[22].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[23].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[24].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[25].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[26].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[27].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[28].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[29].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[30].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[31].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[32].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[33].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[34].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[35].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[36].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[37].Transition = setCubePosition(cubePosition[sideR   ], -6.1f); C[].scalez = ;
        C[38].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[39].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[40].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[41].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[42].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[43].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[44].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[45].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[46].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[47].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[48].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        C[49].Transition = setCubePosition(cubePosition[], ); C[].scalez = ;
        */
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // TexCoord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

    }

    glm::vec3 setCubePosition(glm::vec2 passedCubePosition, GLfloat passedZCord)
    {
        glm::vec3 tempCubePosition;
        tempCubePosition.x = passedCubePosition.x;
        tempCubePosition.y = passedCubePosition.y;
        tempCubePosition.z = passedZCord;
        return tempCubePosition;
    }

    //Calculate the model matrix for object
    void drawBlock(int tilePos, int tileNo)
    {
        if(tilePos>3) tilePos -= 4;

        glm::mat4 model;
        model = glm::translate(model, C[tileNo].Transition);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, C[tileNo].scalez));
        model = glm::rotate(model, float(tilePos * 45 * pi / 180), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    bool mapOne(bool passed_jump, bool passed_fall)
    {  // cout << "mapOne ko activelane\t\t" << activeLane << endl;
        shader.Use();
        glm::mat4 view2;
        view2 = camera.GetViewMatrix();
        glm::mat4 projection2;
        projection2 = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 1000.0f);
        //Get Uniform Location
        modelLoc2 = glGetUniformLocation(shader.Program, "model");
        viewLoc2  = glGetUniformLocation(shader.Program, "view");
        projLoc2  = glGetUniformLocation(shader.Program, "projection");
        //Pass matrices to the shader
        glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view2));
        glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection2));

        glBindVertexArray(VAO);

        //Draw the blocks
      for (int k=0;k<=totNo-1;k++)
        {
        drawBlock(C[k].lane,k);
        }

       /* drawBlock(slantUpR    , 8);
        drawBlock(sideL       , 9);
        drawBlock(slantUpL    ,10);
        drawBlock(sideL       ,11);
        drawBlock(slantUpL    ,12);
        drawBlock(sideL       ,13);
        drawBlock(slantUpL    ,14);*/

        if(passed_fall || passed_jump) return true;
        //if(camera.Position.z < -0.3)
        if(!passed_jump){
            bool collided = collisionData.checkCollision(activeLane, camera.Position);
            if(!collided) return false;//cout << "COLLIDED" << endl;
            else return true;
        }

    }

    void cameraUp(int camPos, bool passed_fall)
    {
        activeLane = camPos;
        if(!passed_fall)
            camera.Position = camera.setCameraPosition(cameraPos[camPos]);

        camera.WorldUp = cameraWorldUp[camPos];
       // cout<<"WORLDUP:: "<< camPos<<endl;
    }

    void setGravity(int per, bool jump, bool fall){
        glm::vec2 temp;
              if(per > 100) per = 100;
        if(jump)
        {
                temp.x = cameraPos[activeLane].x * per / 100;
                temp.y = cameraPos[activeLane].y * per / 100;
        }

        camera.Position = camera.setCameraPosition(temp);
    }
    static int fall_down;
    void fallDown(bool& gameOver, bool& play)
    {
        fall_down++;
        glm::vec2 temp;
        temp.x = cameraPos[activeLane].x * fall_down / 1000;
        temp.y = cameraPos[activeLane].y * fall_down / 1000;
        camera.Position = camera.setCameraPosition(temp);
        if(fall_down > 5000) {gameOver = true; play = false; }

    }
};

Camera Environment::camera = Camera(glm::vec3(0.0f, -0.19f, 0.0f));
GLushort Environment::activeLane = 0;
int Environment::fall_down = 1;
