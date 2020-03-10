#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>
#include <math.h>
#include "text3d.h"

//Loads the specified font file into a new T3DFont object
T3DFont::T3DFont(const char* filename)
{
	std::ifstream input;
	input.open(filename, std::istream::binary);

	unsigned short width;
	unsigned short height;
	unsigned short readShort;
	unsigned short numVerts;
	const int stripResetKey = 65533;
	
	int totalVerts = 0;

	glGenVertexArrays(1, &myShape.VAO);
	glBindVertexArray(myShape.VAO);

	glGenBuffers(1, &myShape.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, myShape.VBO);

	glGenBuffers(1, &myShape.IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myShape.IBO);

	startIndex[0] = 0;
	startVertex[0] = 0;

	// build display list for 94 characters
	for(int i = 0; i < 94; i++) 
	{		
		input.read((char*)&width, 2);
		input.read((char*)&height, 2);
		input.read((char*)&numVerts, 2);

		endVertex[i] = startVertex[i] + numVerts;

		if (i < 97)
			startVertex[i + 1] = endVertex[i];

		// simple math
		widths[i] = (float)width / (float)height;

		// get all vertex data in the file
		for(int j = 0; j < numVerts*2; j++) {
			input.read((char*)&readShort, 2);
			myShape.vertices.push_back(((int)readShort - 32768) / (float)height);
		}

		// Vertex Buffer ^^^
		// Index Buffer vvv

		// initialize values
		unsigned int stripCount = 0;
		unsigned int savedIndex[2];

		// more than stripResetKey means you reached the end
		while(readShort <= stripResetKey) 
		{
			input.read((char*)&readShort, 2);

			// stripResetKey resets the stripCount
			if (readShort == stripResetKey) 
				stripCount = 0;

			// everything else is an index of the vertices
			else if (readShort < stripResetKey)
			{
				stripCount++;

				if (stripCount > 3)
				{
					myShape.indicies.push_back(startVertex[i] + savedIndex[0]);
					myShape.indicies.push_back(startVertex[i] + savedIndex[1]);
				}

				myShape.indicies.push_back(startVertex[i] + readShort);

				// 0 is from two vertices ago
				// 1 is from one vertex ago
				savedIndex[0] = savedIndex[1];
				savedIndex[1] = readShort;
			}

			endIndex[i] = myShape.indicies.size();

			if (i < 93)
				startIndex[i + 1] = endIndex[i];
		}
	}

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * myShape.vertices.size(),
		&myShape.vertices[0],
		GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned short) * myShape.indicies.size(),
		&myShape.indicies[0],
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	input.close();
}

bool firstTime = true;

void T3DFont::draw2D(char c) {

	// All characters share one Vertex Buffer and one Index Buffer,
	// this reduces CPU overhead, by removing the need to constantly
	// change the buffers

	if (firstTime)
	{
		glBindVertexArray(myShape.VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myShape.IBO);
		firstTime = false;
	}

	if (c >= 33 && c <= 126)
	{
		int letterIndex = c - '!';
		glDrawElements(GL_TRIANGLES, endIndex[letterIndex] - startIndex[letterIndex], GL_UNSIGNED_SHORT, (void*)(startIndex[letterIndex] * sizeof(unsigned short)));
	}
}

float T3DFont::GetWidth(char c) {
	if (c >= 33 && c <= 126) 
		return widths[c - 33];
	
	// Treat unknown characters as spacebar
	else return 0.5f;
}