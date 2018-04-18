#include "ee2/WxCompTransformPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/SettingCfg.h>
#include <ee0/MessageID.h>

#include <sm_const.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <node2/CompUniquePatch.h>
#include <node2/CompSharedPatch.h>
#include <node2/EditOp.h>
#else
#include <entity0/World.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace ee2
{

WxCompTransformPanel::WxCompTransformPanel(wxWindow* parent, 
	                                       const ee0::SubjectMgrPtr& sub_mgr,
	                                       ECS_WORLD_PARAM
	                                       const ee0::GameObjWithPos& opw)
	: ee0::WxCompPanel(parent, "Transform")
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_nwp(opw)
{
	InitLayout();
	Expand();
}

void WxCompTransformPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& trans = m_nwp.GetNode()->GetUniqueComp<n2::CompTransform>().GetTrans();

	m_pos_x->SetValue(std::to_string(trans.GetPosition().x));
	m_pos_y->SetValue(std::to_string(trans.GetPosition().y));

	m_angle->SetValue(std::to_string(trans.GetAngle() * SM_RAD_TO_DEG));

	m_scale_x->SetValue(std::to_string(trans.GetScale().x));
	m_scale_y->SetValue(std::to_string(trans.GetScale().y));

	m_shear_x->SetValue(std::to_string(trans.GetShear().x));
	m_shear_y->SetValue(std::to_string(trans.GetShear().y));
#else
	
#endif // GAME_OBJ_ECS
}

void WxCompTransformPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	static const wxSize INPUT_SIZE(65, 19);

#ifndef GAME_OBJ_ECS
	auto& trans = m_nwp.GetNode()->GetUniqueComp<n2::CompTransform>().GetTrans();
#endif // GAME_OBJ_ECS

	// position
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Position  ")));

#ifndef GAME_OBJ_ECS
		auto& pos = trans.GetPosition();
#else
		auto pos = e2::SysTransform::GetPosition(m_world, m_nwp);
#endif // GAME_OBJ_ECS
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("X ")));
		sizer->Add(m_pos_x = new wxTextCtrl(win, wxID_ANY, std::to_string(pos.x), 
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Y ")));
		sizer->Add(m_pos_y = new wxTextCtrl(win, wxID_ANY, std::to_string(pos.y), 
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));

		Connect(m_pos_x->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxCompTransformPanel::EnterTextValue));
		Connect(m_pos_y->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxCompTransformPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}
	// angle
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Angle     ")));

#ifndef GAME_OBJ_ECS
		float angle = trans.GetAngle() * SM_RAD_TO_DEG;
#else
		float angle = e2::SysTransform::GetAngle(m_world, m_nwp) * SM_RAD_TO_DEG;
#endif // GAME_OBJ_ECS
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("X ")));
		sizer->Add(m_angle = new wxTextCtrl(win, wxID_ANY, std::to_string(angle), 
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));

		Connect(m_angle->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxCompTransformPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}
	// scale
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Scale     ")));

#ifndef GAME_OBJ_ECS
		auto& scale = trans.GetScale();
#else
		auto scale = e2::SysTransform::GetScale(m_world, m_nwp);
