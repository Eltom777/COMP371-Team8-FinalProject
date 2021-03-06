#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

uniform vec3 lightPos;
        
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;  // default value for view matrix (identity)
uniform mat4 projectionMatrix;
uniform mat4 light_proj_view_matrix;
        
out vec3 FragPos;
out vec3 Normal;
// out vec3 LightPos; // view-space
out vec3 vertexColor;
out vec2 vertexUV;
out vec4 fragment_position_light_space;
        
void main()
{
   vertexColor = aColor;
   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;

   // Lighting Component
   FragPos = vec3(worldMatrix * vec4(aPos, 1.0));
       Normal = transpose(inverse(mat3(worldMatrix))) * aNormal; // World space technique, kept for debugging purposes
       //Normal = mat3(worldMatrix) * aNormal; // PREVIOUS
   // Normal = mat3(transpose(inverse(viewMatrix * worldMatrix))) * aNormal;  // view-space
   // LightPos = vec3(viewMatrix * vec4(lightPos, 1.0)); // view-space
       fragment_position_light_space = light_proj_view_matrix * vec4(FragPos, 1.0);


   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   vertexUV = aUV;
};