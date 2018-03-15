#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/typedef.h>

#include <SM_Vector.h>
#include <node0/typedef.h>

#include <vector>

namespace ee2
{

class RotateNodeAO : public ee0::AtomicOP
{
public:
	RotateNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes,
		const sm::vec2& start, const sm::vec2& end);
	RotateNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes,
		float angle);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	std::vector<n0::SceneNodePtr> m_nodes;

	sm::vec2 m_start, m_end;

	float m_angle; // in rad

	bool m_inited;

}; // RotateNodeAO

}