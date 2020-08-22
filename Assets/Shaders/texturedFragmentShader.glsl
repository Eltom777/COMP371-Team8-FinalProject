#version 330 core

in vec3 vertexColor;
in vec2 vertexUV;

// Lighting input
// 0 = wood, 1 = cement, 2 = metal
flat in int textureType;
in vec3 Normal;
in vec3 FragPos;
in vec4 fragment_position_light_space;
// in vec3 LightPos; // view-space

uniform sampler2D shadow_map;

uniform bool isTexture;
uniform bool isLightOn;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D textureSampler;
        
out vec4 FragColor;

float shadow_scalar() {
    // this function returns 1.0 when the surface receives light, and 0.0 when it is in a shadow
    // perform perspective divide
    vec3 ndc = fragment_position_light_space.xyz / fragment_position_light_space.w;
    // transform to [0,1] range
    ndc = ndc * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragment_position_light_space as coords)
    float closest_depth = texture(shadow_map, ndc.xy).r;
    // get depth of current fragment from light's perspective
    float current_depth = ndc.z;
    // check whether current frag pos is in shadow
    float bias = -0.35f;  // bias applied in depth map: see shadow_vertex.glsl
    return ((current_depth - bias) > closest_depth) ? 1.0 : 0.0;
}

void main()
{
    float materialShininess;
    vec3 materialAmbient;
    vec3 materialDiffuse;
    vec3 materialSpecular;

    // Selecting the correct material component
    switch (textureType)
    {
        case 0:
            materialAmbient = vec3(0.1f, 0.1f, 0.1f);
            materialDiffuse = vec3(0.6f, 0.6f, 0.6f);
            materialSpecular = vec3(0.5f, 0.5f, 0.5f);
            materialShininess = 8.0f;
            break;
        case 1:
            materialAmbient = vec3(0.15f, 0.15f, 0.15f);
            materialDiffuse = vec3(0.6f, 0.6f, 0.6f);
            materialSpecular = vec3(1.8f, 1.8f, 1.8f);
            materialShininess = 2.0f;
            break;
        case 2:
            materialAmbient = vec3(0.25f, 0.25f, 0.25f);
            materialDiffuse = vec3(0.4f, 0.4f, 0.4f);
            materialSpecular = vec3(0.7f, 0.7f, 0.7f);
            materialShininess = 128.0f;;
            break;
        default:
            materialAmbient = vec3(0.25f, 0.25f, 0.25f);
            materialDiffuse = vec3(0.4f, 0.4f, 0.4f);
            materialSpecular = vec3(0.7f, 0.7f, 0.7f);
            materialShininess = 2.0f;;
            break;
    }

    vec4 textureColor;

    if(isTexture){
        textureColor = texture( textureSampler, vertexUV );
    }
    else{
        textureColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0);
    }

    // Return color using a vec3
    vec3 thiscolor;

    if (isTexture) {
        thiscolor = texture(textureSampler, vertexUV).rgb;
    }
    else {
        thiscolor = vec3(vertexColor.r, vertexColor.g, vertexColor.b);
    }


    // Shadow Calculation
    float shadowScalar = shadow_scalar();
    //float shadowScalar = 1.0f;

    // ambient
	float ambientStrength = 1.0f;
	//vec3 ambient = ambientStrength * (materialAmbient * lightColor);
    vec3 ambient = ambientStrength * thiscolor * materialAmbient;

    // diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // world space

	float diff = max(dot(norm, lightDir), 0.0);

    // Technique in the lab to render lighting and shadow
	//vec3 diffuse = shadowScalar * diff * lightColor * materialDiffuse;
    vec3 diffuse = diff * lightColor * materialDiffuse;


    // specular
	float specularStrength = 3.0;
    vec3 viewDir = normalize(viewPos - FragPos); // World space
	// vec3 viewDir = normalize(FragPos); // view-space
	vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);

	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), materialShininess);
    
    // Technique in the lab to render lighting and shadow
	//vec3 specular = shadowScalar * specularStrength * (materialSpecular * spec * lightColor);
    vec3 specular = specularStrength * (materialSpecular * spec * lightColor);

    // Technique in the lab to render lighting and shadow
    //vec4 tempResult = vec4((ambient + diffuse + specular), 1.0);

    vec3 lighting;

    if (isLightOn) {
        lighting = (ambient + (1.0 - shadowScalar) * (diffuse + specular)) * thiscolor;
    }
    else {
        lighting = ambient* thiscolor;
    }


    //vec3 lighting = (ambient + (1.0 - shadowScalar) * (diffuse + specular)) * thiscolor;
    vec4 result = vec4(lighting, 1.0);

    // Technique in the lab to render lighting and shadow
    //vec4 result = textureColor * tempResult; //tempResult * textureColor; //if you want lighting

    FragColor =  result;
};