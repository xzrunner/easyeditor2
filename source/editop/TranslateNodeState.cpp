#include "ee2/TranslateNodeState.h"
#include "ee2/TranslateNodeAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/KeyType.h>
#include <ee0/EditRecord.h>
#include <ee0/MsgHelper.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <ecsx/World.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

TranslateNodeState::TranslateNodeState(pt2::Camera& cam, 
	                                   ee0::EditRecord& record,
	                                   const ee0::SubjectMgrPtr& sub_mgr,
#ifdef GAME_OBJ_ECS
	                                   ecsx::World& world,
#endif // GAME_OBJ_ECS
		                               const ee0::SelectionSet<ee0::GameObjWithPos>& selection,
		                               const sm::vec2& first_pos)
	: m_cam(cam)
	, m_record(record)
	, m_sub_mgr(sub_mgr)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_selection(selection)
	, m_dirty(false)
{
	m_first_pos = m_last_pos = first_pos;
}

bool TranslateNodeState::OnMousePress(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	m_first_pos = m_last_pos = pos;
	return false;
}

bool TranslateNodeState::OnMouseRelease(int x, int y)
{
	m_last_pos.MakeInvalid();
	if (!m_dirty) {
		return false;
	}

	m_dirty = false;

	// record
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	if (pos != m_first_pos)
	{
		sm::vec2 offset = pos - m_first_pos;
#ifndef GAME_OBJ_ECS
		m_record.Add(std::make_shared<TranslateNodeAO>(m_sub_mgr, m_selection, offset));
#else
		m_record.Add(std::make_shared<TranslateNodeAO>(m_sub_mgr, m_world, m_selection, offset));
#endif // GAME_OBJ_ECS
		ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);
	}

	return false;
}

bool TranslateNodeState::OnMouseDrag(int x, int y)
{
	if (m_selection.IsEmpty() || !m_last_pos.IsValid()) {
		return false;
	}

	m_dirty = true;

	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	auto offset = pos - m_last_pos;
	Translate(offset);

	m_last_pos = pos;

	return true;
}

bool TranslateNodeState::OnDirectionKeyDown(int type)
{
	if (m_selection.IsEmpty()) {
		return false;
	}

	sm::vec2 offset;
	float len = (type & ee0::KEY_SHIFT) ? 10.0f : 1.0f;
	switch (type & 0xf)
	{
	case ee0::KEY_LEFT:
		offset.Set(-len, 0);
		break;
	case ee0::KEY_RIGHT:
		offset.Set(len, 0);
		break;
	case ee0::KEY_DOWN:
		offset.Set(0, -len);
		break;
	case ee0::KEY_UP:
		offset.Set(0, len);
		break;
	}

	if (offset.x == 0 && offset.y == 0) {
		return false;
	}

	Translate(offset);

	return true;
}

void TranslateNodeState::Translate(const sm::vec2& offset)
{
	m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
#ifndef GAME_OBJ_ECS
		auto& ctrans = opw.GetNode()->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*opw.GetNode(), ctrans.GetTrans().GetPosition() + offset);
#else
		e2::SysTransform::Translate(m_world, opw, offset);
#endif // GAME_OBJ_ECS
		return true;
	});
}

}