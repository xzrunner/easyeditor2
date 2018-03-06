#include "ee2/AutoAlign.h"

#include <ee0/NodeContainer.h>

#include <SM_Rect.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <painting2/PrimitiveDraw.h>
#include <painting2/Color.h>

namespace ee2
{

AutoAlign::AutoAlign(ee0::NodeContainer& nodes)
	: m_nodes(nodes)
	, m_open(true)
{
}

void AutoAlign::Align(const std::vector<n0::SceneNodePtr>& nodes)
{
	m_hor[0].Set(0, 0);
	m_hor[1].Set(0, 0);
	m_ver[0].Set(0, 0);
	m_ver[1].Set(0, 0);

	// not support multi source node now
	if (nodes.size() > 1)
		return;

	const float DIS = 5;

	n0::SceneNodePtr hor_nearest, ver_nearest;
	float dis_hor = DIS, dis_ver = DIS;
	// hor
	m_nodes.Traverse([&](const n0::SceneNodePtr& node)->bool 
	{
		auto& dst = nodes[0];
		auto& src = node;
		if (src == dst) {
			return false;
		}

		sm::rect src_rect = src->GetUniqueComp<n2::CompBoundingBox>().GetSize(),
			     dst_rect = dst->GetUniqueComp<n2::CompBoundingBox>().GetSize();

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
	m_nodes.Traverse([&](const n0::SceneNodePtr& node)->bool
	{
		auto& dst = nodes[0];
		auto& src = node;
		if (src == dst) {
			return false;
		}

		sm::rect src_rect = src->GetUniqueComp<n2::CompBoundingBox>().GetSize(),
			     dst_rect = dst->GetUniqueComp<n2::CompBoundingBox>().GetSize();

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

	if (hor_nearest)
		Align(*hor_nearest, *nodes[0]);
	if (ver_nearest && ver_nearest != hor_nearest)
		Align(*ver_nearest, *nodes[0]);
}

void AutoAlign::Align(const n0::SceneNode& src, n0::SceneNode& dst)
{
	const float DIS = 5;
	const float LEN = 400;

	sm::rect src_rect = src.GetUniqueComp<n2::CompBoundingBox>().GetSize(),
		     dst_rect = dst.GetUniqueComp<n2::CompBoundingBox>().GetSize();

	float src_left	= src_rect.xmin,
		  src_right = src_rect.xmax,
		  src_down	= src_rect.ymin,
		  src_up	= src_rect.ymax;
	float src_cx	= src_rect.Center().x,
		  src_cy	= src_rect.Center().y;

	auto& dst_trans = dst.GetUniqueComp<n2::CompTransform>().GetTrans();

	// up
	float nearest = DIS;
	if (float dis = fabs(dst_rect.ymax - src_up) < nearest)
	{
		float dy = src_up - dst_rect.ymax;
		nearest = dis;
		dst_trans.SetPosition(sm::vec2(dst_trans.GetPosition().x, dst_trans.GetPosition().y + dy));
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_up);
		m_hor[1].Set(src_cx + LEN, src_up);
	}
	else if (float dis = fabs(dst_rect.ymax - src_down) < nearest)
	{
		float dy = src_down - dst_rect.ymax;
		nearest = dis;
		dst_trans.SetPosition(sm::vec2(dst_trans.GetPosition().x, dst_trans.GetPosition().y + dy));
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_down);
		m_hor[1].Set(src_cx + LEN, src_down);
	}		
	// down
	if (float dis = fabs(dst_rect.ymin - src_up) < nearest)
	{
		float dy = src_up - dst_rect.ymin;
		nearest = dis;
		dst_trans.SetPosition(sm::vec2(dst_trans.GetPosition().x, dst_trans.GetPosition().y + dy));
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_up);
		m_hor[1].Set(src_cx + LEN, src_up);
	}
	else if (float dis = fabs(dst_rect.ymin - src_down) < nearest)
	{
		float dy = src_down - dst_rect.ymin;
		nearest = dis;
		dst_trans.SetPosition(sm::vec2(dst_trans.GetPosition().x, dst_trans.GetPosition().y + dy));
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
		dst_trans.SetPosition(sm::vec2(dst_trans.GetPosition().x + dx, dst_trans.GetPosition().y));
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_left, src_cy - LEN);
		m_ver[1].Set(src_left, src_cy + LEN);
	}
	else if (float dis = fabs(dst_rect.xmin - src_right) < nearest)
	{
		float dx = src_right - dst_rect.xmin;
		nearest = dis;
		dst_trans.SetPosition(sm::vec2(dst_trans.GetPosition().x + dx, dst_trans.GetPosition().y));
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_right, src_cy - LEN);
		m_ver[1].Set(src_right, src_cy + LEN);
	}
	// right
	if (float dis = fabs(dst_rect.xmax - src_left) < nearest)
	{
		float dx = src_left - dst_rect.xmax;
		nearest = dis;
		dst_trans.SetPosition(sm::vec2(dst_trans.GetPosition().x + dx, dst_trans.GetPosition().y));
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_left, src_cy - LEN);
		m_ver[1].Set(src_left, src_cy + LEN);
	}
	else if (float dis = fabs(dst_rect.xmax - src_right) < nearest)
	{
		float dx = src_right - dst_rect.xmax;
		nearest = dis;
		dst_trans.SetPosition(sm::vec2(dst_trans.GetPosition().x + dx, dst_trans.GetPosition().y));
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