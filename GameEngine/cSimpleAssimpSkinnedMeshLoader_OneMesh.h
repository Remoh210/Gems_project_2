#ifndef _cSimpleAssimpSkinnedMesh_HG
#define _cSimpleAssimpSkinnedMesh_HG

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <vector>
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include "sVertex_xyz_rgba_n_uv2_bt_4Bones.h"
#include "cMesh.h"

//Inspired by this page: http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html

struct sAnimationInfo
{
	std::string friendlyName;
	std::string fileName;
	float duration;
	bool bHasExitTime;
	const aiScene* pAIScene;
};

class cSimpleAssimpSkinnedMesh 
{
public:
	static const int MAX_BONES_PER_VERTEX = 4;
private:

	struct sVertexBoneData
	{
		//std::array<unsigned int, MAX_BONES_PER_VERTEX> ids;
		std::array<float, MAX_BONES_PER_VERTEX> ids;
		std::array<float, MAX_BONES_PER_VERTEX> weights;

		void AddBoneData(unsigned int BoneID, float Weight);
	};

	struct sBoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
		glm::mat4 ObjectBoneTransformation;
	};

public:

	unsigned int m_numberOfVertices;
	unsigned int m_numberOfIndices;
	unsigned int m_numberOfTriangles;

public:
	cSimpleAssimpSkinnedMesh(void);
	virtual ~cSimpleAssimpSkinnedMesh(void);

	bool LoadMeshFromFile( const std::string &friendlyName, 
						   const std::string &filename );		// lodaing  Mesh
	bool LoadMeshAnimation( const std::string &friendlyName,
							const std::string &filename, bool hasExitTime = false);	// lodaing only Animation

	// Added to assist with the forward kinematics (id and bone names)
	void GetListOfBoneIDandNames( std::vector<std::string> &vecBoneNames );

	std::string fileName;
	std::string friendlyName;

	const aiScene* pScene;		// "pretty" mesh we update and draw
	Assimp::Importer mImporter;



	std::map< std::string /*animation FRIENDLY name*/,
		      sAnimationInfo > mapAnimationFriendlyNameTo_pScene;		// Animations

	// Looks in the animation map and returns the total time
	float FindAnimationTotalTime( std::string animationName );
	float GetDurationInSec(std::string animationName);
	sAnimationInfo* GetAnimationInfo(std::string animationName);

	


	cMesh* CreateMeshObjectFromCurrentModel( unsigned int meshIndex = 0 );


	void BoneTransform(float TimeInSeconds, 
					   std::string animationName,		// Now we can pick the animation
					   std::vector<glm::mat4> &FinalTransformation, 
					   std::vector<glm::mat4> &Globals, 
					   std::vector<glm::mat4> &Offsets);
	
	unsigned int GetNums(void) const { return this->mNumBones; }

	//float GetDuration(const std::string animName);
	
	std::vector<sVertexBoneData> vecVertexBoneData;	//Bones;
	glm::mat4 mGlobalInverseTransformation;


	bool Initialize(void);
	
	void CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, aiQuaternion& out);
	void CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);
	void CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);

	void CalcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out);
	void CalcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);
	void CalcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);

	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

	const aiNodeAnim* FindNodeAnimationChannel(const aiAnimation* pAnimation, aiString nodeOrBoneName);
	
	//void ReadNodeHeirarchy(float AnimationTime, 
	//					   const aiNode* pNode, 
	//					   const glm::mat4 &parentTransformMatrix);

	void ReadNodeHeirarchy(float AnimationTime, 
						   std::string animationName,		// Now with more "choose animation"
						   const aiNode* pNode, 
						   const glm::mat4 &parentTransformMatrix);

	void LoadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &Bones);
	
	void ShutErDown(void);

//	std::vector<sMeshEntry> mMeshEntries;
	std::map<std::string /*BoneName*/, unsigned int /*BoneIndex*/> m_mapBoneNameToBoneIndex;	//mMapping;
	std::vector<sBoneInfo> mBoneInfo;
	unsigned int mNumBones;	//mNums;

};

#endif
