#ifndef OVIS_VIDEO_PROPERTYNODE_HH_INCLUDED
#define OVIS_VIDEO_PROPERTYNODE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/managedmemobj.hh"
#include "../base/refcounted.hh"
#include "../base/streamable.hh"

#include "propertytable.hh"
#include "scenenode.hh"

#include <deque>
#include <vector>

namespace ovis {
namespace video {

	class Propertynode:public Scenenode
	{
	public:
		Propertynode(const base::String& identifier);
		Propertynode(const Propertynode &srcPropertynode);
		~Propertynode();

		void onVisit(Visitcontext& visitcontext);

		void spawn(base::Streamable& datasource);
		void serialize(base::Streamable& datasink) const;

		const char *type() const;

		virtual void clone(const Propertynode &srcPropertynode);

		virtual Propertynode& operator =(const Propertynode& src);

		Propertytable& properties();
		const Propertytable& properties() const;
	protected:
		Propertytable m_Properties;
	};

}
}

#endif
