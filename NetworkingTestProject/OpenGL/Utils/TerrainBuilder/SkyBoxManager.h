#pragma once
#include <vector>
#include <string>
#include "./../../Utils/stb_image.h"
#include "./../Shader.h"
class SkyBoxManager {
public:
	SkyBoxManager();
	~SkyBoxManager();
	void RenderSkyBox(Shader shader, glm::mat4 m_Veiw, glm::mat4 m_Proj);
private:
	unsigned int  m_SkyBoxVAO;
};