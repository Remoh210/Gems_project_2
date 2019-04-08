//     ___                 ___ _     
//    / _ \ _ __  ___ _ _ / __| |    
//   | (_) | '_ \/ -_) ' \ (_ | |__  
//    \___/| .__/\___|_||_\___|____| 
//         |_|                       
//
#include "globalOpenGLStuff.h"
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include "Camera.h"
#include <stdlib.h>
#include <stdio.h>		
#include <iostream>		
#include <vector>	

#define GLM_ENABLE_EXPERIMENTAL		
#include <glm/gtx/quaternion.hpp>	
#include <glm/gtx/norm.hpp>	
#include <Interfaces/iRigidBody.h>

#include "cShaderManager.h"
#include "cGameObject.h"
#include "cVAOMeshManager.h"
#include "cSoundManager.h"
#include <algorithm>
#include <windows.h>


#include "DebugRenderer/cDebugRenderer.h"
#include "cLightHelper.h"


//TEST
glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

//Dll 
HINSTANCE hGetProckDll = 0;
//typedef nPhysics::iPhysicsFactory*(*f_createPhysicsFactory)();
ePhysics physics_library = UNKNOWN;

nPhysics::iPhysicsFactory* gPhysicsFactory = NULL;
nPhysics::iPhysicsWorld* gPhysicsWorld = NULL;

glm::vec3 g_Gravity = glm::vec3(0.0f, -1.0f, 0.0f);

GLuint program;
cDebugRenderer* g_pDebugRendererACTUAL = NULL;
iDebugRenderer* g_pDebugRenderer = NULL;
cSimpleDebugRenderer* g_simpleDubugRenderer = NULL;
cSoundManager* g_pSoundManager = NULL;
cLuaBrain* p_LuaScripts = NULL;
cTextRend* g_textRenderer = NULL;
cCharacterManager* g_pCharacterManager = NULL;

//cCommandGroup sceneCommandGroup;
int cou;
int nbFrames = 0;
int FPS = 0;

bool RayHitted = false;
std::vector<cAABB::sAABB_Triangle> vec_cur_AABB_tris;
void UpdateWindowTitle(void);
double currentTime = 0;
double deltaTime = 0;
double FPS_last_Time = 0;
bool bIsDebugMode = false;

//for collision
float time = 0.0f;
bool collided = false;
//for collision
std::vector< cGameObject* > vec_pObjectsToDraw;
//for physics
std::vector< cGameObject* > vec_pSpheres;
unsigned int numberOfObjectsToDraw = 0;

unsigned int SCR_WIDTH = 1000;
unsigned int SCR_HEIGHT = 800;
std::string title = "Default";
std::string scene = "Scene1.json";

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

bool distToCam(cGameObject* leftObj, cGameObject* rightObj) {
	return glm::distance(leftObj->position, camera.Position) > glm::distance(rightObj->position, camera.Position); // here go your sort conditions
}

std::vector <cGameObject*> vec_sorted_drawObj;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 g_CameraEye = glm::vec3( 0.0, 0.0, 250.0f );



cShaderManager* pTheShaderManager = NULL;
cVAOMeshManager* g_pTheVAOMeshManager = NULL;
cSceneManager* g_pSceneManager = NULL;
cLightManager* LightManager = NULL;

std::vector<cGameObject*> vec_transObj;
std::vector<cGameObject*> vec_non_transObj;

cBasicTextureManager* g_pTheTextureManager = NULL;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
glm::mat4 portal_view(glm::mat4 orig_view, cGameObject* src, cGameObject* dst);
cAABBHierarchy* g_pTheTerrain = new cAABBHierarchy();

bool loadConfig();
cFBO* g_pFBOMain;

cFBO* FBO_Portal1;
cFBO* FBO_Portal2;


nPhysics::iRigidBody* bodyHit = NULL;


// Set up the off screen textures to draw to
GLuint g_FBO = 0;
GLuint g_FBO_colourTexture = 0;
GLuint g_FBO_depthTexture = 0;
GLint g_FBO_SizeInPixes = 512;		// = 512 the WIDTH of the framebuffer, in pixels;









