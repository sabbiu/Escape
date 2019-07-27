

// Std. Includes
#include <string>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//#include <ft2build.h>
//#include FT_FREETYPE_H
// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "camera.h"
#include "Model.h"
#include "block.h"
#include "MainCharacter.h"
#include "SplashScreen.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


int scr=0;
// Other Libs
#include "SOIL.h"

#define pi 3.14159265

#define glfwGetKeyOnce(WINDOW, KEY) \
    (glfwGetKey(WINDOW, KEY) ?  \
    (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) : \
    (keyOnce[KEY] = false))

//textstart
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
//textend

// Properties
//GLuint screenWidth = 1000, screenHeight = 800;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement(Environment*);

// Camera
Camera camera(glm::vec3(0.0f, 0.2f, 3.0f));
Camera cameraCharacter(glm::vec3(0.0f, 0.5f,1.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

char keyOnce[GLFW_KEY_LAST + 1];
//release
bool releaseLeft = false;
bool jump = false;//, fall = false;
bool inmap1, inmap2;
//environment position
int envPos = 0;
// The MAIN function, from here we start our application and run our Game loop
bool play = false, help = false, credits = false, exiti = false, noButton = true, gameOver = false;
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
  //  glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    int height = 10, width = 10;
//GLFWimage *image;
//image->pixels = SOIL_load_image("mouse_cur.jpg", &width, &height, 0, SOIL_LOAD_RGB);
  //glfwSetCursor(window, glfwCreateCursor(/*mouse_cur.png*/image, 40, 40));

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);

//textstart
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
      Shader shader("shaders/text.vs", "shaders/text.frag");
    glm::mat4 projectionx = glm::ortho(0.0f, static_cast<GLfloat>(screenWidth), 0.0f, static_cast<GLfloat>(screenHeight));
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionx));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "timeburner.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

