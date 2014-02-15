
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#define PI_OVER2 1.57

Camera::Camera()
	:
	m_fFOV(45.0f),
	m_fAspectRatio(1280.0f/720.0f),
	m_fNearClip(0.1f),
	m_fFarClip(1000.0f),
	m_vPos(glm::vec3(0.0f, 0.0f, -5.0f)),
	m_vTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_vUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_bProjectionDirty(true),
	m_bViewDirty(true)
{
	theta = 0;
	phi = 0;
	m_pFrustum = new Frustum();
	m_pFrustum->setCamInternals(m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
}

Camera::Camera(float p_fFOV, float p_fAspectRatio, float p_fNearClip, float p_fFarClip, const glm::vec3 &p_vPos, const glm::vec3& p_vTarget, const glm::vec3& p_vUp)
	:
	m_fFOV(p_fFOV),
	m_fAspectRatio(p_fAspectRatio),
	m_fNearClip(p_fNearClip),
	m_fFarClip(p_fFarClip),
	m_vPos(p_vPos),
	m_vTarget(p_vTarget),
	m_vUp(p_vUp),
	m_bProjectionDirty(true),
	m_bViewDirty(true)	
{
	theta = 0;
	phi=0;
	m_pFrustum = new Frustum();
	m_pFrustum->setCamInternals(m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
}


Camera::~Camera()
{
}

void Camera::SetFOV(float p_fFOV)
{
	m_bProjectionDirty = true;
	m_fFOV = p_fFOV;
}

float Camera::GetFOV() const
{
	return m_fFOV;
}

void Camera::SetAspectRatio(float p_fAspectRatio)
{
	m_bProjectionDirty = true;
	m_fAspectRatio = p_fAspectRatio;
}

float Camera::GetAspectRatio() const
{
	return m_fAspectRatio;
}

void Camera::SetNearClip(float p_fNearClip)
{
	m_bProjectionDirty = true;
	m_fNearClip = p_fNearClip;
}

float Camera::GetNearClip() const
{
	return m_fNearClip;
}

void Camera::SetFarClip(float p_fFarClip)
{
	m_bProjectionDirty = true;
	m_fFarClip = p_fFarClip;
}

float Camera::GetFarClip() const
{
	return m_fFarClip;
}

void Camera::SetPos(const glm::vec3 &p_vPos)
{
	m_bViewDirty = true;
	m_vPos = p_vPos;
}

const glm::vec3& Camera::GetPos() const
{
	return m_vPos;
}

void Camera::SetTarget(const glm::vec3& p_vTarget)
{
	m_bViewDirty = true;
	m_vTarget = p_vTarget;
}


const glm::vec3& Camera::GetTarget() const
{
	return m_vTarget;
}


void Camera::SetUp(const glm::vec3& p_vUp)
{
	m_bViewDirty = true;
	m_vUp = p_vUp;
}

const glm::vec3& Camera::GetUp() const
{
	return m_vUp;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	if (m_bProjectionDirty)
	{
		m_mProjectionMatrix = glm::perspective(m_fFOV, m_fAspectRatio, m_fNearClip, m_fFarClip);
		m_bProjectionDirty = false;
	}
	return m_mProjectionMatrix;
}

const glm::mat4& Camera::GetViewMatrix() const
{
	//if (m_bViewDirty)
	//{
		m_mViewMatrix = glm::lookAt(m_vPos, m_vTarget, m_vUp);
		//m_bViewDirty = false;
	//}
	return m_mViewMatrix;
}

void Camera::rotateHorizontal(double amt)
{
	double newTheta = theta - amt;
	if(newTheta > -PI_OVER2 && newTheta < PI_OVER2)
		theta = newTheta;
	else
	{
		if(newTheta <= -PI_OVER2)
			theta = -PI_OVER2;
		else
			theta = PI_OVER2;
	}
}

void Camera::rotateVertical(double amt)
{
	phi += amt;
}

void Camera::moveLeftRight(double amt)
{
	glm::vec3 temp = (m_vTarget - m_vPos);
	if (temp.x < 0)
		amt*=-1;

	float y =sqrt( amt*amt/ (pow((temp.z/temp.x),2) + 1) );
	if (amt<0)
		y *=-1;

	if (temp.x == 0) return;
	float x = - (temp.z/temp.x)*y;

	glm::vec3 moveVec = glm::vec3(x,0,y);
	m_vPos = m_vPos + moveVec;
	m_vTarget = m_vTarget + moveVec;
}

void Camera::moveForward(double amt)
{
	glm::vec3 temp = (m_vTarget - m_vPos);
	temp*=amt;
	glm::vec3 direction = temp;
	m_vTarget = m_vTarget + direction;
	m_vPos = m_vPos + direction;
}

void Camera::calculateCameraRotation()
{
	double radius = 7;
	double x, y, z;
	double l = radius * cos(theta);

	y = radius * sin(theta);
	x = l * cos(phi);
	z = l * sin(phi);

	glm::vec3 temp = glm::vec3(x, y, z);
	//if (temp == glm::vec3(7,0,0)) return; // remain original pos
	m_vTarget = glm::vec3(x, y, z) + m_vPos;
}

void Camera::Update(float p_fDelta)
{
	m_pFrustum->setCamDef(m_vPos, m_vTarget, m_vUp);
}

//int Camera::CheckFrustum(Node* p_pNode)
//{
//	return m_pFrustum->sphereInFrustum(p_pNode->GetWorldTranslation(), p_pNode->GetBVRadius());
//}