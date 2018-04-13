#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

namespace ee2
{

class DownLayerNodeAO : public ee0::AtomicOP
{
public:
	DownLayerNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
		const ee0::SelectionSet<ee0::GameObjWithPos>& selection);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	const ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

}; // DownLayerNodeAO

}