
#include <cstdio>

#include "GraphicsEngine.h"
#include "ImageUtilities\ImageUtilities.h"

//Link static variables
vmath::mat4 GraphicsEngine::proj_matrix;

GraphicsEngine::GraphicsEngine()
{
	//Initialize Graphics upon instantiation.  TODO Might want to make the program make an explicit call.  For now this works
	if(!Init())
	{
		printf("Error Initializing Graphics");
		exit(EXIT_FAILURE);
	}

	
}

void GraphicsEngine::Render(double currentTime)
{
	const GLfloat bkColor[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;
	
	glClearBufferfv(GL_COLOR, 0, bkColor);
	glClearBufferfv(GL_DEPTH,0, &one);
	

	glUseProgram(rendering_program);

	GLint mv_location = glGetUniformLocation(rendering_program, "mv_matrix");
	GLint proj_location = glGetUniformLocation(rendering_program, "proj_matrix");
	

	//Draw 24 cubes...
	for (int i=0; i < 24; i++)
	{
		float f = (float)i + (float)currentTime * (float)M_PI * 0.1f;
		vmath:: mat4 mv_matrix = 
		vmath::translate(0.0f, 0.0f, -5.0f) * 
			
			
		vmath::rotate((float) currentTime * 45.0f, 0.0f, 1.0f, 0.0f)*
		vmath::rotate((float) currentTime * 21.0f, 1.0f, 0.0f, 0.0f) *
		vmath::translate(sinf(2.1f * f) * 2.0f,     cosf(1.7f * f) * 2.0f,    sinf(1.3f * f) * cosf(1.5f * f) * 2.0f);

		//set the model-view and projection matrices
		glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj_matrix);
		
	
		//Draw 6 faces of 2 triangles of 3 vertices each = 36 vertices
		glDrawArrays(GL_TRIANGLES,0,36);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool GraphicsEngine::Init()
{
	glfwSetErrorCallback(error_callback);

	//initial OpenGL
	if(!glfwInit())
		//return false;
		exit(EXIT_FAILURE);



	//vars for window dimensions... temporary
	int winWidth = 640;
	int winHeight = 480;

	//	Added so that OSX would use OpenGL 3.2 instead of the default 2.1
	/**
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*/

	window = glfwCreateWindow(winWidth, winHeight,"OpenGL Super Bible", NULL,NULL);
	if(!window)
	{
		glfwTerminate();
		return false;
		//exit(EXIT_FAILURE);
	}



	//create initial projection matrix TODO: remove hardcoded numbers.
	proj_matrix = vmath::perspective(50.0f, (float)winWidth/(float)winHeight, 0.1f, 1000.0f);
	
	glfwMakeContextCurrent(window);



	//initialize openGL extensions.  Must be done after making context current. Otherwise will error
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  //fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		printf("Error: %s\n", glewGetErrorString(err));
	}

	//get some OpenGL info
	printf( "GL_VENDOR: %s\n",glGetString(GL_VENDOR));
	printf( "GL_RENDERER: %s\n",glGetString(GL_RENDERER));
	printf( "GL_VERSION: %s\n",glGetString(GL_VERSION));
	printf( "GL_SHADING_LANGUAGE_VERSION: %s\n\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

	glfwSetKeyCallback(window, key_callback);
	
	glfwSetWindowSizeCallback(window, window_size_callback);
	
	//not sure this is the best place for these, but will work for now
	rendering_program = InitShaders();
	InitBuffers();
	InitTextures();
	
	//Set some options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	
	return true;
}

bool GraphicsEngine::CleanUp()
{

	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
	glDeleteVertexArrays(1, &vertex_array_object);

	glfwDestroyWindow(window);
	glfwTerminate();

	return true;  //eventually change to success or fail.
}


int GraphicsEngine::CheckWindowClose()
{
	return glfwWindowShouldClose(window);
}

double GraphicsEngine::getCurrentTime()
{
	return glfwGetTime();
}

GLuint GraphicsEngine::InitShaders(void)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint tess_control_shader;
	GLuint tess_eval_shader;
	GLuint geometry_shader;
	GLuint program;

	std::string tempString;
	GLint return_code;

	//Create vertex shader
	vertex_shader = shaderMan.CompileShaderFromSource("spinningcubes_vert.txt",GL_VERTEX_SHADER);
	
	//Create tess control shader
	tess_control_shader = shaderMan.CompileShaderFromSource("tess_control_shader_source.txt",GL_TESS_CONTROL_SHADER);

	//Create tess eval shader
	tess_eval_shader = shaderMan.CompileShaderFromSource("tess_eval_shader_source.txt",GL_TESS_EVALUATION_SHADER);

	//Create geometry shader
	geometry_shader = shaderMan.CompileShaderFromSource("geometry_shader_source.txt",GL_GEOMETRY_SHADER);

	//Create fragment shader
	fragment_shader = shaderMan.CompileShaderFromSource("spinningcubes_frag.txt",GL_FRAGMENT_SHADER);
	
	//Create program, attach shaders to it, and link it... need to put this code inside the shader manager
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	//glAttachShader(program, tess_control_shader);
	//glAttachShader(program, tess_eval_shader);
	//glAttachShader(program, geometry_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}
/**
	Place to experiment with Buffers
*/
void GraphicsEngine::InitBuffers(void)
{
	GLuint buffer[2];

	//playin with vertex array objects
	
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	//This is the data that we will place into the buffer object

	
	static const GLfloat vertex_positions[] =
	{
		    -0.25f,  0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f,  0.25f,

            -0.25f,  0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f,  0.25f,  0.25f,

             0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f, -0.25f
	};

	static const GLfloat vertex_texcoords[] =
	{
		    -0.0f,  1.0f,
            -0.0f, -0.0f,
             1.0f, -0.0f,

             1.0f, -0.0f,
             1.0f,  1.0f,
            -0.0f,  1.0f,

             1.0f, -0.0f,
             1.0f, -0.0f,
             1.0f,  1.0f,

             1.0f, -0.0f,
             1.0f,  1.0f,
             1.0f,  1.0f,

             1.0f, -0.0f,
            -0.0f, -0.0f,
             1.0f,  1.0f,

            -0.0f, -0.0f,
            -0.0f,  1.0f,
             1.0f,  1.0f,

            -0.0f, -0.0f,
            -0.0f, -0.0f,
            -0.0f,  0.25f,

            -0.0f, -0.0f,
            -0.0f,  1.0f,
            -0.0f,  1.0f,

            -0.0f, -0.0f,
             1.0f, -0.0f,
             1.0f, -0.0f,

             1.0f, -0.0f,
            -0.0f, -0.0f,
            -0.0f, -0.0f,

            -0.0f,  1.0f,
             1.0f,  1.0f,
             1.0f,  1.0f,

             1.0f,  1.0f,
            -0.0f,  1.0f,
            -0.0f,  1.0f
	};

	glGenBuffers(2, buffer);
	
	glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,buffer[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_texcoords), vertex_texcoords, GL_STATIC_DRAW);

	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE,0,NULL);
	glEnableVertexAttribArray(4);

	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,sizeof(vertex),(void*)offsetof(vertex,r));
	//glEnableVertexAttribArray(2);

}

