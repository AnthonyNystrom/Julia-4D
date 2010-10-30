#include "stdafx.h"

#include "d3dpointsetrenderer.h"
#include "timer.h"
#include "resource.h"
#include "axis_indicators.h"
#include "utilities.h"



#include <iostream>
using std::endl;








D3DPointSetRenderer::D3DPointSetRenderer(void)
{
	vertex_count = 0;
	vertices = 0;
	max_z_slice = 0;
	render_single_slice = false;

	bounding_grid_vertex_count = 0;
	bounding_grid_vertices = 0;
	disable_bounding_grid = false;

	disable_axis_indicators = false;

	target = 0;
	target_width = 0;
	target_height = 0;
	target_perspective = 0;

	thread = 0;
	stop_event = 0;

	zoom_level = 3.8f;
	disable_fog = false;
	point_size = 1.0f;

	playing = false;

	draw_interface_text = true;
	interface_header = "Direct 3D Point Renderer";

	d3d = 0;
	device = 0;
	vertex_buffer = 0;

	unlit_vertex_shader_decl = 0;
	unlit_vertex_shader = 0;
	lit_vertex_shader_decl = 0;
	lit_vertex_shader = 0;



	vertex_shader_v_major = 0;
	vertex_shader_v_minor = 0;
	pixel_shader_v_major = 0;
	pixel_shader_v_minor = 0;
	uses_hardware_vertex_processing = false;
	pure_device = false;

	last_error.str("");

	max_point_size = 1.0f;

	bounding_grid_x_min = 0.0f;
	bounding_grid_x_max = 0.0f;
	bounding_grid_y_min = 0.0f;
	bounding_grid_y_max = 0.0f;
	bounding_grid_z_min = 0.0f;
	bounding_grid_z_max = 0.0f;

	camera_z_pos = -5.0f;

	snooze_factor = 0;

	bg_colour = 0xFF656565;
	fog_colour = 0xFF000000;
	interface_text_colour = 0xAFD5D0BA; // 7FBAD3D5

	drawable_rect.top = 0;
	drawable_rect.left = 0;
	drawable_rect.bottom = 0;
	drawable_rect.right = 0;

	is_generating = false;

	reference_rasterizer = false;

	// these were in the init instead of the constructor
	voxel_opacity_level = 100;
	draw_with_user_ptr = false;
	use_lit_shader = false;
	Specular = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 10.0f);

	gamma = 0;
}

D3DPointSetRenderer::~D3DPointSetRenderer(void)
{
	Shutdown();

	if(0 != vertices)
	{
		delete [] vertices;
		vertices = 0;
	}

	if(0 != bounding_grid_vertices)
	{
		delete [] bounding_grid_vertices;
		bounding_grid_vertices = 0;
	}
}



bool D3DPointSetRenderer::Init(const HWND &src_target)
{
	HRESULT d3d_ret_code = 0;

	Shutdown();

	target = src_target;

	// Create the D3D object.
	if(0 == (d3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		last_error.str("Init() - Direct3DCreate9() failure\nThis application requires Microsoft DirectX 9.0c, or newer.");
		return false;
	}

	// Get the current desktop display mode, so we can set up a back
	// buffer of the same format
	D3DDISPLAYMODE d3ddm;
	if(FAILED(d3d_ret_code = d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		last_error.str("");
		last_error << "Init() - GetAdapterDisplayMode() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return false;
	}


	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferCount = 1;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // leave enabled, causes framerate drop in debug only
	d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; // try antialiasing
	d3dpp.BackBufferFormat = d3ddm.Format; //D3DFMT_R5G6B5;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//D3DFMT_D16;


	// try to find a device that supports antialiasing first, then one that doesn't
	bool found_suitable_device = false;

	D3DDEVTYPE device_type = D3DDEVTYPE_HAL;

	// try hardware vertex processing, pure device
	if(SUCCEEDED(d3d_ret_code = d3d->CreateDevice(D3DADAPTER_DEFAULT, device_type, target, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_PUREDEVICE|D3DCREATE_MULTITHREADED, &d3dpp, &device)))
	{
		if(FAILED(d3d_ret_code = device->GetDeviceCaps(&device_caps)))
		{
			last_error.str("");
			last_error << "Init() - IDirect3DDevice9::GetDeviceCaps() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return false;
		}

		// does it support vs 1.1 or greater in the hardware?
		if(device_caps.VertexShaderVersion < D3DVS_VERSION(1,1))
		{
			if(0 != device)
			{
				device->Release();
				device = 0;
			}
		}
		else
		{
			found_suitable_device = true;
			uses_hardware_vertex_processing = true;
			pure_device = true;
		}
	}


	// if necessary, try hardware vertex processing but not pure device
	if(false == found_suitable_device)
	{
		if(SUCCEEDED(d3d_ret_code = d3d->CreateDevice(D3DADAPTER_DEFAULT, device_type, target, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &device)))
		{
			if(FAILED(d3d_ret_code = device->GetDeviceCaps(&device_caps)))
			{
				last_error.str("");
				last_error << "Init() - IDirect3DDevice9::GetDeviceCaps() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return false;
			}

			// does it support vs 1.1 or greater in the hardware?
			if(device_caps.VertexShaderVersion < D3DVS_VERSION(1,1))
			{
				if(0 != device)
				{
					device->Release();
					device = 0;
				}
			}
			else
			{
				found_suitable_device = true;
				uses_hardware_vertex_processing = true;
				pure_device = false;
			}
		}
	}


	// if necessary, try software vertex processing
	if(false == found_suitable_device)
	{
		if(SUCCEEDED(d3d_ret_code = d3d->CreateDevice(D3DADAPTER_DEFAULT, device_type, target, D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &device)))
		{
			if(FAILED(d3d_ret_code = device->GetDeviceCaps(&device_caps)))
			{
				last_error.str("");
				last_error << "Init() - IDirect3DDevice9::GetDeviceCaps() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return false;
			}

			// does it support vs 1.1 or greater in the hardware?
			if(device_caps.VertexShaderVersion < D3DVS_VERSION(1,1))
			{
				if(0 != device)
				{
					device->Release();
					device = 0;
				}
			}
			else
			{
				found_suitable_device = true;
				uses_hardware_vertex_processing = false;
				pure_device = false;
			}
		}
	}


	// if we didn't find a device, try turning off antialiasing and start over
	if(false == found_suitable_device)
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	}

	if(false == found_suitable_device)
	{
		// try hardware vertex processing, pure device
		if(SUCCEEDED(d3d_ret_code = d3d->CreateDevice(D3DADAPTER_DEFAULT, device_type, target, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_PUREDEVICE|D3DCREATE_MULTITHREADED, &d3dpp, &device)))
		{
			if(FAILED(d3d_ret_code = device->GetDeviceCaps(&device_caps)))
			{
				last_error.str("");
				last_error << "Init() - IDirect3DDevice9::GetDeviceCaps() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return false;
			}

			// does it support vs 1.1 or greater in the hardware?
			if(device_caps.VertexShaderVersion < D3DVS_VERSION(1,1))
			{
				if(0 != device)
				{
					device->Release();
					device = 0;
				}
			}
			else
			{
				found_suitable_device = true;
				uses_hardware_vertex_processing = true;
				pure_device = true;
			}
		}
	}

	// if necessary, try hardware vertex processing but not pure device
	if(false == found_suitable_device)
	{
		if(SUCCEEDED(d3d_ret_code = d3d->CreateDevice(D3DADAPTER_DEFAULT, device_type, target, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &device)))
		{
			if(FAILED(d3d_ret_code = device->GetDeviceCaps(&device_caps)))
			{
				last_error.str("");
				last_error << "Init() - IDirect3DDevice9::GetDeviceCaps() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return false;
			}

			// does it support vs 1.1 or greater in the hardware?
			if(device_caps.VertexShaderVersion < D3DVS_VERSION(1,1))
			{
				if(0 != device)
				{
					device->Release();
					device = 0;
				}
			}
			else
			{
				found_suitable_device = true;
				uses_hardware_vertex_processing = true;
				pure_device = false;
			}
		}
	}

	// if necessary, try software vertex processing
	if(false == found_suitable_device)
	{
		if(SUCCEEDED(d3d_ret_code = d3d->CreateDevice(D3DADAPTER_DEFAULT, device_type, target, D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &device)))
		{
			if(FAILED(d3d_ret_code = device->GetDeviceCaps(&device_caps)))
			{
				last_error.str("");
				last_error << "Init() - IDirect3DDevice9::GetDeviceCaps() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return false;
			}

			// does it support vs 1.1 or greater in the hardware?
			if(device_caps.VertexShaderVersion < D3DVS_VERSION(1,1))
			{
				if(0 != device)
				{
					device->Release();
					device = 0;
				}
			}
			else
			{
				found_suitable_device = true;
				uses_hardware_vertex_processing = false;
				pure_device = false;
			}
		}
	}



	if(false == found_suitable_device)
	{
		device_type = D3DDEVTYPE_REF;

		if(SUCCEEDED(d3d_ret_code = d3d->CreateDevice(D3DADAPTER_DEFAULT, device_type, target, D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &device)))
		{
			if(FAILED(d3d_ret_code = device->GetDeviceCaps(&device_caps)))
			{
				last_error.str("");
				last_error << "Init() - IDirect3DDevice9::GetDeviceCaps() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return false;
			}

			// does it support vs 1.1 or greater? well, technically it better! :)
			if(device_caps.VertexShaderVersion < D3DVS_VERSION(1,1))
			{
				if(0 != device)
				{
					device->Release();
					device = 0;
				}
			}
			else
			{
				found_suitable_device = true;
				uses_hardware_vertex_processing = false;
				pure_device = false;
				reference_rasterizer = true;
			}
		}
	}


	// if nothing found, abort
	if(false == found_suitable_device)
	{
		last_error.str("Init() - No Direct3D graphics device found.\n\nTry rebooting, or reinstalling DirectX 9.0c or newer.");
		return false;
	}


	vertex_shader_v_major = (device_caps.VertexShaderVersion >> 8) & 255;
	vertex_shader_v_minor =  device_caps.VertexShaderVersion & 255;

	pixel_shader_v_major = (device_caps.PixelShaderVersion >> 8) & 255;
	pixel_shader_v_minor =  device_caps.PixelShaderVersion & 255;


	// we should have found at least one graphics device by now :)
	if(d3d->GetAdapterCount() > 0)
	{
		D3DADAPTER_IDENTIFIER9 d3dId;
		ZeroMemory(&d3dId, sizeof(D3DADAPTER_IDENTIFIER9));
		d3d->GetAdapterIdentifier(0, NULL, &d3dId);

		ostringstream out;

		out << "\n  " << d3dId.Description;

		out << "\n  v";
		out << HIWORD(d3dId.DriverVersion.HighPart);
		out << "." << LOWORD(d3dId.DriverVersion.HighPart);
		out << "." << HIWORD(d3dId.DriverVersion.LowPart);
		out << "." << LOWORD(d3dId.DriverVersion.LowPart);

		out << "\n  vs." << vertex_shader_v_major << "." << vertex_shader_v_minor << " ps." << pixel_shader_v_major << "." << pixel_shader_v_minor;

		if(true == reference_rasterizer)
			out << " REF SW VP";
		else if(true == pure_device)
			out << " Pure HAL HW VP";
		else if(true == uses_hardware_vertex_processing)
			out << " HAL HW VP";
		else
			out << " HAL SW VP";

		adapter_info = out.str();
	}




	if(!InitVertexShaders())
		return false;





	int nPointSize = 8;
	HDC hDC = GetDC(NULL);
	int nHeight = -(MulDiv(nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72 ));
	ReleaseDC(NULL, hDC);

	if(FAILED(d3d_ret_code = D3DXCreateFont(device, nHeight, 0, FW_NORMAL, 0, FALSE, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), 
		&interface_font )))
	{
		last_error.str("");
		last_error.str("Init() - Could not create interface font.");
		return false;
	}




	target_width = drawable_rect.right - drawable_rect.left;
	target_height = drawable_rect.bottom - drawable_rect.top;

	target_perspective = static_cast<float>(target_width)/static_cast<float>(target_height);

	arcball.SetParams(target_width, target_height, DEFAULT_ARCBALL_ROT_RADIUSf, DEFAULT_ARCBALL_TRANS_RADIUSf);

	max_point_size = device_caps.MaxPointSize;

	show_help = true;



	return true;
}

