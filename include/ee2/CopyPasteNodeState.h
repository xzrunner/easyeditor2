#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

#include <vector>

ECS_WORLD_DECL

namespace ee2
{

class CopyPasteNodeState : public ee0::EditOpState
{
public:
	CopyPasteNodeState(const std::shared_ptr<pt0::Camera>& camera, 
		const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM 
		ee0::SelectionSet<ee0::GameObjWithPos>& selection);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	sm::vec2 m_last_pos;

	std::vector<ee0::GameObjWithPos> m_objs;
	
}; // CopyPasteNodeState

}