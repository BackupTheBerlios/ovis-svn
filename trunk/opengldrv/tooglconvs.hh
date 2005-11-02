#ifndef OVIS_OPENGLDRV_OPENGLCONVERSIONS_HH_INCLUDED
#define OVIS_OPENGLDRV_OPENGLCONVERSIONS_HH_INCLUDED

#include "ovisgl.hh"

#include "../base/numericarrays.hh"
#include "../video/coreenums.hh"
#include "../video/mempool.hh"
#include "../video/pixelformat.hh"

namespace ovis {
namespace opengldrv {

	GLint oglpixelformat(const video::Pixelformat ionpixelformat);

}
}

#endif
