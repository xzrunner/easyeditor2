#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

namespace ee2
{

class BreakUpAO : public ee0::AtomicOP
{
public:
	BreakUpAO(const ee0::SubjectMgrPtr& sub_mgr,
		const ee0::SelectionSet<ee0::GameObjWithPos>& selection);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	void CopyFromSelection(std::vector<ee0::GameObjWithPos>& objs) const;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	const ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

}; // BreakUpAO

}