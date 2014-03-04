//------------------------------------------------------------------------
// Transform
//
// Created:	2012/12/12
// Author:	Carel Boers
//	
// This class implements a Transform component which is a special (mandatory)
// component that is a part of each GameObject.
//------------------------------------------------------------------------

#ifndef TRANSFORM
#define TRANSFORM

#include <glm/glm.hpp>

namespace Common
{
	class Transform
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		Transform();
		virtual ~Transform();

		// Scale
		void Scale(const glm::vec3& p_vScale);
		void SetScale(const glm::vec3& p_vScale);
		const glm::vec3& GetScale() const;

		// Rotate
		void Rotate(const glm::vec3& p_vRotation);
		void SetRotation(const glm::vec3& p_vRotation);
		const glm::vec3& GetRotation() const;

		// Translate
		void Translate(const glm::vec3& p_vTranslation);
		void SetTranslation(const glm::vec3 &p_vTranslation);
		const glm::vec3& GetTranslation() const;

		// Transformation and DerivedTransformation
		const glm::mat4& GetTransformation() const;
		const glm::mat4& GetDerivedTransformation() const;

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Whether the transformation matrix needs to be recalculated because the transform has changed
		mutable bool m_bDirty;

		// The scale, rotation and translation
		glm::vec3 m_vScale;
		glm::vec3 m_vRotation;
		glm::vec3 m_vTranslation;

		// The combined transform
		mutable glm::mat4 m_mTransform;
	};
}

#endif // TRANSFORM