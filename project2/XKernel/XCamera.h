#ifndef __CAMERA_H__
#define __CAMERA_H__
#ifdef __cplusplus

#include "xbasedefs.h"

typedef enum _XCameraType
{
	camFly,		// Flying camera (Descent style)
	camWalk,	// Walking camera (Quake style)
} _XCameraType;

class _XCamera
{
// public functions ////////////////////////////////////////////////////////
public:
	_XCamera(_XCameraType p_Type = camFly, float fov = 60.0, float aspect = 1.0,
		     float nearplane = 1.0, float farplane = 5000.0);
	~_XCamera() {}

	_XCamera& operator = (const _XCamera& inCam); // assignment

	void ZeroMembers(void);

	void SetType(_XCameraType p_Type);
	_XCameraType GetType() { return mp_Type; }

	void Set(_XCameraType p_Type, float fov, float aspect,
		     float nearplane, float farplane);
	void Set(float pos_x, float pos_y, float pos_z);
	
	inline D3DXMATRIX GetViewMatrix() { return mp_view_matrix; }
	inline D3DXMATRIX GetProjMatrix() { return mp_proj_matrix; }
	inline D3DXMATRIX GetViewprojMatrix() { return mp_viewproj_matrix; }	

	void SetPitchLimits(float min, float max);
	void SetRollLimits(float min, float max);

	void AddYaw(float a);
	void AddPitch(float a);
	void AddRoll(float a);
	float GetYaw();
	float GetPitch();
	float GetRoll();
	
	void MoveSidewards(float delta);
	void MoveUp(float delta);
	void MoveForward(float delta);

	void UpdateViewMatrix();
	void UpdateProjMatrix();
	void UpdateViewprojMatrix();
		
// public members //////////////////////////////////////////////////////////
public:
	D3DXVECTOR3	m_pos;
	D3DXVECTOR3	m_move;	
	D3DXVECTOR3	m_look;
	D3DXVECTOR3	m_up;
	D3DXVECTOR3	m_right;
	
// private functions ///////////////////////////////////////////////////////
private:

// private members /////////////////////////////////////////////////////////
//private:
public:
	_XCameraType	mp_Type;
	float			mp_fYaw;
	float			mp_fPitch, mp_fMinPitchLimit, mp_fMaxPitchLimit;
	float			mp_fRoll, mp_fMinRollLimit, mp_fMaxRollLimit;

	D3DXVECTOR3		mp_ypr;
	D3DXMATRIX		mp_view_matrix;
	D3DXMATRIX		mp_proj_matrix;
	D3DXMATRIX		mp_viewproj_matrix;

	float			mp_fAspect;
	float			mp_fFov;
	float			mp_fNearPlane;
	float			mp_fFarPlane;
};

#endif // __cplusplus
#endif // ___CAMERA_H__