void D3DPointSetRenderer::SetInterfaceHeaderText(const string src_interface_header)
{
	interface_header = src_interface_header;
}

string D3DPointSetRenderer::GetInterfaceHeaderText(void)
{
	return interface_header;
}

void D3DPointSetRenderer::DrawInterfaceText(const bool src_draw_interface_text)
{
	draw_interface_text = src_draw_interface_text;
}

bool D3DPointSetRenderer::ToggleHelp(void)
{
	return show_help = !show_help;

}

bool D3DPointSetRenderer::Play(void)
{
	if(true == playing)
		return true;

	if(0 == d3d)
	{
		last_error.str("Play() - renderer has not been previously initialized (component: Direct 3D)");
		return false;
	}

	if(0 == device)
	{
		last_error.str("Play() - renderer has not been previously initialized (component: device)");
		return false;
	}

	if(0 == (stop_event = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		last_error.str("");
		last_error << "Play() - CreateEvent() failure. Error code: " << GetLastError();
		return false;
	}

	// create thread
	thread = CreateThread(NULL, 0, D3DPointSetRenderer::RenderingThreadFunction, this, 0, NULL);

	if(0 == thread)
	{
		last_error.str("");
		last_error << "Play() - CreateThread() failure. Error code: " << GetLastError();
		return false;
	}

	playing = true;

	return true;
}

bool D3DPointSetRenderer::Stop(void)
{
	if(false == playing)
		return true;

	if(0 == stop_event)
	{
		last_error.str("Stop() - stop event is invalid");
		return false;
	}
	else
	{
		SetEvent(stop_event);
	}

	if(0 == thread)
	{
		last_error.str("Stop() - renderer thread is invalid");
		return false;
	}
	else
	{
		WaitForSingleObject(thread, INFINITE);
	}

	CloseHandle(thread);
	CloseHandle(stop_event);

	playing = false;

	return true;
}



bool D3DPointSetRenderer::SetPoints(POS_NORMAL_COLOUR_VERTEX* &src_vertices, const long unsigned int src_vertex_count, vector<long unsigned int> &src_z_slice_offsets)
{
	Stop();

	// if vertex count is not 0 but the array is null, give error (sorta checks for out of memory conditions when the set was being generated)
	if(0 != src_vertex_count && 0 == src_vertices)
	{
		last_error.str("SetPoints() - source vertex array pointer is invalid");
		return false;
	}

	// clean up the vertex array if the count is 0 (just in case)
	if(0 == src_vertex_count)
	{
		if(0 != src_vertices)
		{
			delete [] src_vertices;
			src_vertices = 0;
		}
	}

	if(0 != vertices)
	{
		delete [] vertices;
		vertices = 0;
	}


	vertices = src_vertices;
	vertex_count = src_vertex_count;

	/*
	max_z_slice = z_slice_offsets.size();
	z_slice_offsets = src_z_slice_offsets;
	*/

	// if the old z slice was set to max, or our new z slice set size is 0...
	// else let's try to make the new z slice something close to the old z slice
	if(max_z_slice == z_slice_offsets.size() || 0 == src_z_slice_offsets.size())
	{
		max_z_slice = src_z_slice_offsets.size();
	}
	else
	{
		double old_z_slice_percent = (static_cast<double>(max_z_slice) / static_cast<double>(z_slice_offsets.size())) * 100.0;

		double potential_new_offset = 100 / static_cast<double>(src_z_slice_offsets.size());

		for(size_t i = 1; i <= src_z_slice_offsets.size(); i++)
		{
			double new_z_slice_percent = (static_cast<double>(i) / static_cast<double>(src_z_slice_offsets.size())) * 100.0;

			bool new_is_even = (src_z_slice_offsets.size() % 2 == 0);
			bool old_is_even = (z_slice_offsets.size() % 2 == 0);

			if(z_slice_offsets.size() > src_z_slice_offsets.size())
				if(!new_is_even && !old_is_even || (new_is_even != old_is_even))
					new_z_slice_percent += potential_new_offset;

			if(new_z_slice_percent >= old_z_slice_percent)
			{
				max_z_slice = i;
				break;
			}
		}
	}

	z_slice_offsets = src_z_slice_offsets;

	return InitVertexBuffer();
}



bool D3DPointSetRenderer::SetBoundingGrid(const float &x_min, const float &x_max, const float &y_min, const float &y_max, const float &z_min, const float &z_max)
{
	if(x_min >= x_max)
	{
		last_error.str("SetBoundingGrid() - X min must be smaller than X max.");
		return false;
	}

	if(y_min >= y_max)
	{
		last_error.str("SetBoundingGrid() - Y min must be smaller than Y max.");
		return false;
	}

	if(z_min >= z_max)
	{
		last_error.str("SetBoundingGrid() - Z min must be smaller than Z max.");
		return false;
	}

	bounding_grid_x_min = x_min;
	bounding_grid_x_max = x_max;
	bounding_grid_y_min = y_min;
	bounding_grid_y_max = y_max;
	bounding_grid_z_min = z_min;
	bounding_grid_z_max = z_max;


	short unsigned int bounding_cube_vertex_count = 24;
	short unsigned int origin_plane_vertex_count = 24;

	bounding_grid_vertex_count = bounding_cube_vertex_count + origin_plane_vertex_count;

	// if bounding_lines is null, allocate memory for it
	if(0 == bounding_grid_vertices)
	{
		bounding_grid_vertices = new(std::nothrow) POS_NORMAL_COLOUR_VERTEX[bounding_grid_vertex_count];

		if(0 == bounding_grid_vertices)
		{
			bounding_grid_vertex_count = 0;
			last_error.str("SetBoundingGrid() - Could not create bounding grid. Memory allocation error.");
			return false;
		}
	}

#define bounding_cube_color 0xFF77818B;
#define origin_plane_color 0xFF6F6F6D;

	POS_NORMAL_COLOUR_VERTEX a[4];
	POS_NORMAL_COLOUR_VERTEX c[4];

	a[0].x = bounding_grid_x_min;
	a[0].y = bounding_grid_y_max;
	a[0].z = bounding_grid_z_min;

	a[1].x = bounding_grid_x_min;
	a[1].y = bounding_grid_y_max;
	a[1].z = bounding_grid_z_max;

	a[2].x = bounding_grid_x_max;
	a[2].y = bounding_grid_y_max;
	a[2].z = bounding_grid_z_max;

	a[3].x = bounding_grid_x_max;
	a[3].y = bounding_grid_y_max;
	a[3].z = bounding_grid_z_min;

	c[0].x = bounding_grid_x_min;
	c[0].y = bounding_grid_y_min;
	c[0].z = bounding_grid_z_min;

	c[1].x = bounding_grid_x_min;
	c[1].y = bounding_grid_y_min;
	c[1].z = bounding_grid_z_max;

	c[2].x = bounding_grid_x_max;
	c[2].y = bounding_grid_y_min;
	c[2].z = bounding_grid_z_max;

	c[3].x = bounding_grid_x_max;
	c[3].y = bounding_grid_y_min;
	c[3].z = bounding_grid_z_min;


	//0-23 bounding cube

	//top
	bounding_grid_vertices[0] = a[0];
	bounding_grid_vertices[1] = a[1];

	bounding_grid_vertices[2] = a[1];
	bounding_grid_vertices[3] = a[2];

	bounding_grid_vertices[4] = a[2];
	bounding_grid_vertices[5] = a[3];

	bounding_grid_vertices[6] = a[3];
	bounding_grid_vertices[7] = a[0];

	//bottom
	bounding_grid_vertices[8] = c[0];
	bounding_grid_vertices[9] = c[1];

	bounding_grid_vertices[10] = c[1];
	bounding_grid_vertices[11] = c[2];

	bounding_grid_vertices[12] = c[2];
	bounding_grid_vertices[13] = c[3];

	bounding_grid_vertices[14] = c[3];
	bounding_grid_vertices[15] = c[0];

	//sides
	bounding_grid_vertices[16] = a[0];
	bounding_grid_vertices[17] = c[0];

	bounding_grid_vertices[18] = a[1];
	bounding_grid_vertices[19] = c[1];

	bounding_grid_vertices[20] = a[2];
	bounding_grid_vertices[21] = c[2];

	bounding_grid_vertices[22] = a[3];
	bounding_grid_vertices[23] = c[3];

	for(short unsigned int i = 0; i < bounding_cube_vertex_count; i++)
		bounding_grid_vertices[i].colour = bounding_cube_color;

	//24-47 origin planes
	float origin_x_max;
	float origin_x_min;
	float origin_y_max;
	float origin_y_min;
	float origin_z_max;
	float origin_z_min;

	if(bounding_grid_x_max < 0.0f)
		origin_x_max = 0.0f;
	else
		origin_x_max = bounding_grid_x_max;

	if(bounding_grid_x_min > 0.0f)
		origin_x_min = 0.0f;
	else
		origin_x_min = bounding_grid_x_min;

	if(bounding_grid_y_max < 0.0f)
		origin_y_max = 0.0f;
	else
		origin_y_max = bounding_grid_y_max;

	if(bounding_grid_y_min > 0.0f)
		origin_y_min = 0.0f;
	else
		origin_y_min = bounding_grid_y_min;

	if(bounding_grid_z_max < 0.0f)
		origin_z_max = 0.0f;
	else
		origin_z_max = bounding_grid_z_max;

	if(bounding_grid_z_min > 0.0f)
		origin_z_min = 0.0f;
	else
		origin_z_min = bounding_grid_z_min;

	POS_NORMAL_COLOUR_VERTEX xz[4];
	POS_NORMAL_COLOUR_VERTEX xy[4];
	POS_NORMAL_COLOUR_VERTEX zy[4];

	xz[0].x = origin_x_min;
	xz[0].y = 0.0f;
	xz[0].z = origin_z_min;

	xz[1].x = origin_x_min;
	xz[1].y = 0.0f;
	xz[1].z = origin_z_max;

	xz[2].x = origin_x_max;
	xz[2].y = 0.0f;
	xz[2].z = origin_z_max;

	xz[3].x = origin_x_max;
	xz[3].y = 0.0f;
	xz[3].z = origin_z_min;


	xy[0].x = origin_x_min;
	xy[0].y = origin_y_min;
	xy[0].z = 0.0f;

	xy[1].x = origin_x_min;
	xy[1].y = origin_y_max;
	xy[1].z = 0.0f;

	xy[2].x = origin_x_max;
	xy[2].y = origin_y_max;
	xy[2].z = 0.0f;

	xy[3].x = origin_x_max;
	xy[3].y = origin_y_min;
	xy[3].z = 0.0f;


	zy[0].x = 0.0f;
	zy[0].y = origin_y_min;
	zy[0].z = origin_z_min;

	zy[1].x = 0.0f;
	zy[1].y = origin_y_max;
	zy[1].z = origin_z_min;

	zy[2].x = 0.0f;
	zy[2].y = origin_y_max;
	zy[2].z = origin_z_max;

	zy[3].x = 0.0f;
	zy[3].y = origin_y_min;
	zy[3].z = origin_z_max;


	//XZ
	bounding_grid_vertices[24] = xz[0];
	bounding_grid_vertices[25] = xz[1];

	bounding_grid_vertices[26] = xz[1];
	bounding_grid_vertices[27] = xz[2];

	bounding_grid_vertices[28] = xz[2];
	bounding_grid_vertices[29] = xz[3];

	bounding_grid_vertices[30] = xz[3];
	bounding_grid_vertices[31] = xz[0];


	//XY
	bounding_grid_vertices[32] = xy[0];
	bounding_grid_vertices[33] = xy[1];

	bounding_grid_vertices[34] = xy[1];
	bounding_grid_vertices[35] = xy[2];

	bounding_grid_vertices[36] = xy[2];
	bounding_grid_vertices[37] = xy[3];

	bounding_grid_vertices[38] = xy[3];
	bounding_grid_vertices[39] = xy[0];

	//ZY
	bounding_grid_vertices[40] = zy[0];
	bounding_grid_vertices[41] = zy[1];

	bounding_grid_vertices[42] = zy[1];
	bounding_grid_vertices[43] = zy[2];

	bounding_grid_vertices[44] = zy[2];
	bounding_grid_vertices[45] = zy[3];

	bounding_grid_vertices[46] = zy[3];
	bounding_grid_vertices[47] = zy[0];


	for(short unsigned int i = bounding_cube_vertex_count; i < bounding_cube_vertex_count + origin_plane_vertex_count; i++)
		bounding_grid_vertices[i].colour = origin_plane_color;

	float offset = 0.025f;

	for(long unsigned int i = 0; i < p_x_axis_size; i++)
	{
		p_x_axis_vertices_copy[i].x = p_x_axis_vertices[i].x;
		p_x_axis_vertices_copy[i].y = p_x_axis_vertices[i].y;
		p_x_axis_vertices_copy[i].z = p_x_axis_vertices[i].z;
		p_x_axis_vertices_copy[i].colour = p_x_axis_vertices[i].colour;

		p_x_axis_vertices_copy[i].x += x_max + offset;
		p_x_axis_vertices_copy[i].y += (y_min + y_max) / 2.0f;
		p_x_axis_vertices_copy[i].z += (z_min + z_max) / 2.0f;

	}

	for(long unsigned int i = 0; i < p_y_axis_size; i++)
	{
		p_y_axis_vertices_copy[i].x = p_y_axis_vertices[i].x;
		p_y_axis_vertices_copy[i].y = p_y_axis_vertices[i].y;
		p_y_axis_vertices_copy[i].z = p_y_axis_vertices[i].z;
		p_y_axis_vertices_copy[i].colour = p_y_axis_vertices[i].colour;

		p_y_axis_vertices_copy[i].x += (x_min + x_max) / 2.0f;
		p_y_axis_vertices_copy[i].y += y_max + offset;
		p_y_axis_vertices_copy[i].z += (z_min + z_max) / 2.0f;
	}

	for(long unsigned int i = 0; i < p_x_axis_size; i++)
	{
		p_z_axis_vertices_copy[i].x = p_z_axis_vertices[i].x;
		p_z_axis_vertices_copy[i].y = p_z_axis_vertices[i].y;
		p_z_axis_vertices_copy[i].z = p_z_axis_vertices[i].z;
		p_z_axis_vertices_copy[i].colour = p_z_axis_vertices[i].colour;

		p_z_axis_vertices_copy[i].x += (x_min + x_max) / 2.0f;
		p_z_axis_vertices_copy[i].y += (y_min + y_max) / 2.0f;
		p_z_axis_vertices_copy[i].z += z_max + offset;
	}


	axis_line_vertices[0].x = (x_min + x_max) / 2.0f;
	axis_line_vertices[0].y = (y_min + y_max) / 2.0f;
	axis_line_vertices[0].z = (z_min + z_max) / 2.0f;

	axis_line_vertices[1].x = x_max;
	axis_line_vertices[1].y = (y_min + y_max) / 2.0f;
	axis_line_vertices[1].z = (z_min + z_max) / 2.0f;

	axis_line_vertices[2].x = (x_min + x_max) / 2.0f;
	axis_line_vertices[2].y = (y_min + y_max) / 2.0f;
	axis_line_vertices[2].z = (z_min + z_max) / 2.0f;

	axis_line_vertices[3].x = (x_min + x_max) / 2.0f;
	axis_line_vertices[3].y = y_max;
	axis_line_vertices[3].z = (z_min + z_max) / 2.0f;

	axis_line_vertices[4].x = (x_min + x_max) / 2.0f;
	axis_line_vertices[4].y = (y_min + y_max) / 2.0f;
	axis_line_vertices[4].z = (z_min + z_max) / 2.0f;

	axis_line_vertices[5].x = (x_min + x_max) / 2.0f;
	axis_line_vertices[5].y = (y_min + y_max) / 2.0f;
	axis_line_vertices[5].z = z_max;






	return true;
}


BOUNDING_GRID_SETTINGS D3DPointSetRenderer::GetBoundingGrid(void)
{
	BOUNDING_GRID_SETTINGS bgs;

	bgs.x_min = bounding_grid_x_min;
	bgs.x_max = bounding_grid_x_max;
	bgs.y_min = bounding_grid_y_min;
	bgs.y_max = bounding_grid_y_max;
	bgs.z_min = bounding_grid_z_min;
	bgs.z_max = bounding_grid_z_max;

	return bgs;
}













bool D3DPointSetRenderer::Reset(const HWND &src_target)
{
	if(!Init(src_target))
		return false;

	if(vertex_count > 0)
		if(!InitVertexBuffer())
			return false;

	return true;
}



void D3DPointSetRenderer::RainbowColouring(const HWND &progressbar, const HWND &statustext)
{
	if(vertices == 0 || vertex_count == 0)
		return;

	bool was_playing = playing;
	Stop();

	ColourVertices(progressbar, statustext, vertices, vertex_count, false);

	::SetWindowText(statustext, "Filling vertex buffer...");
	::UpdateWindow(statustext);
	InitVertexBuffer();
	
	Specular = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 10.0f);

	if(was_playing)
		Play();

	::SetWindowText(statustext, "Done...");
	::UpdateWindow(statustext);
}


