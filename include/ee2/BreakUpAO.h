#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>

#include <node0/NodeWithPos.h>

namespace ee2
{

class BreakUpAO : public ee0::AtomicOP
{
public:
	BreakUpAO(const ee0::SubjectMgrPtr& sub_mgr,
		const ee0::SelectionSet<n0::NodeWithPos>& selection);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	void CopyFromSelection(std::vector<n0::NodeWithPos>& nodes) const;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	const ee0::SelectionSet<n0::NodeWithPos>& m_selection;

}; // BreakUpAO

}