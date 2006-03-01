#include <boost/python.hpp>
#include "renderinstance.hh"

using namespace boost::python;
using namespace ovis;
		
BOOST_PYTHON_MODULE(libovislib) {

    class_<Renderinstance>("Renderinstance",init<>())
        .def("perspectiveViewport", &Renderinstance::perspectiveViewport)
        .def("testinitRenderer", &Renderinstance::testinitRenderer)
        .def("windowmode", &Renderinstance::windowmode)
        .def("scene", &Renderinstance::scene)
        .def("endRenderer", &Renderinstance::endRenderer)
        .def("interactiveLoop", &Renderinstance::interactiveLoop)
    ;

	enum_<Renderinstance::Windowmode>("Windowmode")
	    .value("Window_None",Renderinstance::Window_None)
	    .value("Window_Normal",Renderinstance::Window_Normal)
	    .value("Window_Fullscreen",Renderinstance::Window_Fullscreen)
    ;

	enum_<Renderinstance::UsedRenderer>("UsedRenderer")
	    .value("UsedRenderer_Opengl",Renderinstance::UsedRenderer_Opengl)
	    .value("UsedRenderer_SWRasterizer",Renderinstance::UsedRenderer_SWRasterizer)
	    .value("UsedRenderer_Raytracer",Renderinstance::UsedRenderer_Raytracer)
    ;
}
