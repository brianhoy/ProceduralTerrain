#include "Renderer.h"

Renderer::Renderer()
{
	createWindow(800, 800);
	screenHeight = 800;
	screenWidth = 800;
}


Renderer::~Renderer()
{
}

int Renderer::createWindow(int width, int height)
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	return 0;
}

void Renderer::render(Camera* camera, Scene* scene)
{
	// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view;
	view = camera->GetViewMatrix(); //glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); //
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f); //glm::perspective(1.0f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	//glm::mat4 model;
	//model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	for (int i = 0; i < scene->meshes.size(); i++) {
		Mesh* mesh = scene->meshes.at(i);

		if (mesh->geometry->needsUpdate) {
			std::cout << "uploading geometry" << mesh->material->program << std::endl;
			uploadMesh(*mesh);
		}

		mesh->material->use(); /* use the shader of the mesh */

		GLint modelLoc = glGetUniformLocation(mesh->material->program, "model");
		GLint viewLoc = glGetUniformLocation(mesh->material->program, "view");
		GLint projLoc = glGetUniformLocation(mesh->material->program, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh->getModelMatrix()));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(mesh->geometry->VAO); /* bind to the geometry of the mesh */
		if (mesh->geometry->indices.size() == 0) {
			glDrawArrays(GL_TRIANGLES, 0, mesh->geometry->vertexData.size());
		}
		else {
			glDrawElements(GL_TRIANGLES, mesh->geometry->indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
		}
		glBindVertexArray(0); /* unbind any geometry */
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

void Renderer::uploadMesh(Mesh& mesh) {
	auto geometry = mesh.geometry;
	auto material = mesh.material;
	
	glGenVertexArrays(1, &geometry->VAO);
	glGenBuffers(1, &geometry->VBO);
	glGenBuffers(1, &geometry->EBO);

	glBindVertexArray(geometry->VAO);
		// 2. Copy geometry vertex data into a vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, geometry->VBO);
		glBufferData(GL_ARRAY_BUFFER, geometry->vertexData.size() * sizeof(Vertex), &geometry->vertexData[0], GL_STATIC_DRAW);

		// 2.1 Copy each other attribute into a buffer
		for (int i = 0; i < material->attributes.size(); i++) {
			glBufferData(GL_ARRAY_BUFFER, material->attributes.at(i).size, material->attributes.at(i).location, GL_STATIC_DRAW);
		}

		// 2.2 Upload indices if they exist
		if (geometry->indices.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indices.size() * sizeof(GLuint), &geometry->indices[0], GL_STATIC_DRAW);
		}

		// 3. Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//4. Unbind the VAO
	glBindVertexArray(0);
	geometry->needsUpdate = false;
}

GLuint Renderer::uploadShader(const char* code, unsigned int type)
{
	GLuint id = glCreateShader(type);
	GLint success;
	GLchar infoLog[512];
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);
	// Print compile errors if any
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

}

GLuint Renderer::createProgram(std::vector<GLuint> shaders)
{
	return GLuint();
}