//textend

    //Main Character object
    MainCharacter mainCharacter;
    // Draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Splash screen
    SplashScreenFragment splashScreenFragment;
    double mouseXpos, mouseYpos;
    //move camera forward
    //Environment environment(0.0f);
    Environment* environmentCam = new Environment(0.0f);
    Environment* environment1 = new Environment( 0.0f);
    Environment* environment2 = new Environment(-9.0f);

    int mapChangeVar = 9, mapCount = 1;
    const int mapCountconst = 9;
    long mapMapMap;
    //int start = 0;
    //int fontInc = 0.001;
    //const int fontIncConst = 0.01;
    // Game loop
    while(!glfwWindowShouldClose(window))
    {


        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //splash screen
        if(!play && !help && !credits && !exiti && !gameOver)
        {
        //glClear(GL_DEPTH_BUFFER_BIT);
            splashScreenFragment.draw();
            glfwGetCursorPos(window, &mouseXpos, &mouseYpos);
            //cout << mouseXpos <<"        " <<mouseYpos<<endl;
            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
                splashScreenFragment.getClickedButton(play, help, credits, exiti, noButton, mouseXpos, mouseYpos) = true;
            }

            glClear(GL_DEPTH_BUFFER_BIT);
            //hovering the mouse
            if(mouseXpos > 103.657 &&
               mouseXpos < 171.402 &&
               mouseYpos > 204.657 &&
               mouseYpos < 240.551)
            RenderText(shader, "play"   , 100.0f, 250.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            else
            RenderText(shader, "play"   , 100.0f, 250.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));

            if(mouseXpos > 102.639 &&
               mouseXpos < 172.232 &&
               mouseYpos > 271.253 &&
               mouseYpos < 307.395)
            RenderText(shader, "help"   , 100.0f, 180.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            else
            RenderText(shader, "help"   , 100.0f, 180.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));

            if(mouseXpos > 100.608 &&
               mouseXpos < 208.709 &&
               mouseYpos > 344.648 &&
               mouseYpos < 370.091)
            RenderText(shader, "credits", 100.0f, 110.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            else
            RenderText(shader, "credits", 100.0f, 110.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));

            if(mouseXpos > 101.506 &&
               mouseXpos < 157.920 &&
               mouseYpos > 414.183 &&
               mouseYpos < 439.222)
            RenderText(shader, "exit"   , 100.0f, 040.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            else
            RenderText(shader, "exit"   , 100.0f, 040.0f, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));

            /*if(play){
                delete environmentCam;
                delete environment1;
                delete environment2;
                environmentCam = new Environment(0.0f);
                environment1 = new Environment( 0.0f);
                environment2 = new Environment(-9.0f);
                environmentCam->camera.Position = glm::vec3(0.0f, -0.19f, 0.0f);
            }*/
        }

        if(play)
        {
            //Environment
            //repetition of the map
            mapMapMap = int(environmentCam->camera.Position.z);
            if(int(environmentCam->camera.Position.z) < -3)
            if(mapMapMap % mapChangeVar == 0){

                mapChangeVar = mapCountconst * (++mapCount);
                if(mapMapMap % 2 == 0){
                    delete environment2;
                    environment2 = new Environment((mapChangeVar ) * -1.0f);
                } else {
                    delete environment1;
                    environment1 = new Environment((mapChangeVar ) * -1.0f);
                }
            }

            //print the map
            inmap1 = environment1->mapOne(jump, fall);
            inmap2 = environment2->mapOne(jump, fall);

            //move foreward
            //if(start < 2)
            //{
            //    glClear(GL_DEPTH_BUFFER_BIT);
            //    RenderText(shader, "ready", 300 - fontInc, 200 - fontInc, fontInc, glm::vec3(1.0f, 1.0f, 1.0f));
            //    start = static_cast <int> (fontInc);
            //    fontInc += fontIncConst;
            //}
            //else
                environmentCam->camera.moveTheCameraForward(0.00006);

            if(!fall){

                //check if collided or not
                if (inmap1 || inmap2) fall = false;
                else fall = true;

                Do_Movement(environmentCam);

                //LEFT
                if(glfwGetKeyOnce(window, GLFW_KEY_A )) {
                    if(envPos == 0) envPos = 7;
                    else envPos--;
                    environmentCam->cameraUp(envPos, fall);
                    environmentCam->camera.setCameraPosition(glm::vec2(0.0f, 0.0f));
                    jump = true;
                }

                //RIGHT
                if(glfwGetKeyOnce( window, GLFW_KEY_D )){
                    if(envPos == 7) envPos = 0;
                    else envPos++;
                    environmentCam->cameraUp(envPos, fall);
                    environmentCam->camera.setCameraPosition(glm::vec2(0.0f, 0.0f));
                    jump = true;
                }

                //UP
                if(glfwGetKeyOnce( window, GLFW_KEY_W )){
                    environmentCam->camera.setCameraPosition(glm::vec2(0.0f, 0.0f));
                    jump = true;
                }
            }

            if(fall){
                environmentCam->fallDown(gameOver, play);
            }

            //Clear environment buffer
            glClear(GL_DEPTH_BUFFER_BIT);

            //Main Character
            if(!fall){
                if(!jump) {mainCharacter.run(); environmentCam->cameraUp(envPos, fall);}
                else      mainCharacter.jump();
            } else {

                mainCharacter.jump();
            }

            scr=scr+1;
            string s="j";// std::to_string(scr);

            RenderText(shader, "SCORE  "  +s , 1.0f, 1.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        }

        if(gameOver){
            RenderText(shader, "HIGH SCORE  " , 50.0f, 400.0f, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            RenderText(shader, "YOU SCORED  " , 50.0f, 050.0f, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {    gameOver = false;
                delete environmentCam, environment1, environment2;
            environmentCam = new Environment(0.0f);
            environment1 = new Environment( 0.0f);
            environment2 = new Environment(-9.0f);
            environmentCam->camera.Position = glm::vec3(0.0f, -1.9f, 0.0f);
            inmap1 = true;
            environment1->activeLane = 0;
            environment1->fall_down = 1;
            fall = false;
            }

        }
        if(help)
        {

        }

        if(credits)
        {

        }

        if(exiti)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

#pragma region "User input"
int keyCount = 8;
// Moves/alters the camera positions based on user input
void Do_Movement(Environment* environment1)
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        environment1 -> setGravity(keyCount * 4, jump, fall);
    if(keys[GLFW_KEY_A])
        environment1 -> setGravity(keyCount * 4, jump, fall);
    if(keys[GLFW_KEY_D])
        environment1 -> setGravity(keyCount * 4, jump, fall);
}

// Is called whenever a key is pressed/released via GLFW
inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        play = false; gameOver = true;
    }
    if (action==GLFW_REPEAT)
    {
        keyCount++;
        if(keyCount < 25)
            keys[key]=true;
        else
        {
            jump = false;
            keys[key] = false;
        }
    }
    if(action == GLFW_PRESS)
        keys[key] = true;
    if(action == GLFW_RELEASE)
    {
        jump = false;
        keys[key] = false;
        keyCount = 8;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    shader.Use();
    glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/*
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
*/
#pragma endregion
