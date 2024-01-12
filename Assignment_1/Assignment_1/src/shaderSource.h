

#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H

/******************************************************************************/
/******************************   Default Shader ******************************/
/******************************************************************************/


const char *vertexShaderSource =
        "#version 330\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "uniform vec3 meshColor;\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 normal;\n"
        "out vec3 ocolor;\n"
        "out vec3 facenormal;\n"
        "out vec3 FragPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "	ocolor = meshColor;\n"
        "FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "   facenormal = mat3(transpose(inverse(model))) * normal;\n"
        "}\n";



const char *fragmentShaderSource =
        "#version 330\n"
        "uniform vec3 viewPos;\n"
        "in vec3 ocolor;\n"
        "in vec3 facenormal;\n"
        "in vec3 FragPos;\n"
        "out vec4 Fragment;\n"
        "void main()\n"
        "{\n"

        "vec3 lightColor = vec3(1.0,1.0,1.0);"
        "float ambientStrength = 0.3;"
        "vec3 ambient = ambientStrength * lightColor;"

        "float diffuseStrength = 0.6;"
        "vec3 lightPos = vec3(1.0,1.0,10.0);"
        "vec3 norm = normalize(facenormal);"
        "vec3 lightDir = normalize(lightPos - FragPos);"
        "float diff = max(dot(norm, lightDir), 0.0);"
        "vec3 diffuse = diffuseStrength * diff * lightColor;"

        "float specularStrength = 0.2;"
        "vec3 viewDir = normalize(viewPos - FragPos);"
        "vec3 reflectDir = reflect(-lightDir, norm);"
        "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
        "vec3 specular = specularStrength * spec * lightColor;"

        "vec3 result = (ambient + diffuse + specular) * ocolor;"
        "Fragment = vec4(result, 1.0);"
        "}\n";



#endif //SHADERSOURCE_H
