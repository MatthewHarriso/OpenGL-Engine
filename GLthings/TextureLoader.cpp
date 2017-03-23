#include "TextureLoader.h"
#include "FlyCamera.h"

TextureLoader* TextureLoader::instance;

TextureLoader::TextureLoader()
{
	window = nullptr;

	textureIndex = 0;

	for (int i = 0; i < TEXTURE_LIMIT; i++)
	{
		m_texture[i] = 0;
	}

	glEnable(GL_DEPTH_TEST);

	const char* vsSource = "#version 410\n layout(location=0) in vec4 position; \
							layout(location=1) in vec2 texCoord; \
							out vec2 vTexCoord; \
							uniform mat4 projectionViewWorldMatrix; \
							void main() {vTexCoord = texCoord; \
							gl_Position = projectionViewWorldMatrix * position; }";

	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 fragColor; \
							uniform sampler2D diffuse; \
							uniform sampler2D diffuse2; \
							uniform float Time; \
							void main() { \
							fragColor = texture(diffuse, vec2(vTexCoord.x + sin(sin(Time)), vTexCoord.y + cos(cos(Time)))) - texture(diffuse2, vec2(vTexCoord.x + cos(cos(Time)), vTexCoord.y + sin(sin(Time))));}";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertexData[] = {
		-5, 0, 5, 1, 0, 1,
		5, 0, 5, 1, 1, 1,
		5, 0, -5, 1, 1, 0,
		-5, 0, -5, 1, 0, 0,
	};

	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TextureLoader* TextureLoader::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureLoader();
	}

	return instance;
}

TextureLoader::~TextureLoader()
{

}

int TextureLoader::AddTexture(const char * l_textureFileName)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* data = stbi_load(l_textureFileName,
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture[textureIndex]);
	glBindTexture(GL_TEXTURE_2D, m_texture[textureIndex]);
	if (imageFormat == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	if (imageFormat == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	textureIndex++;

	stbi_image_free(data);

	return m_texture[textureIndex - 1];
}

void TextureLoader::SetScreen(GLFWwindow * l_window)
{
	window = l_window;
}

void TextureLoader::SetCamera(FlyCamera * l_camera)
{
	myCamera = l_camera;
}

void TextureLoader::Update(float l_deltaTime)
{
	//	tell the shader time
	float timeLoc = glGetUniformLocation(m_program, "Time");
	glUniform1f(timeLoc, l_deltaTime);
}

void TextureLoader::Draw()
{
	// use our texture program
	glUseProgram(m_program);

	// bind the camera
	int loc = glGetUniformLocation(m_program, "projectionViewWorldMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(myCamera->GetProjectionView()[0][0]));

	// set texture slot
	/*for (int i = 0; i < textureIndex; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture[i]);
	}*/

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);

	// tell the shader where it is
	loc = glGetUniformLocation(m_program, "diffuse");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(m_program, "diffuse2");
	glUniform1i(loc, 1);

	// draw
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
