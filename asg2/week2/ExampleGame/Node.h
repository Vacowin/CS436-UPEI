#ifndef NODE_H
#define NODE_H

#include "W_MaterialManager.h"
#include "Transform.h"

#include <vector>

using namespace std;
using namespace wolf;
using namespace Common;

struct Vertex
{
	GLfloat x,y,z;
	GLubyte r,g,b,a;
};

class Node
{
public:
	//Node(string p_name);
	Node(const glm::vec3 &p_vPos);

	void AddChildNode(Node *p_pNode) {m_lChildren.push_back(p_pNode); p_pNode->SetParent(this);}
	void SetParent(Node *p_pParent) {m_pParent = p_pParent;}
	void SetBVRadius(float p_fRadius) {m_fBVRadius = p_fRadius;}
	void SetMaterial(Material *p_pMaterial) {m_pMaterial = p_pMaterial;}
	
	float GetBVRadius() { return m_fBVRadius;}

	// Scale
	void Scale(const glm::vec3& p_vScale) {m_Transform.Scale(p_vScale);}
	void SetScale(const glm::vec3& p_vScale) {m_Transform.SetScale(p_vScale);}
	const glm::vec3& GetScale() {return m_Transform.GetScale();}

	// Rotate
	void Rotate(const glm::vec3& p_vRotation) {m_Transform.Rotate(p_vRotation);}
	void SetRotation(const glm::vec3& p_vRotation) {m_Transform.SetRotation(p_vRotation);}
	const glm::vec3& GetRotation() {return m_Transform.GetRotation();}

	// Translate
	void Translate(const glm::vec3& p_vTranslation) {m_Transform.Translate(p_vTranslation);}
	void SetTranslation(const glm::vec3 &p_vTranslation) {m_Transform.SetTranslation(p_vTranslation);}
	const glm::vec3& GetTranslation() {return m_Transform.GetTranslation();}

	const glm::vec3 GetWorldTranslation();
	const glm::vec3 GetWorldRotation();
	const glm::vec3 GetWorldScale();

	const glm::mat4 GetWorldTransform();
	const glm::mat4 GetTransform() { return m_Transform.GetTransformation(); }

	Material* GetMaterial() { return m_pMaterial;}

	virtual void Update(float p_fDelta);
	virtual void Render(){};
protected:
	vector<Node*> m_lChildren;
	Node *m_pParent;
	//string m_sName;
	Transform m_Transform; 
	
	Material *m_pMaterial;
	float m_fBVRadius;
};

#endif