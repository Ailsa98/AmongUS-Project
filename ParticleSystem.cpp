#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
    srand((unsigned)time(NULL));
    GLfloat velocity;
    GLfloat life;
    glm::vec3 direction;
    particles.reserve(partNum);
    positions.assign(partNum, glm::vec3(0));
    for (int i = 0; i < partNum; ++i) {
        velocity = (GLfloat) (rand() % 10) / 100.f;
        direction = glm::normalize(glm::vec3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
        life = (GLfloat) (rand() % 10) / 10.f;
        particles.push_back(Particle(direction, velocity, life));

        //positions.push_back(glm::vec3(0));
    }

    // Set the model matrix to an identity matrix.
    model = glm::mat4(1);

    // Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO to the bound VAO, and store the point data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * partNum, positions.data(), GL_STATIC_DRAW);
    
    // Enable Vertex Attribute 0 to pass point data through to the shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind the VBO/VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem() {
    // Delete the VBO and the VAO.
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void ParticleSystem::draw(glm::mat4 C) {
    newModel = model * C;
    
    // Actiavte the shader program
    glUseProgram(shader);

    // Get the shader variable locations and send the uniform data to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(newModel));
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

    // Bind the VAO
    glBindVertexArray(VAO);

    // Set point size
    glPointSize(3.0f);

    // Draw the points
    glDrawArrays(GL_POINTS, 0, partNum);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void ParticleSystem::update(glm::mat4 C) {
    partSysLife -= dt;

    // update all particles
    for (unsigned int i = 0; i < partNum; ++i) {
        Particle &p = particles[i];
        p.life -= dt; // reduce life
        if (spawn) {
            if (p.life > 0.0f) {    // particle is alive, thus update
                positions[i] += p.velocity * p.direction;
            } else {
                positions[i] = glm::vec3(0);
                if (partSysLife > 0.0f) p.life = partLife;
            }
        }
        else {
            if (p.life > 0.0f) {    // particle is alive, thus update
                positions[i] -= p.velocity * p.direction;
            } else if (partSysLife > 0.0f) {
                positions[i] = p.direction * p.velocity * (partLife / dt);
                p.life = partLife;
            }
        }
    }
    //std::cout << positions[0].x << " " << positions[0].y << " " << positions[0].z << std::endl;
    
    // Update the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Does not allocate memory, more efficient to replace the buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * partNum, positions.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::reSpawn(bool sp) {
    spawn = sp;
    
    srand((unsigned)time(NULL));
    for (int i = 0; i < partNum; ++i) {
        Particle &p = particles[i];
        p.life = partLife;
        partSysLife = 3;
        if (spawn) positions[i] = glm::vec3(0);
        else positions[i] = p.direction * p.velocity * (partLife / dt);
    }
}