void D3DPointSetRenderer::DotProdColouring(const HWND &progressbar, const HWND &statustext)
{
	if(vertices == 0 || vertex_count == 0)
		return;

	bool was_playing = playing;
	Stop();

	DotProdColourVertices(progressbar, statustext, vertices, vertex_count, true);

	::SetWindowText(statustext, "Filling vertex buffer...");
	::UpdateWindow(statustext);
	InitVertexBuffer();
	
	Specular = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 10.0f);

	if(was_playing)
		Play();

	::SetWindowText(statustext, "Done...");
	::UpdateWindow(statustext);
}

void D3DPointSetRenderer::CowColouring(const HWND &progressbar, const HWND &statustext)
{
	if(vertices == 0 || vertex_count == 0)
		return;

	bool was_playing = playing;
	Stop();

	CowColourVertices(progressbar, statustext, vertices, vertex_count);

	::SetWindowText(statustext, "Filling vertex buffer...");
	::UpdateWindow(statustext);
	InitVertexBuffer();
	
	Specular = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 10.0f);

	if(was_playing)
		Play();

	::SetWindowText(statustext, "Done...");
	::UpdateWindow(statustext);
}


void D3DPointSetRenderer::FlatColouring(const DWORD &shader_number, const HWND &progressbar, const HWND &statustext)
{
	if(vertices == 0 || vertex_count == 0)
		return;

	bool was_playing = playing;
	Stop();

	// was at .2, makes for a less harsh highlight. lowering the last option makes for a wider spread

	DWORD shader_colour = 0xFF9F9F9F;
	Specular = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 10.0f);

	switch(shader_number)
	{
	case SHADER_COLOUR_BRONZE:
		{
			shader_colour = 0xFFFFD489;
			Specular = D3DXVECTOR4(0.0f, 0.2f, 0.0f, 100.0f);
			break;
		}
	case SHADER_COLOUR_SILVER:
		{
			shader_colour = 0xFFD0E4F5;
			Specular = D3DXVECTOR4(0.2f, 0.2f, 0.3f, 5.0f);
			break;
		}
	case SHADER_COLOUR_GOLD:
		{
			shader_colour = 0xFFF1EA97;
			Specular = D3DXVECTOR4(0.3f, 0.2f, 0.2f, 5.0f);
			break;
		}
	case SHADER_COLOUR_GREY:
		{
			shader_colour = 0xFFCFCFCF;
			Specular = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 10.0f);
			break;
		}
	case SHADER_COLOUR_RED:
		{
			shader_colour = 0xFFBF7260;
			Specular = D3DXVECTOR4(0.1f, 0.2f, 0.2f, 10.0f);
			break;
		}
	case SHADER_COLOUR_GREEN:
		{
			shader_colour = 0xFF60BF60;
			Specular = D3DXVECTOR4(0.2f, 0.1f, 0.2f, 10.0f);
			break;
		}
	case SHADER_COLOUR_BLUE:
		{
			shader_colour = 0xFF6084BF;
			Specular = D3DXVECTOR4(0.2f, 0.2f, 0.1f, 10.0f);
			break;
		}
	}



	FlatColourVertices(shader_colour, progressbar, statustext, vertices, vertex_count);

	::SetWindowText(statustext, "Filling vertex buffer...");
	::UpdateWindow(statustext);
	InitVertexBuffer();

	if(was_playing)
		Play();

	::SetWindowText(statustext, "Done...");
	::UpdateWindow(statustext);
}








