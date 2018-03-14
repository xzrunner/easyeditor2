#include "ee2/WxCompTransformPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/SettingCfg.h>

#include <sm_const.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <node2/CompUniquePatch.h>
#include <node2/CompSharedPatch.h>
#include <node2/EditOp.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace ee2
{

WxCompTransformPanel::WxCompTransformPanel(wxWindow* parent, 
	                                       n2::CompTransform& trans, 
	                                       ee0::SubjectMgr& sub_mgr,
	                                       const n0::NodeWithPos& nwp)
	: ee0::WxCompPanel(parent, "Transform")
	, m_ctrans(trans)
	, m_sub_mgr(sub_mgr)
	, m_nwp(nwp)
{
	InitLayout();
	Expand();
}

void WxCompTransformPanel::RefreshNodeComp()
{
	auto& trans = m_ctrans.GetTrans();

	m_pos_x->SetValue(std::to_string(trans.GetPosition().x));
	m_pos_y->SetValue(std::to_string(trans.GetPosition().y));

	m_angle->SetValue(std::to_string(trans.GetAngle() * SM_RAD_TO_DEG));

	m_scale_x->SetValue(std::to_string(trans.GetScale().x));
	m_scale_y->SetValue(std::to_string(trans.GetScale().y));

	m_shear_x->SetValue(std::to_string(trans.GetShear().x));
	m_shear_y->SetValue(std::to_string(trans.GetShear().y));
}

void WxCompTransformPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	static const wxSize INPUT_SIZE(65, 19);

	// position
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Position  ")));

		auto& pos = m_ctrans.GetTrans().GetPosition();
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

		float angle = m_ctrans.GetTrans().GetAngle() * SM_RAD_TO_DEG;
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

		auto& scale = m_ctrans.GetTrans().GetScale();
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

		auto& shear = m_ctrans.GetTrans().GetShear();
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
	case ee0::EditOpType::EDIT_SHARED_PATCH:
		UpdateSharedPatchValue(event);
		break;
	case ee0::EditOpType::EDIT_UNIQUE:
		UpdateUniqueValue(event);
		break;
	}
}

void WxCompTransformPanel::UpdateSharedValue(wxCommandEvent& event)
{
	auto& trans = m_ctrans.GetTrans();

	// pos
	if (event.GetId() == m_pos_x->GetId()) 
	{
		double x;
		m_pos_x->GetValue().ToDouble(&x);
		auto& pos = trans.GetPosition();
		m_ctrans.SetPosition(*m_nwp.GetNode(), sm::vec2(x, pos.y));
	}
	else if (event.GetId() == m_pos_y->GetId()) 
	{
		double y;
		m_pos_y->GetValue().ToDouble(&y);
		auto& pos = trans.GetPosition();
		m_ctrans.SetPosition(*m_nwp.GetNode(), sm::vec2(pos.x, y));
	}
	// angle
	else if (event.GetId() == m_angle->GetId())
	{
		double ang;
		m_angle->GetValue().ToDouble(&ang);
		m_ctrans.SetAngle(*m_nwp.GetNode(), ang * SM_DEG_TO_RAD);
	}
	// scale
	else if (event.GetId() == m_scale_x->GetId())
	{
		double x;
		m_scale_x->GetValue().ToDouble(&x);
		auto& scale = trans.GetScale();
		m_ctrans.SetScale(*m_nwp.GetNode(), sm::vec2(x, scale.y));
	}
	else if (event.GetId() == m_scale_y->GetId())
	{
		double y;
		m_scale_y->GetValue().ToDouble(&y);
		auto& scale = trans.GetScale();
		m_ctrans.SetScale(*m_nwp.GetNode(), sm::vec2(scale.x, y));
	}
	// shear
	else if (event.GetId() == m_shear_x->GetId())
	{
		double x;
		m_shear_x->GetValue().ToDouble(&x);
		auto& shear = trans.GetShear();
		m_ctrans.SetShear(*m_nwp.GetNode(), sm::vec2(x, shear.y));
	}
	else if (event.GetId() == m_shear_y->GetId())
	{
		double y;
		m_shear_y->GetValue().ToDouble(&y);
		auto& shear = trans.GetShear();
		m_ctrans.SetShear(*m_nwp.GetNode(), sm::vec2(shear.x, y));
	}

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxCompTransformPanel::UpdateSharedPatchValue(wxCommandEvent& event)
{
	if (!m_nwp.GetRoot()) {
		return;
	}

	auto& trans = m_ctrans.GetTrans();
	auto& cpatch = m_nwp.GetRoot()->HasUniqueComp<n2::CompSharedPatch>() ?
					m_nwp.GetRoot()->GetUniqueComp<n2::CompSharedPatch>() :
					m_nwp.GetRoot()->AddUniqueComp<n2::CompSharedPatch>();
	// pos
	if (event.GetId() == m_pos_x->GetId()) 
	{
		double x;
		m_pos_x->GetValue().ToDouble(&x);
		sm::vec2 new_pos(x, trans.GetPosition().y);

		m_ctrans.SetPosition(*m_nwp.GetNode(), new_pos);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetPositionOp>(new_pos);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	else if (event.GetId() == m_pos_y->GetId()) 
	{
		double y;
		m_pos_y->GetValue().ToDouble(&y);
		sm::vec2 new_pos(trans.GetPosition().x, y);

		m_ctrans.SetPosition(*m_nwp.GetNode(), new_pos);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetPositionOp>(new_pos);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	// angle
	else if (event.GetId() == m_angle->GetId())
	{
		double ang;
		m_angle->GetValue().ToDouble(&ang);
		float new_angle(ang * SM_DEG_TO_RAD);

		m_ctrans.SetAngle(*m_nwp.GetNode(), new_angle);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetAngleOp>(new_angle);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	// scale
	else if (event.GetId() == m_scale_x->GetId())
	{
		double x;
		m_scale_x->GetValue().ToDouble(&x);
		sm::vec2 new_scale(x, trans.GetScale().y);

		m_ctrans.SetScale(*m_nwp.GetNode(), new_scale);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetScaleOp>(new_scale);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	else if (event.GetId() == m_scale_y->GetId())
	{
		double y;
		m_scale_y->GetValue().ToDouble(&y);
		sm::vec2 new_scale(trans.GetScale().x, y);

		m_ctrans.SetScale(*m_nwp.GetNode(), new_scale);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetScaleOp>(new_scale);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	// shear
	else if (event.GetId() == m_shear_x->GetId())
	{
		double x;
		m_shear_x->GetValue().ToDouble(&x);
		sm::vec2 new_shear(x, trans.GetShear().y);

		m_ctrans.SetShear(*m_nwp.GetNode(), new_shear);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetShearOp>(new_shear);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}
	else if (event.GetId() == m_shear_y->GetId())
	{
		double y;
		m_shear_y->GetValue().ToDouble(&y);
		sm::vec2 new_shear(trans.GetShear().x, y);

		m_ctrans.SetShear(*m_nwp.GetNode(), new_shear);

		std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetShearOp>(new_shear);
		cpatch.AddEditOp(m_nwp.GetNodeID(), op);
	}

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxCompTransformPanel::UpdateUniqueValue(wxCommandEvent& event)
{
	if (!m_nwp.GetRoot()) {
		return;
	}

	auto& old_trans = m_ctrans.GetTrans();
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

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}