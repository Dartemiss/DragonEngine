#version 330 core

uniform sampler2D myTexture;

uniform mat4 view;
uniform vec3 lightPos;

uniform float ambientK;
uniform float diffuseK;
uniform float specularK;

uniform float ambient;
uniform float shininess;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 color;

void main()
{
	vec3 pixelNormal = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);
	float diffuseVal = max(0.0, dot(pixelNormal, lightDir));
	float specular   = 0.0;

	if(diffuseVal > 0.0 && specularK > 0.0 && shininess > 0.0)
    {
		vec3 viewPos    = transpose(mat3(view)) * (-view[3].xyz);
        vec3 viewDir    = normalize(viewPos - position);
        vec3 halfDir = normalize(viewDir + lightDir);
        float sp        = max(dot(pixelNormal, halfDir), 0.0);

        if(sp > 0.0)
        {
            specular = pow(sp, shininess);
        }
    }

    float intensity = (ambientK * ambient + diffuseK * diffuse + specularK * specular);

    vec4 texture = texture2D(myTexture, texCoord);

    color = vec4(texture.r * intensity, texture.g * intensity, texture.b * intensity, texture.a);
}