//DWORD WINAPI UpdateDalekPosition(void* pInitialData)
//{
//
//
//
//
//	while (true)
//	{
//
//		gPhysicsWorld->Update(deltaTime);
//
//	}
//
//
//	return 0;
//}










int main(void)
{
	loadConfig();
	
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

		
	pTheShaderManager = new cShaderManager();
	pTheShaderManager->setBasePath("assets/shaders/");

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pTheShaderManager->createProgramFromFile("BasicUberShader",
		vertexShader,
		fragmentShader))
	{		// Shaders are OK
		std::cout << "Compiled shaders OK." << std::endl;
	}
	else
	{
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}


	// Load the uniform location values (some of them, anyway)
	cShaderManager::cShaderProgram* pSP = ::pTheShaderManager->pGetShaderProgramFromFriendlyName("BasicUberShader");
	pSP->LoadUniformLocation("texture00");
	pSP->LoadUniformLocation("texture01");
	pSP->LoadUniformLocation("texture02");
	pSP->LoadUniformLocation("texture03");
	pSP->LoadUniformLocation("texture04");
	pSP->LoadUniformLocation("texture05");
	pSP->LoadUniformLocation("texture06");
	pSP->LoadUniformLocation("texture07");
	pSP->LoadUniformLocation("texBlendWeights[0]");
	pSP->LoadUniformLocation("texBlendWeights[1]");




	program = pTheShaderManager->getIDFromFriendlyName("BasicUberShader");


	::g_pTheVAOMeshManager = new cVAOMeshManager();
	::g_pTheVAOMeshManager->SetBasePath("assets/models");
	::g_pTheTextureManager = new cBasicTextureManager();
	::g_textRenderer = new cTextRend();
	::g_pSceneManager = new cSceneManager();
	::g_pSoundManager = new cSoundManager();
	::g_pSceneManager->setBasePath("scenes");
	::LightManager = new cLightManager();
	::g_pFBOMain = new cFBO();

	FBO_Portal1 = new cFBO();
	FBO_Portal2 = new cFBO();


	std::string FBOErrorString;
	if (::g_pFBOMain->init(800, 600, FBOErrorString))
	{
		std::cout << "Framebuffer 1 is good to go!" << std::endl;
	}
	else
	{
		std::cout << "Framebuffer 1 is NOT complete" << std::endl;
	}


	if (FBO_Portal1->init(800, 600, FBOErrorString))
	{
		std::cout << "Framebuffer 2 is good to go!" << std::endl;
	}
	else
	{
		std::cout << "Framebuffer 2 is NOT complete" << std::endl;
	}

	if (FBO_Portal2->init(800, 600, FBOErrorString))
	{
		std::cout << "Framebuffer 3 is good to go!" << std::endl;
	}
	else
	{
		std::cout << "Framebuffer 3 is NOT complete" << std::endl;
	}
	
	
	//Set Up FBO
	static const GLenum draw_bufers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_bufers);

	// Check for "completenesss"
	GLenum FBOStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (FBOStatus == GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is good to go!" << std::endl;
	}
	else
	{
		std::cout << "Framebuffer is NOT complete" << std::endl;
	}

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// Loading the uniform variables here (rather than the inner draw loop)
	GLint objectColour_UniLoc = glGetUniformLocation(program, "objectColour");
	GLint matModel_location = glGetUniformLocation(program, "matModel");
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");
	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");


	::g_pDebugRendererACTUAL = new cDebugRenderer();
	::g_pDebugRenderer = (iDebugRenderer*)::g_pDebugRendererACTUAL;

	if (!::g_pDebugRendererACTUAL->initialize())
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
		std::cout << "\t" << ::g_pDebugRendererACTUAL->getLastError() << std::endl;
	}
	else
	{
		std::cout << "Debug renderer is OK" << std::endl;
	}
	glm::mat4x4 matProjection = glm::mat4(1.0f);
	glm::mat4x4	matView = glm::mat4(1.0f);

	//Physics Initialization
	hGetProckDll = LoadLibraryA("BulletPhysics.dll");
	physics_library = BULLET;
	f_createPhysicsFactory CreatePhysicsFactory = (f_createPhysicsFactory)GetProcAddress(hGetProckDll, "CreateFactory");
	gPhysicsFactory = CreatePhysicsFactory();
	gPhysicsWorld = gPhysicsFactory->CreatePhysicsWorld();
	gPhysicsWorld->SetGravity(g_Gravity);

	LoadSkinnedMeshModel(::vec_pObjectsToDraw, program);
	g_pSoundManager->InitFmod();
	LoadModelTypes(::g_pTheVAOMeshManager, program);
	::g_pSceneManager->loadScene(scene);
	::LightManager->LoadUniformLocations(program);
	
	LoadModelsIntoScene(::vec_pObjectsToDraw);
	g_simpleDubugRenderer = new cSimpleDebugRenderer(findObjectByFriendlyName("DebugCapsule"), findObjectByFriendlyName("DebugSphere"), findObjectByFriendlyName("DebugCube"), program);
	g_pCharacterManager = new cCharacterManager(vec_pObjectsToDraw);
	g_pCharacterManager->setActiveChar("chan");
	
	
	//for (unsigned int objIndex = 0;
	//	objIndex != (unsigned int)vec_pObjectsToDraw.size();
	//	objIndex++)
	//{
	//	cGameObject* pCurrentMesh = vec_pObjectsToDraw[objIndex];
	//	if (pCurrentMesh->materialDiffuse.a < 1.0f) { vec_transObj.push_back(pCurrentMesh); }
	//	else { vec_non_transObj.push_back(pCurrentMesh); }

	//}//for ( unsigned int objIndex = 0; 

	double lastTime = glfwGetTime();


	
	cLightHelper* pLightHelper = new cLightHelper();
	
	

	//Lua
	//::p_LuaScripts = new cLuaBrain();
	//::p_LuaScripts->SetObjectVector(&(::vec_pObjectsToDraw));
	//::p_LuaScripts->LoadScriptFile("example.lua");


	//FBO
	int renderPassNumber = 1;
	// 1 = 1st pass (the actual scene)
	// 2 = 2nd pass (rendering what we drew to the output)
	GLint renderPassNumber_UniLoc = glGetUniformLocation(program, "renderPassNumber");


	//Copy All Spheres to new Vec to manipulate them later
	for (int i = 0; i < vec_pObjectsToDraw.size(); i++)
	{
		if (vec_pObjectsToDraw[i]->rigidBody != NULL) {
			if (vec_pObjectsToDraw[i]->rigidBody->GetShape()->GetShapeType() != nPhysics::SHAPE_TYPE_PLANE)
			{
				vec_pSpheres.push_back(vec_pObjectsToDraw[i]);
			}
		}
	}
	cGameObject* player = g_pCharacterManager->getActiveChar();
	cGameObject* portal = findObjectByFriendlyName("portal");
	cGameObject* portal2 = findObjectByFriendlyName("portal2");
	camera.setThirdPerson(player);









	//LPDWORD phThread = 0;
	//DWORD hThread = 0;
	//HANDLE hThreadHandle = 0;


	//hThreadHandle = CreateThread(NULL,
	//	0,
	//	&UpdateDalekPosition,
	//	0,
	//	0,
	//	(DWORD*)&phThread);



	// Draw the "scene" (run the program)
	while (!glfwWindowShouldClose(window))
    {

		// Switch to the shader we want
		::pTheShaderManager->useShaderProgram( "BasicUberShader" );




		//First Portal
		float ratio;
		int width, height;
		width = 600;
		height = 800;


		glBindFramebuffer(GL_FRAMEBUFFER, FBO_Portal1->ID);		// Points to the "regular" frame buffer
		FBO_Portal1->clearBuffers(true, true);

													// Get the size of the actual (screen) frame buffer
//		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		
		glm::vec3 portal2CamEye = portal2->position;
		portal2CamEye.y = portal2->position.y;
		//matView = glm::lookAt(portal2CamEye, portal2->getForward() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		
		glm::mat4 portal1View = portal_view(camera.GetViewMatrix(), portal, portal2);
		//glm::mat4 tranMat = glm::translate(portal1View, -camera.Position);
		//Mirror Test


		matProjection = glm::perspective(1.0f,			// FOV
			ratio,		// Aspect ratio
			0.1f,			// Near clipping plane
			10000.0f);	// Far clipping plane

		glUniform3f(eyeLocation_location, camera.Position.x, camera.Position.y, camera.Position.z);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(portal1View));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		glm::mat4 matModel = glm::mat4(1.0f);	// identity
		DrawScene_Simple(vec_pObjectsToDraw, program, 1, FBO_Portal1);







		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4	matView = glm::mat4(1.0f);

		//Second Portal
		width = 600;
		height = 800;
		matProjection = glm::mat4(1.0f);
		matView = glm::mat4(1.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO_Portal2->ID);		// Points to the "regular" frame buffer
		FBO_Portal2->clearBuffers(true, true);
													// Get the size of the actual (screen) frame buffer
