#pragma once

#include <ee0/EditOpState.h>

#include <SM_Vector.h>
#include <node0/typedef.h>

namespace ee0 { class EditRecord; class SubjectMgr; }
namespace pt2 { class Camera; }

namespace ee2
{

class OffsetNodeState : public ee0::EditOpState
{
public:
	OffsetNodeState(pt2::Camera& cam, ee0::EditRecord& record, 
		ee0::SubjectMgr& sub_mgr, const n0::SceneNodePtr& node);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	pt2::Camera& m_cam;

	ee0::EditRecord& m_record;
	ee0::SubjectMgr& m_sub_mgr;

	n0::SceneNodePtr m_node;

	sm::vec2 m_old_offset;

}; // OffsetNodeState

}