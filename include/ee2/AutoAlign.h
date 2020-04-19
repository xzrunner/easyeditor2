#pragma once

#include <ee0/GameObj.h>

#include <SM_Vector.h>

#include <vector>

namespace ur2 { class Device; class Context; }
namespace ee0 { class SceneNodeContainer; }
ECS_WORLD_DECL

namespace ee2
{

class AutoAlign
{
public:
	AutoAlign(ee0::SceneNodeContainer& objs);

	void Align(ECS_WORLD_PARAM const std::vector<ee0::GameObj>& objs);
	void Align(ECS_WORLD_PARAM const ee0::GameObj& src, const ee0::GameObj& dst);

	void Draw(const ur2::Device& dev, ur2::Context& ctx, float cam_scale) const;

	void SetInvisible();

	bool IsOpen() const { return m_open; }
	void SetOpen(bool open) { m_open = open; }

private:
	ee0::SceneNodeContainer& m_objs;

	bool m_open;
	sm::vec2 m_hor[2], m_ver[2];

}; // AutoAlign

}