void D3DPointSetRenderer::HandleInputMessages(const HWND &src_hwnd, const UINT &src_umsg, const WPARAM &src_wparam, const LPARAM &src_lparam)
{
	if(false == playing)
		return;


	static short int cursor_x_pos = 0;
	static short int cursor_y_pos = 0;
	static short int prev_cursor_x_pos = 0;
	static short int prev_cursor_y_pos = 0;
	static WPARAM l_button_down = 0;
	static WPARAM m_button_down = 0;
	static WPARAM r_button_down = 0;

	if(	WM_MOUSEMOVE == src_umsg ||
		WM_LBUTTONDOWN == src_umsg ||
		WM_LBUTTONUP == src_umsg ||
		WM_MBUTTONDOWN == src_umsg ||
		WM_MBUTTONUP == src_umsg ||
		WM_RBUTTONDOWN == src_umsg ||
		WM_RBUTTONUP == src_umsg)
	{
		prev_cursor_x_pos = cursor_x_pos;
		prev_cursor_y_pos = cursor_y_pos;
		cursor_x_pos = GET_X_LPARAM(src_lparam);
		cursor_y_pos = GET_Y_LPARAM(src_lparam);
		l_button_down = src_wparam & MK_LBUTTON;
		m_button_down = src_wparam & MK_MBUTTON;
		r_button_down = src_wparam & MK_RBUTTON;
	}


	short int cursor_x_delta = cursor_x_pos - prev_cursor_x_pos;
	short int cursor_y_delta = prev_cursor_y_pos - cursor_y_pos;
	short int cursor_total_delta = cursor_x_delta + cursor_y_delta;


	bool arcball_rotating    = src_umsg == WM_MOUSEMOVE && (l_button_down);
	bool arcball_translating = src_umsg == WM_MOUSEMOVE && (r_button_down);
	bool bounding_box_sizing = src_umsg == WM_MOUSEMOVE && ((0x8000 & GetKeyState(0x4E)) || (0x8000 & GetKeyState(0x6E))); // N or n
	bool bounding_box_moving = src_umsg == WM_MOUSEMOVE && ((0x8000 & GetKeyState(0x4D)) || (0x8000 & GetKeyState(0x6D))); // M or m


	// do some over-riding. bounding box changes take precedence, etc.
	if(true == bounding_box_sizing)
	{
		bounding_box_moving = false;
		arcball_rotating = false;
		arcball_translating = false;
	}

	if(true == bounding_box_moving)
	{
		arcball_rotating = false;
		arcball_translating = false;
	}

	if(true == arcball_rotating)
	{
		arcball_translating = false;
	}


	// let the fun begin...
	if(true == arcball_rotating)
	{
		if(!arcball.IsRotating())
			arcball.StartRotate(cursor_x_pos, cursor_y_pos);
		else
			arcball.UpdateCursorPos(cursor_x_pos, cursor_y_pos, 0, 0);
	}
	else
	{
		if(arcball.IsRotating())
			arcball.StopRotate();

		if(true == arcball_translating)
			arcball.UpdateCursorPos(cursor_x_pos, cursor_y_pos, cursor_x_delta, cursor_y_delta);
	}

	if(true == bounding_box_moving || true == bounding_box_sizing)
	{
		if(cursor_total_delta != 0)
		{
			float x_offset = 0.0f;
			float y_offset = 0.0f;
			float z_offset = 0.0f;

			float offset = 0.1f * (1.0f / zoom_level);

			if(l_button_down)
				x_offset = offset;

			if(m_button_down)
				y_offset = offset;

			if(r_button_down)
				z_offset = offset;

			if(true == bounding_box_moving)
			{
				if(cursor_total_delta > 0)
					SetBoundingGrid(bounding_grid_x_min + x_offset, bounding_grid_x_max + x_offset, bounding_grid_y_min + y_offset, bounding_grid_y_max + y_offset, bounding_grid_z_min + z_offset, bounding_grid_z_max + z_offset);
				else if(cursor_total_delta < 0)
					SetBoundingGrid(bounding_grid_x_min - x_offset, bounding_grid_x_max - x_offset, bounding_grid_y_min - y_offset, bounding_grid_y_max - y_offset, bounding_grid_z_min - z_offset, bounding_grid_z_max - z_offset);
			}

			if(true == bounding_box_sizing)
			{
				if(cursor_total_delta > 0)
				{
					SetBoundingGrid(bounding_grid_x_min - x_offset, bounding_grid_x_max + x_offset, bounding_grid_y_min - y_offset, bounding_grid_y_max + y_offset, bounding_grid_z_min - z_offset, bounding_grid_z_max + z_offset);
				}
				else if(cursor_total_delta < 0)
				{
					// if something's as small as it gets, don't resize anything to keep the shape of the box intact
					if( bounding_grid_x_min + x_offset < bounding_grid_x_max - x_offset &&
						bounding_grid_y_min + y_offset < bounding_grid_y_max - y_offset &&
						bounding_grid_z_min + z_offset < bounding_grid_z_max - z_offset)
					{
						SetBoundingGrid(bounding_grid_x_min + x_offset, bounding_grid_x_max - x_offset, bounding_grid_y_min + y_offset, bounding_grid_y_max - y_offset, bounding_grid_z_min + z_offset, bounding_grid_z_max - z_offset);						
					}
				}
			}

			if(x_offset > 0.0f || y_offset > 0.0f || z_offset > 0.0f)
				SendNotifyMessage(target, WM_BOUNDING_GRID_CHANGED, 0, 0);

		}
	}
}







