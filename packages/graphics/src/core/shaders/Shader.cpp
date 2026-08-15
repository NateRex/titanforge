#include <graphics/core/shaders/Shader.h>
#include <graphics/lights/PointLight.h>
#include <graphics/lights/SpotLight.h>
#include <graphics/cameras/Camera.h>
#include <graphics/lights/Light.h>
#include <graphics/materials/Material.h>
#include <graphics/objects/Mesh.h>
#include <graphics/core/RenderState.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/InstantiationException.h>
#include <common/Utils.h>
#include <glad/glad.h>
#include <sstream>
#include <cmath>

Shader::Shader(const char* prgmName, const char* vertexShader, const char* fragmentShader)
{
	unsigned int vId = compileSource(prgmName, GL_VERTEX_SHADER, vertexShader);
	unsigned int fId = compileSource(prgmName, GL_FRAGMENT_SHADER, fragmentShader);

	// Create program
	_id = glCreateProgram();
	if (_id == 0)
	{
		std::ostringstream oss;
		oss << "Failed to construct shader program: " << prgmName;
		throw InstantiationException(oss.str());
	}

	// Attach shaders and link
	glAttachShader(_id, vId);
	glAttachShader(_id, fId);
	glLinkProgram(_id);

	// Detach shaders
	glDeleteShader(vId);
	glDeleteShader(fId);

	// Check for errors
	int success;
	char infoLog[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		glDeleteProgram(_id);

		_id = 0;

		std::ostringstream oss;
		oss << "Linking failed for shader program " << prgmName << ": " << infoLog;
		throw InstantiationException(oss.str());
	}
}

Shader::~Shader()
{
	GLint boundProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &boundProgram);
	if (boundProgram == _id)
	{
		// Program is currently in use. Make sure to unbind it first.
		glUseProgram(0);
	}

	glDeleteProgram(_id);
	_id = 0;
}

unsigned int Shader::compileSource(const char* prgmName, int type, const char* source)
{
	unsigned int id = glCreateShader(type);
	if (id == 0)
	{
		throw InstantiationException("Failed to construct shader");
	}

	// Load shader source
	glShaderSource(id, 1, &source, NULL);

	// Compile
	glCompileShader(id);

	// Check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		glDeleteShader(id);

		std::ostringstream oss;
		oss << "Shader " << type << " compilation failed for " << prgmName << ": " << infoLog;
		throw InstantiationException(oss.str());
	}

	return id;
}

void Shader::activate() const
{
	glUseProgram(_id);
}

int Shader::getUniformLocation(const char* variableName) const
{
	int loc = glGetUniformLocation(_id, variableName);
	if (loc < 0)
	{
		std::ostringstream oss;
		oss << "Could not find uniform variable: " << variableName;
		throw IllegalArgumentException(oss.str());
	}

	return loc;
}

void Shader::setState(const RenderState& state)
{
	setCamera(state.camera);
	setLighting(state.lighting);
}

void Shader::setItem(const RenderItem& item)
{
	setModelMatrix(item.modelTransform);
	setNormalMatrix(item.normalTransform);
	setMaterial(item.mesh->material);
}

void Shader::setModelMatrix(const Matrix4& matrix)
{
	int loc = getUniformLocation("uTransforms.model");
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.getValues());
}

void Shader::setNormalMatrix(const Matrix3& matrix)
{
	int loc = getUniformLocation("uTransforms.normal");
	glUniformMatrix3fv(loc, 1, GL_TRUE, matrix.getValues());
}