//		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glm::vec3 portal1CamEye = portal->position;
		portal1CamEye.y = portal->position.y - 10.0f;
		matView = glm::lookAt(portal1CamEye, portal->getForward() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		//Mirror Test


		matProjection = glm::perspective(0.8f,			// FOV
			ratio,		// Aspect ratio
			0.1f,			// Near clipping plane
			10000.0f);	// Far clipping plane

		glUniform3f(eyeLocation_location, portal1CamEye.x, portal1CamEye.y, portal1CamEye.z);

		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		matModel = glm::mat4(1.0f);	// identity

		DrawScene_Simple(vec_pObjectsToDraw, program, 1, FBO_Portal2);




	
		
		// Set for the 1st pass
		glBindFramebuffer(GL_FRAMEBUFFER, g_FBO);		// Point output to FBO



		//**********************************************************
		// Clear the offscreen frame buffer
		glViewport(0, 0, g_FBO_SizeInPixes, g_FBO_SizeInPixes);
		GLfloat	zero = 0.0f;
		GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, &zero);
		glClearBufferfv(GL_DEPTH, 0, &one);
		//**********************************************************

		


		//glUniform1f(renderPassNumber_UniLoc, 1.0f);	// Tell shader it's the 1st pass

        ratio;
        width, height;



		matProjection = glm::mat4(1.0f);
		matView = glm::mat4(1.0f);
 

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);


		glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
		glEnable( GL_CULL_FACE );	// Discared "back facing" triangles

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		
		matProjection = glm::perspective( 1.0f,			// FOV
			                                ratio,		// Aspect ratio
			                                0.1f,		// Near clipping plane
			                                15000.0f );	// Far clipping plane


		matView = camera.GetViewMatrix();

		glUniform3f(eyeLocation_location, camera.Position.x, camera.Position.y, camera.Position.z);

		//matView = glm::lookAt( g_CameraEye,	// Eye
		//	                    g_CameraAt,		// At
		//	                    glm::vec3( 0.0f, 1.0f, 0.0f ) );// Up

		glUniformMatrix4fv( matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv( matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));
		LightManager->CopyLightValuesToShader();
			

		glUniform1f(renderPassNumber_UniLoc, 2.0f);
		portal->bIsVisible = true;
		portal2->bIsVisible = true;
		matModel = glm::mat4(1.0f);	// identity
		DrawObject(portal, matModel, program, FBO_Portal1);
		matModel = glm::mat4(1.0f);	// identity
		DrawObject(portal2, matModel, program, FBO_Portal2);
		portal->bIsVisible = false;
		portal2->bIsVisible = false;
		glUniform1f(renderPassNumber_UniLoc, 1.0f);
		DrawScene_Simple(vec_pObjectsToDraw, program, 1);














		g_pSceneManager->LoadtoVAO();
		 
