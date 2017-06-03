#ifndef owl_platform_image_h
#define owl_platform_image_h

#include <vector>

class pixel
{
	public:
								pixel	() { m_r=m_g=m_b=NULL; }
								pixel	(unsigned char* _buffer) { set(_buffer); }

		void					set		(unsigned char* _buffer) { m_b = _buffer; m_g = (_buffer+1); m_r = (_buffer+2); }
		void					set		(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b) {r(_r); g(_g); b(_b);}

		const unsigned char&	r		() { return *m_r; }
		const unsigned char&	g		() { return *m_g; }
		const unsigned char&	b		() { return *m_b; }
		const unsigned char		gray	() { return unsigned char((float(r())+float(g())+float(b()))/3.0f); }

		void			r				(const unsigned char& _r) { (*m_r)=_r; }
		void			g				(const unsigned char& _g) { (*m_g)=_g; }
		void			b				(const unsigned char& _b) { (*m_b)=_b; }

	protected:
		unsigned char*	m_r;
		unsigned char*	m_g;
		unsigned char*	m_b;
};

class image
{
	public:
									image		();
		virtual						~image		();
		void						create		(const int& width, const int& height);
		void						destroy		();

		const int&					width		();
		const int&					height		();
		
		const size_t				pixelcount	();
		unsigned char*				bytes		();
		const size_t				size		();

		void						get			(const size_t& _pos, pixel& _pixel);

	protected:
		int							m_width;
		int							m_height;

		std::vector<unsigned char>	m_buffer;
		size_t						m_pixelcount;
		class implementation;
		implementation* m_imp;
};

#endif //owl_platform_image_h