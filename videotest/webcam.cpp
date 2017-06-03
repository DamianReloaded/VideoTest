#include "webcam.h"
#include "videoInput.h"

class webcam::implementation
{
	public:
		implementation() : dev(0) {}
		videoInput	VI;
		int			dev;
};

webcam::webcam	()
{
	m_imp = new implementation();
}

webcam::~webcam	()
{
	stop();
	delete m_imp;
}

bool webcam::start(const int& _width, const int& _height)
{
	//uncomment for silent setup
	//videoInput::setVerbose(false); 

	//uncomment for multithreaded setup
	//videoInput::setComMultiThreaded(true); 

	//optional static function to list devices
	//for silent listDevices use listDevices(true);
	int numDevices = videoInput::listDevices();	

	//you can also now get the device list as a vector of strings 
	std::vector <std::string> list = videoInput::getDeviceList(); 
	for(size_t i = 0; i < list.size(); i++){
		printf("[%i] device is %s\n", i, list[i].c_str());
	}

	//by default we use a callback method
	//this updates whenever a new frame
	//arrives if you are only ocassionally grabbing frames
	//you might want to set this to false as the callback caches the last
	//frame for performance reasons. 
	m_imp->VI.setUseCallback(true);

	//try and setup device with id 0 and id 1
	//if only one device is found the second 
	//setupDevice should return false

	//if you want to capture at a different frame rate (default is 30) 
	//specify it here, you are not guaranteed to get this fps though.
	//m_imp->VI.setIdealFramerate(dev, 60);

	//we can specifiy the dimensions we want to capture at
	//if those sizes are not possible VI will look for the next nearest matching size
	//m_imp->VI.setRequestedMediaSubType((int)MEDIASUBTYPE_MJPG);
	m_imp->VI.setupDevice(m_imp->dev,   _width, _height, VI_COMPOSITE); 
	
	//once the device is setup you can try and
	//set the format - this is useful if your device
	//doesn't remember what format you set it to
	//m_imp->VI.setFormat(dev, VI_NTSC_M);					//optional set the format

	//we allocate our buffer based on the number
	//of pixels in each frame - this will be width * height * 3
	//frame_size = m_imp->VI.getSize(dev);
	//frame.resize(frame_size);

	if (m_imp->VI.getSize(m_imp->dev)<1) return false;

	frame.create(m_imp->VI.getWidth(m_imp->dev), m_imp->VI.getHeight(m_imp->dev));

	return true;
}

bool webcam::ready ()
{
	if (m_imp->VI.isFrameNew(m_imp->dev) && frame.size()>0)
	{
		m_imp->VI.getPixels(m_imp->dev, frame.bytes(), false);
		return true;
	}
	return false;
}

void webcam::stop()
{
	m_imp->VI.stopDevice(m_imp->dev);
}