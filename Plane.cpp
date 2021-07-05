#include "Plane.h"

Plane::Plane()
{
	// Model matrix. Since the original size of the Plane is 2, in order to
	// have a Plane of some size, we need to scale the Plane by size / 2.

	// The color of the Plane. Try setting it to something else!
	//color = glm::vec3(1.0f, 0.95f, 0.1f);

	/*
	 * Plane indices used below.
	 *    4----7
     *   /|   /|
     *  0-+--3 |
     *  | 5--+-6
     *  |/   |/
     *  1----2
	 *
	 */

	// The 8 vertices of a Plane.
	std::vector<glm::vec3> vertices
	{
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
	std::vector<glm::ivec3> indices
	{
        // Top face.
        glm::ivec3(4, 0, 3),
        glm::ivec3(3, 7, 4),
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
}

Plane::~Plane()
{
	// Delete the VBO/EBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Plane::draw(GLuint shader, glm::mat4 C)
{
	// Actiavte the shader program 
	glUseProgram(shader);
    
    model = C;

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// Bind the VAO
	glBindVertexArray(VAO);
    
	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

