#ifndef OVIS_D3D9DRV_D3D9INDEXSTREAM_HH_INCLUDED
#define OVIS_D3D9DRV_D3D9INDEXSTREAM_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <windows.h>
#include <d3d9.h>

#include "../video/indexstream.hh"

namespace ovis {
namespace d3d9drv {

	class D3D9Device;

	class D3D9Indexstream:public video::Indexstream
	{
	public:
		D3D9Indexstream(D3D9Device& d3d9device,const video::Indexformat format,const ovis_uint32 flags,
			const video::Memorypool mempool,const ovis_uint32 _capacity);
		~D3D9Indexstream();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void map(const ovis_uint32 flags);
		void unmap();

		void* mappedPointer();
		bool isMapped() const;

		void d3d9releaseBeforeReset();
		void d3d9restoreAfterReset();

		ovis_uint32 index(const ovis_uint32 position);
		void index(const ovis_uint32 position,const ovis_uint32 newindex);

	protected:

		void* m_pMappedPointer;
		D3D9Device& m_rD3D9Device;
		LPDIRECT3DINDEXBUFFER9 m_pIB;
		bool m_IsDataOK;
	};

}
}

#endif
