#pragma once

class GLFramebuffer
{
public:
	GLFramebuffer();
	~GLFramebuffer();

	void create();
protected:
	int m_buffer;
};
