#include "ee2/ArrangeNodeOP.h"
#include "ee2/ArrangeNodeImpl.h"
#include "ee2/WxStageCanvas.h"
#include "ee2/NodeSelectOP.h"

#include <ee0/WxStagePage.h>

#include <guard/check.h>
#include <painting2/OrthoCamera.h>

namespace ee2
{


ArrangeNodeOP::ArrangeNodeOP(const std::shared_ptr<pt0::Camera>& camera,
	                         ee0::WxStagePage& stage, 
	                         ECS_WORLD_PARAM
	                         const ArrangeNodeCfg& cfg,
	                         const std::shared_ptr<ee0::EditOP>& prev_op)
	: ee0::EditOP(camera)
	ECS_WORLD_SELF_ASSIGN
{
	m_impl = std::make_unique<ArrangeNodeImpl>(stage, camera, ECS_WORLD_VAR cfg);

	if (prev_op) {
		SetPrevEditOP(prev_op);
	} else {
		SetPrevEditOP(std::make_shared<NodeSelectOP>(camera, ECS_WORLD_VAR stage));
	}
}

ArrangeNodeOP::ArrangeNodeOP(const std::shared_ptr<pt0::Camera>& camera,
	                         ee0::WxStagePage& stage,
	                         ECS_WORLD_PARAM
	                         std::unique_ptr<ArrangeNodeImpl>& impl,
	                         const std::shared_ptr<ee0::EditOP>& prev_op)
	: ee0::EditOP(camera)
	ECS_WORLD_SELF_ASSIGN
{
	m_impl = std::move(impl);

	if (prev_op) {
		SetPrevEditOP(prev_op);
	} else {
		SetPrevEditOP(std::make_shared<NodeSelectOP>(camera, ECS_WORLD_VAR stage));
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
	if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>()) {
		cam_scale = std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera)->GetScale();
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

void ArrangeNodeOP::SetCamera(const std::shared_ptr<pt0::Camera>& camera)
{
	ee0::EditOP::SetCamera(camera);

	m_impl->SetCamera(camera);
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
