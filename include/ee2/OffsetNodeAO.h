#pragma once

#include <ee0/AtomicOP.h>

#include <SM_Vector.h>
#include <node0/typedef.h>

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class OffsetNodeAO : public ee0::AtomicOP
{
public:
	OffsetNodeAO(ee0::SubjectMgr& sub_mgr, const n0::SceneNodePtr& node,
		const sm::vec2& new_offset, const sm::vec2& old_offset);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgr& m_sub_mgr;

	n0::SceneNodePtr m_node;

	sm::vec2 m_new_offset, m_old_offset;

}; // OffsetNodeAO

}