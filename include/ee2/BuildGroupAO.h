#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/SelectionSet.h>

#include <node0/typedef.h>

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class BuildGroupAO : public ee0::AtomicOP
{
public:
	BuildGroupAO(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::SceneNode>& selection);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	void BuildGroup();
	void BreakUpGroup();

private:
	ee0::SubjectMgr& m_sub_mgr;

	const ee0::SelectionSet<n0::SceneNode>& m_selection;

}; // BuildGroupAO

}