#endif // GAME_OBJ_ECS
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("X ")));
		sizer->Add(m_scale_x = new wxTextCtrl(win, wxID_ANY, std::to_string(scale.x), 
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Y ")));
		sizer->Add(m_scale_y = new wxTextCtrl(win, wxID_ANY, std::to_string(scale.y), 
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));

		Connect(m_scale_x->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxCompTransformPanel::EnterTextValue));
		Connect(m_scale_y->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxCompTransformPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}
	// shear
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Shear     ")));

#ifndef GAME_OBJ_ECS
		auto& shear = trans.GetShear();
#else
		auto shear = e2::SysTransform::GetShear(m_world, m_nwp);
#endif // GAME_OBJ_ECS
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("X ")));
		sizer->Add(m_shear_x = new wxTextCtrl(win, wxID_ANY, std::to_string(shear.x), 
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Y ")));
		sizer->Add(m_shear_y = new wxTextCtrl(win, wxID_ANY, std::to_string(shear.y),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));

		Connect(m_shear_x->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxCompTransformPanel::EnterTextValue));
		Connect(m_shear_y->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxCompTransformPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompTransformPanel::EnterTextValue(wxCommandEvent& event)
{
	switch (ee0::SettingCfg::Instance()->GetEditOpType())
	{
	case ee0::EditOpType::EDIT_SHARED:
		UpdateSharedValue(event);
		break;
#ifndef GAME_OBJ_ECS
	case ee0::EditOpType::EDIT_SHARED_PATCH:
		UpdateSharedPatchValue(event);
		break;
	case ee0::EditOpType::EDIT_UNIQUE:
		UpdateUniqueValue(event);
		break;
#endif // GAME_OBJ_ECS
	}
}

void WxCompTransformPanel::UpdateSharedValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_nwp.GetNode()->GetUniqueComp<n2::CompTransform>();
	auto& trans = ctrans.GetTrans();
#endif // GAME_OBJ_ECS

	// pos
	if (event.GetId() == m_pos_x->GetId()) 
	{
		double x;
		m_pos_x->GetValue().ToDouble(&x);
#ifndef GAME_OBJ_ECS
		auto& pos = trans.GetPosition();
		ctrans.SetPosition(*m_nwp.GetNode(), sm::vec2(x, pos.y));
#else
		auto pos = e2::SysTransform::GetPosition(m_world, m_nwp);
		e2::SysTransform::SetPosition(m_world, m_nwp, sm::vec2(x, pos.y));
#endif // GAME_OBJ_ECS
	}
	else if (event.GetId() == m_pos_y->GetId()) 
	{
		double y;
		m_pos_y->GetValue().ToDouble(&y);
#ifndef GAME_OBJ_ECS
		auto& pos = trans.GetPosition();
		ctrans.SetPosition(*m_nwp.GetNode(), sm::vec2(pos.x, y));
#else
		auto pos = e2::SysTransform::GetPosition(m_world, m_nwp);
		e2::SysTransform::SetPosition(m_world, m_nwp, sm::vec2(pos.x, y));
#endif // GAME_OBJ_ECS
	}
	// angle
	else if (event.GetId() == m_angle->GetId())
	{
		double ang;
		m_angle->GetValue().ToDouble(&ang);
#ifndef GAME_OBJ_ECS
		ctrans.SetAngle(*m_nwp.GetNode(), ang * SM_DEG_TO_RAD);
#else
		e2::SysTransform::SetAngle(m_world, m_nwp, ang * SM_DEG_TO_RAD);
#endif // GAME_OBJ_ECS
	}
	// scale
	else if (event.GetId() == m_scale_x->GetId())
	{
		double x;
		m_scale_x->GetValue().ToDouble(&x);
#ifndef GAME_OBJ_ECS
		auto& scale = trans.GetScale();
		ctrans.SetScale(*m_nwp.GetNode(), sm::vec2(x, scale.y));
#else
		auto scale = e2::SysTransform::GetScale(m_world, m_nwp);
		e2::SysTransform::SetScale(m_world, m_nwp, sm::vec2(x, scale.y));
#endif // GAME_OBJ_ECS
	}
	else if (event.GetId() == m_scale_y->GetId())
	{
		double y;
		m_scale_y->GetValue().ToDouble(&y);
#ifndef GAME_OBJ_ECS
		auto& scale = trans.GetScale();
		ctrans.SetScale(*m_nwp.GetNode(), sm::vec2(scale.x, y));
#else
		auto scale = e2::SysTransform::GetScale(m_world, m_nwp);
		e2::SysTransform::SetScale(m_world, m_nwp, sm::vec2(scale.x, y));
#endif // GAME_OBJ_ECS
	}
	// shear
	else if (event.GetId() == m_shear_x->GetId())
	{
		double x;
		m_shear_x->GetValue().ToDouble(&x);
#ifndef GAME_OBJ_ECS
		auto& shear = trans.GetShear();
		ctrans.SetShear(*m_nwp.GetNode(), sm::vec2(x, shear.y));
#else
		auto shear = e2::SysTransform::GetShear(m_world, m_nwp);
		e2::SysTransform::SetShear(m_world, m_nwp, sm::vec2(x, shear.y));
#endif // GAME_OBJ_ECS
	}
	else if (event.GetId() == m_shear_y->GetId())
	{
		double y;
		m_shear_y->GetValue().ToDouble(&y);
#ifndef GAME_OBJ_ECS
		auto& shear = trans.GetShear();
		ctrans.SetShear(*m_nwp.GetNode(), sm::vec2(shear.x, y));
#else
		auto shear = e2::SysTransform::GetShear(m_world, m_nwp);
		e2::SysTransform::SetShear(m_world, m_nwp, sm::vec2(shear.x, y));
#endif // GAME_OBJ_ECS
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

#ifndef GAME_OBJ_ECS
void WxCompTransformPanel::UpdateSharedPatchValue(wxCommandEvent& event)
{
	if (!m_nwp.GetRoot()) {
		return;
	}

	auto& ctrans = m_nwp.GetNode()->GetUniqueComp<n2::CompTransform>();
	auto& trans = ctrans.GetTrans();
	auto& cpatch = m_nwp.GetRoot()->HasUniqueComp<n2::CompSharedPatch>() ?
					m_nwp.GetRoot()->GetUniqueComp<n2::CompSharedPatch>() :
					m_nwp.GetRoot()->AddUniqueComp<n2::CompSharedPatch>();
	// pos
	if (event.GetId() == m_pos_x->GetId()) 
	{
		double x;
		m_pos_x->GetValue().ToDouble(&x);
		sm::vec2 new_pos(x, trans.GetPosition().y);

		ctrans.SetPosition(*m_nwp.GetNode(), new_pos);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetPositionOp>(new_pos);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	else if (event.GetId() == m_pos_y->GetId()) 
	{
		double y;
		m_pos_y->GetValue().ToDouble(&y);
		sm::vec2 new_pos(trans.GetPosition().x, y);

		ctrans.SetPosition(*m_nwp.GetNode(), new_pos);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetPositionOp>(new_pos);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	// angle
	else if (event.GetId() == m_angle->GetId())
	{
		double ang;
		m_angle->GetValue().ToDouble(&ang);
		float new_angle(ang * SM_DEG_TO_RAD);

		ctrans.SetAngle(*m_nwp.GetNode(), new_angle);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetAngleOp>(new_angle);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	// scale
	else if (event.GetId() == m_scale_x->GetId())
	{
		double x;
		m_scale_x->GetValue().ToDouble(&x);
		sm::vec2 new_scale(x, trans.GetScale().y);

		ctrans.SetScale(*m_nwp.GetNode(), new_scale);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetScaleOp>(new_scale);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	else if (event.GetId() == m_scale_y->GetId())
	{
		double y;
		m_scale_y->GetValue().ToDouble(&y);
		sm::vec2 new_scale(trans.GetScale().x, y);

		ctrans.SetScale(*m_nwp.GetNode(), new_scale);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetScaleOp>(new_scale);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	// shear
	else if (event.GetId() == m_shear_x->GetId())
	{
		double x;
		m_shear_x->GetValue().ToDouble(&x);
		sm::vec2 new_shear(x, trans.GetShear().y);

		ctrans.SetShear(*m_nwp.GetNode(), new_shear);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetShearOp>(new_shear);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	else if (event.GetId() == m_shear_y->GetId())
	{
		double y;
		m_shear_y->GetValue().ToDouble(&y);
		sm::vec2 new_shear(trans.GetShear().x, y);

		ctrans.SetShear(*m_nwp.GetNode(), new_shear);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetShearOp>(new_shear);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxCompTransformPanel::UpdateUniqueValue(wxCommandEvent& event)
{
	if (!m_nwp.GetRoot()) {
		return;
	}

	auto& old_trans = m_nwp.GetNode()->GetUniqueComp<n2::CompTransform>().GetTrans();
	auto new_trans = old_trans;

	// pos
	if (event.GetId() == m_pos_x->GetId())
	{
		double x;
		m_pos_x->GetValue().ToDouble(&x);
		auto& pos = old_trans.GetPosition();
		new_trans.SetPosition(sm::vec2(x, pos.y));
	}
	else if (event.GetId() == m_pos_y->GetId())
	{
		double y;
		m_pos_y->GetValue().ToDouble(&y);
		auto& pos = old_trans.GetPosition();
		new_trans.SetPosition(sm::vec2(pos.x, y));
	}
	// angle
	else if (event.GetId() == m_angle->GetId())
	{
		double ang;
		m_angle->GetValue().ToDouble(&ang);
		new_trans.SetAngle(ang * SM_DEG_TO_RAD);
	}
	// scale
	else if (event.GetId() == m_scale_x->GetId())
	{
		double x;
		m_scale_x->GetValue().ToDouble(&x);
		auto& scale = old_trans.GetScale();
		new_trans.SetScale(sm::vec2(x, scale.y));
	}
	else if (event.GetId() == m_scale_y->GetId())
	{
		double y;
		m_scale_y->GetValue().ToDouble(&y);
		auto& scale = old_trans.GetScale();
		new_trans.SetScale(sm::vec2(scale.x, y));
	}
	// shear
	else if (event.GetId() == m_shear_x->GetId())
	{
		double x;
		m_shear_x->GetValue().ToDouble(&x);
		auto& shear = old_trans.GetShear();
		new_trans.SetShear(sm::vec2(x, shear.y));
	}
	else if (event.GetId() == m_shear_y->GetId())
	{
		double y;
		m_shear_y->GetValue().ToDouble(&y);
		auto& shear = old_trans.GetShear();
		new_trans.SetShear(sm::vec2(shear.x, y));
	}

	auto& cpatch = m_nwp.GetRoot()->HasUniqueComp<n2::CompUniquePatch>() ?
		           m_nwp.GetRoot()->GetUniqueComp<n2::CompUniquePatch>() :
		           m_nwp.GetRoot()->AddUniqueComp<n2::CompUniquePatch>();
	std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetTransformMatOp>(new_trans.GetMatrix());
	cpatch.AddEditOp(m_nwp.GetNodeID(), op);

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

#endif // GAME_OBJ_ECS

}