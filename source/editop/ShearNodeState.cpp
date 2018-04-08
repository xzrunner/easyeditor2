#include "ee2/ShearNodeState.h"

#include <ee0/CameraHelper.h>

#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>

namespace ee2
{

ShearNodeState::ShearNodeState(pt2::Camera& cam, 
	                           const ee0::GameObj& obj,
	                           const NodeCtrlPoint::Node& ctrl_point)
	: m_cam(cam)
	, m_obj(obj)
	, m_ctrl_point(ctrl_point)
{
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	m_first_shear = ctrans.GetTrans().GetShear();
}

bool ShearNodeState::OnMouseRelease(int x, int y)
{
	return false;
}

bool ShearNodeState::OnMouseDrag(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	Shear2(pos);
	return true;
}

void ShearNodeState::Shear(const sm::vec2& curr)
{
	if (!m_obj) {
		return;
	}

	// fix pos
	sm::vec2 pos;
	sm::vec2 ctrls[8];
	NodeCtrlPoint::GetNodeCtrlPoints(*m_obj, ctrls);
	if (m_ctrl_point.type == NodeCtrlPoint::UP) {
		sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_UP], ctrls[NodeCtrlPoint::RIGHT_UP], curr, &pos);
	} else if (m_ctrl_point.type == NodeCtrlPoint::DOWN) {
		sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_DOWN], ctrls[NodeCtrlPoint::RIGHT_DOWN], curr, &pos);
	} else if (m_ctrl_point.type == NodeCtrlPoint::LEFT) {
		sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_UP], ctrls[NodeCtrlPoint::LEFT_DOWN], curr, &pos);
	} else if (m_ctrl_point.type == NodeCtrlPoint::RIGHT) {
		sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::RIGHT_UP], ctrls[NodeCtrlPoint::RIGHT_DOWN], curr, &pos);
	}

	// M * p = p'
	//
	// x' = e0 * x + e4 * y + e12
	// y' = e1 * x + e5 * y + e13
	// 
	// e0 = c * sx - ky * s * sy
	// e4 = kx * c * sx - s * sy
	// e12 = px
	//
	// pos.x = (c * sx - ky * s * sy) * x + (kx * c * sx - s * sy) * y + px
	// pos.y = (s * sx + ky * c * sy) * x + (kx * s * sx + c * sy) * y + py
	// pos.x = c*sx*x - ky*s*sy*x + kx*c*sx*y - s*sy*y + px
	// pos.y = s*sx*x + ky*c*sy*x + kx*s*sx*y + c*sy*y + py
	// 
	// kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y)
	// kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y)
	// ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x)
	// ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x)
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	auto& srt = ctrans.GetTrans().GetSRT();
	float c = cos(srt.angle), s = sin(srt.angle);
	float sx = srt.scale.x, sy = srt.scale.y;
	float px = srt.position.x, py = srt.position.y;
	float kx = srt.shear.x,
		  ky = srt.shear.y;

	auto& cbb = m_obj->GetUniqueComp<n2::CompBoundingBox>();
	const sm::rect& r = cbb.GetSize();
//  	pos.x -= px;
//  	pos.y -= py;

	const sm::vec2& offset = srt.offset;
// 	offset.x += px - r.CenterX();
// 	offset.y += py - r.CenterY();

// 	px = r.CenterX();
// 	py = r.CenterY();

	float x, y;
	sm::vec2 sz = r.Size();
	float hw = sz.x * 0.5f, hh = sz.y * 0.5f;
	if (m_ctrl_point.type == NodeCtrlPoint::UP)
	{
		x = 0; y = hh - offset.y;		
		if (c != 0)
			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
		else
			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
	}
	else if (m_ctrl_point.type == NodeCtrlPoint::DOWN)
	{
		x = 0; y = -hh - offset.y;
		if (c != 0)
			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
		else
			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
	}
	else if (m_ctrl_point.type == NodeCtrlPoint::LEFT)
	{
		x = -hw - offset.x; y = 0;
		if (s != 0)
			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
		else
			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
	}
	else if (m_ctrl_point.type == NodeCtrlPoint::RIGHT)
	{
		x = hw - offset.x; y = 0;
		if (s != 0)
			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
		else
			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
	}

	ctrans.SetShear(*m_obj, sm::vec2(kx, ky));
}

void ShearNodeState::Shear2(const sm::vec2& curr)
{
	if (!m_obj) {
		return;
	}

	auto& cbb = m_obj->GetUniqueComp<n2::CompBoundingBox>();
	auto& region = cbb.GetSize();

	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	auto& srt = ctrans.GetTrans().GetSRT();

	sm::vec2 sz = region.Size();
	float hw = sz.x * 0.5f, 
		  hh = sz.y * 0.5f;
	float kx = srt.shear.x,
		  ky = srt.shear.y;
	float sx = srt.scale.x,
		  sy = srt.scale.y;
	sm::vec2 ctrls[8];
	NodeCtrlPoint::GetNodeCtrlPoints(*m_obj, ctrls);

	sm::vec2 center = (ctrls[NodeCtrlPoint::LEFT] + ctrls[NodeCtrlPoint::RIGHT]) * 0.5f;

	switch (m_ctrl_point.type)
	{
	case NodeCtrlPoint::UP: case NodeCtrlPoint::DOWN:
		{
			sm::vec2 ori, now;
			if (m_ctrl_point.type == NodeCtrlPoint::UP) {
				sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_UP], ctrls[NodeCtrlPoint::RIGHT_UP], center, &ori);
				sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_UP], ctrls[NodeCtrlPoint::RIGHT_UP], curr, &now);
			} else {
				sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_DOWN], ctrls[NodeCtrlPoint::RIGHT_DOWN], center, &ori);
				sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_DOWN], ctrls[NodeCtrlPoint::RIGHT_DOWN], curr, &now);
			}

			float dis = sm::dis_pos_to_pos(ori, now);
			kx = dis / hh;
			if ((center - ori).Cross(now - ori) < 0) {
				kx = -kx;
			}
			kx /= sx;
			ctrans.SetShear(*m_obj, sm::vec2(kx, ky));
		}
		break;
	case NodeCtrlPoint::LEFT: case NodeCtrlPoint::RIGHT:
		{
			sm::vec2 ori, now;
			if (m_ctrl_point.type == NodeCtrlPoint::LEFT) {
				sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_DOWN], ctrls[NodeCtrlPoint::LEFT_UP], center, &ori);
				sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::LEFT_DOWN], ctrls[NodeCtrlPoint::LEFT_UP], curr, &now);
			} else {
				sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::RIGHT_DOWN], ctrls[NodeCtrlPoint::RIGHT_UP], center, &ori);
				sm::get_foot_of_perpendicular(ctrls[NodeCtrlPoint::RIGHT_DOWN], ctrls[NodeCtrlPoint::RIGHT_UP], curr, &now);
			}

			float dis = sm::dis_pos_to_pos(ori, now);
			ky = dis / hw;
			if ((center - ori).Cross(now - ori) > 0) {
				ky = -ky;
			}
			ky /= sy;
			ctrans.SetShear(*m_obj, sm::vec2(kx, ky));
		}
		break;
	}
}

}