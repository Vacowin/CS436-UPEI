#include "Node.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"


class DebugCube : public Node
{

public:
	DebugCube(const glm::vec3 &p_vPos, glm::vec3 p_vDim);

	void Render();

private:

	wolf::VertexDeclaration* m_pDecl ;

};

