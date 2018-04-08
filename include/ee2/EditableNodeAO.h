#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <vector>

namespace ee2
{

class EditableNodeAO : public ee0::AtomicOP
{
public:
	EditableNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const std::vector<ee0::GameObj>& objs);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	std::vector<ee0::GameObj> m_objs;

}; // EditableNodeAO

}