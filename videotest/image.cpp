#include "image.h"
#include <windows.h>

class image::implementation
{
	public:

};

image::image	()
{
	m_imp = new implementation();
}

image::~image	()
{
	destroy();
	delete m_imp;
}

void image::create(const int& width, const int& height)
{
	m_width = width;
	m_height = height;

	m_pixelcount = width*height;
	m_buffer.resize(m_pixelcount*3);
}

void image::destroy()
{

}

unsigned char* image::bytes()
{
	return &m_buffer[0];
}

const int& image::width()
{
	return m_width;
}

const int& image::height()
{
	return m_height;
}

const size_t image::size()
{
	return m_buffer.size();
}

const size_t image::pixelcount()
{
	return m_pixelcount;
}

void image::get	(const size_t& _pos, pixel& _pixel)
{
	_pixel.set(&m_buffer[_pos*3]);
}