#include "Effect.h"
#include "tinyxml\tinyxml.h"

Effect::Effect(std::string p_sPath, const glm::vec3 &p_vPos) : Node(0, p_vPos)
{
	SetTranslation(p_vPos);

	TiXmlDocument doc(p_sPath.c_str());
	if (doc.LoadFile() == false)
	{
		return;
	}
	TiXmlNode* pRoot = doc.FirstChild("Effect");
	if (pRoot == NULL)
	{
		return;
	}
	TiXmlElement* pElement = pRoot->ToElement();
	m_sName = pElement->Attribute("name");

	TiXmlNode* pChildNode = pRoot->FirstChild();
	while (pChildNode != NULL )
	{
		const char* szNodeName = pChildNode->Value();
		if (strcmp(szNodeName, "Emitter") == 0)
		{
			pElement = pChildNode->ToElement();
			std::string sFile = pElement->Attribute("file");
			float x,y,z;
			pElement->QueryFloatAttribute("x", &x);
			pElement->QueryFloatAttribute("y", &y);
			pElement->QueryFloatAttribute("z", &z);

			Emitter *pEmitter = new Emitter(sFile, glm::vec3(x,y,z));
			pEmitter->SetParent(this);
			m_lChildren.push_back(pEmitter);
			m_emitterList.push_back(pEmitter);
		}

		pChildNode = pChildNode->NextSibling();
	}
}

Effect::~Effect(void)
{
	for (int i=0;i<m_emitterList.size();i++)
	{
		Emitter *pEmitter = m_emitterList.at(i);
		delete pEmitter;
		pEmitter = nullptr;
	}
	m_emitterList.clear();
	m_lChildren.clear();
}

void Effect::Update(float p_fDelta)
{
	std::vector<Emitter*>::iterator it;
	for(it = m_emitterList.begin(); it != m_emitterList.end(); ++it) 
	{
		static_cast<Emitter*>(*it)->Update(p_fDelta);
	}
}

void Effect::Render(const glm::mat4& p_mView, const glm::mat4& p_mProj)
{
	std::vector<Emitter*>::iterator it;
	for(it = m_emitterList.begin(); it != m_emitterList.end(); ++it) 
	{
		static_cast<Emitter*>(*it)->Render(p_mView, p_mProj);
	}
}
