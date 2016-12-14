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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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

	void APIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	glDebugMessageCallback(glErrorCallback, nullptr);

	initializeUniformBuffer();
	initializeTextureArray();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEBUG_OUTPUT);
	return 0;
}

void Renderer::updateScene(Scene* scene) {
	auto t_start = std::chrono::high_resolution_clock::now();

	bool textures_uploaded = false; // if true, mipmaps need to be regenerated

	for (int i = 0; i < scene->renderedMeshes.size(); i++) {
		auto mesh = scene->renderedMeshes.at(i);
		if (!mesh->noDraw) {
			// If the geometry needs an update, upload it
			if (mesh->geometry->needsUpdate == true) {
				//std::cout << "uploading geometry" << std::endl;
				uploadGeometry(mesh->geometry.get());
			}

			// If the material needs an update, upload it
			if (mesh->material->needsUpdate == true) {
				//std::cout << "uploading material" << std::endl;
				uploadMaterial(mesh->material.get());
				mesh->modelLocation = glGetUniformLocation(mesh->material->program, "model");
			}

			// If any texture needs to be uploaded, upload it
			for (int i = 0; i < mesh->textures.size(); i++) {
				if (mesh->textures.at(i).needsUpdate == true) {
					//std::cout << "uploading textures" << std::endl;
					textures_uploaded = true;
					uploadTexture(&mesh->textures.at(i));
				}
			}
		}
	}
	lastVersion = scene->version;

	if (textures_uploaded) {
		generateTextureArrayMips();
	}

	auto t_end = std::chrono::high_resolution_clock::now();
	auto loadTime = std::chrono::duration<double, std::milli>(t_end - t_start).count() / 1000.0f;

	std::ostringstream strs;
	strs << loadTime;
	std::string str = strs.str();

	std::cout << "Total Scene Load Time: " << str << std::endl;
}

void Renderer::render(Camera* camera, Scene* scene)
{
	frameNumber++;
	if (scene->version != lastVersion) {
		updateScene(scene);
	}

	auto t_start = std::chrono::high_resolution_clock::now();

	// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
	glfwPollEvents();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update view and projection matrices
	updateViewMatrix(camera->GetViewMatrix()); // camera->GetViewMatrix == error
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	updateProjectionMatrix(projection);

	auto viewmat_upd = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count();

	double texBindTime = 0.0f;
	double usePrgAndUniformTime = 0.0f;
	double vrtxBindTime = 0.0f;

	for (int i = 0; i < scene->renderedMeshes.size(); i++) {

		auto mesh = scene->renderedMeshes.at(i);
		if (!mesh->noDraw) {
			auto msh_start = std::chrono::high_resolution_clock::now(); // BENCHMARK

			if (mesh->material->program != lastBoundMeshId) { // check if new shader needs to be bound
				glUseProgram(mesh->material->program);
				lastBoundMeshId = mesh->material->program;
			}

			if (mesh->version != mesh->lastVersionUploaded) { // check if object's model matrix needs to be updated/uploaded
				glUniformMatrix4fv(mesh->modelLocation, 1, GL_FALSE, glm::value_ptr(mesh->getModelMatrix()));
				mesh->lastVersionUploaded = mesh->version;
			}

			// BENCHMARK 16ms -> 0.4ms
			usePrgAndUniformTime += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - msh_start).count();
			msh_start = std::chrono::high_resolution_clock::now();

			bindTextures(&mesh->textures, mesh->material->program);

			// BENCHMARK 7ms
			texBindTime += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - msh_start).count();
			msh_start = std::chrono::high_resolution_clock::now();


			glBindVertexArray(mesh->geometry->VAO); // Bind to VAO (stores attribute pointer data)
			if (mesh->geometry->indices.size() == 0) { // If it's not indexed, use glDrawArrays
				glDrawArrays(GL_TRIANGLES, 0, mesh->geometry->vertexData.size());
			}
			else { // If it is, use glDrawElements
				glDrawElements(GL_TRIANGLES, mesh->geometry->indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
			}
			glBindVertexArray(0);
			//unbindTextures(mesh->textures);

			// BENCHMARK 0.04ms
			vrtxBindTime += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - msh_start).count();
		}
	}

	auto meshdraw_upd = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count();

	// Swap the screen buffers
	glfwSwapBuffers(window);

	auto fps = 1000.0f / std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - t_start).count();


	if (frameNumber % 300 == 0) {
		std::cout << "viewmat_upd: " << viewmat_upd << ", meshdraw_upd: " << meshdraw_upd << ", vrtxBindTime: " << vrtxBindTime << ", texBindTime: " << texBindTime << ", usePrgAndUniformTime: " << usePrgAndUniformTime << std::endl;
	}

	std::ostringstream strs;
	strs << fps;
	std::string str = strs.str();


	std::string title = "Procedural Terrain" + str + "FPS";
	glfwSetWindowTitle(window, title.c_str()); 
}

