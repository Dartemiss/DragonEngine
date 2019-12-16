#version 330 core

struct Material
{
    sampler2D diffuse_map;
    vec4 diffuse_color;
    float k_diffuse;
    sampler2D specular_map;
    vec3 specular_color;
    float shininess;
    float k_specular;
    sampler2D occlusion_map;
    float k_ambient;
    sampler2D emissive_map;
    vec3 emissive_color;
};

//General functions
float lambert(vec3 normal, vec3 light)
{
  vec3 nrmN = normalize(normal);
  vec3 nrmL = normalize(light);
  float result = dot(nrmN, nrmL);
  return max(result, 0.0);
}

float specular_blinn(vec3 lightDir, vec3 position, vec3 normal, mat4 view, float shininess)
{
    float specular   = 0.0;

    vec3 viewPos    = transpose(mat3(view)) * (-view[3].xyz);
    vec3 viewDir    = normalize(viewPos - position);
    vec3 halfDir = normalize(viewDir + lightDir);
    float sp        = max(dot(normal, halfDir), 0.0);

    if(sp > 0.0)
    {
        specular = pow(sp, shininess);
    }
    return specular;
}


// Type specification
subroutine vec4 GetColor4(const Material mat, const vec2 uv);
subroutine vec3 GetColor3(const Material mat, const vec2 uv);

// Uniform function pointer
subroutine uniform GetColor4 get_diffuse_color;
subroutine uniform GetColor4 get_specular_color;
subroutine uniform GetColor3 get_occlusion_color;
subroutine uniform GetColor3 get_emissive_color;


//Diffuse color
layout(index=0) subroutine(GetColor4) vec4 get_diffuse_from_color(const Material mat, const vec2 uv)
{
    return mat.diffuse_color;
}

layout(index=1) subroutine(GetColor4) vec4 get_diffuse_from_texture(const Material mat, const vec2 uv)
{
    //return texture(mat.diffuse_map, uv);
    return texture(mat.diffuse_map, uv) * mat.diffuse_color;
}


//Specular color
layout(index=2) subroutine(GetColor4) vec4 get_specular_from_color(const Material mat, const vec2 uv)
{
    return vec4(mat.specular_color, mat.shininess);
}

layout(index=3) subroutine(GetColor4) vec4 get_specular_from_texture(const Material mat, const vec2 uv)
{
    return vec4(texture(mat.specular_map, uv).rgb, mat.shininess);
}


//Occlusion color
layout(index=4) subroutine(GetColor3) vec3 get_occlusion_from_default(const Material mat, const vec2 uv)
{
    return mat.diffuse_color;
}

layout(index=5) subroutine(GetColor3) vec3 get_occlusion_from_texture(const Material mat, const vec2 uv)
{
    return texture(mat.occlusion_map, uv).rgb;
}


//Emissive color
layout(index=6) subroutine(GetColor3) vec3 get_emissive_from_color(const Material mat, const vec2 uv)
{
    return mat.emissive_color;
}

layout(index=7) subroutine(GetColor3) vec3 get_emissive_from_texture(const Material mat, const vec2 uv)
{
    return texture(mat.emissive_map, uv).rgb;
}


uniform Material material;

uniform mat4 view;
uniform vec3 directionalLight;

//uniform vec3 ambientColor
//uniform vec3 lightDirColor

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 color;

void main()
{
    vec4 diffuse_color = get_diffuse_color(material, texCoord);
    vec4 specular_color = get_specular_color(material, texCoord);
    vec3 occlusion_color = get_occlusion_color(material, texCoord);
    vec3 emissive_color = get_emissive_color(material, texCoord);

    float diffuse = lambert(normal, directionalLight);
    float specular = specular_blinn(directionalLight, position, normal, view, specular_color.a);

    vec3 colorSum = emissive_color + // emissive
    diffuse_color.rgb * (occlusion_color * material.k_ambient) + // ambient
    diffuse_color.rgb * diffuse * material.k_diffuse + // diffuse
    specular_color.rgb * specular * material.k_specular; // specular

    color = vec4(colorSum, diffuse_color.a);
}