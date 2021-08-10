#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "Structures.h"
#include <array>

/*
	Reads the ambient, specular and diffuse information from an .MTL file.
*/
static std::vector<std::array<float, 3>> ReadFromObjMaterial(std::string filePath);
static std::vector<std::array<float, 3>> ReadFromObjMaterial(std::string filePath)
{
	std::vector<std::array<float, 3>> mat;
	std::fstream reader;
	std::string line;
	reader.open(filePath, std::fstream::in);// Reads only from file

	while (std::getline(reader, line))
	{
		int type_finder;
		type_finder = (int)line.find(" ", 0);
		std::string type = line.substr(0, type_finder);

		// Ambient
		if (type == "Ka")
		{
			float x, y, z;
			std::string coords = line.substr(type_finder);
			std::stringstream ss(coords);
			ss >> x >> y >> z;
			std::array<float, 3> ambient = { x, y, z };
			mat.push_back(ambient);
		}
		else if (type == "Kd") // Diffuse
		{
			float x, y, z;
			std::string coords = line.substr(type_finder);
			std::stringstream ss(coords);
			ss >> x >> y >> z;
			std::array<float, 3> diffuse = { x, y, z };
			mat.push_back(diffuse);
		}
		else if (type == "Ks") // Specular
		{
			float x, y, z;
			std::string coords = line.substr(type_finder);
			std::stringstream ss(coords);
			ss >> x >> y >> z;
			std::array<float, 3> specular = { x, y, z };
			mat.push_back(specular);
		}
	}

	reader.close();
	return mat;
}

/* 
	Keep in mind when importing objects that they HAVE to be triangulated before
	you read from them! Otherwise D3D11 will read from them completely wrong.
	This function only works with context->draw() command. Vertex integer will add
	the amount of vertexes from current loaded object.
	This only reads in .OBJ files.
*/
static std::vector<Vertex> ReadFromObj(std::string filePath, int & vertex);
static std::vector<Vertex> ReadFromObj(std::string filePath, int & vertex)
{
	// To be returned complete value of all vectors
	std::vector<Vertex> vertexVector;

	std::vector<std::array<float, 3>> position; // Position value
	std::vector<std::array<float, 2>> texCoord; // UV value
	std::vector<std::array<float, 3>> normal; // Normal value

	// Holds our faces from obj file
	std::vector<std::array<int, 3>> faces;

	std::fstream reader;
	std::fstream writer;

	writer.open("objectInfo.txt", std::fstream::out);

	reader.open(filePath, std::fstream::in);// Reads only from file
	if (!reader.is_open()) // return an error vector if file can't open
	{
		std::vector<Vertex> errorVector;
		return errorVector;
	}

	// Read the information from the file
	std::string line;
	while (std::getline(reader, line))
	{
		int type_finder;
		type_finder = (int)line.find(" ", 0);
		std::string type = line.substr(0, type_finder);

		// Coordinate and Normal
		if (type == "v" || type == "vn")
		{
			float x, y, z;
			// Read from file
			std::string coords = line.substr(type_finder);
			std::stringstream ss(coords);
			ss >> x >> y >> z;

			// Put into coordinate
			if (type == "v")
			{
				std::array<float, 3> newPos = { x, y, z };
				position.push_back(newPos);
			}
			// Put into Normal
			else
			{
				std::array<float, 3> newNormal = { x, y, z };
				normal.push_back(newNormal);
			}
		}
		// UV
		else if (type == "vt")
		{
			float x, y;
			// Read from file
			std::string coords = line.substr(type_finder);
			std::stringstream ss(coords);
			ss >> x >> y;

			std::array<float, 2> newUV = { x, y };
			texCoord.push_back(newUV);
		}

		// Read in the faces
		else if (type == "f")
		{
			int pos, uv, norm;
			char f, s;
			std::string coords = line.substr(type_finder);
			std::stringstream ss(coords);
			while (!ss.eof()) // More than one face in one line
			{
				/*
					Format is P/U/N so we read in int char int char int
					and then throw away the char since its not interesting
				*/
				ss >> pos >> f >> uv >> s >> norm;

				// Since vectors start at pos 0 and obj start at pos 1
				pos -= 1;
				uv -= 1;
				norm -= 1;

				std::array<int, 3> newFace = { pos, uv, norm };
				faces.push_back(newFace);
			}
		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		int pos = faces[i][0];
		int uv = faces[i][1];
		int norm = faces[i][2];
		Vertex newVertex(position[pos], texCoord[uv], normal[norm]);

		// If debug is needed
		/*
		writer << "Pos: x: " << newVertex.pos.x << " y: " << newVertex.pos.y << " z: " << newVertex.pos.z << " | UV: x:" <<
			newVertex.uv.x << " y: " << newVertex.uv.y << " | Normal: x:" <<
			newVertex.normal.x << " y: " <<newVertex.normal.y << " z: " <<  newVertex.normal.z << "\n";
		*/
		vertexVector.push_back(newVertex);
	}

	vertex += (int)faces.size();

	reader.close();
	writer.close();

	return vertexVector;
}
