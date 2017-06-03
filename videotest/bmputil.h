#ifndef owl_platform_bmputil_h
#define owl_platform_bmputil_h

#include <windows.h>

class bmputil
{
public:
	bmputil(){}
	
	~bmputil(){}
	//initializing of buffers, must be done before all other operations
	// num is the number of buffers that will be used
    void initialize_buffers(HWND handle,int num);
	//prepares buffer[which] (1st index is 0) for following operations 
	void create_buffer( int which);
	//frees the resources of buffer[which] (1st index = 0)
	void free_buffer(int which);
	//frees all resources that have been allocated 
    void free_buffers(); 
	//Copies an area from a buffer (=rectangle coordinates->arguments 3-6)
	// to a buffer (coordinates ->arguments 7,8) 
	void copy_area_from_to(int frombuf, int tobuf,int top_from_x, int top_from_y, int width, int height, int top_to_x, int top_to_y);
	//Similar to Copy_Area_from_to,Copies an area from a buffer to the screen 
	void copy_area_to_screen(int frombuf,int top_from_x, int top_from_y,int width, int height ,int top_to_x, int top_to_y); 
	// Copies whole content of buffer[which] to screen
	void copy_to_screen(int which);
    // returns number of buffers
    int num_of_buffers(); 
	// Copies an area defined by rect_from (RECT structure) and an handle (h_from)
	// to the buffer[which]
    void copy_surface_to_buffer(HWND h_from,RECT rect_from,int which);
	HDC & get_hdc();
	HDC & get_dc_buffer(int which);

	void resize(HWND handle)
	{
		free_buffers();
		initialize_buffers(handle,num_buf);
		for (int i=0; i<num_buf; i++)
		{
			create_buffer(i);
		}
	}
	 
protected:
	HDC			my_DC;
	HDC*		my_DC_Buffer;
	HWND		my_handle;
	RECT*		dc_rect;
	HBITMAP*	hbm_Buffer;
	HBITMAP*	hbm_oldBuffer;
    int			num_buf;
   
};

#endif //owl_platform_bmputil_h