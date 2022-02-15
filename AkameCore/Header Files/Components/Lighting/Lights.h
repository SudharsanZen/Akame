#pragma once
#include"glm/glm/glm.hpp"
#include"Rendering/Shader.h"
#include"Components/Components.h"
#include"Core/AkameCore.h"
enum class LIGHT {DIRECTIONAL=0,POINT=1,SPOT=2};
class LightSystem;

class Lights:public Components
{
private:
	LIGHT type;
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	glm::vec3 ambient;

	glm::vec3 pointLightConstants;
	float intensity;

	friend class RenderingSystem;
	friend class InspectorWindow;
	friend class DeferredRendererFragmentBuffer;
	friend class LightSystem;
	friend class Editor;


	float pointLightRadius;
	
	AKAME_API void notifyLightSystemToUpdate();
public:
	AK_SERIALIZABLES
	(
		AK_ID_COMPX(lightDirection)
		AK_ID_COMPX(lightColor)
		AK_ID_COMPX(ambient)
		AK_ID_COMPX(pointLightConstants)
		AK_ID(pointLightRadius)
	)
	AKAME_API float calPointLightRadius(glm::vec3 C, float intensity);
	AKAME_API float getPointLightRadius();
	AKAME_API Lights();
	AKAME_API Lights(LIGHT ty);
	
	AKAME_API void setColor(glm::vec3 rgb);
	AKAME_API void setColor(float r,float g,float b);
	
	AKAME_API void setDirection(float x,float y,float z);
	AKAME_API void setDirection(glm::vec3 angles);

	AKAME_API void ambientLigting(glm::vec3 rgb);
	AKAME_API void ambientLigting(float r,float g,float b);

	AKAME_API void setIntensity(float intenseity);
	AKAME_API void setPointLightConst(float Kc,float Kl,float Kq);
	AKAME_API void setPointLightConst(glm::vec3 constants);
	AKAME_API LIGHT getType();
	AKAME_API void setType(LIGHT ty);;
	AKAME_API void reset();;
	AKAME_API glm::vec3 getDirection();
};

