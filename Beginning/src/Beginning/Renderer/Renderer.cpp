#include "Renderer.h"

#include "glad/glad.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "../../bgpch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd
#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

namespace Beginning {
	
	Renderer* Renderer::instance = nullptr;

	GLint m_width;
	GLint m_height;

	const char* vertexShaderSourceSprite = R"glsl(
		#version 330 core

		in vec3 position;
		in vec3 color;
		in vec2 texCoord;
		
		out vec3 Color;
		out vec2 TexCoord;

		uniform mat4 transform;
		uniform vec2 spriteSheetDims;
		uniform vec2 spriteIndex;
		
		uniform vec2 textureSize; // New uniform for sprite size

		void main()
		{
			Color = color;

			vec2 spriteSize = 1.0 / spriteSheetDims;
			TexCoord = texCoord * spriteSize + spriteIndex * spriteSize;

			//TexCoord = (texCoord + spriteIndex) / spriteSheetDims;

			gl_Position = transform * vec4(position, 1.0);
			// Adjust position based on sprite size

			//vec2 adjustedPosition = position.xy * vec2(textureSize.x / textureSize.y, 1.0);
			//gl_Position = transform * vec4(adjustedPosition, position.z, 1.0);
		}
		)glsl";

	const char* fragmentShaderSourceSprite = R"glsl(
		#version 330 core
		in vec3 Color;
		in vec2 TexCoord;

		out vec4 outColor;

		uniform sampler2D ourTexture;
		uniform sampler2D ourTexture2;

		uniform float opacity;

		// Define the color key (pink in this case)
		const vec3 colorKey = vec3(0.956, 0.0, 0.99); // Pink color key
		const float threshold = 0.1; // Threshold for color matching

		void main()
		{
			vec4 colTex1 = texture(ourTexture, TexCoord);
				
			// If the color of the texture matches the color key, discard the fragment
			if (distance(colTex1.rgb, colorKey) < threshold) {
				discard;
			}

			//outColor = colTex1;
			outColor = vec4(colTex1.rgb, colTex1.a * opacity);

		}
)glsl";

	const char* vertexShaderSourceText = R"glsl(
		#version 330 core

		in vec3 position;
		in vec2 texCoord;
		
		out vec2 TexCoord;

		uniform mat4 transform;

		void main()
		{
			TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);

			gl_Position = transform * vec4(position, 1.0);
		}
		)glsl";

	const char* fragmentShaderSourceText = R"glsl(
		#version 330 core
		in vec2 TexCoord;

		out vec4 outColor;

		uniform sampler2D ourTexture;

		uniform vec4 textColor;

		void main()
		{
			// Sample the texture
			vec4 sampled = texture(ourTexture, TexCoord);

			// Discard fragments with very low alpha (transparent parts)
			if (sampled.a < 0.1)
				discard;

			// Multiply the text color with the sampled texture
			 outColor = vec4(textColor.rgb, sampled.a * textColor.a);

		}
)glsl";

	void Renderer::RemoveObjectInstance(Object* object)
	{
		for (auto it = objectInstances.begin(); it != objectInstances.end(); ) {
			if (it->m_Owner == object) {
				glDeleteTextures(1, &it->texture);
				glBindTexture(GL_TEXTURE_2D, 0);
				it = objectInstances.erase(it);  // Update iterator after erase
				break;
			}
			else {
				++it;
			}
		}
	}

	Renderer::Renderer() : m_window(nullptr), m_glContext(nullptr), m_vboRect(0), m_eboRect(0), m_vaoRect(0), m_shaderProgram(0), objectInstances()
	{
		instance = this;
	}

	Renderer::~Renderer() {

		//for (auto& objStruct : objectInstances) {
		//	delete objStruct.m_Owner;  // Only if Renderer owns the objects
		//}
		
		for (auto& objStruct : objectInstances) {
			objStruct.componentID = 0;
			glDeleteTextures(1, &objStruct.texture);
			objStruct.texture = 0;
			objStruct.textureName = "";
			objStruct.position = glm::vec3(0.0f);
			objStruct.scale = glm::vec3(0.0f);
			objStruct.spriteSheetDims = glm::ivec2(0, 0);
			objStruct.spriteIndex = glm::ivec2(0, 0);
			objStruct.width = 0;
			objStruct.height = 0;
			objStruct.opacity = 0.0f;
			objStruct.lastFrameTime = 0;
			objStruct.frameDuration = 0.0f;
			objStruct.currentFrame = 0;
		}
		objectInstances.clear();


		// Delete buffers
		if (m_vboRect != 0) {
			glDeleteBuffers(1, &m_vboRect);
			m_vboRect = 0;
		}
		if (m_eboRect != 0) {
			glDeleteBuffers(1, &m_eboRect);
			m_eboRect = 0;
		}
		if (m_vaoRect != 0) {
			glDeleteVertexArrays(1, &m_vaoRect);
			m_vaoRect = 0;
		}

		// Delete shader programs
		if (m_shaderProgram != 0) {
			glDeleteProgram(m_shaderProgram);
			m_shaderProgram = 0;
		}
		
		// Destroy SDL window and OpenGL context
		if (m_window != nullptr) {
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}
		if (m_glContext != nullptr) {
			SDL_GL_DeleteContext(m_glContext);
			m_glContext = nullptr;
		}

		fragmentShaderSourceSprite = nullptr;
		vertexShaderSourceSprite = nullptr;

		// Quit SDL
		//SDL_Quit();
	}

	int Renderer::InitRenderer(int screenWidth, int screenHeight) {

		// Initialize SDL
		SDL_Init(SDL_INIT_VIDEO);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		

		//Create an SDL window
		m_window = SDL_CreateWindow("Beginning", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
		if (m_window == nullptr)
		{
			std::cout << "Failed to create SDL Window" << std::endl;
			SDL_Quit();
			return -1;
		}

		//Create an OpenGL context
		SDL_GLContext m_glContext = SDL_GL_CreateContext(m_window);

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			SDL_Quit();
			return -2;
		}

		TTF_Init();


		InitVertexData();
		ShaderProgram(vertexShaderSourceSprite, fragmentShaderSourceSprite);

		InitVertexDataText();
		TextShaderProgram(vertexShaderSourceText, fragmentShaderSourceText);

		return 0;
	}

	int Renderer::InitVertexData()
	{
		//Generates the vertex buffer object, VBO (that will store the vertex data (such as positions, colors, normals, texture coordinates, etc.)
		glGenBuffers(1, &m_vboRect);

		//Generates the element buffer object (EBO), which will store the indices used for drawing the shape.
		glGenBuffers(1, &m_eboRect);

		//Generates the vertex array object (VAO), which will store the configuration of the vertex attribute pointers.
		glGenVertexArrays(1, &m_vaoRect);


		float verticess[] = {
			// positions         // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
		};

		unsigned int indicess[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};


		glBindVertexArray(m_vaoRect);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboRect);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticess), (const void*)&verticess, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboRect);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicess), (const void*)&indicess, GL_STATIC_DRAW);


		return 0;
	}

	int Renderer::InitVertexDataText()
	{
		//Generates the vertex buffer object, VBO (that will store the vertex data (such as positions, colors, normals, texture coordinates, etc.)
		glGenBuffers(1, &m_vbo);

		//Generates the element buffer object (EBO), which will store the indices used for drawing the shape.
		glGenBuffers(1, &m_ebo);

		//Generates the vertex array object (VAO), which will store the configuration of the vertex attribute pointers.
		glGenVertexArrays(1, &m_vao);


		float verticess[] = {
			// positions       // texture coords
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top left
		};

		unsigned int indicess[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};


		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticess), (const void*)&verticess, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicess), (const void*)&indicess, GL_STATIC_DRAW);


		return 0;
	}

	int Renderer::ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		//Creates the vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		GLint  success;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//Creates the fragment shader

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//Creates the shader program
		m_shaderProgram = glCreateProgram();

		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//Interprets the data in the vertex buffer object (VBO) and pass it to the shader program. 
		GLint posAttrib = glGetAttribLocation(m_shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		GLint colorAttrib = glGetAttribLocation(m_shaderProgram, "color");
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		GLint texCoordAttrib = glGetAttribLocation(m_shaderProgram, "texCoord");
		glEnableVertexAttribArray(texCoordAttrib);
		glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		return 0;
	}

	int Renderer::TextShaderProgram(const char* vertexShaderSourceText, const char* fragmentShaderSourceText)
	{
		//Creates the vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSourceText, NULL);
		glCompileShader(vertexShader);

		GLint  success;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//Creates the fragment shader

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSourceText, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//Creates the shader program
		m_shaderProgramText = glCreateProgram();

		glAttachShader(m_shaderProgramText, vertexShader);
		glAttachShader(m_shaderProgramText, fragmentShader);
		glLinkProgram(m_shaderProgramText);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glGetProgramiv(m_shaderProgramText, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_shaderProgramText, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Position attribute (layout(location = 0) in the shader)
		GLint posAttrib = glGetAttribLocation(m_shaderProgramText, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		// Texture coordinate attribute (layout(location = 1) in the shader)
		GLint texCoordAttrib = glGetAttribLocation(m_shaderProgramText, "texCoord");
		glEnableVertexAttribArray(texCoordAttrib);
		glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		return 0;
	}


	void Renderer::Update()
	{
		// Clear the screen at the start
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(m_vaoRect);
		

		// Use the shader program for objects
		glUseProgram(m_shaderProgram);
		glValidateProgram(m_shaderProgram);


		// Render each object instance
		for (unsigned int i = 0; i < objectInstances.size(); ++i) {

			//Matrix used to apply scaling and translation transformations to the object.
			glm::mat4 transform(1.0f);
			float opacity = objectInstances[i].opacity;

			//Uniforms are constant across all vertices in a single draw call. They provide the shader with information that is the same for the entire object being rendered.

			//The transform uniform is used to apply scaling and translation transformations to the object.
			GLuint transformLocation = glGetUniformLocation(m_shaderProgram, "transform");

			//The spriteSheetDims uniform is used to specify the dimensions of the sprite sheet. This uniform helps the shader understand how to divide the texture into individual sprites.
			GLuint spriteSheetDimsLocation = glGetUniformLocation(m_shaderProgram, "spriteSheetDims");

			//The spriteIndex uniform is used to specify the index of the sprite to render. This uniform helps the shader understand which sprite to render from the sprite sheet.
			GLuint spriteIndexLocation = glGetUniformLocation(m_shaderProgram, "spriteIndex");

			//The textureSize uniform is used to specify the size of the texture. This uniform helps the shader understand the aspect ratio of the texture.
			GLuint textureSizeLocation = glGetUniformLocation(m_shaderProgram, "textureSize");

			//The opacity uniform is used to specify opacity of the texture.
			GLuint opacityLocation = glGetUniformLocation(m_shaderProgram, "opacity");

			//Calculates the scale and position of the object
			transform = glm::translate(transform, glm::vec3(objectInstances[i].position / 100.f));
			glm::vec3 scale = glm::vec3(objectInstances[i].width / 100.f, objectInstances[i].height / 100.f, 1.0f);
			transform = glm::scale(transform, scale);

			// Apply the rotation matrix to rotate the scene 90 degrees clockwise
			transform = glm::rotate(transform, glm::radians(objectInstances[i].rotation), glm::vec3(0.0f, 0.0f, 1.0f));

			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
			glUniform2f(spriteSheetDimsLocation, objectInstances[i].spriteSheetDims.x, objectInstances[i].spriteSheetDims.y);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glUniform1f(opacityLocation, opacity);


			//Checks if the object has an AnimatorComponent.
			if (objectInstances[i].hasAnimator == true)
			{
				AnimatorComponent* animator = objectInstances[i].m_Owner->GetComponent<AnimatorComponent>();
				if (animator != nullptr)
				{
					if (animator->GetAnimationCount() > 0)
					{
						// Updates the current frame of the object, getting the current frame of the animation
						int currentFrameX = animator->GetCurrentFrame().x;
						int currentFrameY = animator->GetCurrentFrame().y;

						//Redefines the sprite index based on the current frame.
						glUniform2f(spriteIndexLocation, currentFrameX, currentFrameY);
					}
				}
			}
			else
			{
				glUniform2f(spriteIndexLocation, objectInstances[i].spriteIndex.x, objectInstances[i].spriteIndex.y);
			}


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, objectInstances[i].texture);

			glBindVertexArray(m_vaoRect);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);

		}

		// Disable depth testing for UI elements to render them on top
		//glDisable(GL_DEPTH_TEST);

		// Render UI elements (UIImages)
		for (unsigned int y = 0; y < imageInstances.size(); ++y) {

			if (imageInstances[y].isVisible == false)
			{
				imageInstances[y].opacity = 0.0f;
			}
			else if (imageInstances[y].isVisible == true)
			{
				imageInstances[y].opacity = 1.0f;
			}
			//Matrix used to apply scaling and translation transformations to the object.
			glm::mat4 transform(1.0f);
			float opacity = imageInstances[y].opacity;

			//Uniforms are constant across all vertices in a single draw call. They provide the shader with information that is the same for the entire object being rendered.

			//The transform uniform is used to apply scaling and translation transformations to the object.
			GLuint transformLocation = glGetUniformLocation(m_shaderProgram, "transform");

			//The spriteSheetDims uniform is used to specify the dimensions of the sprite sheet. This uniform helps the shader understand how to divide the texture into individual sprites.
			GLuint spriteSheetDimsLocation = glGetUniformLocation(m_shaderProgram, "spriteSheetDims");

			//The spriteIndex uniform is used to specify the index of the sprite to render. This uniform helps the shader understand which sprite to render from the sprite sheet.
			GLuint spriteIndexLocation = glGetUniformLocation(m_shaderProgram, "spriteIndex");

			//The textureSize uniform is used to specify the size of the texture. This uniform helps the shader understand the aspect ratio of the texture.
			GLuint textureSizeLocation = glGetUniformLocation(m_shaderProgram, "textureSize");

			//The opacity uniform is used to specify opacity of the texture.
			GLuint opacityLocation = glGetUniformLocation(m_shaderProgram, "opacity");

			//Calculates the scale and position of the object
			transform = glm::translate(transform, glm::vec3(imageInstances[y].position / 100.f));
			glm::vec3 scale = glm::vec3(imageInstances[y].width / 100.f, imageInstances[y].height / 100.f, 1.0f);
			transform = glm::scale(transform, scale);

			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
			glUniform2f(spriteSheetDimsLocation, imageInstances[y].spriteSheetDims.x, imageInstances[y].spriteSheetDims.y);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glUniform1f(opacityLocation, opacity);

			glUniform2f(spriteIndexLocation, imageInstances[y].spriteIndex.x, imageInstances[y].spriteIndex.y);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, imageInstances[y].texture);

			glBindVertexArray(m_vaoRect);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

		glBindVertexArray(0);
		
		for (unsigned int w = 0; w < textInstances.size(); ++w) {


			glBindVertexArray(m_vao);

			// Use the shader program for objects
			glUseProgram(m_shaderProgramText);
			glValidateProgram(m_shaderProgramText);

			//Matrix used to apply scaling and translation transformations to the object.
			glm::mat4 transform(1.0f);

			//The transform uniform is used to apply scaling and translation transformations to the object.
			GLuint transformLocation = glGetUniformLocation(m_shaderProgramText, "transform");

			GLuint textColorLocation = glGetUniformLocation(m_shaderProgramText, "textColor");

			GLuint textureLocation = glGetUniformLocation(m_shaderProgramText, "ourTexture");
			if (textureLocation == -1) {
				std::cout << "Failed to get the uniform location for ourTexture" << std::endl;
			}

			glm::vec4 textColor = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);  // White color

			//Calculates the scale and position of the object
			transform = glm::translate(transform, glm::vec3(textInstances[w].position / 100.f));
			glm::vec3 scale = glm::vec3(glm::vec3(m_textWidth / 100.f, m_textHeight /100.f, 1.0f));
			transform = glm::scale(transform, scale);

			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
			
			glUniform4fv(textColorLocation, 1, glm::value_ptr(textColor));

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textInstances[w].texture);
			glUniform1i(textureLocation, 0);
			
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		// Re-enable depth testing after rendering UI, if needed

		glBindVertexArray(0);  // Unbind VAO

		// Swap window buffers to display the rendered image
		SDL_GL_SwapWindow(m_window);


	}

	void Renderer::UpdateRender(Object* owner, int componentID, glm::vec3 position, glm::ivec2 spriteIndex, float opacity)
	{
		for (auto& instance : objectInstances) {
			if (instance.m_Owner == owner && instance.componentID == componentID && instance.spriteIndex == spriteIndex) {
				instance.m_Owner = owner;
				instance.position = position;
				instance.opacity = opacity;
				break;
			}
		}
	}

	void Renderer::UpdateUIRender(UIImage* owner, glm::vec3 position, int width, int height, bool isVisible)
	{
		for (auto& instance : imageInstances) {
			if (instance.m_Owner == owner) {
				instance.m_Owner = owner;
				instance.position = position;
				instance.width = width;
				instance.height = height;
				instance.isVisible = isVisible;
				break;
			}
		}
	}


	unsigned int Renderer::LoadTexture(std::string fileName, Object* owner)
	{
		
		//Instead of returning a new texture ID, if texture already exists, return the existing texture ID::RESOURCE_MANAGEMENT
		for (unsigned int i = 0; i < objectInstances.size(); ++i) {
			if (objectInstances[i].textureName == fileName && objectInstances[i].m_Owner == owner) {
 				glBindTexture(GL_TEXTURE_2D, objectInstances[i].texture);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_width);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_height);

				glBindTexture(GL_TEXTURE_2D, 0);
				return objectInstances[i].texture;
			}
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_set_flip_vertically_on_load(true);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			if (nrChannels == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				if (nrChannels == 3) { // RGB
					size_t buf_size = width * height * 4;
					unsigned char* rgba_data = new unsigned char[buf_size];
					for (int i = 0; i < width * height; ++i) {
						rgba_data[i * 4 + 0] = data[i * 3 + 0]; // R
						rgba_data[i * 4 + 1] = data[i * 3 + 1]; // G
						rgba_data[i * 4 + 2] = data[i * 3 + 2]; // B
						rgba_data[i * 4 + 3] = 255; // Alpha, set to opaque
					}
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_data);
					delete[] rgba_data;
				}
			}
		}
		else
		{
			std::cout << "Failed to load texture " << stbi_failure_reason() << std::endl;
			glDeleteTextures(1, &texture);  // Delete the texture if loading failed
		}
		

		glBindTexture(GL_TEXTURE_2D, texture);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_height);

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
		return texture;
	}

	unsigned int Renderer::LoadText(TTF_Font* font, std::string text, glm::vec4 color)
	{
		// Use Blended mode to generate a surface with alpha values (text opaque, background transparent)
		SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255, 255});

		if (textSurface == nullptr) {
			std::cout << "Failed to convert surface: " << SDL_GetError() << std::endl;
		}
		else {
			// Now create the OpenGL texture with the formatted surface
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurface->w, textSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textSurface->pixels);
			SDL_FreeSurface(textSurface);  // Free the formatted surface when done
		}

		m_textWidth = textSurface->w;
		m_textHeight = textSurface->h;

		SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);

		// Generate an OpenGL texture from this surface
		unsigned int textTexture;
		glGenTextures(1, &textTexture);
		glBindTexture(GL_TEXTURE_2D, textTexture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Set byte alignment to 1 to avoid alignment issues


		// Set texture parameters for OpenGL
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		// Load the surface data into OpenGL (alpha channel only)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurface->w, textSurface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, textSurface->pixels);
		glBindTexture(GL_TEXTURE_2D, textTexture);



		//SDL_FreeSurface(textSurface);  // Free the SDL surface
		return textTexture;
	}

	void Renderer::AddRender(Object* owner, int componentID, std::string textureName, glm::vec3 position, int width, int height, glm::vec3 scale, glm::ivec2 spriteSheetDims, glm::ivec2 spriteIndex, float opacity, float rotation)
	{
		// Look for an existing instance with the same owner
		for (auto& instance : objectInstances) {
			//Tilemap uses the same owner for multiple renders of the same rendercomponent, so we need to check more variables before assuming its the same sprite
			if (instance.m_Owner == owner && instance.textureName == textureName && instance.spriteIndex == spriteIndex) {
				instance.m_Owner = owner;
				instance.position = position;
				instance.width = width;
				instance.height = height;
				instance.scale = scale;
				instance.spriteSheetDims = spriteSheetDims;
				instance.spriteIndex = spriteIndex;
				instance.opacity = opacity;
				instance.rotation = rotation;
				return; // Update existing instance and return
			}
		}

		bool hasAnimator = false;
		if (owner->GetComponent<AnimatorComponent>() != nullptr)
		{
			hasAnimator = true;
		}

		//If no existing instance found, create a new one
		objectInstances.push_back({ owner, componentID, LoadTexture(textureName, owner), textureName,position, scale, spriteSheetDims, spriteIndex, width, height, rotation, opacity, hasAnimator});

		
		// Sort instances by z-value 
		std::sort(objectInstances.begin(), objectInstances.end(), [](const auto& a, const auto& b) {
			return a.position.z < b.position.z;
		});
	}

	void Renderer::AddUIRender(UIImage* owner, std::string textureName, glm::vec3 position, int width, int height, glm::vec3 scale, glm::ivec2 spriteSheetDims, glm::ivec2 spriteIndex, float opacity, bool isVisible, float rotation)
	{

		// Look for an existing instance with the same owner
		for (auto& instance : imageInstances) {
			//Tilemap uses the same owner for multiple renders of the same rendercomponent, so we need to check more variables before assuming its the same sprite
			if (instance.m_Owner == owner && instance.textureName == textureName && instance.spriteIndex == spriteIndex) {
				instance.m_Owner = owner;
				instance.position = position;
				instance.width = width;
				instance.height = height;
				instance.scale = scale;
				instance.spriteSheetDims = spriteSheetDims;
				instance.spriteIndex = spriteIndex;
				instance.opacity = opacity;
				instance.isVisible = isVisible;
				instance.rotation = rotation;
				return; // Update existing instance and return
			}
		}

		//If no existing instance found, create a new one
		imageInstances.push_back({ owner, LoadTexture(textureName, nullptr), textureName,position, scale, spriteSheetDims, spriteIndex, width, height, rotation, opacity, isVisible});


		// Sort instances by z-value 
		std::sort(imageInstances.begin(), imageInstances.end(), [](const auto& a, const auto& b) {
			return a.position.z < b.position.z;
			});
	}

	void Renderer::AddTextRender(UIText* owner, std::string text, glm::vec4 color, TTF_Font* font, int size, glm::vec3 position)
	{
		// Look for an existing instance with the same owner
		for (auto& instance : textInstances) {
			//Tilemap uses the same owner for multiple renders of the same rendercomponent, so we need to check more variables before assuming its the same sprite
			if (instance.m_Owner == owner) {
				instance.m_Owner = owner;
				instance.size = size;
				instance.position = position;
				return; // Update existing instance and return
			}
		}

		unsigned int texture = LoadText(font, text, color);

		//If no existing instance found, create a new one
		textInstances.push_back({ owner, texture, size, position});
	}


}