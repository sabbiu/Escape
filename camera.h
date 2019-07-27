

#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#define p(a) cout << "#a" << a <<endl
//int justfun = 1;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//Changes up vector for the world as specified

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    //Camera(){}
    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = -90.0f,
           GLfloat pitch = 0.0f)
           : Front(glm::vec3(0.0f, 0.0f, -2.0f)), MovementSpeed(5.0f), MouseSensitivity(0.25f), Zoom(45.0f)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)  : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.0f), MouseSensitivity(0.25f), Zoom(45.0f)
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->WorldUp);
    }

    glm::vec3 setCameraPosition(glm::vec2 camPos)
    {
        this->Position.x = camPos.x;
        this->Position.y = camPos.y;
        return this->Position;
    }

    //always moving the camera forward
    void moveTheCameraForward(GLfloat deltaTime, GLfloat MovementSpeed = 5.0f){
        GLfloat velocity = this->MovementSpeed * deltaTime;
        this->Position += this->Front * velocity;
    }

    void rotateTheCamera(GLfloat posX=0.05f, GLfloat posY=0.05f, GLfloat posZ=0.0f, //for camera pos
                         GLfloat upX = 0.0f, GLfloat upY = 0.0f, GLfloat upZ = 0.0f, //for camera up
                         GLfloat angle=20.0f,
                         glm::vec3 identityVector= glm::vec3(1.0f, 1.0f, 1.0f)
                         ){
        GLfloat abc= sin(glfwGetTime());
        GLfloat def = cos(glfwGetTime());
  //      abc = 1;
//        def = 1;
        this->WorldUp=glm::vec3(abc,def,0.0f);




        //GLfloat radius = 0.05f;
//posX = sin(glfwGetTime()) * radius;
//posY = cos(glfwGetTime()) * radius;
       /* glm::vec3 tempPos = this->Position;
        this->Position += glm::vec3(posX, posY, posZ);
        glm::vec3 vecOne = tempPos - this->Position;
        glm::vec3 vecTwo = this->Front - this->Position;
        this->WorldUp = glm::cross(vecOne, vecTwo);
*/
       // cout<< tempPos << endl << vecOne <<endl << vecTwo << endl << WorldUp <<endl <<endl;
        //this->Position = glm::dot(this->Position, identityVector);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if(direction == FORWARD)
            this->Position += this->Front * velocity;
        if(direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if(direction == LEFT)
            this->Position -= this->Right * velocity;
        if(direction == RIGHT)
            this->Position += this->Right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;
        this->Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if(constrainPitch)
        {
            if(this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if(this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset)
    {
        if(this->Zoom >= 1.0f && this->Zoom <= 45.0f)
            this->Zoom -= yoffset;
        if(this->Zoom <= 1.0f)
            this->Zoom = 1.0f;
        if(this->Zoom >= 45.0f)
            this->Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};


