#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform int jump;

void main()
{
	vec4 texColor = texture(texture1, TexCoord);
    float dist = distance(TexCoord, vec2(0.5, 0.5));
	if(texColor.a < 0.1)
        if(jump == 0){
            discard;
        }else{
            texColor = mix(vec4(0.7, 0.4, 0.0, 0.4), texColor, smoothstep(0.4, 0.6, dist));
        }
    FragColor = texColor;
}