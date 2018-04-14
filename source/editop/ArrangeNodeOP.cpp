#include "ee2/ArrangeNodeOP.h"
#include "ee2/ArrangeNodeImpl.h"
#include "ee2/WxStageCanvas.h"
#include "ee2/NodeSelectOP.h"

#include <ee0/WxStagePage.h>

#include <guard/check.h>
#include <painting2/Camera.h>
#include <painting2/OrthoCamera.h>

namespace ee2
{


ArrangeNodeOP::ArrangeNodeOP(ee0::WxStagePage& stage, 
	                         pt2::Camera& cam,
#ifdef GAME_OBJ_ECS
	                         ecsx::World& world,
#endif // GAME_OBJ_ECS
	                         const ArrangeNodeCfg& cfg,
	                         const std::shared_ptr<ee0::EditOP>& prev_op)
	: m_cam(cam)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
{
	m_impl = std::make_unique<ArrangeNodeImpl>(
		cam, 
		stage.GetImpl().GetEditRecord(), 
		stage.GetSubjectMgr(), 
#ifdef GAME_OBJ_ECS
		m_world,
#endif // GAME_OBJ_ECS
		stage.GetSelection(), 
		stage, 
		stage.GetImpl().GetKeyState(), 
		cfg);

	if (prev_op) {
		SetPrevEditOP(prev_op);
	} else {
#ifndef GAME_OBJ_ECS
		SetPrevEditOP(std::make_shared<NodeSelectOP>(stage));
#else
		SetPrevEditOP(std::make_shared<NodeSelectOP>(m_world, stage));
#endif // GAME_OBJ_ECS
	}
}

ArrangeNodeOP::ArrangeNodeOP(ee0::WxStagePage& stage,
	                         pt2::Camera& cam,
#ifdef GAME_OBJ_ECS
	                         ecsx::World& world,
#endif // GAME_OBJ_ECS
	                         std::unique_ptr<ArrangeNodeImpl>& impl,
	                         const std::shared_ptr<ee0::EditOP>& prev_op)
	: m_cam(cam)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
{
	m_impl = std::move(impl);

	if (prev_op) {
		SetPrevEditOP(prev_op);
	} else {
#ifndef GAME_OBJ_ECS
		SetPrevEditOP(std::make_shared<NodeSelectOP>(stage));
#else
		SetPrevEditOP(std::make_shared<NodeSelectOP>(m_world, stage));
#endif // GAME_OBJ_ECS
	}
}

bool ArrangeNodeOP::OnKeyDown(int key_code)
{
	if (EditOP::OnKeyDown(key_code)) {
		return true;
	}
	if (m_impl->OnKeyDown(key_code)) {
		return true;
	}
	return false;
}

bool ArrangeNodeOP::OnKeyUp(int key_code)
{
	if (EditOP::OnKeyUp(key_code)) {
		return true;
	}
	m_impl->OnKeyUp(key_code);
	return false;
}

bool ArrangeNodeOP::OnChar(int key_code)
{
	if (EditOP::OnChar(key_code)) {
		return true;
	}
	m_impl->OnChar(key_code);
	return false;
}

bool ArrangeNodeOP::OnMouseLeftDown(int x, int y)
{
	if (EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}
	m_impl->OnMouseLeftDown(x, y);
	return false;
}

bool ArrangeNodeOP::OnMouseLeftUp(int x, int y)
{
	if (EditOP::OnMouseLeftUp(x, y)) {
		return true;
	}
	m_impl->OnMouseLeftUp(x, y);
	return false;
}

bool ArrangeNodeOP::OnMouseRightDown(int x, int y)
{
	if (EditOP::OnMouseRightDown(x, y)) {
		return true;
	}
	m_impl->OnMouseRightDown(x, y);
	return false;
}

bool ArrangeNodeOP::OnMouseRightUp(int x, int y)
{
	if (EditOP::OnMouseRightUp(x, y)) {
		return true;
	}
	m_impl->OnMouseRightUp(x, y);
	return false;
}

bool ArrangeNodeOP::OnMouseMove(int x, int y)
{
	if (EditOP::OnMouseMove(x, y)) {
		return true;
	}
	m_impl->OnMouseMove(x, y);
	return false;
}

bool ArrangeNodeOP::OnMouseDrag(int x, int y)
{
	if (EditOP::OnMouseDrag(x, y)) {
		return true;
	}
	m_impl->OnMouseDrag(x, y);
	return false;
}

bool ArrangeNodeOP::OnPopMenuSelected(int type)
{
	if (EditOP::OnPopMenuSelected(type)) {
		return true;
	}
	m_impl->OnPopMenuSelected(type);
	return false;
}

bool ArrangeNodeOP::OnDraw() const
{
	if (EditOP::OnDraw()) {
		return true;
	}

	float cam_scale = 1;
	if (m_cam.Type() == pt2::CAM_ORTHO2D) {
		cam_scale = static_cast<pt2::OrthoCamera&>(m_cam).GetScale();
	}

	m_impl->OnDraw(cam_scale);

	return false;
}

bool ArrangeNodeOP::Clear()
{
	if (EditOP::Clear()) {
		return true;
	}
	m_impl->Clear();
	return false;
}

//bool ArrangeNodeOP::IsEmpty() const
//{
//	return m_impl->IsSelectionEmpty();
//}

ee0::GameObj ArrangeNodeOP::SelectByPos(const sm::vec2& pos) const
{
	auto spr = m_impl->QueryEditedNode(pos);
	//if (!spr) {
	//	spr = TBase::SelectByPos(pos);
	//}
	return spr;
}

}