string D3DPointSetRenderer::GetLastError(void)
{
	return last_error.str();
}


bool D3DPointSetRenderer::InitVertexShaders(void)
{
	if(!InitUnlitVertexShader())
		return false;

	if(!InitLitVertexShader())
		return false;

	return true;
}


bool D3DPointSetRenderer::InitUnlitVertexShader(void)
{
	HRESULT d3d_ret_code = 0;

	Stop();

	D3DVERTEXELEMENT9 decl[] = 
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};

	if(FAILED(d3d_ret_code = device->CreateVertexDeclaration(decl, &unlit_vertex_shader_decl)))
	{
		last_error.str("");
		last_error << "InitVertexShader() - CreateVertexDeclaration() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return false;
	}


	// unlit shader var/const mapping:
	//
	// v0     = position
	// v1     = normal
	// v2     = colour
	//transposed matrix c0-3
	//light dir c4
	//eyepos c5
	//specular c6
	//lightcolor c7
	//ambient c8
	//fog c9
	//alpha c10
	//gamma vars c11

	string vertex_shader_src;
	vertex_shader_src  = "vs.1.1\n";
	vertex_shader_src += "dcl_position v0\n";
	vertex_shader_src += "dcl_normal v1\n";
	vertex_shader_src += "dcl_color v2\n";
	vertex_shader_src += "m4x4 r0, v0, c0\n"; // apply transposed matrix, have to do this into r0 so we can have a variable that contains the final z value for fogging later on
	vertex_shader_src += "mov oPos, r0\n"; // set position
	vertex_shader_src += "mov oD0, v2\n"; // set colour

	// tranparency
	vertex_shader_src += "mov oD0.w, c10.xxxx\n"; // set alpha

	// fog
	vertex_shader_src += "mad oFog, -r0.zz, c9.x, c9.yy\n"; // set fog - swizzling r0.z and c8.y because oFog.x is the target (couldn't use a write mask on oFog.x for some reason)




	ID3DXBuffer *vertex_shader_tokens = 0;
	ID3DXBuffer *vertex_shader_errors = 0;

	if(FAILED(d3d_ret_code = D3DXAssembleShader(vertex_shader_src.c_str(), static_cast<UINT>(vertex_shader_src.length()), 0, 0, 0, &vertex_shader_tokens, &vertex_shader_errors)))
	{
		if(0 != vertex_shader_tokens )
		{
			vertex_shader_tokens->Release(); 
			vertex_shader_tokens = 0; 
		}

		last_error.str("");
		last_error << "InitVertexShader() - D3DXAssembleShader() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);

		if(0 != vertex_shader_errors)
		{
			last_error << "\n\nVertex shader errors:\n" << static_cast<TCHAR *>(vertex_shader_errors->GetBufferPointer());
			vertex_shader_errors->Release();
			vertex_shader_errors = 0;
		}

		return false;
	}

	if(0 != vertex_shader_errors)
	{
		vertex_shader_errors->Release();
		vertex_shader_errors = 0;
	}

	if(0 != unlit_vertex_shader)
	{
		unlit_vertex_shader->Release();
		unlit_vertex_shader = 0;
	}

	if(FAILED(d3d_ret_code = device->CreateVertexShader( (DWORD*)vertex_shader_tokens->GetBufferPointer(), &unlit_vertex_shader)))
	{
		if(0 != vertex_shader_tokens )
		{
			vertex_shader_tokens->Release(); 
			vertex_shader_tokens = 0; 
		}

		last_error.str("");
		last_error << "InitVertexShader() - CreateVertexShader() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);

		return false;
	}

	if(0 != vertex_shader_tokens )
	{
		vertex_shader_tokens->Release(); 
		vertex_shader_tokens = 0; 
	}

	return true;
}



bool D3DPointSetRenderer::InitLitVertexShader(void)
{
	HRESULT d3d_ret_code = 0;

	Stop();

	D3DVERTEXELEMENT9 decl[] = 
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};

	if(FAILED(d3d_ret_code = device->CreateVertexDeclaration(decl, &lit_vertex_shader_decl)))
	{
		last_error.str("");
		last_error << "InitVertexShader() - CreateVertexDeclaration() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return false;
	}


	// lit shader var/const mapping:
	//
	// v0     = position
	// v1     = normal
	// v2     = colour
	//transposed matrix c0-3
	//light dir c4
	//eyepos c5
	//specular c6
	//lightcolor c7
	//ambient c8
	//fog c9
	//alpha c10
	//gamma vars c11

	string vertex_shader_src;
	vertex_shader_src  = "vs.1.1\n";

	vertex_shader_src += "dcl_position v0\n";
	vertex_shader_src += "dcl_normal v1\n";
	vertex_shader_src += "dcl_color v2\n";

	vertex_shader_src += ";Do the final transformation and emit it in oPos. This\n";
	vertex_shader_src += ";is completely independent of the lighting operations\n";
	vertex_shader_src += "m4x4 oPos, v0, c0\n";

	vertex_shader_src += ";Dot product of light vector with normal vector\n";
	vertex_shader_src += ";and store it in r0.x for use with the lit instruction\n";
	vertex_shader_src += "dp3 r0.x, v1, -c4\n";

	vertex_shader_src += ";Compute the Vector from eye to vertex in object space\n";
	vertex_shader_src += ";and store the vector in r1\n";
	vertex_shader_src += "sub r1, c5, v0\n";

	vertex_shader_src += ";Normalize the eye vector\n";
	vertex_shader_src += "dp3 r1.w, r1, r1\n";
	vertex_shader_src += "rsq r1.w, r1.w\n";
	vertex_shader_src += "mul r1, r1, r1.w\n";

	vertex_shader_src += ";Compute half angle between the eye vector and the light\n";
	vertex_shader_src += ";vector. The result in stored in r2\n";
	vertex_shader_src += "add r2, r1, -c4\n";
	vertex_shader_src += "dp3 r2.w, r2, r2\n";
	vertex_shader_src += "rsq r2.w, r2.w\n";
	vertex_shader_src += "mul r2, r2, r2.w\n";

	vertex_shader_src += ";Find N dot H and store the result in r0.y\n";
	vertex_shader_src += "dp3 r0.y, r2, v1\n";

	vertex_shader_src += ";Place the power in r0.w\n";
	vertex_shader_src += "mov r0.w, c6.w\n";

	vertex_shader_src += ";Find the final diffuse and specular intensities\n";
	vertex_shader_src += "lit r4, r0\n";

	vertex_shader_src += ";Compute the specular light value\n";
	vertex_shader_src += "mul r5, r4.z, c6\n";

	vertex_shader_src += ";Compute the diffuse light value and add the ambient light\n";
	vertex_shader_src += "mul r6, r4.y, c7\n";
	vertex_shader_src += "mad r6, r6, v2, c8\n";

//	vertex_shader_src += ";Add the specular component and output the final color\n";
//	vertex_shader_src += "add oD0, r5, r6\n";

//	vertex_shader_src += ";Add the specular component and output the final color, with gamma adjustment\n";
	vertex_shader_src += "add r5, r5, r6\n";
	vertex_shader_src += "mad oD0, r5, c11.yyyy, c11.xxxx\n";

	// transparency
	vertex_shader_src += "mov oD0.w, c10.xxxx\n";	





	ID3DXBuffer *vertex_shader_tokens = 0;
	ID3DXBuffer *vertex_shader_errors = 0;

	if(FAILED(d3d_ret_code = D3DXAssembleShader(vertex_shader_src.c_str(), static_cast<UINT>(vertex_shader_src.length()), 0, 0, 0, &vertex_shader_tokens, &vertex_shader_errors)))
	{
		if(0 != vertex_shader_tokens )
		{
			vertex_shader_tokens->Release(); 
			vertex_shader_tokens = 0; 
		}

		last_error.str("");
		last_error << "InitVertexShader() - D3DXAssembleShader() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);

		if(0 != vertex_shader_errors)
		{
			last_error << "\n\nVertex shader errors:\n" << static_cast<TCHAR *>(vertex_shader_errors->GetBufferPointer());
			vertex_shader_errors->Release();
			vertex_shader_errors = 0;
		}

		return false;
	}

	if(0 != vertex_shader_errors)
	{
		vertex_shader_errors->Release();
		vertex_shader_errors = 0;
	}

	if(0 != lit_vertex_shader)
	{
		lit_vertex_shader->Release();
		lit_vertex_shader = 0;
	}

	if(FAILED(d3d_ret_code = device->CreateVertexShader( (DWORD*)vertex_shader_tokens->GetBufferPointer(), &lit_vertex_shader)))
	{
		if(0 != vertex_shader_tokens )
		{
			vertex_shader_tokens->Release(); 
			vertex_shader_tokens = 0; 
		}

		last_error.str("");
		last_error << "InitVertexShader() - CreateVertexShader() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);

		return false;
	}

	if(0 != vertex_shader_tokens )
	{
		vertex_shader_tokens->Release(); 
		vertex_shader_tokens = 0; 
	}

	return true;
}


