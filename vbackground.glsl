attribute highp vec4 a_position;
attribute highp vec2 a_textcoord;
//attribute highp vec3 a_normal;

uniform highp mat4 u_projectionMatrix;
//uniform highp mat4 u_viewMatrix;
//uniform highp mat4 u_modelMatrix;
varying highp vec2 v_textcoord;

void main(void)
{
    gl_Position = u_projectionMatrix * a_position;
    v_textcoord = a_textcoord;
}
