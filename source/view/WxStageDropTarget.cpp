#include "ee2/WxStageDropTarget.h"
#include "ee2/WxStagePage.h"
#include "ee2/NodeFactory.h"
#include "ee2/WxStageCanvas.h"

#include <ee0/MessageID.h>
#include <ee0/VariantSet.h>
#include <ee0/CameraHelper.h>
#include <ee0/WxDropTarget.h>
#include <ee0/WxLibraryPanel.h>
#include <ee0/WxLibraryItem.h>
#include <ee0/CompNodeEditor.h>
#include <ee0/MsgHelper.h>

#include <guard/check.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#include <painting2/OrthoCamera.h>
#include <gum/StringHelper.h>
#include <gum/SymbolPool.h>

namespace ee2
{

WxStageDropTarget::WxStageDropTarget(ee0::WxLibraryPanel* library, WxStagePage* stage)
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

		auto sym = gum::SymbolPool::Instance()->Fetch(item->GetFilepath().c_str());
		bool handled = OnDropSymbol(sym, pos);
		if (handled) {
			continue;
		}

		auto node = NodeFactory::Instance()->Create(sym);
		if (!node) {
			continue;
		}

		InsertNode(node);
		InitNodeComp(node, pos, item->GetFilepath());
	}

	m_stage->GetSubjectMgr().NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxStageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
}

void WxStageDropTarget::InsertNode(n0::SceneNodePtr& node)
{
	bool succ = ee0::MsgHelper::InsertNode(m_stage->GetSubjectMgr(), node);
	GD_ASSERT(succ, "no MSG_INSERT_SCENE_NODE");
}

void WxStageDropTarget::InitNodeComp(const n0::SceneNodePtr& node, 
	                                 const sm::vec2& pos,
	                                 const std::string& filepath)
{
	// transform
	auto& ctrans = node->AddComponent<n2::CompTransform>();
	auto parent = node->GetParent();
	if (parent) {
		auto p_pos = parent->GetComponent<n2::CompTransform>().GetTrans().GetMatrix() * sm::vec2(0, 0);
		ctrans.GetTrans().SetPosition(pos - p_pos);
	} else {
		ctrans.GetTrans().SetPosition(pos);
	}

	// bounding box
	auto& cbounding = node->GetComponent<n2::CompBoundingBox>();
	cbounding.Build(ctrans.GetTrans().GetSRT());

	// editor
	auto& ceditor = node->GetComponent<ee0::CompNodeEditor>();
	ceditor.SetFilepath(filepath);
}

}