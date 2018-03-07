#pragma once

#include <ee0/AtomicOP.h>

#include <node0/typedef.h>

#include <vector>

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class ReorderNodeAO : public ee0::AtomicOP
{
public:
	ReorderNodeAO(ee0::SubjectMgr& sub_mgr, 
		const std::vector<n0::SceneNodePtr>& nodes, bool up);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	void Reorder(bool up);

private:
	ee0::SubjectMgr& m_sub_mgr;

	std::vector<n0::SceneNodePtr> m_nodes;

	bool m_up;

}; // ReorderNodeAO

}