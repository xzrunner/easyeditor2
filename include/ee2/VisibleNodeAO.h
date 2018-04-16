#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <vector>

ECS_WORLD_DECL

namespace ee2
{

class VisibleNodeAO : public ee0::AtomicOP
{
public:
	VisibleNodeAO(const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM
		const std::vector<ee0::GameObj>& objs);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	std::vector<ee0::GameObj> m_objs;

}; // VisibleNodeAO

}