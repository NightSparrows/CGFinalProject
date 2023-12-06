#version 450

#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 vs_position;

out vec4 out_Color;

layout (std140, binding = 0) uniform globalBuffer {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 cameraPosition;
	mat4 inverseProjectionMatrix;
}GlobalUbo;

uniform vec3 u_sunPosition;

uniform int u_viewSamples;
uniform int u_lightSamples;

uniform float u_sunIntensity;
uniform float u_radiusPlanet;
uniform float u_radiusAtmoSphere;		// radius of the atmosphere
uniform vec3 u_rayleighCoeff;
uniform float u_minScatteringCoeff;
uniform float u_rayleighScaleHeight;
uniform float u_mieScaleHeight;
uniform float u_mieScatterDir;

// where the height is simulate
uniform float u_seeHeight;

/**
 * @brief Computes intersection between a ray and a sphere
 * @param o Origin of the ray
 * @param d Direction of the ray
 * @param r Radius of the sphere
 * @return Roots depending on the intersection
 */
vec2 raySphereIntersection(vec3 o, vec3 d, float r)
{
	float a = dot(d, d);
	float b = 2.0 * dot(d, o);
	float c = dot(o, o) - r * r;
	
	float delta = b * b - 4.0 * a * c;
	
	// Roots not found
	if (delta < 0.0) {
		// TODO
		return vec2(1e5, -1e5);
	}
	
	float sqrtDelta = sqrt(delta);
	
	return vec2((-b - sqrtDelta) / (2.0 * a),
				(-b + sqrtDelta) / (2.0 * a));
}

vec3 computeSkyColor(vec3 ray, vec3 origin)
{
	vec3 sunDir = normalize(u_sunPosition);
	
	vec2 t = raySphereIntersection(origin, ray, u_radiusAtmoSphere);
	
	if (t.x > t.y) {
		return vec3(0.0);
	}
	
	t.y = min(t.y, raySphereIntersection(origin, ray, u_radiusPlanet).x);
	float segmentLen = (t.y - t.x) / float(u_viewSamples);
	
	// TODO t min
	float tCurrent = 0.0;
	
	vec3 sum_R = vec3(0);
	vec3 sum_M = vec3(0);

    // Optical depth 
    float optDepth_R = 0.0;
    float optDepth_M = 0.0;

    // Mu: the cosine angle between the sun and ray direction
    float mu = dot(ray, sunDir);
    float mu_2 = mu * mu;
    
    //--------------------------------
    // Rayleigh and Mie Phase functions
    float phase_R = 3.0 / (16.0 * M_PI) * (1.0 + mu_2);

    float g_2 = u_mieScatterDir * u_mieScatterDir;
    float phase_M = 3.0 / (8.0 * M_PI) * 
                          ((1.0 - g_2) * (1.0 + mu_2)) / 
                          ((2.0 + g_2) * pow(1.0 + g_2 - 2.0 * u_mieScatterDir * mu, 1.5));
    // Sample along the view ray
    for (int i = 0; i < u_viewSamples; ++i)
    {
        // Middle point of the sample position
        vec3 vSample = origin + ray * (tCurrent + segmentLen * 0.5);

        // Height of the sample above the planet
        float height = length(vSample) - u_radiusPlanet;

        // Optical depth for Rayleigh and Mie scattering for current sample
        float h_R = exp(-height / u_rayleighScaleHeight) * segmentLen;
        float h_M = exp(-height / u_mieScaleHeight) * segmentLen;
        optDepth_R += h_R;
        optDepth_M += h_M;

        //--------------------------------
        // Secondary - light ray
        float segmentLenLight = raySphereIntersection(vSample, sunDir, u_radiusAtmoSphere).y / float(u_lightSamples);
        float tCurrentLight = 0.0;

        // Light optical depth 
        float optDepthLight_R = 0.0;
        float optDepthLight_M = 0.0;

        // Sample along the light ray
        for (int j = 0; j < u_lightSamples; ++j)
        {
            // Position of the light ray sample
            vec3 lSample = vSample + sunDir * 
                           (tCurrentLight + segmentLenLight * 0.5);
            // Height of the light ray sample
            float heightLight = length(lSample) - u_radiusPlanet;

            // TODO check sample above the ground
            
            optDepthLight_R += exp(-heightLight / u_rayleighScaleHeight) * segmentLenLight;
            optDepthLight_M += exp(-heightLight / u_mieScaleHeight) * segmentLenLight;

            // Next light sample
            tCurrentLight += segmentLenLight;
        }
        // TODO check sample above ground

        // Attenuation of the light for both Rayleigh and Mie optical depth
        //  Mie extenction coeff. = 1.1 of the Mie scattering coeff.
        vec3 att = exp(-(u_rayleighCoeff * (optDepth_R + optDepthLight_R) + 
                         u_minScatteringCoeff * 1.1f * (optDepth_M + optDepthLight_M)));
        // Accumulate the scattering 
        sum_R += h_R * att;
        sum_M += h_M * att;

        // Next view sample
        tCurrent += segmentLen;
    }

    return u_sunIntensity * (sum_R * u_rayleighCoeff * phase_R + sum_M * u_minScatteringCoeff * phase_M);
}

void main() 
{
	vec3 viewPos = vec3(0.0, u_seeHeight, 0.0);

	vec3 aColor = computeSkyColor(normalize(vs_position - viewPos), viewPos);
	
	out_Color = vec4(aColor, 1.0);
	//out_Color = vec4(1.0);
	
}



