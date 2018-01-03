#pragma once

#include <vector>
#include "renderable.h"

class RenderManager
{
public:
	void submit();
	void render();
private:
	std::vector<Renderable> m_queue;
	
};
