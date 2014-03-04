#include "Node.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"


class DebugCube : public Node
{

public:
	DebugCube(int p_iID, const glm::vec3 &p_vPos, glm::vec3 p_vDim);

	void Render(const glm::mat4& p_mView, const glm::mat4& p_mProj);

private:

	wolf::VertexDeclaration* m_pDecl ;
	static Material *s_pMaterial;
};