void Shader::setLighting(const Lighting& lighting)
{
	constexpr size_t MAX_LIGHTS = 16;
	float ambientRed = 0.f;
	float ambientGreen = 0.f;
	float ambientBlue = 0.f;
	size_t lightCount = 0;

	for (const RenderLight& renderLight : lighting.lights)
	{
		const LightPtr& light = renderLight.light;
		if (!light)
		{
			continue;
		}

		const Color color = light->color;
		const float intensity = std::max(light->intensity, 0.f);

		// In the case of ambient lighting, aggregate values across all ambient lights
		if (light->lightType == LightType::AMBIENT)
		{
			ambientRed += color.red() * intensity;
			ambientGreen += color.green() * intensity;
			ambientBlue += color.blue() * intensity;
			continue;
		}

		// Stop setting uniforms if we've reached the maximum number of lights supported
		if (lightCount >= MAX_LIGHTS)
		{
			continue;
		}

		// Compute the homogeneous light vector, which works differently depending on the type of light
		Vector3 vector;
		float w = 1.f;
		if (light->lightType == LightType::DIRECTIONAL)
		{
			// Forward is the direction the rays travel; shading needs the
			// opposite direction, from the surface toward the light.
			vector = renderLight.direction.scale(-1.f);
			w = 0.f;
		}
		else
		{
			vector = renderLight.position;
		}

		// Compute distance attenuation for local lights
		float constantAttenuation = 1.f;
		float linearAttenuation = 0.f;
		float quadraticAttenuation = 0.f;
		float range = light->range;
		if (light->attenuation && range > 0.f)
		{
			// Scale the familiar lamp-like (1, 0.09, 0.032) curve so
			// `range` remains the only distance users need to reason about.
			linearAttenuation = 4.5f / range;
			quadraticAttenuation = 80.f / (range * range);
		}

		// Encode cone falloff homogeneously. The neutral values make smoothstep
		// evaluate to one for every possible dot product on non-spot lights.
		Vector3 direction = Vector3::ZERO;
		float outerCutoff = -2.f;
		float innerCutoff = -1.f;
		if (light->lightType == LightType::SPOTLIGHT)
		{
			const SpotLightPtr spotLight = std::static_pointer_cast<SpotLight>(light);
			const float innerAngle = clamp(spotLight->innerAngle, 0.f, 180.f);
			const float outerAngle = clamp(spotLight->outerAngle, innerAngle, 180.f);
			direction = renderLight.direction;
			outerCutoff = std::cos(deg2Rad(outerAngle));
			innerCutoff = std::cos(deg2Rad(innerAngle));
			// GLSL smoothstep requires distinct edges.
			if (innerCutoff - outerCutoff < 0.00001f)
			{
				outerCutoff = innerCutoff - 0.00001f;
			}
		}

		// Assign uniforms
		const std::string prefix = "uLights[" + std::to_string(lightCount) + "].";
		glUniform4f(getUniformLocation((prefix + "vector").c_str()), vector.x, vector.y, vector.z, w);
		glUniform3f(getUniformLocation((prefix + "color").c_str()), color.red(), color.green(), color.blue());
		glUniform1f(getUniformLocation((prefix + "intensity").c_str()), intensity);
		glUniform3f(
			getUniformLocation((prefix + "attenuation").c_str()),
			constantAttenuation,
			linearAttenuation,
			quadraticAttenuation);
		glUniform3f(getUniformLocation((prefix + "direction").c_str()), direction.x, direction.y, direction.z);
		glUniform2f(getUniformLocation((prefix + "cone").c_str()), outerCutoff, innerCutoff);
		++lightCount;
	}

	glUniform3f(getUniformLocation("uAmbient"), ambientRed, ambientGreen, ambientBlue);
	glUniform1i(getUniformLocation("uLightCount"), lightCount);
}

void Shader::setCamera(const CameraPtr camera)
{
	int loc = getUniformLocation("uCameraPos");
	Vector3 cameraPos = camera->getPosition();
	glUniform3f(loc, cameraPos.x, cameraPos.y, cameraPos.z);

	setViewMatrix(camera->getViewMatrix());
	setProjectionMatrix(camera->getProjectionMatrix());
}

void Shader::setViewMatrix(const Matrix4& matrix)
{
	int loc = getUniformLocation("uTransforms.view");
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.getValues());
}

void Shader::setProjectionMatrix(const Matrix4& matrix)
{
	int loc = getUniformLocation("uTransforms.proj");
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.getValues());
}

void Shader::setMaterial(const MaterialPtr material)
{
	int loc = getUniformLocation("uMaterial.color");
	Color color = material->color;
	glUniform4f(loc, color.red(), color.green(), color.blue(), color.alpha());

	loc = getUniformLocation("uMaterial.reflectivity");
	glUniform1f(loc, clamp(material->reflectivity, 0.f, 1.f));

	loc = getUniformLocation("uMaterial.shine");
	glUniform1f(loc, clamp(material->shine, 0.f, 1.f));
}
