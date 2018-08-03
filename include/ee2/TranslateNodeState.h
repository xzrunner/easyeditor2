#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

ECS_WORLD_DECL

namespace ee2
{

class TranslateNodeState : public ee0::EditOpState
{
public:
	TranslateNodeState(const std::shared_ptr<pt0::Camera>& camera, 
		const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM 
		const ee0::SelectionSet<ee0::GameObjWithPos>& selection, const sm::vec2& first_pos);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

	virtual bool OnDirectionKeyDown(int type) override;

private:
	void Translate(const sm::vec2& offset);

private:
	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	const ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	bool m_dirty;
	
}; // TranslateState

}