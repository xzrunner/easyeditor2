#pragma once

#include <ee0/EditOpState.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

namespace pt0 { class Camera; }
ECS_WORLD_DECL

namespace ee2
{

class OffsetNodeState : public ee0::EditOpState
{
public:
	OffsetNodeState(pt0::Camera& cam, const ee0::SubjectMgrPtr& sub_mgr, 
		ECS_WORLD_PARAM const ee0::GameObj& obj);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	pt0::Camera& m_cam;

	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	ee0::GameObj m_obj;

	sm::vec2 m_old_offset;

}; // OffsetNodeState

}