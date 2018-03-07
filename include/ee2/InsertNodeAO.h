#pragma once

#include <ee0/AtomicOP.h>

#include <node0/typedef.h>

#include <vector>

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class InsertNodeAO : public ee0::AtomicOP
{
public:
	InsertNodeAO(ee0::SubjectMgr& sub_mgr, const n0::SceneNodePtr& node);
	InsertNodeAO(ee0::SubjectMgr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgr& m_sub_mgr;

	std::vector<n0::SceneNodePtr> m_nodes;

}; // InsertNodeAO

}