//		DrawScene_Simple(vec_non_transObj, program, 1);
//		DrawScene_Simple(vec_transObj, program, 1);

		//for ( unsigned int objIndex = 0; 
		//	  objIndex != (unsigned int)vec_non_transObj.size();
		//	  objIndex++ )
		//{	
		//	cGameObject* pCurrentMesh = vec_non_transObj[objIndex];
		//	
		//	glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

		//	DrawObject(pCurrentMesh, matModel, program);

		//}//for ( unsigned int objIndex = 0; 

		//for (unsigned int objIndex = 0;
		//	objIndex != (unsigned int)vec_transObj.size();
		//	objIndex++)
		//{
		//	cGameObject* pCurrentMesh = vec_transObj[objIndex];

		//	glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

		//	DrawScene_Simple(pCurrentMesh, matModel, program);

		//}//for ( unsigned int objIndex = 0; 

		//DrawScene_Simple(std::vector<cGameObject*> vec_pMeshSceneObjects,
		//	GLuint shaderProgramID,
		//	unsigned int passNumber)
		//{

		//}


		double FPS_currentTime = glfwGetTime();
		nbFrames++;
		if (FPS_currentTime - FPS_last_Time >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			FPS = nbFrames * 1;
			nbFrames = 0;
			FPS_last_Time += 1.0;
		}
		g_textRenderer->drawText(width, height, ("FPS: " + std::to_string(FPS)).c_str());
		
		switch (physics_library)
		{
		case SIMPLE:
			g_textRenderer->drawText(width, height, ("Physics: My crappy physics"), 100.0f);
			break;
		case BULLET:
			g_textRenderer->drawText(width, height, ("Physics: Bullet"), 100.0f);
			break;
		case UNKNOWN:
			break;
		default:
			break;
		}
		g_textRenderer->drawText(width, height, ("Gravity: " + std::to_string((int)g_Gravity.y)).c_str(), 150.0f);
		
		std::string strhited;
		if (bodyHit)
			strhited = "Ray Hit: " + bodyHit->GetGOName();
		else
			strhited = "Ray Hit: Nothing";

		g_textRenderer->drawText(width, height, strhited.c_str(), 200.0f);
		//g_textRenderer->drawText(width, height,"Ray hit: " + RayHitted ? "no": "yes" , 200.0f);
		





		// High res timer (likely in ms or ns)
		currentTime = glfwGetTime();		
		deltaTime = currentTime - lastTime; 



		double MAX_DELTA_TIME = 0.1;	// 100 ms
		if (deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}
		// update the "last time"
		lastTime = currentTime;

		for ( unsigned int objIndex = 0; 
			  objIndex != (unsigned int)vec_pObjectsToDraw.size(); 
			  objIndex++ )
		{	
			cGameObject* pCurrentMesh = vec_pObjectsToDraw[objIndex];
			
			pCurrentMesh->Update( deltaTime );

		}//for ( unsigned int objIndex = 0; 

		//sceneCommandGroup.Update(deltaTime);

		

		// The physics update loop



		//New Dll physics




		gPhysicsWorld->Update(deltaTime);

		

		
		//Collision listner, kind of....

		std::string PairF = gPhysicsWorld->GetLastColPair().first;
		std::string PairS = gPhysicsWorld->GetLastColPair().second;
		if (PairF == "chan" && PairS == "HingeCube" || PairS == "chan" && PairF == "HingeCube")
		{

			collided = true;

			
		}
		if (collided)
		{
			cGameObject* hgcube = findObjectByFriendlyName("HingeCube");
			time += deltaTime;
			if (time < 2.0f)
			{
				hgcube->vecTextures[1].strength = 1.0f;
				time += deltaTime;

			}
			else
			{
				hgcube->vecTextures[1].strength = 0.0f;
				time = 0.f;
				collided = false;
			}
		}


		for (int i = 0; i < vec_pObjectsToDraw.size(); i++)
		{
			cGameObject* curMesh = vec_pObjectsToDraw[i];

			
			if (curMesh->rigidBody != NULL && curMesh->rigidBody->GetShape()->GetShapeType() != nPhysics::SHAPE_TYPE_PLANE) {
				

				if (curMesh->rigidBody->GetShape()->GetShapeType() == nPhysics::SHAPE_TYPE_CAPSULE) {
					float Totalheight;
					Totalheight = curMesh->rigidBody->GetShape()->GetCapsuleRadius() * 2 + 2.0f;
					curMesh->position = curMesh->rigidBody->GetPosition();
					curMesh->position.y = curMesh->rigidBody->GetPosition().y - Totalheight;


					

				}
				else {
					curMesh->position = curMesh->rigidBody->GetPosition();
					curMesh->m_meshQOrientation = glm::mat4(curMesh->rigidBody->GetMatRotation());

				}

			}


		}

	
		::g_pDebugRendererACTUAL->RenderDebugObjects(matView, matProjection, deltaTime);

		//::p_LuaScripts->UpdateCG(deltaTime);
		//::p_LuaScripts->Update(deltaTime);

		for (std::vector<sLight*>::iterator it = LightManager->vecLights.begin(); it != LightManager->vecLights.end(); ++it)
		{

			sLight* CurLight = *it;
			if (CurLight->AtenSphere == true)
			{


				cGameObject* pDebugSphere = findObjectByFriendlyName("DebugSphere");
				pDebugSphere->bIsVisible = true;
				pDebugSphere->bDontLight = true;
				glm::vec4 oldDiffuse = pDebugSphere->materialDiffuse;
				glm::vec3 oldScale = pDebugSphere->nonUniformScale;
				pDebugSphere->setDiffuseColour(glm::vec3(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f));
				pDebugSphere->bUseVertexColour = false;
				pDebugSphere->position = glm::vec3(CurLight->position);
				glm::mat4 matBall(1.0f);


				pDebugSphere->materialDiffuse = oldDiffuse;
				pDebugSphere->setUniformScale(0.1f);			// Position
				DrawObject(pDebugSphere, matBall, program);

				const float ACCURACY_OF_DISTANCE = 0.0001f;
				const float INFINITE_DISTANCE = 10000.0f;

				float distance90Percent =
					pLightHelper->calcApproxDistFromAtten(0.90f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);

				pDebugSphere->setUniformScale(distance90Percent);			// 90% brightness
				//pDebugSphere->objColour = glm::vec3(1.0f,1.0f,0.0f);
				pDebugSphere->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
				DrawObject(pDebugSphere, matBall, program);

				//			pDebugSphere->objColour = glm::vec3(0.0f,1.0f,0.0f);	// 50% brightness
				pDebugSphere->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
				float distance50Percent =
					pLightHelper->calcApproxDistFromAtten(0.50f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				pDebugSphere->setUniformScale(distance50Percent);
				DrawObject(pDebugSphere, matBall, program);

				//			pDebugSphere->objColour = glm::vec3(1.0f,0.0f,0.0f);	// 25% brightness
				pDebugSphere->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
				float distance25Percent =
					pLightHelper->calcApproxDistFromAtten(0.25f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				pDebugSphere->setUniformScale(distance25Percent);
				DrawObject(pDebugSphere, matBall, program);

				float distance1Percent =
					pLightHelper->calcApproxDistFromAtten(0.01f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				//			pDebugSphere->objColour = glm::vec3(0.0f,0.0f,1.0f);	// 1% brightness
				pDebugSphere->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
				pDebugSphere->setUniformScale(distance1Percent);
				DrawObject(pDebugSphere, matBall, program);

				//			pDebugSphere->objColour = oldColour;
				pDebugSphere->materialDiffuse = oldDiffuse;
				pDebugSphere->nonUniformScale = oldScale;
				pDebugSphere->bIsVisible = false;
			}
		}


		player = g_pCharacterManager->getActiveChar();
		if (camera.mCameraType == THIRD_PERSON)
		
		{
			camera.updateCameraVectors();
			glm::vec3 lookDirection = camera.Position - player->position;
			lookDirection.y = 0.0f;
			lookDirection = glm::normalize(lookDirection);
			glm::vec3 worldUP(0.0f, 1.0f, 0.0f);
			glm::mat4 finalOrientation = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), lookDirection, worldUP));
			player->m_meshQOrientation = glm::toQuat(finalOrientation);


			//camera.updateCameraVectors();
			//glm::vec3 lookDirection = camera.Position - player->position;
			//lookDirection.y = 1.0f;
			//glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), -lookDirection);

			//glm::vec3 desiredUp(0.0f, 1.0f, 0.0f);
			//glm::vec3 right = glm::cross(-lookDirection, desiredUp);
			//desiredUp = cross(right, -lookDirection);
			//glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
			//glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);
			//glm::quat targetOrientation = rot2 * rot1;
			//player->m_meshQOrientation = targetOrientation;


		}


		//Update collision flag
		for (int i = 0; i < vec_pObjectsToDraw.size(); i++)
		{
			cGameObject* CurGo = vec_pObjectsToDraw[i];
			if (CurGo->rigidBody != NULL)
			{
				//if (CurGo->friendlyName == "chan")
				//{
				//	std::cout<< "GO col: " << CurGo->bHadCollision << std::endl;
				//	std::cout << "RB col: " << CurGo->rigidBody->GetCollision() << std::endl;
				//}
				CurGo->bHadCollision = CurGo->rigidBody->GetCollision();
				CurGo->rigidBody->SetCollision(false);

				
			}
		}
		
		



		//Ray Cast
		glm::vec3 from = player->position + glm::vec3(0.0f, 10.0f, 0.0f);

		glm::vec3 to = player->getForward();
		to *= 50.0f;
		to = to + player->position;
		to.y = 10.0f;

		g_pDebugRendererACTUAL->addLine(from, to, glm::vec3(0.0f, 1.0f, 0.0f));
		
		bodyHit = gPhysicsWorld->RayCastGetObject(from, to);


		//Kill Velocity for inactive characters
		
		g_pCharacterManager->setAllChatVel();
		//{
		//	std::cout << "hit" << std::endl;
		//}
		//else
		//{
		//	std::cout << "notHit" << std::endl;
		//}

		UpdateWindowTitle();

		glfwSwapBuffers(window);		// Shows what we drew

        glfwPollEvents();
		ProcessAsynKeys(window);




    }//while (!glfwWindowShouldClose(window))

	// Delete stuff
	delete pTheShaderManager;
	delete ::g_pTheVAOMeshManager;
	delete ::g_pTheTextureManager;

	// 
	delete ::g_pDebugRenderer;

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}





