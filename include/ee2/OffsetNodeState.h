#pragma once

#include <ee0/EditOpState.h>

#include <SM_Vector.h>
#include <node0/typedef.h>

namespace pt2 { class Camera; }

namespace ee2
{

class OffsetNodeState : public ee0::EditOpState
{
public:
	OffsetNodeState(pt2::Camera& cam, const n0::SceneNodePtr& node);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	pt2::Camera& m_cam;

	const n0::SceneNodePtr& m_node;

	sm::vec2 m_old_offset;

}; // OffsetNodeState

}