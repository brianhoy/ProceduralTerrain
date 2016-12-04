#include "Renderer.h"

Renderer::Renderer() : 
	materialBasicUploader(MaterialBasicUploader())
{
	createWindow(800, 800);
	screenHeight = 800;
	screenWidth = 800;
}

int Renderer::createWindow(int width, int height)
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	std::cout << "Starting GLFW context, version 4.5" << std::endl;

	// Create a GLFWwindow object that we can use for GLFW's functions
	this->window = glfwCreateWindow(width, height, "Procedural Terrain", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int vwidth, vheight;
	glfwGetFramebufferSize(window, &vwidth, &vheight);
	glViewport(0, 0, vwidth, vheight);

	initializeUniformBuffer();
	glEnable(GL_DEPTH_TEST);

	return 0;
}

void Renderer::render(Camera* camera, Scene* scene)
{
	// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update view and projection matrices
	updateViewMatrix(camera->GetViewMatrix());
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	updateProjectionMatrix(projection);

	// Render each mesh
	for (int i = 0; i < scene->meshes.size(); i++) {
		Mesh* mesh = scene->meshes.at(i);

		// If the geometry needs an update, upload it
		if (mesh->geometry->needsUpdate) {
			std::cout << "uploading geometry" << std::endl;
			uploadGeometry(mesh->geometry);
		}

		// If the material needs an update, call the upload function
		if (mesh->material->needsUpdate == true) {
			std::cout << "uploading material" << std::endl;
			uploadMaterial(mesh->material);
		}
		glUseProgram(mesh->material->program);

		GLint modelLoc = glGetUniformLocation(mesh->material->program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh->getModelMatrix()));

		if (mesh->material->getType() == "MaterialBasic") {
			materialBasicUploader.bindTexture(static_cast<MaterialBasic*>(mesh->material));
		}

		glBindVertexArray(mesh->geometry->VAO); // Bind to VAO (stores attribute pointer data)
			if (mesh->geometry->indices.size() == 0) { // If it's not indexed, use glDrawArrays
				glDrawArrays(GL_TRIANGLES, 0, mesh->geometry->vertexData.size());
			}
			else { // If it is, use glDrawElements
				glDrawElements(GL_TRIANGLES, mesh->geometry->indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
			}
		glBindVertexArray(0);
	}
	// Swap the screen buffers
	glfwSwapBuffers(window);
}

void Renderer::terminate() {
	glfwTerminate();
}

bool Renderer::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void Renderer::uploadGeometry(Geometry* geometry) {
	
	glGenVertexArrays(1, &geometry->VAO);
	glGenBuffers(1, &geometry->VBO);
	glGenBuffers(1, &geometry->EBO);

	// 1. Bind to Geomtry VAO (Stores all vertex attributes for an object (normals, indices, texCoords, positions)
	glBindVertexArray(geometry->VAO);
		// 2. Copy geometry vertex data into a vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, geometry->VBO); // Generate VBO for vertex positions
		glBufferData(GL_ARRAY_BUFFER, geometry->vertexData.size() * sizeof(Vertex), &geometry->vertexData[0], GL_STATIC_DRAW);

		// 2.2 Upload indices if they exist
		if (geometry->indices.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indices.size() * sizeof(GLuint), &geometry->indices[0], GL_STATIC_DRAW);
		}

		// 3. Then set our vertex attributes pointers

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0); 
		glEnableVertexAttribArray(0);

		// Normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

	//4. Unbind the VAO
	glBindVertexArray(0);
	geometry->needsUpdate = false;
}

void Renderer::uploadMaterial(Material * material)
{
	if (material->getType() == "MaterialBasic") {
		materialBasicUploader.upload(static_cast<MaterialBasic*>(material));
	}
}

void Renderer::uploadTexture(Texture * texture)
{
}

GLuint Renderer::createProgram(std::vector<GLuint> shaders)
{
	return GLuint();
}

void Renderer::initializeUniformBuffer()
{
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4)); // Bind it to point 0
}

void Renderer::updateProjectionMatrix(glm::mat4 projection)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateViewMatrix(glm::mat4 view)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(
		GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
