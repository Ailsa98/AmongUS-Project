#include "PointCloud.h"
GLint PointCloud::colorMode;

PointCloud::PointCloud(std::string objFilename) {
    /*
     * Read points from an obj file.
     */
    std::ifstream objFile(objFilename); // The obj file we are reading.
        
    // Check whether the file can be opened.
    if (objFile.is_open()) {
        std::string line; // A line in the file.
        // Read lines from the file.
        while (std::getline(objFile, line)) {
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;
            // Read the first word of the line.
            std::string label;
            ss >> label;
            // If the line is about vertex (starting with a "v").
            if (label == "v") {
                // Read the later three float numbers and use them as the // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                // Process the point. For example, you can save it to a.
                points.push_back(point);
            }
            // 'vn' lines to load normals
            if (label == "vn") {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            // 'f' lines to load triangles
            if (label == "f") {
                glm::ivec3 face;
                char delimiter;
                unsigned temp;
                ss >> temp >> delimiter >> delimiter >> face.x >> temp >> delimiter >> delimiter >> face.y >> temp >> delimiter >> delimiter >> face.z;
                
                face.x--;
                face.y--;
                face.z--;
                
                faces.push_back(face);
            }
        }
    }
    else {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    objFile.close();
    

    /*
     * Normalize the object to fit in the screen.
     */

    // Find max and min points
    for (int i = 0; i < points.size(); i++) {
        if (points[i].x > maxX)
            maxX = points[i].x;
        if (points[i].x < minX)
            minX = points[i].x;
        if (points[i].y > maxY)
            maxY = points[i].y;
        if (points[i].y < minY)
            minY = points[i].y;
        if (points[i].z > maxZ)
            maxZ = points[i].z;
        if (points[i].z < minZ)
            minZ = points[i].z;
    }
    
    // Find center
    GLfloat centerX = (GLfloat)((minX + maxX) / 2);
    GLfloat centerY = (GLfloat)((minY + maxY) / 2);
    GLfloat centerZ = (GLfloat)((minZ + maxZ) / 2);
    center = glm::vec3(centerX, centerY, centerZ);
    
    // Set new coordinates
    for (int i = 0; i < points.size(); i++) {
        GLfloat newX = points[i].x - center.x;
        GLfloat newY = points[i].y - center.y;
        GLfloat newZ = points[i].z - center.z;
        points[i] = glm::vec3(newX, newY, newZ);
    }
    
    // Find maximum distance from the center
    for (int i = 0; i < points.size(); i++) {
        GLfloat dist = sqrt(pow(points[i].x, 2) + pow(points[i].y, 2) + pow(points[i].z, 2));
        if (dist > maxDist){
            maxDist = dist;
        }
    }
    
    GLfloat scale = 10 / maxDist;
    
    // Set the model matrix to an identity matrix and change based on the scale.
    model = glm::mat4(1);
    model = glm::scale(glm::vec3(scale)) * model;

    // Set the color.
    //color = glm::vec3(1, 0, 0);
    colorMode = 0;
    ambient = glm::vec3(0.1, 0.2, 0.5);
    diffuse = glm::vec3(0.0, 0.0, 0.0);
    specular = glm::vec3(1, 0.9, 0.8);
    shininess = 0.5;

    // Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);
    
    // Set point size
    //glPointSize(pointSize);

    // Bind VBO to the bound VAO, and store the point data
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    
    // Enable Vertex Attribute 0 to pass point data through to the shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);
    
    // Bind VBO2, and store the normal data
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    
    // Enable Vertex Attribute 1 to pass point data through to the shader
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind the VBO/VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

PointCloud::~PointCloud() {
    // Delete the VBO and the VAO.
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void PointCloud::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader) {
    // Actiavte the shader program
    glUseProgram(shader);

    // Get the shader variable locations and send the uniform data to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));
    glUniform1i(glGetUniformLocation(shader, "colorMode"), colorMode);
    glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(diffuse));
    glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(specular));
    glUniform1f(glGetUniformLocation(shader, "material.shininess"), shininess);

    // Bind the VAO
    glBindVertexArray(VAO);
    
    // Draw the faces
    glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void PointCloud::update() {
}

void PointCloud::spin(float deg) {
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PointCloud::rotateObj(float rotAngle, glm::vec3 rotAxis) {
    model = glm::rotate(glm::mat4(1.0), rotAngle, rotAxis) * model;
}

void PointCloud::scale(GLfloat multiple) {
    model = glm::scale(model, glm::vec3(multiple, multiple, multiple));
}

void PointCloud::changeColorMode() {
    if (colorMode == 0) {
        colorMode = 1;
    }
    else {
        colorMode = 0;
    }
}

void PointCloud::setColor(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
}

void PointCloud::translate(glm::vec3 trans) {
    model = glm::translate(glm::mat4(1.0), trans) * model;
}
