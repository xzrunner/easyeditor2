#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/typedef.h>

#include <SM_Vector.h>
#include <node0/typedef.h>

namespace ee2
{

class ShearNodeAO : public ee0::AtomicOP
{
public:
	ShearNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const n0::SceneNodePtr& node,
		const sm::vec2& new_shear, const sm::vec2& old_shear);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	n0::SceneNodePtr m_node;

	sm::vec2 m_old_shear, m_new_shear;

}; // ShearNodeAO 

}