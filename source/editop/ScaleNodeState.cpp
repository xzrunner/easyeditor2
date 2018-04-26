#include "ee2/ScaleNodeState.h"
#include "ee2/CombineAO.h"
#include "ee2/TranslateNodeAO.h"
#include "ee2/ScaleNodeAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>

#include <SM_Calc.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <entity0/World.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

ScaleNodeState::ScaleNodeState(pt2::Camera& cam, 
	                           const ee0::SubjectMgrPtr& sub_mgr, 
	                           ECS_WORLD_PARAM
	                           const ee0::GameObj& obj,
	                           const NodeCtrlPoint::Node& ctrl_point)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
	, m_ctrl_point(ctrl_point)
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
	m_first_pos = ctrans.GetTrans().GetPosition();
	m_first_scale = ctrans.GetTrans().GetScale();
#else
	m_first_pos = e2::SysTransform::GetPosition(m_world, m_obj);
	m_first_scale = e2::SysTransform::GetScale(m_world, m_obj);
#endif // GAME_OBJ_ECS
}

bool ScaleNodeState::OnMouseRelease(int x, int y)
{
	sm::vec2 new_pos, new_scale;
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	new_pos = ctrans.GetTrans().GetPosition();
	new_scale = ctrans.GetTrans().GetScale();
#else
	new_pos = e2::SysTransform::GetPosition(m_world, m_obj);
	new_scale = e2::SysTransform::GetScale(m_world, m_obj);
#endif // GAME_OBJ_ECS

	if (new_pos == m_first_pos && new_scale == m_first_scale) {
		return false;
	}

	// record
	auto comb = std::make_shared<CombineAO>();
	auto offset = new_pos - m_first_pos;
#ifndef GAME_OBJ_ECS
	comb->Add(std::make_shared<TranslateNodeAO>(m_sub_mgr, m_obj, offset));
	comb->Add(std::make_shared<ScaleNodeAO>(m_sub_mgr, m_obj, new_scale, m_first_scale));
#else
	comb->Add(std::make_shared<TranslateNodeAO>(m_sub_mgr, m_world, m_obj, offset));
	comb->Add(std::make_shared<ScaleNodeAO>(m_sub_mgr, m_world, m_obj, new_scale, m_first_scale));
#endif // GAME_OBJ_ECS

	ee0::MsgHelper::AddAtomicOP(*m_sub_mgr, comb);

	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	return false;
}

bool ScaleNodeState::OnMouseDrag(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	Scale(pos);
	return true;
}

void ScaleNodeState::Scale(const sm::vec2& curr)
{
#ifndef GAME_OBJ_ECS
	if (!m_obj) {
		return;
	}
#endif // GAME_OBJ_ECS

	sm::vec2 ctrls[8];
	NodeCtrlPoint::GetNodeCtrlPoints(ECS_WORLD_SELF_VAR m_obj, ctrls);
	
	sm::vec2 ori = ctrls[m_ctrl_point.type];
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	sm::vec2 center = ctrans.GetTrans().GetPosition() + ctrans.GetTrans().GetOffset();
#else
	auto center = e2::SysTransform::GetPosition(m_world, m_obj) + e2::SysTransform::GetOffset(m_world, m_obj);
#endif // GAME_OBJ_ECS
	sm::vec2 fix;
	sm::get_foot_of_perpendicular(center, ori, curr, &fix);

	float scale_times = sm::dis_pos_to_pos(center, fix) / sm::dis_pos_to_pos(center, ori);
	if (fabs(scale_times - 1) < FLT_EPSILON) {
		return;
	}

	sm::vec2 st(1, 1);
	if (m_ctrl_point.type == NodeCtrlPoint::UP || m_ctrl_point.type == NodeCtrlPoint::DOWN) {
		st.y = scale_times;
	} else if (m_ctrl_point.type == NodeCtrlPoint::LEFT || m_ctrl_point.type == NodeCtrlPoint::RIGHT) {
		st.x = scale_times;
	} else {
		st.Set(scale_times, scale_times);
	}
	SetScaleTimes(st);
}

void ScaleNodeState::SetScaleTimes(const sm::vec2& st)
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	sm::vec2 scale = ctrans.GetTrans().GetScale();
	scale *= st;
	ctrans.SetScale(*m_obj, scale);
#else
	auto scale = e2::SysTransform::GetScale(m_world, m_obj);
	scale *= st;
	e2::SysTransform::SetScale(m_world, m_obj, scale);
#endif // GAME_OBJ_ECS
}

}