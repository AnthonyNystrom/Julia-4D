/*
Julia4D - Real-time quaternion-based fractal generator / renderer for Win32.
Written in C++, uses the Microsoft DirectX 9 library for graphics display.

Copyright (C) 2004 Shawn Halayka

email: shalayka@sasktel.net

This program is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details. 

You should have received a copy of the GNU General Public License along with this program;
if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#ifndef D3DPOINTSETRENDERER_H
#define D3DPOINTSETRENDERER_H


#include "defines.h"
#include "arc_ball.h"

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
using std::ostringstream;
using std::string;
using std::vector;
using std::ofstream;


#include <d3d9.h>
#include <dxerr9.h>
#include <d3dx9core.h>
#include <windows.h>
#include <windowsx.h>







class D3DPointSetRenderer
{
public:
	D3DPointSetRenderer(void);
	~D3DPointSetRenderer(void);
	bool Init(const HWND &src_target);
	void SetInterfaceHeaderText(const string src_interface_header);
	string GetInterfaceHeaderText(void);
	void DrawInterfaceText(const bool src_draw_interface_text);
	bool Play(void);
	bool Stop(void);

	bool SetPoints(POS_NORMAL_COLOUR_VERTEX* &src_vertices, const long unsigned int src_vertex_count, vector<long unsigned int> &src_z_slice_offsets);

	bool SetBoundingGrid(const float &x_min, const float &x_max, const float &y_min, const float &y_max, const float &z_min, const float &z_max);
	BOUNDING_GRID_SETTINGS GetBoundingGrid(void);



	bool Reset(const HWND &src_target);
	void HandleInputMessages(const HWND &src_hwnd, const UINT &src_umsg, const WPARAM &src_wparam, const LPARAM &src_lparam);
	string GetLastError(void);

	bool ToggleHelp();

	DWORD SetSnoozeFactor(const DWORD &src_snooze_factor);


	void IncreasePointSize(void)
	{
		if(point_size < max_point_size)
			point_size += 1.0f;

	}
	void DecreasePointSize(void)
	{
		if(point_size > 1.0f)
			point_size -= 1.0f;
	}

	void ResetTranslationAndRotation(void)
	{
		zoom_level = 3.8f; // was 6.0
		camera_z_pos = -5.0f;
		arcball.Init();
	}

	bool ToggleFog(void)
	{
		disable_fog = !disable_fog;

		return disable_fog;
	}

	bool ToggleInterfaceText(void)
	{
		draw_interface_text = !draw_interface_text;
		return draw_interface_text;
	}

	void SetFogColour(unsigned char r, unsigned char g, unsigned char b)
	{
		fog_colour = D3DCOLOR_XRGB(r, g, b);
	}

	void SetBackgroundColour(unsigned char r, unsigned char g, unsigned char b)
	{
		bg_colour = D3DCOLOR_XRGB(r, g, b);
	}

	void SetInterfaceTextColour(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		interface_text_colour = D3DCOLOR_ARGB(a, r, g, b);
	}

	unsigned char GetFogColourR(void)
	{
		return static_cast<unsigned char>(fog_colour >> 16);
	}

	unsigned char GetFogColourG(void)
	{
		return static_cast<unsigned char>(fog_colour >> 8);
	}

	unsigned char GetFogColourB(void)
	{
		return static_cast<unsigned char>(fog_colour);
	}

	unsigned char GetBackgroundColourR(void)
	{
		return static_cast<unsigned char>(bg_colour >> 16);
	}

	unsigned char GetBackgroundColourG(void)
	{
		return static_cast<unsigned char>(bg_colour >> 8);
	}

	unsigned char GetBackgroundColourB(void)
	{
		return static_cast<unsigned int>(bg_colour);
	}

	unsigned char GetInterfaceTextColourA(void)
	{
		return static_cast<unsigned char>(interface_text_colour >> 24);
	}

	unsigned char GetInterfaceTextColourR(void)
	{
		return static_cast<unsigned char>(interface_text_colour >> 16);
	}

	unsigned char GetInterfaceTextColourG(void)
	{
		return static_cast<unsigned char>(interface_text_colour >> 8);
	}

	unsigned char GetInterfaceTextColourB(void)
	{
		return static_cast<unsigned int>(interface_text_colour);
	}

	void SetDrawableRect(const RECT &src_drawable_rect)
	{
		drawable_rect = src_drawable_rect;

		target_width = drawable_rect.right - drawable_rect.left;
		target_height = drawable_rect.bottom - drawable_rect.top;

		arcball.SetParams(target_width, target_height, DEFAULT_ARCBALL_ROT_RADIUSf, DEFAULT_ARCBALL_TRANS_RADIUSf);

		target_perspective = static_cast<float>(target_width)/static_cast<float>(target_height);
	}

	void SetScreenEraseRect(const RECT &src_screen_erase_rect)
	{
		screen_erase_rect = src_screen_erase_rect;
	}

	void IncreaseZoom(void)
	{
		if(0x8000 & GetKeyState(0x50) || 0x8000 & GetKeyState(0x70)) // p or P
		{
			if(camera_z_pos < 0.0f)
			{
				camera_z_pos += 0.2f;
			}
		}
		else
		{
			if(zoom_level <= 2048.0f)
			{
				zoom_level += 0.2f;
			}
		}
	}

	void DecreaseZoom(void)
	{
		if(0x8000 & GetKeyState(0x50) || 0x8000 & GetKeyState(0x70)) // p or P
		{
			camera_z_pos -= 0.2f;
		}
		else
		{
			if(zoom_level >= 1.5f)
			{
				zoom_level -= 0.2f;
			}
		}
	}

	void ToggleBoundingGrid(void)
	{
		disable_bounding_grid = !disable_bounding_grid;
	}

	void ToggleAxisIndicators(void)
	{
		disable_axis_indicators = !disable_axis_indicators;
	}

	void ZSliceMinus(void)
	{
		if(0 != z_slice_offsets.size() && 1 < max_z_slice)
			max_z_slice--;
	}

	void ZSlicePlus(void)
	{
		if(max_z_slice < z_slice_offsets.size())
			max_z_slice++;
	}

	void ResetZSlicing(void)
	{
		max_z_slice = z_slice_offsets.size();
		render_single_slice = false;
	}

	void ToggleSingleZSlice(void)
	{
		render_single_slice = !render_single_slice;
	}

	bool IsFogEnabled(void)
	{
		return !disable_fog;
	}

	bool IsBoundingGridEnabled(void)
	{
		return !disable_bounding_grid;
	}

	bool IsSingleZSliceModeEnabled(void)
	{
		return render_single_slice;
	}

	bool IsInterfaceTextEnabled(void)
	{
		return draw_interface_text;
	}

	bool IsAxisIndicatorsEnabled(void)
	{
		return !disable_axis_indicators;
	}

	void RestoreDefaultColours(void)
	{
		bg_colour = 0xFF656565;
		fog_colour = 0xFF000000;
		interface_text_colour = 0xAFD5D0BA;
	}

	arc_ball GetArcBall(void)
	{
		return arcball;
	}

	void SetArcBall(const arc_ball &src_arcball)
	{
		arcball = src_arcball;
	}

	void SetGeneratingFlag(const bool &src_is_generating)
	{
		is_generating = src_is_generating;
	}

	void IncreaseOpacity(void)
	{
		if(voxel_opacity_level < 10)
		{
			voxel_opacity_level++;
			return;
		}

		if(voxel_opacity_level < 90)
		{
			voxel_opacity_level += 10;
			return;
		}

		if(voxel_opacity_level < 100)
		{
			voxel_opacity_level++;
			return;
		}
	}

	void DecreaseOpacity(void)
	{
		if(voxel_opacity_level > 90)
		{
			voxel_opacity_level--;
			return;
		}

		if(voxel_opacity_level > 10)
		{
			voxel_opacity_level -= 10;
			return;
		}

		if(voxel_opacity_level > 0)
		{
			voxel_opacity_level--;
			return;
		}
	}

	void AmbiencePlus(void)
	{
		if(gamma <= 95)
			gamma += 5;
	}

	void AmbienceMinus(void)
	{
		if(gamma >= 5)
			gamma -= 5;
	}

	unsigned char GetAmbience(void)
	{
		return gamma;
	}

	void SetAmbience(const unsigned char &src_ambience)
	{
//		ostringstream out;
//		out << src_ambience;
//		MessageBox(0, out.str().c_str(), APP_NAME, MB_OK);

//		if(!(src_ambience%5))
//		{
//			gamma = 0;
//		}
//		else
//		{
			gamma = src_ambience;

			if(gamma > 100)
				gamma = 100;
//		}
	}




	void UseLitShader(const bool &src_use_lit_shader)
	{
		use_lit_shader = src_use_lit_shader;
	}

	void RainbowColouring(const HWND &progressbar, const HWND &statustest);
	void DotProdColouring(const HWND &progressbar, const HWND &statustest);
	void CowColouring(const HWND &progressbar, const HWND &statustest);
	void FlatColouring(const DWORD &shader_number, const HWND &progressbar, const HWND &statustest);


protected:
	bool InitVertexShaders(void);
	bool InitUnlitVertexShader(void);
	bool InitLitVertexShader(void);
	bool EnableUnlitShader(void);
	bool EnableLitShader(void);




	bool InitVertexBuffer(void);

	void Shutdown(void);
	__forceinline DWORD Render(DWORD fps = 0) throw();
	static DWORD WINAPI RenderingThreadFunction(LPVOID D3DPointSetRenderer_obj) throw();

	long unsigned int vertex_count;
	POS_NORMAL_COLOUR_VERTEX *vertices;
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer;
	vector<long unsigned int> z_slice_offsets;
	size_t max_z_slice;
	bool render_single_slice;

	long unsigned int bounding_grid_vertex_count;
	POS_NORMAL_COLOUR_VERTEX *bounding_grid_vertices;
	bool disable_bounding_grid;


	bool disable_axis_indicators;


	HWND target;
	int target_width;
	int target_height;
	float target_perspective;

	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 device;

	LPDIRECT3DVERTEXDECLARATION9 unlit_vertex_shader_decl;
	LPDIRECT3DVERTEXSHADER9 unlit_vertex_shader;
	LPDIRECT3DVERTEXDECLARATION9 lit_vertex_shader_decl;
	LPDIRECT3DVERTEXSHADER9 lit_vertex_shader;


	bool uses_hardware_vertex_processing;
	bool pure_device;
	D3DCAPS9 device_caps;
	DWORD vertex_shader_v_major;
	DWORD vertex_shader_v_minor;
	DWORD pixel_shader_v_major;
	DWORD pixel_shader_v_minor;
	float max_point_size;

	HANDLE thread;
	HANDLE stop_event;

	arc_ball arcball;
	float zoom_level;
	bool disable_fog;
	float point_size;

	bool playing;

	bool draw_interface_text;

	string interface_header;

	LPD3DXFONT interface_font;

	ostringstream last_error;

	float bounding_grid_x_min;
	float bounding_grid_x_max;
	float bounding_grid_y_min;
	float bounding_grid_y_max;
	float bounding_grid_z_min;
	float bounding_grid_z_max;

	float camera_z_pos;

	bool show_help;

	DWORD snooze_factor;

	DWORD bg_colour;
	DWORD fog_colour;
	DWORD interface_text_colour;

	RECT drawable_rect;
	RECT screen_erase_rect;

	string adapter_info;

	bool is_generating;

	bool reference_rasterizer;

	unsigned char voxel_opacity_level;

	bool draw_with_user_ptr;

	bool use_lit_shader;

	D3DXVECTOR4 Specular;

	unsigned char gamma;
};


#endif