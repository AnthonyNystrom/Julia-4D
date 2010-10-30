#include "stdafx.h"

#include "set_generation_package.h"
#include "quaternion_julia_set.h"


#include <iostream>
#include <fstream>
using namespace std;

set_generation_package::set_generation_package(void)
{
	hWndCaller = 0;
	hWndProgressBar = 0;
	hWndStatusText = 0;

	vertices = 0;
	vertex_count = 0;

	thread = 0;

	shutdown_event = 0;
	pause_event = 0;
	cancel_event = 0;
	begin_generation_event = 0;
}

set_generation_package::~set_generation_package(void)
{
	if(!Cancel(false))
		return;

	if(0 != thread)
	{
		SetEvent(shutdown_event);
		WaitForSingleObject(thread, INFINITE);
		CloseHandle(thread);
	}

	if(0 != shutdown_event)
		CloseHandle(shutdown_event);

	if(0 != pause_event)
		CloseHandle(pause_event);

	if(0 != cancel_event)
		CloseHandle(cancel_event);

	if(0 != begin_generation_event)
		CloseHandle(begin_generation_event);
}

string set_generation_package::GetLastError(void)
{
	return last_error.str();
}

bool set_generation_package::Init(const HWND &src_hWndCaller, const HWND &src_hWndProgressBar, const HWND &src_hWndStatusText)
{
	if(0 == (shutdown_event = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		last_error.str("");
		last_error << "Init() - CreateEvent() failure. Error code: " << GetLastError();
		return false;
	}

	if(0 == (pause_event = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		last_error.str("");
		last_error << "Init() - CreateEvent() failure. Error code: " << GetLastError();
		return false;
	}

	if(0 == (cancel_event = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		last_error.str("");
		last_error << "Init() - CreateEvent() failure. Error code: " << GetLastError();
		return false;
	}

	if(0 == (begin_generation_event = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		last_error.str("");
		last_error << "Init() - CreateEvent() failure. Error code: " << GetLastError();
		return false;
	}

	if(0 != thread)
	{
		if(!Cancel(false))
		{
			last_error.str("");
			last_error << "Init() - Stop() failure. Error code: " << GetLastError();
			return false;
		}
	}
	else
	{
		// create thread
		thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)set_generation_package::GenerationThreadFunction, this, 0, NULL);

		if(0 == thread)
		{
			last_error.str("");
			last_error << "Init() - CreateThread() failure. Error code: " << GetLastError();
			return false;
		}
	}

	hWndCaller = src_hWndCaller;
	hWndProgressBar = src_hWndProgressBar;
	hWndStatusText = src_hWndStatusText;


//	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
//	SetThreadPriority(thread, THREAD_PRIORITY_TIME_CRITICAL);



	return true;
}

bool set_generation_package::Generate(const Cjulia4d2Doc &src_params)
{
	if(!Cancel(false))
	{
		last_error.str("");
		last_error << "GenerateSet() - Stop() failure. Error code: " << GetLastError();
		return false;
	}

	params = src_params;

	SetEvent(begin_generation_event);

	return true;
}

bool set_generation_package::Cancel(const bool &send_cancel_complete_msg)
{
	if(!Unpause())
	{
		return false;
	}

	SetEvent(cancel_event);

	if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
		Sleep(1);

	return true;
}

bool set_generation_package::Pause(void)
{
	SetEvent(pause_event);
	return true;
}

bool set_generation_package::Unpause(void)
{
	ResetEvent(pause_event);
	return true;
}

void set_generation_package::SendSuccessMessage(void)
{
	// was SendNotifyMessage
	PostMessage(hWndCaller, WM_SET_GEN_SUCCESS, 0, 0);
}

void set_generation_package::SendFailureMessage(void)
{
	// was SendNotifyMessage
	PostMessage(hWndCaller, WM_SET_GEN_FAILURE, 0, 0);
}


void set_generation_package::get_z_slices(POS_NORMAL_COLOUR_VERTEX *&vertices, const long unsigned int &vertex_count, vector<long unsigned int> &z_slices)
{
	z_slice_offsets.clear();

	if(vertices == 0 || vertex_count == 0)
	{
		z_slice_offsets.push_back(0);
		return;
	}

	float last_z = vertices[0].z;

	for(long unsigned int i = 1; i < vertex_count; i++)
	{
		if(vertices[i].z < last_z)
		{
			z_slice_offsets.push_back(i);
			last_z = vertices[i].z;
		}
	}

	if(z_slice_offsets.size() == 0)
	{
		z_slice_offsets.push_back(vertex_count);
	}
	else
	{
		if(z_slice_offsets[z_slice_offsets.size() - 1] != vertex_count)
			z_slice_offsets.push_back(vertex_count);
	}
}


// thread function
VOID set_generation_package::GenerationThreadFunction(LPVOID set_gen_pkg_obj)
{
	set_generation_package *set_gen_pkg = reinterpret_cast<set_generation_package*>(set_gen_pkg_obj);

	while(WAIT_OBJECT_0 != WaitForSingleObject(set_gen_pkg->shutdown_event, 0))
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(set_gen_pkg->cancel_event, 0))
		{
			ResetEvent(set_gen_pkg->cancel_event);
		}
		else if(WAIT_OBJECT_0 == WaitForSingleObject(set_gen_pkg->begin_generation_event, 0))
		{
			ResetEvent(set_gen_pkg->cancel_event);
			ResetEvent(set_gen_pkg->begin_generation_event);

			int ret_code = 0;			

			// do generation here
			quaternion_julia_set q_julia_set(set_gen_pkg->params);

			if(set_gen_pkg->params.formula == CUSTOM_ITERATOR)
			{
				string error;

				if(!q_julia_set.SetupCustomFormulaText(set_gen_pkg->params.formula_text, error))
				{
					error = "Formula error: " + error;
					MessageBox(0, error.c_str(), APP_NAME, MB_OK|MB_ICONSTOP);
					set_gen_pkg->SendFailureMessage();
					continue;
				}
			}

			try
			{
				if(set_gen_pkg->params.render_to_type == POINT_SET)
				{
					if(set_gen_pkg->params.hopf_map == true)
					{
						ret_code = q_julia_set.GetHopfMap(
												set_gen_pkg->hWndProgressBar,
												set_gen_pkg->hWndStatusText,
												set_gen_pkg->vertices,
												set_gen_pkg->vertex_count,
												set_gen_pkg->cancel_event);

						if(ret_code == 1)
							set_gen_pkg->get_z_slices(set_gen_pkg->vertices, set_gen_pkg->vertex_count, set_gen_pkg->z_slice_offsets);
					}
					else
					{
						ret_code = q_julia_set.GetPointSet(
												set_gen_pkg->hWndProgressBar,
												set_gen_pkg->hWndStatusText,
												set_gen_pkg->vertices,
												set_gen_pkg->vertex_count,
												set_gen_pkg->cancel_event);

						if(ret_code == 1)
							set_gen_pkg->get_z_slices(set_gen_pkg->vertices, set_gen_pkg->vertex_count, set_gen_pkg->z_slice_offsets);

					}
				}
				else if(set_gen_pkg->params.render_to_type == CSV_FILE)
				{
					if(set_gen_pkg->params.hopf_map == true)
					{
						ret_code = q_julia_set.SaveHopfMapCSV(
												set_gen_pkg->hWndProgressBar,
												set_gen_pkg->hWndStatusText,
												set_gen_pkg->cancel_event);
					}
					else
					{
						ret_code = q_julia_set.SavePointSetCSV(
												set_gen_pkg->hWndProgressBar,
												set_gen_pkg->hWndStatusText,
												set_gen_pkg->cancel_event);
					}
				}
				else if(set_gen_pkg->params.render_to_type == VOXEL_FILE)
				{
					if(set_gen_pkg->params.hopf_map == true)
					{
						ret_code = q_julia_set.SaveHopfMapJ4D2VOX(
												set_gen_pkg->hWndProgressBar,
												set_gen_pkg->hWndStatusText,
												set_gen_pkg->cancel_event);
					}
					else
					{
						ret_code = q_julia_set.SaveJ4D2VOX(
												set_gen_pkg->hWndProgressBar,
												set_gen_pkg->hWndStatusText,
												set_gen_pkg->cancel_event);
					}
				}
				else if(set_gen_pkg->params.render_to_type == MESH_FILE)
				{
					if(set_gen_pkg->params.hopf_map == true)
					{
						ret_code = q_julia_set.SaveHopfMapOBJ(
												set_gen_pkg->hWndProgressBar,
												set_gen_pkg->hWndStatusText,
												set_gen_pkg->cancel_event);
					}
					else
					{
						ret_code = q_julia_set.SaveOBJ(
												set_gen_pkg->hWndProgressBar,
												set_gen_pkg->hWndStatusText,
												set_gen_pkg->cancel_event);
					}
				}

				if(ret_code < 0)
				{
					if(ret_code == -2)
						MessageBox(0, "An error has occurred while creating (or opening) file.\nPlease check that you have sufficient privelages, and that it is not currently open in another application.", APP_NAME, MB_OK|MB_ICONSTOP);
					else if(ret_code == -1)
						MessageBox(0, "Insufficient memory was available to complete the set generation.", APP_NAME, MB_OK|MB_ICONSTOP);

					set_gen_pkg->SendFailureMessage();
				}
				else
				{
					set_gen_pkg->SendSuccessMessage();
				}
			}
			catch(exception &e)
			{
				ostringstream out;
				out << "An unexpected error has occurred during set generation: " << e.what() << "\n\nPlease report this behaviour to shalayka@sasktel.net";
				MessageBox(0, out.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);
			}
		}

		Sleep(1);
	}

	ExitThread(0);
}