#ifndef GCONSTANTBUFFER_H
#define GCONSTANTBUFFER_H

#include "D3D11.h"
#include "malloc.h"

#include "GObject.h"

enum ShaderStage
{
	VS = 0b00001,
	HS = 0b00010,
	DS = 0b00100,
	GS = 0b01000,
	PS = 0b10000
};

enum UpdateFrequency
{
	PER_FRAME = 0,
	PER_OBJECT = 1
};

class GObject;

__declspec(align(16))
class GConstantBuffer
{
public:
	GConstantBuffer();
	~GConstantBuffer();

	void* operator new(size_t i){ return _mm_malloc(i,16); }
	void operator delete(void* p) { _mm_free(p); }

	virtual void Init() = 0;
	virtual void Update(D3D11_MAPPED_SUBRESOURCE resource) = 0;
	virtual void Update(D3D11_MAPPED_SUBRESOURCE resource, GObject* obj) = 0;

	void SetShaderStages(BYTE stages);

	bool IsBoundToVS();
	bool IsBoundToHS();
	bool IsBoundToDS();
	bool IsBoundToGS();
	bool IsBoundToPS();

	inline ID3D11Buffer* GetBuffer() { return mBuffer; }
	inline void SetBuffer(ID3D11Buffer* buffer) { mBuffer = buffer; }

	inline UINT GetByteWidth() { return mByteWidth; }

	inline UpdateFrequency GetUpdateFrequency() { return mFreq; }
	inline void SetUpdateFrequency(UpdateFrequency freq) { mFreq = freq; }

protected:
	ID3D11Buffer* mBuffer;

	UINT mByteWidth;

	BYTE mShaderStages;
	UpdateFrequency mFreq;
};

#endif // GCONSTANTBUFFER_H
