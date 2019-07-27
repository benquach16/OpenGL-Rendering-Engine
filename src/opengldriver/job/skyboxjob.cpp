#include "skyboxjob.h"
#include "../../util/debug.h"
#include "directlightingjob.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../3rdparty/stb_image.h"

using namespace std;

float skyboxVertices[] = {
	// positions
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

SkyboxJob::SkyboxJob()
{
	setVertexShader("shaders/skybox.vert");
	setFragmentShader("shaders/skybox.frag");
	
	GLuint textureId = 0;
	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	vector<std::string> faces = 
		{
			"assets/skybox/right.jpg",
			"assets/skybox/left.jpg",
			"assets/skybox/top.jpg",
			"assets/skybox/bottom.jpg",
			"assets/skybox/front.jpg",
			"assets/skybox/back.jpg"
		};
	int width, height, nrChannels;
	for(unsigned i = 0; i < faces.size(); ++i)
	{
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

SkyboxJob::~SkyboxJob()
{
	m_skyboxTexture = 0;
}

void SkyboxJob::run()
{
	// make sure that our parent is a directlighting job (DAG strictly enforced)
	ASSERT(m_parent != nullptr, "DAG initialized incorrectly");
	ASSERT(m_parent->getJobType() == eRenderPasses::DirectLighting, "Parent job of incorrect type");

	DirectLightingJob* parent = static_cast<DirectLightingJob*>(m_parent);

	glBindFramebuffer(GL_FRAMEBUFFER, parent->getFramebuffer());
	glDepthMask(false);

	glEnable(GL_DEPTH_TEST);
	m_pipeline->bindPipeline();
	m_pipeline->setUniform(GLProgram::eShaderType::Fragment, "uSkybox", 0);
	//glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	GET_GL_ERROR("Error setting skybox texture");
	//todo : defer this so we dont alloc memory every frame
	GLuint vertarray;
	glGenVertexArrays(1, &vertarray);
	glBindVertexArray(vertarray);
	GLuint vertices;
	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glDepthFunc(GL_LEQUAL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, BUFFER_OFFSET(0));
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &vertices);
	glDeleteBuffers(1, &vertarray);

	glDepthMask(true);
}
