#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct shape {

	std::vector<float> vertices;

	GLuint VAO;
	GLuint VBO;
	GLuint IBO;

	std::vector<unsigned short> indicies;
};

class T3DFont {
private:
	shape myShape;
	float widths[94];
	unsigned short startIndex[94];
	unsigned short endIndex[94];
	unsigned short startVertex[94];
	unsigned short endVertex[94];
public:
	//Loads the specified font file into a new T3DFont object
	T3DFont(const char* filename);

	void draw2D(char c);
	float GetWidth(char c);
};
