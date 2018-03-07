#include "ee2/ReorderNodeAO.h"

namespace ee2
{

ReorderNodeAO::ReorderNodeAO(ee0::SubjectMgr& sub_mgr,
	                         const std::vector<n0::SceneNodePtr>& nodes, 
	                         bool up)
	: m_sub_mgr(sub_mgr)
	, m_nodes(nodes)
	, m_up(up)
{
}

void ReorderNodeAO::Undo()
{
	Reorder(!m_up);
}

void ReorderNodeAO::Redo()
{
	Reorder(m_up);
}

void ReorderNodeAO::Reorder(bool up)
{
	// todo
	//if (up) {
	//	for (int i = m_sprs.size() - 1; i >= 0; --i) {
	//		ReorderSpriteSJ::Instance()->Reorder(m_sprs[i], up);	
	//	}
	//} else {
	//	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
	//		ReorderSpriteSJ::Instance()->Reorder(m_sprs[i], up);
	//	}
	//}
}

}