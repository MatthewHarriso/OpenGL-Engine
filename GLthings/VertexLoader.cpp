#include "VertexLoader.h"
#include "Vertex.h"
#include "FlyCamera.h"

#include <GLM/ext.hpp>
#include <GLM/glm.hpp>

#define VERTEX_DIST 0.25f

VertexLoader* VertexLoader::instance = nullptr;

VertexLoader::VertexLoader()
{
	//
	//	if eLight is 1 then it is a direction light.
	//
	//	if eLight is 2 then it is a point light.
	//
	//	if eLight is 3 then it is a spot light.
	//
	eLight = 1;

	timer = 0;

	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 position; \
							layout(location=1) in vec4 normal; \
							layout(location=2) in vec4 colour; \
							out vec4 vColour; \
							out vec4 vNormal; \
							out vec4 vPosition; \
							uniform mat4 projectionViewWorldMatrix; \
							uniform float time; \
							uniform float heightScale; \
							void main() \
							{ \
								vPosition = position; \
								vColour = vec4(78/255.0f, 46 / 255.0f, 40 / 255.0f, 1); \
								vec4 P = position; \
								P.y += sin((time * 10) + (-position.x * 0.125f)) * 4; \
								P.y += (heightScale + (position.x * 0.25f) * 0.75f) * cos(time * 2); \
								P.y += sin((time * 7.5f) + (abs(position.x * 0.05f))) * (cos(time * 2) * 2); \
								vNormal = normal; \
								gl_Position = projectionViewWorldMatrix * P;}";
							
							//vColour.b = -P.y; \
							//
							//	This makes the image wave like an ocean/flag.
							//

	const char* fsSource = "#version 410\n \
							in vec4 vColour; \
							in vec4 vNormal; \
							in vec4 vPosition; \
							out vec4 fragColor; \
							uniform vec3 LightDir; \
							uniform vec3 LightColour; \
							uniform vec3 CameraPos; \
							uniform float SpecPow; \
							uniform int lightType; \
							void main() \
							{ \
								float d = max(0, dot(normalize(vNormal.xyz), LightDir)); \
								vec3 E = normalize(CameraPos - vPosition.xyz); \
								vec3 R = reflect(-LightDir, vNormal.xyz); \
								float s = max(0, dot(E, R)); \
								s = pow(s, SpecPow); \
								if (s < 0.1) \
								{ \
									s = 0.2; \
								} \
								fragColor = vec4((vec4((LightColour * d + LightColour * s),1) * vColour)); \
							}";
	
	int success = GL_FALSE;

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();

	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	char* infoLog;
	if (success == GL_FALSE)
	{ 
		int infoLogLength = 0;

		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		 infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
	}
	
	glDeleteShader(fragmentShader); 
	glDeleteShader(vertexShader);
}

VertexLoader* VertexLoader::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new VertexLoader();
	}

	return instance;
}

VertexLoader::~VertexLoader()
{

}

std::vector<OpenGLInfo>* VertexLoader::CreateOpenGLBuffers(tinyobj::attrib_t & attribs, std::vector<tinyobj::shape_t>& shapes)
{
	std::vector<OpenGLInfo>* m_glInfo = new std::vector<OpenGLInfo>();

	m_glInfo->resize(shapes.size());
	
	// grab each shape
	int shapeIndex = 0;

	for (auto& shape : shapes)
	{
		// setup OpenGL data
		glGenVertexArrays(1, &m_glInfo->at(shapeIndex).m_VAO);
		glGenBuffers(1, &m_glInfo->at(shapeIndex).m_VBO);
		glBindVertexArray(m_glInfo->at(shapeIndex).m_VAO);

		m_glInfo->at(shapeIndex).m_faceCount = shape.mesh.num_face_vertices.size();

		// collect triangle vertices
		std::vector<OBJVertex> vertices;

		int index = 0;

		for (auto face : shape.mesh.num_face_vertices)
		{
			for (int i = 0; i < 3; ++i)
			{
				tinyobj::index_t idx = shape.mesh.indices[index + i];

				OBJVertex v = { 0 };

				// positions
				v.x = attribs.vertices[3 * idx.vertex_index + 0];
				v.y = attribs.vertices[3 * idx.vertex_index + 1];
				v.z = attribs.vertices[3 * idx.vertex_index + 2];

				// normals
				if (attribs.normals.size() > 0)
				{
					v.nx = attribs.normals[3 * idx.normal_index + 0];
					v.ny = attribs.normals[3 * idx.normal_index + 1];
					v.nz = attribs.normals[3 * idx.normal_index + 2];
				}

				// texture coordinates
				if (attribs.texcoords.size() > 0)
				{
					v.u = attribs.texcoords[2 * idx.texcoord_index + 0];
					v.v = attribs.texcoords[2 * idx.texcoord_index + 1];
				}

				vertices.push_back(v);
			}

			index += face;
		}

		// bind vertex data
		glBindBuffer(GL_ARRAY_BUFFER, m_glInfo->at(shapeIndex).m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(OBJVertex), vertices.data(), GL_STATIC_DRAW);

		//position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), 0);

		//normal data
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), (void*)12);

		//texture data
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void*)24);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		shapeIndex++;
	}

	return m_glInfo;
}

void VertexLoader::SetCamera(FlyCamera * l_camera)
{
	myCamera = l_camera;
}

void VertexLoader::GenerateGrid(unsigned int rows, unsigned int cols)
{
	Vertex* aoVertices = new Vertex[rows * cols];

	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;

	indexCount = (rows - 1) * (cols - 1) * 6;

	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			aoVertices[r * cols + c].position = glm::vec4 ( ((float) (c - VERTEX_DIST)), 0, ((float) (r - VERTEX_DIST)), 1);

			glm::vec3 colour = glm::vec3 (sinf ( (c / ((float) (cols - 1))) * (r / ((float) (rows - 1)))));

			aoVertices[r * cols + c].colour = glm::vec4 (colour, 1);
		}
	}

	for (unsigned int r = 0; r < (rows - 1); r++)
	{
		for (unsigned int c = 0; c < (cols - 1); c++)
		{
			//	traingle 1.
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			//	triangle 2.
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	
	glGenBuffers(1, &m_IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] aoVertices;
	delete[] auiIndices;
}

std::vector<OpenGLInfo>* VertexLoader::LoadGeometry(const char * l_textureFileName)
{
	for (std::map<char*, std::vector<OpenGLInfo>*>::iterator it = models.begin(); it != models.end(); it++)
	{
		if (it->first == l_textureFileName)
		{
			return it->second;
		}
	}

	tinyobj::attrib_t attribs;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool success = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, l_textureFileName);
	
	models.at[l_textureFileName] = CreateOpenGLBuffers(attribs, shapes);
}

void VertexLoader::Update(float l_deltaTime)
{
	
}

void VertexLoader::Draw()
{
	
}
