#include "Skybox.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

Skybox::Skybox(float size) {
    // Model matrix. Since the original size of the cube is 2, in order to
    // have a cube of some size, we need to scale the cube by size / 2.
    model = glm::scale(glm::vec3(size / 2.f));

    /*
     * Skybox indices used below.
     *    4----7
     *   /|   /|
     *  0-+--3 |
     *  | 5--+-6
     *  |/   |/
     *  1----2
     *
     */

    // The 8 vertices of a cube.
    std::vector<glm::vec3> vertices {
        glm::vec3(-1, 1, 1),
        glm::vec3(-1, -1, 1),
        glm::vec3(1, -1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(-1, 1, -1),
        glm::vec3(-1, -1, -1),
        glm::vec3(1, -1, -1),
        glm::vec3(1, 1, -1)
    };

    // Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2
    // and v3 in counter-clockwise order.
    std::vector<glm::ivec3> indices {
        // Front face.
        glm::ivec3(0, 1, 2),
        glm::ivec3(2, 3, 0),
        // Back face.
        glm::ivec3(7, 6, 5),
        glm::ivec3(5, 4, 7),
        // Right face.
        glm::ivec3(3, 2, 6),
        glm::ivec3(6, 7, 3),
        // Left face.
        glm::ivec3(4, 5, 1),
        glm::ivec3(1, 0, 4),
        // Top face.
        glm::ivec3(4, 0, 3),
        glm::ivec3(3, 7, 4),
        // Bottom face.
        glm::ivec3(1, 5, 6),
        glm::ivec3(6, 2, 1),
    };

    // Generate a vertex array (VAO) and vertex buffer object (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind VBO to the bound VAO, and store the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    // Enable Vertex Attribute 0 to pass the vertex data through to the shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO, and send the index data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBO/VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
        
    cubemapTexture = loadCubemap(box);
}

Skybox::~Skybox() {
    // Delete the VBO/EBO and the VAO.
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Skybox::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader) {
    // change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);
    
    // Actiavte the shader program
    glUseProgram(shader);
    
    // Display the inside of the cube (counter-clockwise triangles)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    //glDisable(GL_CULL_FACE);
    
    glUniform1i(glGetUniformLocation(shader, "Skybox"), 0);
    
    // Get the shader variable locations and send the uniform data to the shader
    glm::mat4 newView = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(newView));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    // Bind the VAO
    glBindVertexArray(VAO);
    
    // bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        
    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Use bilinear interpolation:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Use clamp to edge to hide skybox edges
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Draw the points using triangles, indexed with the EBO
    //glDrawArrays(GL_TRIANGLES, 0, 36); -> strange radical effect
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //glDisable(GL_CULL_FACE);
    
    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glUseProgram(0);
}

unsigned int Skybox::loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        // Loop through all the texture targets
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    // Specify wrapping and filtering methods
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Skybox::rotate(float rotAngle, glm::vec3 rotAxis) {
    model = glm::rotate(glm::mat4(1.0), rotAngle, rotAxis) * model;
}

void Skybox::update() {
    
}
