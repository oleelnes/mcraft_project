#version 430 core
out vec4 FragColor;

//Source 5

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D uTexture; //gets texture input from ACTIVETEXTURE0 

uniform vec3 viewPos;

uniform vec3 lightPosition;  
uniform vec3 lightDirection;
uniform float lightCutOff;
uniform float lightOuterCutOff;
  
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
	
uniform float lightConstant;
uniform float lightLinear;
uniform float lightQuadratic;
uniform vec4 u_color = vec4(1.0, 0.0, 0.0, 1.0);

uniform int withTexture = 0;
uniform int overlayfs = 0;

#define NR_POINT_LIGHTS 1

uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform float materialShininess;


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
    if(overlayfs == 0){
	    // ambient
        vec4 texColor = texture(uTexture, TexCoords);
        if(texColor.a < 0.1)
            discard;
        vec3 ambient = lightAmbient;// * texture(uTexture, TexCoords).rgb;
        if(withTexture == 1){ ambient *= texColor.rgb; }
    
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPosition - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lightDiffuse * diff; // * texture(uTexture, TexCoords).rgb;
        if(withTexture == 1){ diffuse *= texColor.rgb; }
    
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
        vec3 specular = lightSpecular * spec; //* texture(uTexture, TexCoords).rgb;
        if(withTexture == 1){ specular *= texColor.rgb; }
    
        // spotlight (soft edges)
        float theta = dot(lightDir, normalize(-lightDirection)); 
        float epsilon = (lightCutOff - lightOuterCutOff);
        float intensity = clamp((theta - lightOuterCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
    
        // attenuation
        float distance    = length(lightPosition - FragPos);
        float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));    
        ambient  *= attenuation; 
        diffuse   *= attenuation;
        specular *= attenuation;   
        
        vec3 result = ambient + diffuse; //+ specular;

        for(int i = 0; i < NR_POINT_LIGHTS; i++)
           result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  

        if(withTexture == 0) {
            FragColor = vec4(result, 1.0) * u_color;
            }
        else{
            FragColor = vec4(result, 1.0) * texColor.a;
        }
    }
    else{
       FragColor = texture(uTexture, TexCoords);
       //FragColor = vec4(0.8, 0.5, 0.6, 1.0);
    }
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec4 texColor = texture(uTexture, TexCoords);
    if(texColor.a < 0.1)
        discard;
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    /// combine results
    vec3 ambient = light.ambient; //* vec3(texture(uTexture, TexCoords));
    vec3 diffuse = light.diffuse * diff;// * vec3(texture(uTexture, TexCoords));
    vec3 specular = light.specular * spec;// * vec3(texture(uTexture, TexCoords));
    if(withTexture == 1){
         ambient *= vec3(texColor);
         diffuse *= vec3(texColor);
         specular *= vec3(texColor);
    }
    //ambient *= attenuation;
    //diffuse *= attenuation;
    //specular *= attenuation;
    return (ambient + diffuse) * (4.0f, 4.0f, 4.0f) * texColor.a; //FJERNET + specular

}