void GraphicsEngine::InitTextures(void)
{
	
	//	testing loading textures here

	GEImage testImage = LoadBitmap("../../OpenGLSuperBible/textures/test2.bmp");
	//GEImage testImage = LoadBitmap("F:\\Projects\\Programming\\OpenGL\\TestProject01\\SuperBibleChapter01\\textures\\test2.bmp");
	//GEImage testImage = LoadTarga("F:\\Projects\\Programming\\OpenGL\\TestProject01\\SuperBibleChapter01\\textures\\testAlphaTest.tga");

	//	get data to pass to glTex.  TODO: see if way to pass data directly to OpenGL without this intermediate step.
	float* data = (float*)malloc(testImage.getWidth() * testImage.getHeight() * testImage.getNumChannels() * sizeof(float));

	
	testImage.getData(data);

	GLuint texture;

	GLenum internalFormat;
	GLenum imageFormat;

	if (testImage.getNumChannels() == 3)
	{
		internalFormat = GL_RGB32F;
		imageFormat = GL_RGB;
	}
	else
	{
		internalFormat = GL_RGBA32F;
		imageFormat = GL_RGBA;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D ,texture);
	glTexStorage2D(GL_TEXTURE_2D, 5, internalFormat, testImage.getWidth(), testImage.getHeight());
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, testImage.getWidth(), testImage.getHeight(), imageFormat, GL_FLOAT,data);

	//	create the mipmaps.
	glGenerateMipmap(GL_TEXTURE_2D);

	//	now we generate the sampler... optional (a default sampler will be assigned otherwise)
	GLuint sampler;
	glGenSamplers(1, &sampler);

	//	set some sampler options
	glSamplerParameteri(sampler,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(sampler,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//	now bind it to a texture unit
	glBindSampler(0, sampler);	//bind it to texture unit 0... the only one we are using currently.
	delete [] data;

}

void GraphicsEngine::UpdateWindowSize(int x, int y, int width, int height)
{
	float aspect = (float)width/(float)height;
	proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
	glViewport(x,y,width,height);
}

