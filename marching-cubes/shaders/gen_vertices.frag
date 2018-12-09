#version 330

uniform vec4 my_color = vec4(1.0, 1.0, 0.0, 1.0);
out vec4 color;
void main(void)
{
  color = my_color;
}
