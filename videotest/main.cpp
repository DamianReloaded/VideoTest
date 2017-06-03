#include <windows.h>
#include <iostream>
#include "window.h"
#include "webcam.h"

class mywin : public window
{
	public:
		virtual void on_init()
		{
			int width = 320;
			int height = 240;
			resize(width, height);
			if (!cam.start(width, height))
			{
				MessageBox(NULL, L"Webcam failed", L"Error", 0);
				running = false;
				return;
			}
		
			running = true;
		}

		virtual void update_frame ()
		{
			if( cam.ready() )	
			{
				pixel pix;
				for (size_t i=0; i<cam.frame.pixelcount(); i++)
				{
					cam.frame.get(i,pix);
				}

				draw(&cam.frame);
			}
		}

		virtual void	on_close	(bool& _cancel)
		{
			cam.stop();
			running = false;
		}

		webcam cam;
		bool running;
};



int main()
{
	mywin win;
	win.create();

	while (win.running) 
	{
		window::update();
		win.update_frame();
	}

	win.destroy();

	return 0;
}
