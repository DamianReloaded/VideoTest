#ifndef owl_platform_window_h
#define owl_platform_window_h

#include "image.h"

class window
{
	public:
						window		();
		virtual			~window		();
	
		bool			create		();
		void			destroy		();
		void			refresh		();
		void			resize		(const int& _w, const int& _h);
		void			draw		(image* _imge, const int& _x=0, const int& _y=0, const int& _w=0, const int& _h=0);
		
		virtual void	on_init		();
		virtual void	on_draw		();
		virtual void	on_close	(bool& _cancel);

		static void		update		();

	protected:
		int				m_width;
		int				m_height;
		class implementation;
		implementation* m_imp;

		friend class implementation;
};

#endif owl_platform_window_h