
class SplashScreenFragment
{
public:
    SplashScreenFragment()
    {
        shader = Shader("shaders/splashScr.vs", "shaders/splashScr.frag");
        GLfloat vertices[] = {
            // Positions          // Colors           // Texture Coords
             0.5f * 2,  0.5f* 2, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
             0.5f * 2, -0.5f* 2, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
            -0.5f * 2, -0.5f* 2, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
            -0.5f * 2,  0.5f* 2, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
        };

        GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        // Load and create a texture
        // ====================
        // Texture 1
        // ====================
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Load, create texture and generate mipmaps
        int width, height;
        unsigned char* image = SOIL_load_image("escape.png", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
        // ===================
        // Texture 2
        // ===================
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Load, create texture and generate mipmaps
        image = SOIL_load_image("face.png", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void draw()
    {
         // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture2"), 1);

        //Activate shader
        shader.Use();

        // Draw Container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    bool& getClickedButton(bool& passed_play, bool& passed_help, bool& passed_credits, bool& passed_exiti, bool& passed_noButton, double xPos, double yPos)
    {

        ixPos = static_cast <int> (xPos);
        iyPos = static_cast <int> (yPos);

        if(xPos > 103.657 &&
           xPos < 171.402 &&
           yPos > 204.657 &&
           yPos < 240.551)
        return passed_play;

        if(xPos > 102.639 &&
           xPos < 172.232 &&
           yPos > 271.253 &&
           yPos < 307.395)
        return passed_help;

        if(xPos > 100.608 &&
           xPos < 208.709 &&
           yPos > 344.648 &&
           yPos < 370.091)
        return passed_credits;

        if(xPos > 101.506 &&
           xPos < 157.920 &&
           yPos > 414.183 &&
           yPos < 439.222)
        return passed_exiti;



        return passed_noButton;
    }

    ~SplashScreenFragment()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
private:
    Shader shader;
    static int ixPos, iyPos;
    GLuint VBO, VAO, EBO;
    GLuint texture1, texture2;
};

int SplashScreenFragment::ixPos;
int SplashScreenFragment::iyPos;
