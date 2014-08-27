

#include <cstdio>

#include "GraphicsEngine.h"
#include "GEImage.h"
#include "TextureManager.h"
#include "ImageUtilities\ImageUtilities.h"

//Link static variables
vmath::mat4 GraphicsEngine::proj_matrix;

GraphicsEngine::GraphicsEngine()
{
	 seed = 0x13371337;  //here just for the alien rain example

	//Initialize Graphics upon instantiation.  TODO Might want to make the program make an explicit call.  For now this works
	if(!Init())
	{
		printf("Error Initializing Graphics");
		exit(EXIT_FAILURE);
	}

	
}

GraphicsEngine::~GraphicsEngine()
{
	// clean up all pointers here

	//voa

	//vb
}

void GraphicsEngine::Render(double currentTime)
{
	const GLfloat bkColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat one = 1.0f;
	
	glClearBufferfv(GL_COLOR, 0, bkColor);
	glClearBufferfv(GL_DEPTH,0, &one);

	float t = (float)currentTime;

	glUseProgram(rendering_program);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, rain_buffer); 

	vmath::vec4 * droplet = (vmath::vec4 *) glMapBufferRange(GL_UNIFORM_BUFFER,0,256* sizeof (vmath::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < 256; i++)
	{
		droplet[i][0] = droplet_x_offset[i];
		droplet[i][1] = 2.0f - fmodf( (t + float(i) ) * droplet_fall_speed[i], 4.31f);
		droplet[i][2] = t * droplet_rot_speed[i];
		droplet[i][3] = 0.0f;
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	int alien_index;

	for ( alien_index = 0; alien_index < 256; alien_index++)
	{
		glVertexAttribI1i(0, alien_index);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	GLint mv_location = glGetUniformLocation(rendering_program, "mv_matrix");
	GLint proj_location = glGetUniformLocation(rendering_program, "proj_matrix");

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool GraphicsEngine::Init()
{
	glfwSetErrorCallback(error_callback);

	//initial OpenGL (glfw)
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

	// create the window
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

	// print some OpenGL info
	printf( "GL_VENDOR: %s\n",glGetString(GL_VENDOR));
	printf( "GL_RENDERER: %s\n",glGetString(GL_RENDERER));
	printf( "GL_VERSION: %s\n",glGetString(GL_VERSION));
	printf( "GL_SHADING_LANGUAGE_VERSION: %s\n\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

	// specify some callback functions
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

double GraphicsEngine::getCurrentTime() const
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
	vertex_shader = shaderMan.CompileShaderFromSource("alien_rain_vert.glsl", GL_VERTEX_SHADER );
	
	//Create tess control shader
	tess_control_shader = shaderMan.CompileShaderFromSource("tess_control_shader_source.txt", GL_TESS_CONTROL_SHADER);

	//Create tess eval shader
	tess_eval_shader = shaderMan.CompileShaderFromSource("tess_eval_shader_source.txt", GL_TESS_EVALUATION_SHADER);

	//Create geometry shader
	geometry_shader = shaderMan.CompileShaderFromSource("geometry_shader_source.txt", GL_GEOMETRY_SHADER);

	//Create fragment shader
	fragment_shader = shaderMan.CompileShaderFromSource("alien_rain_frag.glsl", GL_FRAGMENT_SHADER);
	
	//Create program, attach shaders to it, and link it... need to put this code inside the shader manager
	//TODO: Error handling.
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

	// playin with vertex array objects
	
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	// This is the data that we will place into the buffer object

	glGenBuffers(1, &rain_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, rain_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 256 * sizeof(vmath::vec4), NULL, GL_DYNAMIC_DRAW);

    for (int i = 0; i < 256; i++)
    {
        droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
        droplet_rot_speed[i] = (random_float() + 0.5f) * ((i & 1) ? -3.0f : 3.0f);
        droplet_fall_speed[i] = random_float() + 0.2f;
    }

}

void GraphicsEngine::InitTextures(void)
{
	
	//	testing loading textures here

	TextureManager texMan;
	GLenum glError = 0;

	//GEImage testImage = LoadBitmap("../../OpenGLSuperBible/textures/test2.bmp");
	GEImage testImage1 = texMan.LoadTexture( "test2.bmp", GE_TEXTYPE_BMP );
	GEImage testImage2 = texMan.LoadTexture( "test2r.bmp", GE_TEXTYPE_BMP );
	//GEImage testImage = LoadBitmap("F:\\Projects\\Programming\\OpenGL\\TestProject01\\SuperBibleChapter01\\textures\\test2.bmp");
	//GEImage testImage = LoadTarga("F:\\Projects\\Programming\\OpenGL\\TestProject01\\SuperBibleChapter01\\textures\\testAlphaTest.tga");

	//	get data to pass to glTex.  TODO: see if way to pass data directly to OpenGL without this intermediate step.
	float* data[2];
	data[0] = (float*)malloc(testImage1.getWidth() * testImage1.getHeight() * testImage1.getNumChannels() * sizeof(float));
	data[1] = (float*)malloc(testImage2.getWidth() * testImage2.getHeight() * testImage2.getNumChannels() * sizeof(float));

	
	testImage1.getData(data[0]);
	testImage2.getData(data[1]);

	GLuint texture;

	GLenum internalFormat;
	GLenum imageFormat;

	if (testImage1.getNumChannels() == 3)
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
	glBindTexture(GL_TEXTURE_2D_ARRAY ,texture);

	glError = glGetError();
	if (glError != GL_NO_ERROR)
	{
		printf( "Error binding texture: %d\n", glError );
	}

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 5, GL_RGB32F, 256, 256, 2);

	glError = glGetError();
	if (glError != GL_NO_ERROR)
	{
		printf( "Error generating texture buffer: %d\n", glError );
	}

	for (int i = 0; i < 2; i++)
	{
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,i,256,256,1, GL_RGB, GL_FLOAT,data[i]);
		
		glError = glGetError();
		if (glError != GL_NO_ERROR)
		{
			printf( "Error copying texture to buffer: %d\n", glError );
		}
	}

	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//	create the mipmaps.
	//glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	//	now we generate the sampler... optional (a default sampler will be assigned otherwise)
	//GLuint sampler;
	//glGenSamplers(1, &sampler);

	//	set some sampler options
	//glSamplerParameteri(sampler,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	//glSamplerParameteri(sampler,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//	now bind it to a texture unit
	//glBindSampler(0, sampler);	//bind it to texture unit 0... the only one we are using currently.
	
	//	clean up
	delete [] data[0];
	delete [] data[1];

}

void GraphicsEngine::UpdateWindowSize(int x, int y, int width, int height)
{
	float aspect = (float)width/(float)height;
	proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
	glViewport(x,y,width,height);
}

bool GraphicsEngine::isMeshBuffered( std::string meshPath)
{
	// check if mesh already loaded
	bool loaded = false;

	if (vaoMap.find( meshPath ) == vaoMap.end() && vbMap.find( meshPath ) == vbMap.end() )
	{
		// TODO: what happens if only one is true.
		loaded = true;
	}
}

bool GraphicsEngine::BufferMesh( std::string meshPath, GEVertex* mesh, int numVerts )
{
	// Create GLuint to hold new voa.  Each Mesh gets its own voa.
	GLuint* newVoa = new GLuint;
	
	// Create a new buffer
	GLuint* newBuffer = new GLuint;
	
	glGenVertexArrays( 1, newVoa );
	glBindVertexArray( *newVoa );

	glGenBuffers( 1, newBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, *newBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GEVertex ) * numVerts, mesh, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( GEVertex ), ( void* )offsetof( GEVertex, x ));
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( GEVertex ), ( void* )offsetof( GEVertex, r ));
	glEnableVertexAttribArray(1);

	// Next put into the maps.

	// NOTE: DO NOT delete[] newVoa and newBuffer, that cleanup will happen when they are removed from the buffer maps.
}