glm::mat4 portal_view(glm::mat4 orig_view, cGameObject* src, cGameObject* dst) {
	orig_view = glm::translate(orig_view, -camera.Position);
	glm::mat4 mv = orig_view * src->matTransform;
	glm::mat4 portal_cam =
		// 3. transformation from source portal to the camera - it's the
		//    first portal's ModelView matrix:
		mv

		// 2. object is front-facing, the camera is facing the other way:
		* glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0))
		// 1. go the destination portal; using inverse, because camera
		//    transformations are reversed compared to object
		//    transformations:
		* glm::inverse(dst->matTransform);
	return portal_cam;
}

void UpdateWindowTitle(void)
{



	return;
}

cGameObject* findObjectByFriendlyName(std::string theNameToFind)
{
	for ( unsigned int index = 0; index != vec_pObjectsToDraw.size(); index++ )
	{
		if ( vec_pObjectsToDraw[index]->friendlyName == theNameToFind )
		{
			return vec_pObjectsToDraw[index];
		}
	}

	// Didn't find it.
	return NULL;	// 0 or nullptr
}


cGameObject* findObjectByUniqueID(unsigned int ID_to_find)
{
	for ( unsigned int index = 0; index != vec_pObjectsToDraw.size(); index++ )
	{
		if ( vec_pObjectsToDraw[index]->getUniqueID() == ID_to_find )
		{
			return vec_pObjectsToDraw[index];
		}
	}

	// Didn't find it.
	return NULL;	// 0 or nullptr
}

