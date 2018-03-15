#include "ee2/WxStageDropTarget.h"
#include "ee2/WxStageCanvas.h"
#include "ee2/InsertNodeAO.h"

#include <ee0/MessageID.h>
#include <ee0/VariantSet.h>
#include <ee0/CameraHelper.h>
#include <ee0/WxDropTarget.h>
#include <ee0/WxLibraryPanel.h>
#include <ee0/WxLibraryItem.h>
#include <ee0/CompNodeEditor.h>
#include <ee0/MsgHelper.h>
#include <ee0/WxStagePage.h>
#include <ee0/SubjectMgr.h>

#include <guard/check.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <painting2/OrthoCamera.h>
#include <gum/StringHelper.h>
#include <ns/NodeFactory.h>

namespace ee2
{

WxStageDropTarget::WxStageDropTarget(ee0::WxLibraryPanel* library, ee0::WxStagePage* stage)
	: WxDropTarget()
	, m_library(library)
	, m_stage(stage)
{
}

void WxStageDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	CU_VEC<CU_STR> keys;
	gum::StringHelper::Split(text.ToStdString().c_str(), ",", keys);

	if (keys.size() <= 1) {
		return;
	}

	auto& cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage->GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(*cam, x, y);

	for (int i = 1, n = keys.size(); i < n; ++i)
	{
		int idx = std::stoi(keys[i].c_str());
		auto item = m_library->GetItem(idx);
		if (!item) {
			continue;
		}

		auto node = ns::NodeFactory::Create(item->GetFilepath());
		if (!node) {
			continue;
		}

		InitNodeComp(node, pos, item->GetFilepath());

		InsertNode(node);
	}

	m_stage->GetSubjectMgr()->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxStageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
}

void WxStageDropTarget::InsertNode(n0::SceneNodePtr& node)
{
	bool succ = ee0::MsgHelper::InsertNode(*m_stage->GetSubjectMgr(), node);
	GD_ASSERT(succ, "no MSG_INSERT_SCENE_NODE");

	m_stage->GetImpl().GetEditRecord().Add(std::make_shared<InsertNodeAO>(
		m_stage->GetSubjectMgr(), node));
	ee0::MsgHelper::SetEditorDirty(*m_stage->GetSubjectMgr(), true);
}

void WxStageDropTarget::InitNodeComp(const n0::SceneNodePtr& node, 
	                                 const sm::vec2& pos,
	                                 const std::string& filepath)
{
	// transform
	auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
	// todo
	//auto parent = node->GetParent();
	//if (parent) {
	//	auto p_pos = parent->GetUniqueComp<n2::CompTransform>().GetTrans().GetMatrix() * sm::vec2(0, 0);
	//	ctrans.SetPosition(node, pos - p_pos);
	//} else {
	//	ctrans.SetPosition(node, pos);
	//}
	ctrans.SetPosition(*node, pos);

	// editor
	auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetFilepath(filepath);
}

}