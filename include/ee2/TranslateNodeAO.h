#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>
#include <node0/NodeWithPos.h>

#include <vector>

namespace ee2
{

class TranslateNodeAO : public ee0::AtomicOP
{
public:
	TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr, 
		const ee0::SelectionSet<n0::NodeWithPos>& selection, const sm::vec2& offset);
	TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr, 
		const ee0::GameObj& obj, const sm::vec2& offset);
	TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr, 
		const std::vector<ee0::GameObj>& objs, const sm::vec2& offset);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	std::vector<ee0::GameObj> m_objs;

	sm::vec2 m_offset;

}; // TranslateNodeAO 

}