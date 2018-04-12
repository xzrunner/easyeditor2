#include "ee2/ScaleNodeState.h"
#include "ee2/CombineAO.h"
#include "ee2/TranslateNodeAO.h"
#include "ee2/ScaleNodeAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/EditRecord.h>
#include <ee0/MsgHelper.h>

#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

ScaleNodeState::ScaleNodeState(pt2::Camera& cam, 
	                           ee0::EditRecord& record,
	                           const ee0::SubjectMgrPtr& sub_mgr, 
	                           const ee0::GameObj& obj,
	                           const NodeCtrlPoint::Node& ctrl_point)
	: m_cam(cam)
	, m_record(record)
	, m_sub_mgr(sub_mgr)
	, m_obj(obj)
	, m_ctrl_point(ctrl_point)
{
	auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
	m_first_pos = ctrans.GetTrans().GetPosition();
	m_first_scale = ctrans.GetTrans().GetScale();
}

bool ScaleNodeState::OnMouseRelease(int x, int y)
{
	// record
	auto comb = std::make_shared<CombineAO>();

	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	comb->Add(std::make_shared<TranslateNodeAO>(m_sub_mgr, m_obj, ctrans.GetTrans().GetPosition() - m_first_pos));
	comb->Add(std::make_shared<ScaleNodeAO>(m_sub_mgr, m_obj, ctrans.GetTrans().GetScale(), m_first_scale));

	m_record.Add(comb);

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
	if (!m_obj) {
		return;
	}

	sm::vec2 ctrls[8];
	NodeCtrlPoint::GetNodeCtrlPoints(m_obj, ctrls);
	
	sm::vec2 ori = ctrls[m_ctrl_point.type];
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	sm::vec2 center = ctrans.GetTrans().GetPosition() + ctrans.GetTrans().GetOffset();
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
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	sm::vec2 scale = ctrans.GetTrans().GetScale();
	scale *= st;
	ctrans.SetScale(*m_obj, scale);
}

}