#ifndef owl_platform_webcam_h
#define owl_platform_webcam_h

#include "image.h"

class webcam
{
	public:
									webcam	();
		virtual						~webcam	();

		bool						start	(const int& _width, const int& _height);
		void						stop	();

		bool						ready	();

		image						frame;

	protected:
		class implementation;
		implementation* m_imp;

};

#endif //owl_platform_webcam_h