bool D3DPointSetRenderer::EnableUnlitShader(void)
{
	HRESULT d3d_ret_code = 0;

	if(FAILED(d3d_ret_code = device->SetVertexDeclaration(unlit_vertex_shader_decl)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexDeclaration() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return false;
	}

	if(FAILED(d3d_ret_code = device->SetVertexShader(unlit_vertex_shader)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShader() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return false;
	}

	return true;
}

bool D3DPointSetRenderer::EnableLitShader(void)
{
	HRESULT d3d_ret_code = 0;

	if(FAILED(d3d_ret_code = device->SetVertexDeclaration(lit_vertex_shader_decl)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexDeclaration() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return false;
	}

	if(FAILED(d3d_ret_code = device->SetVertexShader(lit_vertex_shader)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShader() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return false;
	}
	return true;
}



bool D3DPointSetRenderer::InitVertexBuffer(void)
{
	HRESULT d3d_ret_code = 0;

	Stop();
	draw_with_user_ptr = false; // assume that we will be creating a vertex buffer


	if(0 == d3d || 0 == device)
	{
		last_error.str("InitVertexBuffer() - renderer has not been previously initialized");
		return false;
	}

	//	if(0 == vertices || 0 == vertex_count)
	//	{
	//		last_error.str("InitVertexBuffer() - point set has not been previously initialized");
	//		return false;
	//	return true;
	//	}

	if(0 != vertex_buffer)
	{
		vertex_buffer->Release();
		vertex_buffer = 0;
	}

	if(0 != vertex_count)
	{
		DWORD vertex_buffer_usage = D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC;

		if(false == uses_hardware_vertex_processing)
			vertex_buffer_usage |= D3DUSAGE_SOFTWAREPROCESSING;

		// Create the vertex buffer.
		if(FAILED(d3d_ret_code = device->CreateVertexBuffer(vertex_count * sizeof(POS_NORMAL_COLOUR_VERTEX), vertex_buffer_usage, D3DFVF_POS_NORMAL_COLOUR_VERTEX, D3DPOOL_DEFAULT, &vertex_buffer, NULL )))
		{
			vertex_buffer_usage = D3DUSAGE_WRITEONLY;

			if(false == uses_hardware_vertex_processing)
				vertex_buffer_usage |= D3DUSAGE_SOFTWAREPROCESSING;

			if(FAILED(d3d_ret_code = device->CreateVertexBuffer(vertex_count * sizeof(POS_NORMAL_COLOUR_VERTEX), 0, D3DFVF_POS_NORMAL_COLOUR_VERTEX, D3DPOOL_DEFAULT, &vertex_buffer, NULL )))
			{
				// we have to draw using the vertices ptr using DrawPrimitiveUP
				draw_with_user_ptr = true;
				return true;

				/*
				last_error.str("");

				if(d3d_ret_code == D3DERR_OUTOFVIDEOMEMORY)
					last_error << "Insufficient video memory was available to hold the generated vertices.\n\nPlease lower the set resolution and try again." << endl;
				else
					last_error << "InitVertexBuffer() - CreateVertexBuffer() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);

				return false;				
				*/
			}
		}

		// Fill the vertex buffer.
		VOID* vb_pointer;

		if(FAILED(d3d_ret_code = vertex_buffer->Lock(0, 0, (VOID**)&vb_pointer, D3DLOCK_NOOVERWRITE)))
		{
			last_error.str("");
			last_error << "InitVertexBuffer() - Lock() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return false;
		}

		memcpy(vb_pointer, vertices, vertex_count * sizeof(POS_NORMAL_COLOUR_VERTEX));

		if(FAILED(d3d_ret_code = vertex_buffer->Unlock()))
		{
			last_error.str("");
			last_error << "InitVertexBuffer() - Unlock() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return false;
		}
	}

	return true;
}


void D3DPointSetRenderer::Shutdown(void)
{
	Stop();

	if(0 != unlit_vertex_shader_decl)
	{
		unlit_vertex_shader_decl->Release();
		unlit_vertex_shader_decl = 0;
	}

	if(0 != unlit_vertex_shader)
	{
		unlit_vertex_shader->Release();
		unlit_vertex_shader = 0;
	}

	if(0 != lit_vertex_shader_decl)
	{
		lit_vertex_shader_decl->Release();
		lit_vertex_shader_decl = 0;
	}

	if(0 != lit_vertex_shader)
	{
		lit_vertex_shader->Release();
		lit_vertex_shader = 0;
	}

	if(0 != interface_font)
	{
		interface_font->Release();
		interface_font = 0;
	}

	if(0 != vertex_buffer)
	{
		vertex_buffer->Release();
		vertex_buffer = 0;
	}

	if(0 != device)
	{
		device->Release();
		device = 0;
	}

	if(0 != d3d)
	{
		d3d->Release();
		d3d = 0;
	}
}

DWORD D3DPointSetRenderer::SetSnoozeFactor(const DWORD &src_snooze_factor)
{
	DWORD last_snooze_factor = snooze_factor;
	snooze_factor = src_snooze_factor;

	return last_snooze_factor;
}




DWORD D3DPointSetRenderer::Render(DWORD fps)
{
	HRESULT d3d_ret_code = 0;



	static DWORD last_moved_at = 0;
	DWORD elapsed_time;

	long unsigned int stride = 1;
	static DWORD max_stride = device_caps.MaxStreamStride / sizeof(POS_NORMAL_COLOUR_VERTEX);



	// Setup viewing position from ArcBall
	D3DXMATRIXA16 object_matrix = *arcball.GetRotationMatrix();
	D3DXMatrixMultiply(&object_matrix, &object_matrix, arcball.GetTranslationMatrix());

	static D3DXMATRIXA16 last_object_matrix = object_matrix;
	static float last_zoom_level = zoom_level;
	static float last_camera_z_pos = camera_z_pos;

	// did the user rotate/translate/zoom the fractal?
	if(last_zoom_level != zoom_level || last_camera_z_pos != camera_z_pos || last_object_matrix != object_matrix)
		last_moved_at = timeGetTime();

	last_object_matrix = object_matrix;
	last_zoom_level = zoom_level;
	last_camera_z_pos = camera_z_pos;


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXMATRIX view_matrix;
	D3DXMatrixLookAtLH( &view_matrix,	&D3DXVECTOR3( 0.0f, 0.0f, camera_z_pos ),
		&D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
		&D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );





	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIX projection_matrix;
	D3DXMatrixPerspectiveFovLH(&projection_matrix, D3DX_PI/zoom_level, target_perspective, 0.5f, 10.0f);




//new layout:
//transposed matrix c0-3
//light dir 4
//eyepos 5
//specular 6
//lightcolor 7
//ambient 8
//fog 9
//alpha 10

	// transpose final matrix
	D3DXMATRIX transpose_matrix = object_matrix * view_matrix * projection_matrix;
	D3DXMatrixTranspose(&transpose_matrix, &transpose_matrix);
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(0, (float*)&transpose_matrix, 4)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c0-3) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}

	//Transform the light position and direction to object space
	D3DXVECTOR4 LightPos(0.0f, 5.0f, -10.0f, 0.0f);
	D3DXVECTOR4 LightDir(0.0f, 0.0f, 10.0f, 0.0f);
	D3DXMATRIX InverseWorld;
	D3DXMatrixInverse(&InverseWorld, NULL, &object_matrix);
	D3DXVec4Transform(&LightDir, &LightDir, &InverseWorld);
	D3DXVec4Transform(&LightPos, &LightPos, &InverseWorld);
	D3DXVec4Normalize(&LightDir, &LightDir);
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(4, (float*)&LightDir, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c4) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}


	//Transform the eye position to object space.
	D3DXVECTOR4 EyePos(0.0, 0.0f, camera_z_pos, 0.0f);
	D3DXVec4Transform(&EyePos, &EyePos, &InverseWorld);
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(5, (float*)&EyePos, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c5) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}



	// was at .2, makes for a less harsh highlight. lowering the last option makes for a wider spread
//	D3DXVECTOR4 Specular(0.1f, 0.1f, 0.1f, 10.0f);
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(6, (float*)&Specular, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c6) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}

	// .75 was too washed out
	D3DXVECTOR4 LightColor(1.0, 1.0f, 1.0f, 1.0f);
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(7, (float*)&LightColor, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c7) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}

	// .25 was too washed out
	D3DXVECTOR4 Ambient(0.0f, 0.0f, 0.0f, 0.25f);
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(8, (float*)&Ambient, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c8) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}


	float fog_min = 3.5f, fog_max = 5.5f;
	D3DXVECTOR4 fog_constants(1.0f/(fog_max-fog_min), fog_max/(fog_max - fog_min), 0.0f, 0.0f);

	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(9, (float*)&fog_constants, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c9) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}


	float opacity_level = static_cast<float>(voxel_opacity_level) / 100.0f;
	D3DXVECTOR4 opacity_constant(opacity_level, 0.0f, 0.0f, 0.0f);
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(10, (float*)&opacity_constant, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c10) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}

	D3DXVECTOR4 gamma_constants(0.0f, 0.0f, 0.0f, 0.0f);
	gamma_constants.x = static_cast<float>(gamma)/100.0f;
	gamma_constants.y = 1.0f - (static_cast<float>(gamma)/100.0f);
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(11, (float*)&gamma_constants, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c11) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}




	// disabled zbuffer, it conflicts with large point sizes
	//	if(FAILED(d3d_ret_code = device->Clear(1, (D3DRECT*)&screen_erase_rect, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, bg_colour, 1.0f, 0)))
	if(FAILED(d3d_ret_code = device->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, bg_colour, 1.0f, 0)))
	{
		last_error.str("");
		last_error << "Render() - Clear() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}

	if(FAILED(d3d_ret_code = device->BeginScene()))
	{
		last_error.str("");
		last_error << "Render() - BeginScene() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}


	// set general render states
	if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
	{
		last_error.str("");
		last_error << "Render() - SetRenderState() failure: D3DRS_CULLMODE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}

	if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		last_error.str("");
		last_error << "Render() - SetRenderState() failure: D3DRS_LIGHTING.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}

	if(100 == voxel_opacity_level)
	{
		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE )))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ZENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE )))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ZWRITEENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ZFUNC.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ALPHABLENDENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}
	}
	else
	{
/*
		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE )))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ZENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE )))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ZWRITEENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}
*/
		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE )))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ZENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE )))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ZWRITEENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ZFUNC.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ALPHABLENDENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}
