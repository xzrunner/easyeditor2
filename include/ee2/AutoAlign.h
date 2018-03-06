#pragma once

#include <SM_Vector.h>
#include <node0/typedef.h>

#include <vector>

namespace ee0 { class NodeContainer; }

namespace ee2
{

class AutoAlign
{
public:
	AutoAlign(ee0::NodeContainer& nodes);

	void Align(const std::vector<n0::SceneNodePtr>& nodes);
	void Align(const n0::SceneNode& src, n0::SceneNode& dst);

	void Draw() const;

	void SetInvisible();

	bool IsOpen() const { return m_open; }
	void SetOpen(bool open) { m_open = open; }

private:
	ee0::NodeContainer& m_nodes;

	bool m_open;
	sm::vec2 m_hor[2], m_ver[2];

}; // AutoAlign

}