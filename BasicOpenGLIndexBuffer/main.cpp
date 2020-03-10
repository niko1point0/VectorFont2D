
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "text3d.h"

const char* STRS[4] = { "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRST", "UVWXYZ123456790", "!@#$%^&*()" };
T3DFont* font = nullptr;

// Variables for the Height and width of the window
const GLint WIDTH = 400, HEIGHT = 400;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shaders Tutorial", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	font = new T3DFont("charset");

	Shader basicShader;
	basicShader.load();
	basicShader.use();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 worldMatrix;
		glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);

		for (int i = 0; i < 4; i++)
		{
			glm::vec3 position = glm::vec3(-0.9f, 0.9f - 0.2*(i+1), 0.0f);

			for (int j = 0; STRS[i][j] != '\0'; j++)
			{
				float width = font->GetWidth(STRS[i][j]);

				position.x += scale.x * width / 2;

				worldMatrix =
					glm::translate(position) *
					glm::scale(scale);

				glUniformMatrix4fv(0, 1, GL_FALSE, &worldMatrix[0][0]);

				font->draw2D(STRS[i][j]);

				position.x += scale.x * width / 2;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete font;
	glfwTerminate();

	return EXIT_SUCCESS;
}