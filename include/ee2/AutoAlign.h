#pragma once

#include <ee0/GameObj.h>

#include <SM_Vector.h>

#include <vector>

namespace ee0 { class NodeContainer; }

namespace ee2
{

class AutoAlign
{
public:
	AutoAlign(ee0::NodeContainer& objs);

	void Align(const std::vector<ee0::GameObj>& objs);
	void Align(const n0::SceneNode& src, n0::SceneNode& dst);

	void Draw() const;

	void SetInvisible();

	bool IsOpen() const { return m_open; }
	void SetOpen(bool open) { m_open = open; }

private:
	ee0::NodeContainer& m_objs;

	bool m_open;
	sm::vec2 m_hor[2], m_ver[2];

}; // AutoAlign

}