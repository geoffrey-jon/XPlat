#ifndef GSTILLCAMERA_H
#define GSTILLCAMERA_H

#include "GCamera.h"
#include "MathHelper.h"

__declspec(align(16))
class GStillCamera : public GCamera
{
public:
	GStillCamera();
	~GStillCamera();

	void* operator new(size_t i) { return _mm_malloc(i,16);	}
	void operator delete(void* p) { _mm_free(p); }

private:

};

#endif // GSTILLCAMERA_H