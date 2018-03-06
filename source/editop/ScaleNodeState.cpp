#include "ScaleNodeState.h"
#include "Symbol.h"
#include "Sprite.h"
#include "CombineAOP.h"
#include "TranslateSpriteAOP.h"
#include "ScaleSpriteAOP.h"
#include "panel_msg.h"
#include "Math2D.h"
#include "EditSprMsg.h"

#include <SM_Calc.h>

namespace ee2
{

ScaleNodeState::ScaleNodeState(const SprPtr& spr, const NodeCtrlPoint::Node& ctrl_node)
	: m_spr(spr)
	, m_ctrl_node(ctrl_node)
{
	m_first_pos = m_spr->GetPosition();
	m_first_scale = m_spr->GetScale();
}

void ScaleNodeState::OnMouseRelease(int x, int y)
{
	CombineAOP* comb = new CombineAOP();

	comb->Insert(new TranslateSpriteAOP(m_spr, m_spr->GetPosition() - m_first_pos));
	comb->Insert(new ScaleSpriteAOP(m_spr, m_spr->GetScale(), m_first_scale));

	EditAddRecordSJ::Instance()->Add(comb);

	EditSprMsg::SetScale(m_spr.get(), m_spr->GetPosition(), m_spr->GetScale());
}

bool ScaleNodeState::OnMouseDrag(int x, int y)
{
	Scale(pos);
	return true;
}

void ScaleNodeState::Scale(const sm::vec2& curr)
{
	if (!m_spr) {
		return;
	}

	sm::vec2 ctrls[8];
	NodeCtrlPoint::GetNodeCtrlPoints(*m_spr, ctrls);
	
	sm::vec2 ori = ctrls[m_ctrl_node.type];
	sm::vec2 center = m_spr->GetPosition() + m_spr->GetOffset();
	sm::vec2 fix;
	sm::get_foot_of_perpendicular(center, ori, curr, &fix);

	float scale_times = sm::dis_pos_to_pos(center, fix) / sm::dis_pos_to_pos(center, ori);
	if (fabs(scale_times - 1) < FLT_EPSILON) {
		return;
	}

	sm::vec2 st(1, 1);
	if (m_ctrl_node.type == NodeCtrlPoint::UP || m_ctrl_node.type == NodeCtrlPoint::DOWN) {
		st.y = scale_times;
	} else if (m_ctrl_node.type == NodeCtrlPoint::LEFT || m_ctrl_node.type == NodeCtrlPoint::RIGHT) {
		st.x = scale_times;
	} else {
		st.Set(scale_times, scale_times);
	}
	SetScaleTimes(st);
}

void ScaleNodeState::SetScaleTimes(const sm::vec2& st)
{
	sm::vec2 scale = m_spr->GetScale();
	scale *= st;
	m_spr->SetScale(scale);
}

}