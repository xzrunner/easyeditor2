#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

namespace pt0 { class Camera; }
ECS_WORLD_DECL

namespace ee2
{

class RotateNodeState : public ee0::EditOpState
{
public:
	RotateNodeState(pt0::Camera& cam, const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM 
		ee0::SelectionSet<ee0::GameObjWithPos>& selection, const sm::vec2& first_pos);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	void Rotate(const sm::vec2& dst);

private:
	pt0::Camera& m_cam;

	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	float m_angle;

}; // RotateNodeState

}