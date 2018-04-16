#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

ECS_WORLD_DECL

namespace ee2
{

class BuildGroupAO : public ee0::AtomicOP
{
public:
	BuildGroupAO(const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM
		const ee0::SelectionSet<ee0::GameObjWithPos>& selection);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	void CopyFromSelection(std::vector<ee0::GameObjWithPos>& objs) const;

private:
	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	const ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

}; // BuildGroupAO

}