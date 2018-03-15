#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/typedef.h>

#include <SM_Vector.h>
#include <node0/typedef.h>

namespace ee2
{

class ScaleNodeAO : public ee0::AtomicOP
{
public:
	ScaleNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const n0::SceneNodePtr& node,
		const sm::vec2& new_scale, const sm::vec2& old_scale);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	n0::SceneNodePtr m_node;

	sm::vec2 m_old_scale, m_new_scale;

}; // ScaleNodeAO 

}