/*
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_SRCBLEND, )))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ALPHABLENDENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_DESTBLEND, )))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ALPHABLENDENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}
*/
	}



	// do we use the lit shader for drawing the point cloud (ie: do they have usable normal data?)
	if(use_lit_shader)
	{
		if(!EnableLitShader())
			return 1;
	}
	else
	{
		if(!EnableUnlitShader())
			return 1;
	}





	long unsigned int render_vertex_count = 0;
	long unsigned int render_vertex_offset = 0;

	if(0 == z_slice_offsets.size())
	{
		render_vertex_count = 0;
		render_vertex_offset = 0;
	}
	else
	{
		if(false == render_single_slice || 1 == max_z_slice)
		{
			render_vertex_count = z_slice_offsets[max_z_slice - 1];
			render_vertex_offset = 0;
		}
		else
		{
			render_vertex_count = z_slice_offsets[max_z_slice - 1] - z_slice_offsets[max_z_slice - 2];
			render_vertex_offset = z_slice_offsets[max_z_slice - 2];
		}
	}









	if(render_vertex_count > 0)
	{
		// fog
		if(true == disable_fog || true == use_lit_shader)
		{
			if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_FOGENABLE, FALSE)))
			{
				last_error.str("");
				last_error << "Render() - SetRenderState() failure: D3DRS_FOGENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return 1;
			}
		}
		else
		{
			if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_FOGENABLE, TRUE)))
			{
				last_error.str("");
				last_error << "Render() - SetRenderState() failure: D3DRS_FOGENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return 1;
			}

			if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_FOGCOLOR, fog_colour)))
			{
				last_error.str("");
				last_error << "Render() - SetRenderState() failure: D3DRS_FOGCOLOR.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return 1;
			}

			if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR)))
			{
				last_error.str("");
				last_error << "Render() - SetRenderState() failure: D3DRS_FOGVERTEXMODE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
				return 1;
			}
		}




		if(FAILED(device->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&point_size))))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_POINTSIZE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}



		// if we are drawing from a vertex buffer instead of a user pointer
		if(false == draw_with_user_ptr)
		{
			// draw vertices
			DWORD now = timeGetTime();

			if(now < last_moved_at)
				elapsed_time = MAXDWORD - last_moved_at + now;
			else
				elapsed_time = now - last_moved_at;


			DWORD sensitivity_time = 1000;
			DWORD fade_time = 500;

			// to prevent division by 0 when calculating fade_time / max_stride later on
			// ie: radeon 9700 has a "max_stride" of 64, so no worries yet :)
			if(fade_time < max_stride)
				fade_time = max_stride;


			// when rotating/translating/zooming, temporarily reduce vertex count to increase responsiveness
			if(/*false == use_lit_shader &&*/ (elapsed_time <= sensitivity_time + fade_time))
			{
				if(render_vertex_count < 100000)
					stride = 1;
				else
					stride = render_vertex_count / 100000;

				if(stride > max_stride)
					stride = max_stride;

				if(elapsed_time >= sensitivity_time)
				{
					stride -= (elapsed_time-sensitivity_time) / (fade_time/max_stride);

					if(stride < 1 || stride > max_stride)
					{
						stride = 1;
						last_moved_at -= sensitivity_time + fade_time;
					}
				}

				if(FAILED(d3d_ret_code = device->SetStreamSource(0, vertex_buffer, 0, sizeof(POS_NORMAL_COLOUR_VERTEX) * stride)))
				{
					last_error.str("");
					last_error << "Render() - SetStreamSource() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
					return 1;
				}

				if(render_vertex_count > device_caps.MaxPrimitiveCount)
				{
					DWORD pos = render_vertex_offset;
					DWORD to_draw;

					while(pos < render_vertex_count)
					{
						if(render_vertex_count - pos < device_caps.MaxPrimitiveCount)
							to_draw = render_vertex_count - pos;
						else
							to_draw = device_caps.MaxPrimitiveCount;

						if(FAILED(d3d_ret_code = device->DrawPrimitive(D3DPT_POINTLIST, pos / stride, to_draw / stride)))
						{
							last_error.str("");
							last_error << "Render() - DrawPrimitive() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
							return 1;
						}

						pos += to_draw;
					}
				}
				else
				{
					if(FAILED(d3d_ret_code = device->DrawPrimitive(D3DPT_POINTLIST, render_vertex_offset, render_vertex_count / stride)))
					{
						last_error.str("");
						last_error << "Render() - DrawPrimitive() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
						return 1;
					}
				}
			}
			else
			{
				if(FAILED(d3d_ret_code = device->SetStreamSource(0, vertex_buffer, 0, sizeof(POS_NORMAL_COLOUR_VERTEX))))
				{
					last_error.str("");
					last_error << "Render() - SetStreamSource() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
					return 1;
				}

				if(render_vertex_count > device_caps.MaxPrimitiveCount)
				{
					DWORD pos = render_vertex_offset;
					DWORD to_draw;

					while(pos < render_vertex_count)
					{
						if(render_vertex_count - pos < device_caps.MaxPrimitiveCount)
							to_draw = render_vertex_count - pos;
						else
							to_draw = device_caps.MaxPrimitiveCount;

						if(FAILED(d3d_ret_code = device->DrawPrimitive(D3DPT_POINTLIST, pos, to_draw)))
						{
							last_error.str("");
							last_error << "Render() - DrawPrimitive() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
							return 1;
						}

						pos += to_draw;
					}
				}
				else
				{
					if(FAILED(d3d_ret_code = device->DrawPrimitive(D3DPT_POINTLIST, render_vertex_offset, render_vertex_count)))
					{
						last_error.str("");
						last_error << "Render() - DrawPrimitive() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
						return 1;
					}
				}
			}
		}
		else // draw using user pointer because we couldn't allocate a vertex buffer
		{
			// draw vertices
			DWORD now = timeGetTime();

			if(now < last_moved_at)
				elapsed_time = MAXDWORD - last_moved_at + now;
			else
				elapsed_time = now - last_moved_at;


			DWORD sensitivity_time = 1000;
			DWORD fade_time = 500;

			// to prevent division by 0 when calculating fade_time / max_stride later on
			// ie: radeon 9700 has a "max_stride" of 64, so no worries yet :)
			if(fade_time < max_stride)
				fade_time = max_stride;


			// when rotating/translating/zooming, temporarily reduce vertex count to increase responsiveness
			if(/*false == use_lit_shader &&*/ (elapsed_time <= sensitivity_time + fade_time))
			{
				if(render_vertex_count < 100000)
					stride = 1;
				else
					stride = render_vertex_count / 100000;

				if(stride > max_stride)
					stride = max_stride;

				if(elapsed_time >= sensitivity_time)
				{
					stride -= (elapsed_time-sensitivity_time) / (fade_time/max_stride);

					if(stride < 1 || stride > max_stride)
					{
						stride = 1;
						last_moved_at -= sensitivity_time + fade_time;
					}
				}


				if(render_vertex_count > device_caps.MaxPrimitiveCount)
				{
					DWORD pos = render_vertex_offset;
					DWORD to_draw;

					while(pos < render_vertex_count)
					{
						if(render_vertex_count - pos < device_caps.MaxPrimitiveCount)
							to_draw = render_vertex_count - pos;
						else
							to_draw = device_caps.MaxPrimitiveCount;


						if(FAILED(d3d_ret_code = device->DrawPrimitiveUP(D3DPT_POINTLIST, to_draw / stride, vertices + pos, sizeof(POS_NORMAL_COLOUR_VERTEX) * stride)))
						{
							last_error.str("");
							last_error << "Render() - DrawPrimitive() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
							return 1;
						}

						pos += to_draw;
					}
				}
				else
				{
					if(FAILED(d3d_ret_code = device->DrawPrimitiveUP(D3DPT_POINTLIST, render_vertex_count / stride, vertices + render_vertex_offset, sizeof(POS_NORMAL_COLOUR_VERTEX))))
					{
						last_error.str("");
						last_error << "Render() - DrawPrimitive() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
						return 1;
					}
				}
			}
			else // no stride
			{
				if(render_vertex_count > device_caps.MaxPrimitiveCount)
				{
					DWORD pos = render_vertex_offset;
					DWORD to_draw;

					while(pos < render_vertex_count)
					{
						if(render_vertex_count - pos < device_caps.MaxPrimitiveCount)
							to_draw = render_vertex_count - pos;
						else
							to_draw = device_caps.MaxPrimitiveCount;

						if(FAILED(d3d_ret_code = device->DrawPrimitiveUP(D3DPT_POINTLIST, to_draw, vertices + pos, sizeof(POS_NORMAL_COLOUR_VERTEX))))
						{
							last_error.str("");
							last_error << "Render() - DrawPrimitiveUP() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
							return 1;
						}

						pos += to_draw;
					}
				}
				else
				{
					if(FAILED(d3d_ret_code = device->DrawPrimitiveUP(D3DPT_POINTLIST, render_vertex_count, vertices + render_vertex_offset, sizeof(POS_NORMAL_COLOUR_VERTEX))))
					{
						last_error.str("");
						last_error << "Render() - DrawPrimitive() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
						return 1;
					}
				}
			}



		}

	} // end if(render_vertex_count > 0)




	// switch back to unlit shader for drawing the grid/axis indicators
	if(!EnableUnlitShader())
		return 1;

	// reset alpha blending			
	opacity_constant.x = 1.0f;
	if(FAILED(d3d_ret_code = device->SetVertexShaderConstantF(10, (float*)&opacity_constant, 1)))
	{
		last_error.str("");
		last_error << "Render() - SetVertexShaderConstantF(c10) failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}


	if(false == disable_bounding_grid && bounding_grid_vertex_count > 0 && 0 != bounding_grid_vertices)
	{
		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_FOGENABLE, FALSE)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_FOGENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ALPHABLENDENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_SRCBLEND.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_DESTBLEND.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE)))
		{
			//			last_error.str("");
			//			last_error << "Render() - SetRenderState() failure: D3DRS_ANTIALIASEDLINEENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			//			return 1;
		}

		if(FAILED(d3d_ret_code = device->DrawPrimitiveUP(D3DPT_LINELIST, bounding_grid_vertex_count / 2, bounding_grid_vertices, sizeof(POS_NORMAL_COLOUR_VERTEX))))
		{
			last_error.str("");
			last_error << "Render() - DrawPrimitiveUP() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE)))
		{
			//			last_error.str("");
			//			last_error << "Render() - SetRenderState() failure: D3DRS_ANTIALIASEDLINEENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			//			return 1;
		}
	}




	if(false == disable_axis_indicators)
	{
		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_FOGENABLE, FALSE)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_FOGENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_ALPHABLENDENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_SRCBLEND.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))
		{
			last_error.str("");
			last_error << "Render() - SetRenderState() failure: D3DRS_DESTBLEND.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE)))
		{
			//			last_error.str("");
			//			last_error << "Render() - SetRenderState() failure: D3DRS_ANTIALIASEDLINEENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			//			return 1;
		}

		if(FAILED(d3d_ret_code = device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, p_x_axis_size / 3, p_x_axis_vertices_copy, sizeof(POS_NORMAL_COLOUR_VERTEX))))
		{
			last_error.str("");
			last_error << "Render() - DrawPrimitiveUP() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, p_y_axis_size / 3, p_y_axis_vertices_copy, sizeof(POS_NORMAL_COLOUR_VERTEX))))
		{
			last_error.str("");
			last_error << "Render() - DrawPrimitiveUP() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, p_z_axis_size / 3, p_z_axis_vertices_copy, sizeof(POS_NORMAL_COLOUR_VERTEX))))
		{
			last_error.str("");
			last_error << "Render() - DrawPrimitiveUP() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		if(FAILED(d3d_ret_code = device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE)))
		{
			//			last_error.str("");
			//			last_error << "Render() - SetRenderState() failure: D3DRS_ANTIALIASEDLINEENABLE.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			//			return 1;
		}

	}









	if(true == draw_interface_text)
	{
		// draw diagnostic output
		ostringstream out;

		out << interface_header;

		if(is_generating)
		{
			out << "\n\nGenerating set...";
		}
		else
		{
			out << "\n\nPoints: " << render_vertex_count << " of " << vertex_count;


			if(vertex_count == render_vertex_count)
				out << " (100% of set)";	
			else if(0 == render_vertex_count) // possible?
				out << " (0% of set)";
			else
			{
				double percent_of_set = static_cast<double>(render_vertex_count) / static_cast<double>(vertex_count) * 100.0f;
				out << " (" << percent_of_set << "% of set)";
			}


			out << "\nMax z-slice: " << static_cast<long unsigned int>(max_z_slice) << " of " << static_cast<long unsigned int>(z_slice_offsets.size());

			if(true == render_single_slice)
				out << " (Single slice mode)";

			out << "\nPoint Size: " << point_size << " of " << max_point_size;
			out << "\nOpacity:" << (int)voxel_opacity_level << "%";

			out << "\nAmbience:" << (int)gamma << "%";


			out	<< "\n\nFPS: ";

			if(fps == 0)
				out << "<1";
			else
				out << fps;


			out << "\n\nMouse control modes:\n  Left Button: Rotate\n  Mouse Wheel Up/Down: Zoom +/-\n  Right Button: Translate\n  N + Any Button: Resize bounding grid\n  M + Any Button: Move bounding grid";
		}

		out << "\n\nAdapter Info:";

		out << adapter_info;
		out << "\n  Max Primitive Count: " << device_caps.MaxPrimitiveCount << "\n  Stride: " << stride << " of " << max_stride << " (" << device_caps.MaxStreamStride << ")";

		double available_mem = static_cast<double>(device->GetAvailableTextureMem());

		available_mem /= (1024.0 * 1024.0);

		out << "\n  Available Video RAM: " << static_cast<long unsigned int>(available_mem) << "MB";


		if(false == is_generating && true == draw_with_user_ptr)
		{
			out << "\n\n** Vertex set size exceeds available video RAM **\n** This may degrade renderer performance **";
		}
			


		RECT destRect;
		SetRect(&destRect, 7, 3, 1023, 767);

		if(FAILED(d3d_ret_code = interface_font->DrawText( NULL, out.str().c_str(), -1, &destRect, DT_NOCLIP, 0xFF585955)))
		{
			last_error.str("");
			last_error << "Render() - DrawText() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}

		SetRect(&destRect, 6, 2, 1023, 767);

		if(FAILED(d3d_ret_code = interface_font->DrawText( NULL, out.str().c_str(), -1, &destRect, DT_NOCLIP, interface_text_colour)))
		{
			last_error.str("");
			last_error << "Render() - DrawText() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
			return 1;
		}
	}





	if(FAILED(d3d_ret_code = device->EndScene()))
	{
		last_error.str("");
		last_error << "Render() - EndScene() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}

	if(FAILED(d3d_ret_code = device->Present(0, &drawable_rect, 0, 0)))
	{
		if(D3DERR_DEVICELOST == d3d_ret_code)
		{
			DWORD reset_start = timeGetTime();
			DWORD reset_elapsed = 0;

			// wait for 5000 milliseconds before giving up on reobtaining the device
			// while(1); would make it an infinite loop
			do
			{
				if(D3DERR_DEVICENOTRESET == device->TestCooperativeLevel())
					return 2;

				DWORD reset_end = timeGetTime();

				// check for overflow condition, Win32 low-precision timing API
				if(reset_end < reset_start)
					reset_elapsed = MAXDWORD - reset_start + reset_end;
				else
					reset_elapsed = reset_end - reset_start;

				Sleep(1);
			}
			while(reset_elapsed < 5000);
		}

		last_error.str("");
		last_error << "Render() - Present() failure.\n\nError code: " << DXGetErrorString9(d3d_ret_code) << "\nError description: " << DXGetErrorDescription9(d3d_ret_code);
		return 1;
	}



	return 0;
}




// thread function
DWORD WINAPI D3DPointSetRenderer::RenderingThreadFunction(LPVOID D3DPointSetRenderer_obj)
{
	DWORD ret = 0;
	DWORD wait_result = 0; // used to determine the state of the stop event
	DWORD fps = 0;
	static DWORD last_fps_at = timeGetTime();

	CTimer timer;
	timer.Reset();

	// cast the *void argument to the type of *D3DPointSetRenderer,
	// point *D3DPointSetRenderer to the memory location of the argument's value
	D3DPointSetRenderer *renderer = reinterpret_cast<D3DPointSetRenderer*>(D3DPointSetRenderer_obj);

	// as long as the event is not signaled (infinite loop)
	while(WAIT_OBJECT_0 != (wait_result = WaitForSingleObject(renderer->stop_event, 0)))
	{
		// did our check fail because of a Win32 error?
		if(WAIT_FAILED == wait_result)
		{
			ret = 1;
			PostMessage(renderer->target, WM_RECREATE_RENDERER, ret, 0);
			break;
		}

		if(renderer->playing)
		{
			// ret codes from render/renderingthreadfunction:
			// 0 OK
			// 1 error, completely shutdown and reinit the renderer
			// 2 device lost, reset the renderer
			if(0 != (ret = renderer->Render(fps)))
			{
				PostMessage(renderer->target, WM_RECREATE_RENDERER, ret, 0);
				break;
			}
		}
		else
		{
			Sleep(1);
		}

		timer.Tick();
		fps = static_cast<DWORD>(timer.GetFPS());

		if(renderer->snooze_factor != 0)
			Sleep(renderer->snooze_factor);
	}

	// exit gracefully
	ExitThread(ret);

	return 0;
}