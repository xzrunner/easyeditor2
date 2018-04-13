#include "ee2/WxStageDropTarget.h"
#include "ee2/WxStageCanvas.h"
#include "ee2/InsertNodeAO.h"

#include <ee0/MessageID.h>
#include <ee0/VariantSet.h>
#include <ee0/CameraHelper.h>
#include <ee0/WxDropTarget.h>
#include <ee0/WxLibraryPanel.h>
#include <ee0/WxLibraryItem.h>
#ifndef GAME_OBJ_ECS
#include <ee0/CompNodeEditor.h>
#else
#include <ee0/CompEntityEditor.h>
#endif // GAME_OBJ_ECS
#include <ee0/MsgHelper.h>
#include <ee0/WxStagePage.h>
#include <ee0/SubjectMgr.h>

#include <guard/check.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <ns/NodeFactory.h>
#else
#include <es/EntityFactory.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS
#include <painting2/OrthoCamera.h>
#include <sx/StringHelper.h>

namespace ee2
{

WxStageDropTarget::WxStageDropTarget(
#ifdef GAME_OBJ_ECS
	ecsx::World& world,
#endif // GAME_OBJ_ECS
	ee0::WxLibraryPanel* library, 
	ee0::WxStagePage* stage
	)
	: WxDropTarget()
	, m_library(library)
	, m_stage(stage)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
{
}

void WxStageDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	std::vector<std::string> keys;
	sx::StringHelper::Split(text.ToStdString().c_str(), ",", keys);

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

#ifndef GAME_OBJ_ECS
		auto obj = ns::NodeFactory::Create(item->GetFilepath());
		if (!obj) {
			continue;
		}
#else
		auto obj = es::EntityFactory::Create(m_world, item->GetFilepath());
#endif // GAME_OBJ_ECS

		InitNodeComp(obj, pos, item->GetFilepath());

		InsertNode(obj);
	}

	m_stage->GetSubjectMgr()->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxStageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
}

void WxStageDropTarget::InsertNode(ee0::GameObj& obj)
{
	bool succ = ee0::MsgHelper::InsertNode(*m_stage->GetSubjectMgr(), obj);
	GD_ASSERT(succ, "no MSG_INSERT_SCENE_NODE");

	m_stage->GetImpl().GetEditRecord().Add(std::make_shared<InsertNodeAO>(
		m_stage->GetSubjectMgr(), obj));
	ee0::MsgHelper::SetEditorDirty(*m_stage->GetSubjectMgr(), true);
}

void WxStageDropTarget::InitNodeComp(const ee0::GameObj& obj, 
	                                 const sm::vec2& pos,
	                                 const std::string& filepath)
{
#ifndef GAME_OBJ_ECS
	// transform
	auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
	// todo
	//auto parent = obj->GetParent();
	//if (parent) {
	//	auto p_pos = parent->GetUniqueComp<n2::CompTransform>().GetTrans().GetMatrix() * sm::vec2(0, 0);
	//	ctrans.SetPosition(obj, pos - p_pos);
	//} else {
	//	ctrans.SetPosition(obj, pos);
	//}
	ctrans.SetPosition(*obj, pos);

	// editor
	auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetFilepath(filepath);
#else
	e2::SysTransform::SetPosition(m_world, obj, pos);

	// editor
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	ceditor.filepath = filepath;
#endif // GAME_OBJ_ECS
}

}