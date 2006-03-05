/*#include <boost/python.hpp>
#include "renderinstance.hh"

using namespace boost::python;
using namespace ovis;
		
BOOST_PYTHON_MODULE(libovislib) {

    class_<Renderinstance>("Renderinstance",init<>())
       // .def("perspectiveViewport", &Renderinstance::perspectiveViewport)
        .def("testinitRenderer", &Renderinstance::testinitRenderer)
        .def("initRenderer", &Renderinstance::initRenderer)
        .def("shutdownRenderer", &Renderinstance::shutdownRenderer)
        .def("windowmode", &Renderinstance::windowmode)
        .def("interactive", &Renderinstance::interactive)
        .def("fetchSceneFromFile", &Renderinstance::fetchSceneFromFile)
    ;

	enum_<Renderinstance::Windowmode>("Windowmode")
	    .value("None",Renderinstance::Window_None)
	    .value("Normal",Renderinstance::Window_Normal)
	    .value("Fullscreen",Renderinstance::Window_Fullscreen)
    ;

	enum_<Renderinstance::UsedRenderer>("UsedRenderer")
	    .value("Opengl",Renderinstance::UsedRenderer_Opengl)
	    .value("SWRasterizer",Renderinstance::UsedRenderer_SWRasterizer)
	    .value("Raytracer",Renderinstance::UsedRenderer_Raytracer)
    ;
}
*/
