#ifndef JULIA4D2_SET_GENERATION_PACKAGE_H
#define JULIA4D2_SET_GENERATION_PACKAGE_H



#include <windows.h>

#include "defines.h"
#include "julia4d2Doc.h"

#include <sstream>
#include <string>
#include <vector>
using std::ostringstream;
using std::string;
using std::vector;


class set_generation_package
{
public:

	friend class CMainFrame; // grant access to protected params object

	set_generation_package(void);
	~set_generation_package(void);
	string GetLastError(void);

	bool Init(const HWND &src_hWndCaller, const HWND &src_hWndProgressBar, const HWND &src_hWndStatusText);
	bool Generate(const Cjulia4d2Doc &src_params);
	bool Cancel(const bool &send_cancel_complete_msg = true);
	bool Pause(void);
	bool Unpause(void);

	POS_NORMAL_COLOUR_VERTEX *vertices;
	long unsigned int vertex_count;
	vector<long unsigned int> z_slice_offsets;

protected:

	void SendSuccessMessage(void);
	void SendFailureMessage(void);

	void get_z_slices(POS_NORMAL_COLOUR_VERTEX *&vertices, const long unsigned int &vertex_count, vector<long unsigned int> &z_slices);
	static VOID GenerationThreadFunction(LPVOID set_gen_pkg_param) throw();

	HWND hWndCaller;
	HWND hWndProgressBar;
	HWND hWndStatusText;

	Cjulia4d2Doc params;

	HANDLE thread;
	
	HANDLE shutdown_event;
	HANDLE pause_event;
	HANDLE cancel_event;
	HANDLE begin_generation_event;

	ostringstream last_error;
};


#endif