const char *SimpleFragmentShader = STRINGFY(

varying lowp vec4 DestinationColor;

void main(void)
{
	gl_FragColor = DestinationColor;
}

);