void Renderer::renderMeshCollectionRecursive(Camera* camera, MeshCollection* collection) {
	/*for (auto mesh : collection->meshes) {
		if (!mesh.noDraw) {
			// If the geometry needs an update, upload it
			if (mesh.geometry->needsUpdate) {
				std::cout << "uploading geometry" << std::endl;
				uploadGeometry(mesh.geometry.get());
			}

			// If the material needs an update, upload it
			if (mesh.material->needsUpdate == true) {
				std::cout << "uploading material" << std::endl;
				uploadMaterial(mesh.material.get());
			}

			// If any texture needs to be uploaded, upload it
			for (int i = 0; i < mesh.textures.size(); i++) { 
				if (mesh.textures.at(i).needsUpdate) {
					uploadTexture(&mesh.textures.at(i));
				}
			}

			glUseProgram(mesh.material->program);
				GLint modelLoc = glGetUniformLocation(mesh.material->program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh.getModelMatrix()));

				bindTextures(mesh.textures, mesh.material->program);
					glBindVertexArray(mesh.geometry->VAO); // Bind to VAO (stores attribute pointer data)
						if (mesh.geometry->indices.size() == 0) { // If it's not indexed, use glDrawArrays
							glDrawArrays(GL_TRIANGLES, 0, mesh.geometry->vertexData.size());
						}
						else { // If it is, use glDrawElements
							glDrawElements(GL_TRIANGLES, mesh.geometry->indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
						}
					glBindVertexArray(0);
				unbindTextures(mesh.textures);
		}
	}
	for (auto collection : collection->meshCollections) {
		renderMeshCollectionRecursive(camera, collection.get());
	} */
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



void Renderer::uploadTexture(Texture* texture)
{
	// first check if the texture already exists in GPU memory
	bool found = false;
	for (int i = 0; i < loadedTextures.size(); i++) {
		if (texture->path == loadedTextures.at(i).first.path) {
			loadedTextures.at(i).second++; // increment count
			*texture = loadedTextures.at(i).first;
			texture->needsUpdate = false;

			return;
		}
	}
	if (!found) { // new texture? upload it!
		texture->loadImageData();

		if (texture->width != 1024 || texture->height != 1024) {
			std::cout << "texture width aint 1024: " << texture->path << ", " << texture->width << "x" << texture->height << std::endl;

			glGenTextures(1, &texture->textureId);
			glBindTexture(GL_TEXTURE_2D, texture->textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, texture->image);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			texture->freeImageData();
		}
		else {
			addTextureToTextureArray(texture);

		}

		texture->needsUpdate = false;
		loadedTextures.push_back(std::pair<Texture, int>(*texture, 1));
	}
}

void Renderer::bindTextures(std::vector<Texture>* textures, GLuint program)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for (int i = 0; i < textures->size(); i++) {
		if (boundTextures.at(i) == textures->at(i).textureId) {
			//continue;
		}

		boundTextures.at(i) = textures->at(i).textureId;

		std::stringstream ss;
		std::string number;
		std::string name = textures->at(i).type;


		if (textures->at(i).textureArrayLocation != -1) {
			if (name == "texture_diffuse" && diffuseNr == 1)
			{
				glUniform1i(1, textures->at(i).textureArrayLocation);
			}
		}
		else {
			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
											  // Retrieve texture number (the N in diffuse_textureN)
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			number = ss.str();
			// Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			// And finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures->at(i).textureId);
		}

	}
}

void Renderer::unbindTextures(std::vector<Texture> textures) {
	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::initializeTextureArray() {
	std::cout << "initializing texture array" << std::endl;

	glGenTextures( 1, &arrayTexture );
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, arrayTexture);
 
	//Create storage for the texture. (100 layers of 1x1 texels)
	glTexStorage3D( GL_TEXTURE_2D_ARRAY,
				5,                    //No mipmaps as textures are 1x1
				GL_RGB8,              //Internal format
				1024, 1024,                 //width,height
				100                   //Number of layers
			);

	/*GLubyte red[] = { 255, 0, 0 };
	GLubyte green[] = { 0, 255, 0 };
	GLubyte blue[] = { 0, 0, 255 };

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
	0,                      //Mipmap number
	0, 0, 0, //xoffset, yoffset, zoffset
	1, 1, 1,          //width, height, depth
	GL_RGB,                 //format
	GL_UNSIGNED_BYTE,       //type
	red); //pointer to data

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
	0,                      //Mipmap number
	0, 0, 1, //xoffset, yoffset, zoffset
	1, 1, 1,          //width, height, depth
	GL_RGB,                 //format
	GL_UNSIGNED_BYTE,       //type
	green); //pointer to data

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
	0,                      //Mipmap number
	0, 0, 2, //xoffset, yoffset, zoffset
	1, 1, 1,          //width, height, depth
	GL_RGB,                 //format
	GL_UNSIGNED_BYTE,       //type
	blue); //pointer to data */

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	std::cout << "done initializing texture array" << std::endl;
}

void Renderer::addTextureToTextureArray(Texture* texture)
{
	//if (true) return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, arrayTexture);

	if (!texture->loaded) texture->loadImageData();
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
		0,                      //Mipmap number
		0, 0, currentSpotInArrayTexture, //xoffset, yoffset, zoffset
		1024, 1024, 1,          //width, height, depth
		GL_RGB,                 //format
		GL_UNSIGNED_BYTE,       //type
		texture->image); //pointer to data
	texture->freeImageData();

	texture->textureArrayLocation = currentSpotInArrayTexture;
	std::cout << "added " << texture->path << " to texture array location " << texture->textureArrayLocation << std::endl;

	currentSpotInArrayTexture++;
}

void Renderer::generateTextureArrayMips() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, arrayTexture);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
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