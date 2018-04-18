#include "ee2/AutoAlign.h"

#include <ee0/NodeContainer.h>

#include <SM_Rect.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#else
#include <entity0/World.h>
#include <entity2/CompBoundingBox.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS
#include <painting2/PrimitiveDraw.h>
#include <painting2/Color.h>

namespace ee2
{

AutoAlign::AutoAlign(ee0::NodeContainer& objs)
	: m_objs(objs)
	, m_open(true)
{
}

void AutoAlign::Align(ECS_WORLD_PARAM const std::vector<ee0::GameObj>& objs)
{
	m_hor[0].Set(0, 0);
	m_hor[1].Set(0, 0);
	m_ver[0].Set(0, 0);
	m_ver[1].Set(0, 0);

	// not support multi source obj now
	if (objs.size() > 1)
		return;

	const float DIS = 5;

#ifndef GAME_OBJ_ECS
	ee0::GameObj hor_nearest = nullptr, ver_nearest = nullptr;
#else
	ee0::GameObj hor_nearest, ver_nearest;
#endif // GAME_OBJ_ECS
	float dis_hor = DIS, dis_ver = DIS;
	// hor
	m_objs.Traverse([&](const ee0::GameObj& obj)->bool 
	{
		auto& dst = objs[0];
		auto& src = obj;
		if (src == dst) {
			return false;
		}

#ifndef GAME_OBJ_ECS
		auto &src_rect = src->GetUniqueComp<n2::CompBoundingBox>().GetSize(),
			 &dst_rect = dst->GetUniqueComp<n2::CompBoundingBox>().GetSize();
#else
		auto &src_rect = world.GetComponent<e2::CompBoundingBox>(src).rect,
			 &dst_rect = world.GetComponent<e2::CompBoundingBox>(dst).rect;
#endif // GAME_OBJ_ECS

		float src_cy = src_rect.Center().y;
		float src_down = src_rect.ymin;
		float src_up = src_rect.ymax;

		float dst_cy = dst_rect.Center().y;
		float dst_down = dst_rect.ymin;
		float dst_up = dst_rect.ymax;

		// up
		if (float dis = fabs(dst_up - src_up) < dis_hor)
		{
			dis_hor = dis;
			hor_nearest = src;
		}
		else if (float dis = fabs(dst_up - src_down) < dis_hor)
		{
			dis_hor = dis;
			hor_nearest = src;
		}
		// down
		if (float dis = fabs(dst_down - src_up) < dis_hor)
		{
			dis_hor = dis;
			hor_nearest = src;
		}
		else if (float dis = fabs(dst_down - src_down) < dis_hor)
		{
			dis_hor = dis;
			hor_nearest = src;
		}

		return true;
	});

	// ver
	m_objs.Traverse([&](const ee0::GameObj& obj)->bool
	{
		auto& dst = objs[0];
		auto& src = obj;
		if (src == dst) {
			return false;
		}

#ifndef GAME_OBJ_ECS
		auto &src_rect = src->GetUniqueComp<n2::CompBoundingBox>().GetSize(),
			 &dst_rect = dst->GetUniqueComp<n2::CompBoundingBox>().GetSize();
#else
		auto &src_rect = world.GetComponent<e2::CompBoundingBox>(src).rect,
			 &dst_rect = world.GetComponent<e2::CompBoundingBox>(dst).rect;
#endif // GAME_OBJ_ECS

		float src_cx = src_rect.Center().x;
		float src_left = src_rect.xmin;
		float src_right = src_rect.xmax;

		float dst_cx = dst_rect.Center().x;
		float dst_left = dst_rect.xmin;
		float dst_right = dst_rect.xmax;

		// left
		if (float dis = fabs(dst_left - src_left) < dis_ver)
		{
			dis_ver = dis;
			ver_nearest = src;
		}
		else if (float dis = fabs(dst_left - src_right) < dis_ver)
		{
			dis_ver = dis;
			ver_nearest = src;
		}
		// right
		if (float dis = fabs(dst_right - src_left) < dis_ver)
		{
			dis_ver = dis;
			ver_nearest = src;
		}
		else if (float dis = fabs(dst_right - src_right) < dis_ver)
		{
			dis_ver = dis;
			ver_nearest = src;
		}

		return true;
	});

	if (GAME_OBJ_VALID(hor_nearest)) {
		Align(ECS_WORLD_VAR hor_nearest, objs[0]);
	}
	if (GAME_OBJ_VALID(ver_nearest) && ver_nearest != hor_nearest) {
		Align(ECS_WORLD_VAR ver_nearest, objs[0]);
	}
}

void AutoAlign::Align(ECS_WORLD_PARAM const ee0::GameObj& src, const ee0::GameObj& dst)
{
	const float DIS = 5;
	const float LEN = 400;

#ifndef GAME_OBJ_ECS
	auto &src_rect = src->GetUniqueComp<n2::CompBoundingBox>().GetSize();
	// todo
	sm::rect dst_rect = dst->GetUniqueComp<n2::CompBoundingBox>().GetSize();
#else
	auto &src_rect = world.GetComponent<e2::CompBoundingBox>(src).rect,
	     &dst_rect = world.GetComponent<e2::CompBoundingBox>(dst).rect;
#endif // GAME_OBJ_ECS

	float src_left	= src_rect.xmin,
		  src_right = src_rect.xmax,
		  src_down	= src_rect.ymin,
		  src_up	= src_rect.ymax;
	float src_cx	= src_rect.Center().x,
		  src_cy	= src_rect.Center().y;

#ifndef GAME_OBJ_ECS
	auto& dst_ctrans = dst->GetUniqueComp<n2::CompTransform>();
	auto& dst_trans = dst_ctrans.GetTrans();
#endif // GAME_OBJ_ECS

	// up
	float nearest = DIS;
	if (float dis = fabs(dst_rect.ymax - src_up) < nearest)
	{
		float dy = src_up - dst_rect.ymax;
		nearest = dis;
#ifndef GAME_OBJ_ECS
		dst_ctrans.SetPosition(*dst, sm::vec2(dst_trans.GetPosition().x, dst_trans.GetPosition().y + dy));
#else
		e2::SysTransform::Translate(world, dst, sm::vec2(0, dy));
#endif // GAME_OBJ_ECS
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_up);
		m_hor[1].Set(src_cx + LEN, src_up);
	}
	else if (float dis = fabs(dst_rect.ymax - src_down) < nearest)
	{
		float dy = src_down - dst_rect.ymax;
		nearest = dis;
#ifndef GAME_OBJ_ECS
		dst_ctrans.SetPosition(*dst, sm::vec2(dst_trans.GetPosition().x, dst_trans.GetPosition().y + dy));
#else
		e2::SysTransform::Translate(world, dst, sm::vec2(0, dy));
#endif // GAME_OBJ_ECS
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_down);
		m_hor[1].Set(src_cx + LEN, src_down);
	}		
	// down
	if (float dis = fabs(dst_rect.ymin - src_up) < nearest)
	{
		float dy = src_up - dst_rect.ymin;
		nearest = dis;
#ifndef GAME_OBJ_ECS
		dst_ctrans.SetPosition(*dst, sm::vec2(dst_trans.GetPosition().x, dst_trans.GetPosition().y + dy));
#else
		e2::SysTransform::Translate(world, dst, sm::vec2(0, dy));
#endif // GAME_OBJ_ECS
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_up);
		m_hor[1].Set(src_cx + LEN, src_up);
	}
	else if (float dis = fabs(dst_rect.ymin - src_down) < nearest)
	{
		float dy = src_down - dst_rect.ymin;
		nearest = dis;
#ifndef GAME_OBJ_ECS
		dst_ctrans.SetPosition(*dst, sm::vec2(dst_trans.GetPosition().x, dst_trans.GetPosition().y + dy));
#else
		e2::SysTransform::Translate(world, dst, sm::vec2(0, dy));
#endif // GAME_OBJ_ECS
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_down);
		m_hor[1].Set(src_cx + LEN, src_down);
	}	
	// left
	nearest = DIS;
	if (float dis = fabs(dst_rect.xmin - src_left) < nearest)
	{
		float dx = src_left - dst_rect.xmin;
		nearest = dis;
#ifndef GAME_OBJ_ECS
		dst_ctrans.SetPosition(*dst, sm::vec2(dst_trans.GetPosition().x + dx, dst_trans.GetPosition().y));
#else
		e2::SysTransform::Translate(world, dst, sm::vec2(dx, 0));
#endif // GAME_OBJ_ECS
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_left, src_cy - LEN);
		m_ver[1].Set(src_left, src_cy + LEN);
	}
	else if (float dis = fabs(dst_rect.xmin - src_right) < nearest)
	{
		float dx = src_right - dst_rect.xmin;
		nearest = dis;
#ifndef GAME_OBJ_ECS
		dst_ctrans.SetPosition(*dst, sm::vec2(dst_trans.GetPosition().x + dx, dst_trans.GetPosition().y));
#else
		e2::SysTransform::Translate(world, dst, sm::vec2(dx, 0));
#endif // GAME_OBJ_ECS
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_right, src_cy - LEN);
		m_ver[1].Set(src_right, src_cy + LEN);
	}
	// right
	if (float dis = fabs(dst_rect.xmax - src_left) < nearest)
	{
		float dx = src_left - dst_rect.xmax;
		nearest = dis;
#ifndef GAME_OBJ_ECS
		dst_ctrans.SetPosition(*dst, sm::vec2(dst_trans.GetPosition().x + dx, dst_trans.GetPosition().y));
#else
		e2::SysTransform::Translate(world, dst, sm::vec2(dx, 0));
#endif // GAME_OBJ_ECS
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_left, src_cy - LEN);
		m_ver[1].Set(src_left, src_cy + LEN);
	}
	else if (float dis = fabs(dst_rect.xmax - src_right) < nearest)
	{
		float dx = src_right - dst_rect.xmax;
		nearest = dis;
#ifndef GAME_OBJ_ECS
		dst_ctrans.SetPosition(*dst, sm::vec2(dst_trans.GetPosition().x + dx, dst_trans.GetPosition().y));
#else
		e2::SysTransform::Translate(world, dst, sm::vec2(dx, 0));
#endif // GAME_OBJ_ECS
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_right, src_cy - LEN);
		m_ver[1].Set(src_right, src_cy + LEN);
	}
}

void AutoAlign::Draw() const
{
	if (m_open)
	{
		pt2::PrimitiveDraw::SetColor(pt2::Color(0, 0, 0));
		if (m_hor[0] != m_hor[1]) {
			pt2::PrimitiveDraw::DashLine(nullptr, m_hor[0], m_hor[1]);
		}
		if (m_ver[0] != m_ver[1]) {
			pt2::PrimitiveDraw::DashLine(nullptr, m_ver[0], m_ver[1]);
		}
	}
}

void AutoAlign::SetInvisible() 
{ 
	m_hor[0] = m_hor[1]; 
	m_ver[0] = m_ver[1]; 
}

}