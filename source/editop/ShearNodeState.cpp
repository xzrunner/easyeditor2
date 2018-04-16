#include "ee2/ShearNodeState.h"

#include <ee0/CameraHelper.h>

#include <SM_Calc.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#else
#include <ecsx/World.h>
#include <entity2/SysTransform.h>
#include <entity2/CompBoundingBox.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

ShearNodeState::ShearNodeState(pt2::Camera& cam, 
	                           ECS_WORLD_PARAM
	                           const ee0::GameObj& obj,
	                           const NodeCtrlPoint::Node& ctrl_point)
	: m_cam(cam)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
	, m_ctrl_point(ctrl_point)
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	m_first_shear = ctrans.GetTrans().GetShear();
#else
	m_first_shear = e2::SysTransform::GetShear(m_world, m_obj);
#endif // GAME_OBJ_ECS
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
#ifndef GAME_OBJ_ECS
	if (!m_obj) {
		return;
	}
#endif // GAME_OBJ_ECS

	// fix pos
	sm::vec2 pos;
	sm::vec2 ctrls[8];
	NodeCtrlPoint::GetNodeCtrlPoints(ECS_WORLD_SELF_VAR m_obj, ctrls);
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
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	auto& srt = ctrans.GetTrans().GetSRT();
	float c = cos(srt.angle), s = sin(srt.angle);
	float sx = srt.scale.x, sy = srt.scale.y;
	float px = srt.position.x, py = srt.position.y;
	float kx = srt.shear.x, ky = srt.shear.y;
	auto& offset = srt.offset;

	auto& cbb = m_obj->GetUniqueComp<n2::CompBoundingBox>();
	const sm::rect& r = cbb.GetSize();
#else
	float angle = e2::SysTransform::GetAngle(m_world, m_obj);
	float c = cos(angle), s = sin(angle);
	auto position = e2::SysTransform::GetPosition(m_world, m_obj);
	float px = position.x, py = position.y;
	auto scale = e2::SysTransform::GetScale(m_world, m_obj);
	float sx = scale.x, sy = scale.y;
	auto shear = e2::SysTransform::GetShear(m_world, m_obj);
	float kx = shear.x, ky = shear.y;
	auto offset = e2::SysTransform::GetOffset(m_world, m_obj);

	auto& cbb = m_world.GetComponent<e2::CompBoundingBox>(m_obj);
	auto& r = cbb.rect;
#endif // GAME_OBJ_ECS

//  	pos.x -= px;
//  	pos.y -= py;

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

#ifndef GAME_OBJ_ECS
	ctrans.SetShear(*m_obj, sm::vec2(kx, ky));
#else
	e2::SysTransform::SetShear(m_world, m_obj, sm::vec2(kx, ky));
#endif // GAME_OBJ_ECS
}

void ShearNodeState::Shear2(const sm::vec2& curr)
{
#ifndef GAME_OBJ_ECS
	if (!m_obj) {
		return;
	}
#endif // GAME_OBJ_ECS

#ifndef GAME_OBJ_ECS
	auto& cbb = m_obj->GetUniqueComp<n2::CompBoundingBox>();
	auto& region = cbb.GetSize();

	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	auto& srt = ctrans.GetTrans().GetSRT();
	float kx = srt.shear.x, ky = srt.shear.y;
	float sx = srt.scale.x, sy = srt.scale.y;

	sm::vec2 ctrls[8];
	NodeCtrlPoint::GetNodeCtrlPoints(m_obj, ctrls);
#else
	auto& cbb = m_world.GetComponent<e2::CompBoundingBox>(m_obj);
	auto& region = cbb.rect;

	auto shear = e2::SysTransform::GetShear(m_world, m_obj);
	float kx = shear.x, ky = shear.y;
	auto scale = e2::SysTransform::GetScale(m_world, m_obj);
	float sx = scale.x, sy = scale.y;

	sm::vec2 ctrls[8];
	NodeCtrlPoint::GetNodeCtrlPoints(m_world, m_obj, ctrls);
#endif // GAME_OBJ_ECS
	sm::vec2 sz = region.Size();
	float hw = sz.x * 0.5f, 
		  hh = sz.y * 0.5f;

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
#ifndef GAME_OBJ_ECS
			ctrans.SetShear(*m_obj, sm::vec2(kx, ky));
#else
			e2::SysTransform::SetShear(m_world, m_obj, sm::vec2(kx, ky));
#endif // GAME_OBJ_ECS
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
#ifndef GAME_OBJ_ECS
			ctrans.SetShear(*m_obj, sm::vec2(kx, ky));
#else
			e2::SysTransform::SetShear(m_world, m_obj, sm::vec2(kx, ky));
#endif // GAME_OBJ_ECS
		}
		break;
	}
}

}