#ifndef _cSceneManager_HG_
#define _cSceneManager_HG_
#include <string>
#include "cGameObject.h"

class cSceneManager
{
public:

	cSceneManager();

	bool saveScene(std::string filename);
	bool loadScene(std::string filename);
	//bool loadScene(std::string filename);
	void setBasePath(std::string basepath);

	void LoadHighResMeshes();
	void LoadtoVAO();
	std::vector <cGameObject*> vec_Load_toVAO_meshes;
	bool LoadModels_Async();
private:
	
	std::string m_basePath;


};


















#endif