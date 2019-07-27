//#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//GLuint screenWidth = 1000, screenHeight = 800;


class MainCharacter
{
private:
    Model runModel[6];
    Model jumpModel;
    int i;
    int run_count;
    Shader shaderer;
    Camera camera;

public:
    MainCharacter(){
        runModel[0] = Model("new/run_01.obj");
        runModel[1] = Model("new/run_02.obj");
        runModel[2] = Model("new/run_03.obj");
        runModel[3] = Model("new/run_04.obj");
        runModel[4] = Model("new/run_05.obj");
        runModel[5] = Model("new/run_06.obj");
        jumpModel   = Model("Jump/jump.obj");
        i=0;
        run_count=0;
        shaderer = Shader("shaders/model_loading.vs", "shaders/model_loading.frag");
        camera = Camera(glm::vec3(0.0f, 0.9f, 2.0f));
        cout<<"MOdel and shader lloaded";
    }

    glm::mat4 scale(){
        glm::mat4 run01;
        run01 = glm::scale(run01, glm::vec3(0.05f, 0.05f, 0.05f));
        run01 = glm::translate(run01, glm::vec3(0.0f, 0.0f, 0.0f));
        run01 = glm::rotate(run01, 90.0f * float(3.1415 / 2), glm::vec3(0,1,0));
        return run01;
    }

    void run(){
        //scale(shader);
        shaderer.Use();

        glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shaderer.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderer.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 run01 = scale();
        glUniformMatrix4fv(glGetUniformLocation(shaderer.Program, "model"),1,GL_FALSE, glm::value_ptr(run01));
        runModel[run_count].Draw(shaderer);

        if(i%400==0) run_count ++;
        if(run_count==6) run_count = 0;
        i++;
    }

    void jump(){
        shaderer.Use();

        glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shaderer.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderer.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 jumping = scale();
        glUniformMatrix4fv(glGetUniformLocation(shaderer.Program, "model"),1,GL_FALSE, glm::value_ptr(jumping));
        jumpModel.Draw(shaderer);

    }
};