bool loadConfig()
{
	rapidjson::Document doc;
	FILE* fp = fopen("config/config.json", "rb"); // non-Windows use "r"
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	doc.ParseStream(is);
	fclose(fp);
	rapidjson::Value Window(rapidjson::kObjectType);
	Window = doc["Window"];

	SCR_WIDTH = Window["Width"].GetInt();
	SCR_HEIGHT = Window["Height"].GetInt();

	title = Window["Title"].GetString();
	if (doc.HasMember("Scene")) {
		scene = doc["Scene"].GetString();
	}
	if (doc.HasMember("Gravity")) {
		const rapidjson::Value& grArray = doc["Gravity"];
		for (int i = 0; i < 3; i++) {
			g_Gravity[i] = grArray[i].GetFloat();
		}
		
	}

	return true;

	//std::string language = doc["Language"].GetString();
	//ASSERT_NE(language, "");
	//if (language == "English") { TextRend.setLang(ENGLISH); }
	//else if (language == "Spanish") { TextRend.setLang(SPANISH); }
	//else if (language == "Japanese") { TextRend.setLang(JAPANESE); }
	//else if (language == "Ukrainian") { TextRend.setLang(UKRAINAN); }
	//else if (language == "Polish") { TextRend.setLang(POLSKA); }

}


glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}



