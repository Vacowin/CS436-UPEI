#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "Node.h"

class Camera
{
public:
	
	Camera();
	Camera(float p_fFOV, float p_fAspectRatio, float p_fNearClip, float p_fFarClip, const glm::vec3 &p_vPos, const glm::vec3& p_vTarget, const glm::vec3& p_vUp);
	virtual ~Camera();

	void SetFOV(float p_fFOV);
	float GetFOV() const;

	void SetAspectRatio(float p_fAspectRatio);
	float GetAspectRatio() const;

	void SetNearClip(float p_fNearClip);
	float GetNearClip() const;

	void SetFarClip(float p_fFarClip);
	float GetFarClip() const;

	void SetPos(const glm::vec3 &p_vPos);
	const glm::vec3& GetPos() const;

	void SetTarget(const glm::vec3& p_vTarget);
	const glm::vec3& GetTarget() const;

	void SetUp(const glm::vec3& p_vUp);
	const glm::vec3& GetUp() const;

	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewMatrix() const;

	void Update(float p_fDelta);

	void rotateHorizontal(double amt);
	void rotateVertical(double amt);
	void moveLeftRight(double amt);
	void moveForward(double amt);
	void calculateCameraRotation();

private:
	//Frustum *m_pFrustum;

	double theta;	
	double phi;

	float m_fFOV;
	float m_fAspectRatio;
	float m_fNearClip;
	float m_fFarClip;

	// Position, target and up vectors
	glm::vec3 m_vPos;
	glm::vec3 m_vTarget;
	glm::vec3 m_vUp;

	mutable bool m_bProjectionDirty;
	mutable bool m_bViewDirty;

	mutable glm::mat4 m_mProjectionMatrix;
	mutable glm::mat4 m_mViewMatrix;
};

#endif