#version 330 core

in vec3 colorOut;
in vec2 UV;
in vec3 normals;
in vec3 fragPos;
out vec4 color;

//this struct represents a directional light
struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//this struct represents a point light
struct PointLight {
	vec3 position;
};

//this describes how the surface responds to the light
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

//these are constants used by all
//point lights, they are kept here to reduce transfers witht he gpu

//x is the constant term, y is the linear terma and z is the quadratic term
uniform vec3 constLinQuad;

//these are the ambient, diffuse and specular modifier on 
//all the point lights, respectively
uniform vec3 lightMasks[3];

uniform sampler2D image1;
uniform DirectionalLight dirLight;
uniform Material material;
uniform vec3 cameraPosition;

uniform int howManyPointLights;
#define MAX_POINT_LIGHTS 30
uniform PointLight lights[MAX_POINT_LIGHTS];

/*
	Calculate the effect of a directional light on this fragment
	@param dirLight
		A description of the directional light
	@param normal
		The normal from the fragment
	@param viewDirection
		The direction the camera looks from
	@return
		A sum of the ambient, diffuse and specular components of the directional light with the fragment
*/
vec3 calculateDirectionalLight(DirectionalLight dirLight, vec3 normal, vec3 viewDirection, vec3 fragPosition);

/*
	Calculate the effect of a point light on this fragment
	@param light
		A description of the point light
	@param normal
		The normal from the fragment
	@param viewDirection
		The direction the camera looks from
	@param fragPosition
		Where in world space this fragment is located
	@return
		A sum of the ambient, diffuse and specular components of the directional light with the fragment
*/
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDirection, vec3 fragPosition);

void main() {

	vec3 viewDirection = normalize(cameraPosition - fragPos);

	vec3 dirLightSum = calculateDirectionalLight(dirLight, normals, viewDirection, fragPos);

	for(int i = 0; i < howManyPointLights; ++i) {
		dirLightSum += calculatePointLight(lights[i], normals, viewDirection, fragPos);
	}

	color = vec4(dirLightSum * vec3(texture(image1, UV)), 1.0);
}

vec3 calculateDirectionalLight(DirectionalLight dirLight, vec3 normal, vec3 viewDirection, vec3 fragPosition) {
	
	vec3 lightDirection = normalize(-dirLight.direction);

	//diffuse shading, we use cos(angle between the light and the normal)
	//this approaches 0 as the light goes perpendicular
	//and goes - as the light hits the back of the face,
	//thus we set the minimum to 0 with max(val, 0)
	float diffuse = max(dot(normal, lightDirection), 0.0);

	vec3 reflectDirection = reflect(-lightDirection, normal);

	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

	vec3 ambientV = dirLight.ambient * material.ambient;
	vec3 diffuseV = dirLight.diffuse * diffuse * material.diffuse;
	vec3 specularV = dirLight.specular * specular * material.specular;

	return (ambientV + diffuseV + specularV);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDirection, vec3 fragPosition) {
	//get the direction from the fragment to the light
	vec3 lightDir = normalize(light.position - fragPosition);

	float diffuse = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float specular = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);

	//attenuation, first get difference
	float distance = length(light.position - fragPos);
	
	float attenuation = 1.0f / (constLinQuad.x + constLinQuad.y * distance + constLinQuad.z * distance * distance);
		
	 // Combine results
   	vec3 ambientV = lightMasks[0] * material.ambient;
	vec3 diffuseV = lightMasks[1] * diffuse * material.diffuse;
	vec3 specularV = lightMasks[2] * specular * material.specular;

    return (ambientV + diffuseV + specularV) * attenuation;
}