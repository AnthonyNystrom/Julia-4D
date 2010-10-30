#ifndef JULIA4D2_ARCBALL_H
#define JULIA4D2_ARCBALL_H


#include <d3dx9math.h>


/*
Something of a redo of the D3DX arcball class, basically to abstract it from the
WM_ mouse related code (I thought I was bad at abusing the abstraction concept of OOP).
*/



//-----------------------------------------------------------------------------
// Name: D3DXQuaternionUnitAxisToUnitAxis2
// Desc: Axis to axis quaternion double angle (no normalization)
//       Takes two points on unit sphere an angle THETA apart, returns
//       quaternion that represents a rotation around cross product by 2*THETA.
//-----------------------------------------------------------------------------
inline D3DXQUATERNION* WINAPI D3DXQuaternionUnitAxisToUnitAxis2
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo)
{
    D3DXVECTOR3 vAxis;
    D3DXVec3Cross(&vAxis, pvFrom, pvTo);    // proportional to sin(theta)
    pOut->x = vAxis.x;
    pOut->y = vAxis.y;
    pOut->z = vAxis.z;
    pOut->w = D3DXVec3Dot( pvFrom, pvTo );
    return pOut;
}




//-----------------------------------------------------------------------------
// Name: D3DXQuaternionAxisToAxis
// Desc: Axis to axis quaternion 
//       Takes two points on unit sphere an angle THETA apart, returns
//       quaternion that represents a rotation around cross product by theta.
//-----------------------------------------------------------------------------
inline D3DXQUATERNION* WINAPI D3DXQuaternionAxisToAxis
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo)
{
    D3DXVECTOR3 vA, vB;
    D3DXVec3Normalize(&vA, pvFrom);
    D3DXVec3Normalize(&vB, pvTo);
    D3DXVECTOR3 vHalf(vA + vB);
    D3DXVec3Normalize(&vHalf, &vHalf);
    return D3DXQuaternionUnitAxisToUnitAxis2(pOut, &vA, &vHalf);
}


class arc_ball
{
public:

	arc_ball(void)
	{
		rotation_radius = DEFAULT_ARCBALL_ROT_RADIUSf;
		translation_radius = DEFAULT_ARCBALL_TRANS_RADIUSf;
		Init();
	}

	void Init(void)
	{
		D3DXQuaternionIdentity( &down_quat );
		D3DXQuaternionIdentity( &now_quat );
		D3DXMatrixIdentity( &rotation_matrix );
		D3DXMatrixIdentity( &translation_matrix );
		D3DXMatrixIdentity( &translation_delta_matrix );
		rotating = false;
	}

	void SetParams(int src_window_width, int src_window_height, float src_rotation_radius = DEFAULT_ARCBALL_ROT_RADIUSf, float src_translation_radius = DEFAULT_ARCBALL_TRANS_RADIUSf)
	{
		window_width = src_window_width;
		window_height = src_window_height;
		rotation_radius = src_rotation_radius;
		translation_radius = src_translation_radius;
	}

    D3DXMATRIXA16* GetRotationMatrix()
	{
		return &rotation_matrix;
	}

    D3DXMATRIXA16* GetTranslationMatrix()
	{
		return &translation_matrix;
	}




	D3DXVECTOR3 ScreenToVector( int sx, int sy )
	{
		// Scale to screen
		float x = -(sx - window_width/2)  / (rotation_radius*window_width/2);
		float y =  (sy - window_height/2) / (rotation_radius*window_height/2);

		float z   = 0.0f;
		float mag = x*x + y*y;

		if( mag > 1.0f )
		{
			float scale = 1.0f/sqrtf(mag);
			x *= scale;
			y *= scale;
		}
		else
			z = sqrtf( 1.0f - mag );

		// Return vector
		return D3DXVECTOR3( x, y, z );
	}

	void StartRotate(unsigned short src_mouse_x_pos, unsigned short src_mouse_y_pos)
	{
		// Start drag mode
		rotating = true;
		down_vector = ScreenToVector(src_mouse_x_pos, src_mouse_y_pos);
		down_quat = now_quat;
	}

	void UpdateCursorPos(short int src_cursor_x_pos, short int src_cursor_y_pos, short int src_cursor_x_delta, short int src_cursor_y_delta)
	{
		if(true == rotating)
		{
			// recompute m_qNow
			D3DXVECTOR3 curr_vector = ScreenToVector( src_cursor_x_pos, src_cursor_y_pos );
			D3DXQUATERNION axis_to_axis_quat;
			D3DXQuaternionAxisToAxis(&axis_to_axis_quat, &down_vector, &curr_vector);
			now_quat = down_quat;
			now_quat *= axis_to_axis_quat;
			D3DXMatrixRotationQuaternion(&rotation_matrix, &now_quat);
		}
		else
		{
			float fDeltaX = src_cursor_x_delta * translation_radius / window_width;
			float fDeltaY = src_cursor_y_delta * translation_radius / window_height;

			D3DXMatrixTranslation(&translation_delta_matrix, 2*fDeltaX, 2*fDeltaY, 0.0f);
			D3DXMatrixMultiply(&translation_matrix, &translation_matrix, &translation_delta_matrix);
		}
	}

	void StopRotate(void)
	{
		rotating = false;
	}

	bool IsRotating(void)
	{
		return rotating;
	}

public:
	int window_width;
	int window_height;
	float rotation_radius;
	float translation_radius; // ArcBall's rotation_radius for translating the target
	bool rotating;               // Whether user is dragging arcball

	D3DXQUATERNION down_quat;               // Quaternion before button down
	D3DXQUATERNION now_quat;                // Composite quaternion for current drag
	D3DXMATRIXA16 rotation_matrix;         // Matrix for arcball's orientation
	D3DXMATRIXA16 translation_matrix;      // Matrix for arcball's position
	D3DXMATRIXA16 translation_delta_matrix; // Matrix for arcball's position

	D3DXVECTOR3 down_vector;         // Button down vector

};


#endif