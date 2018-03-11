#include "ee2/WxCompTransformPanel.h"

#include <ee0/SubjectMgr.h>

#include <sm_const.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <node2/CompUniquePatch.h>
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
		sizer->Add(m_pos_x = new wxTextCtrl(win, wxID_ANY, std::to_string(pos.x), wxDefaultPosition, INPUT_SIZE));
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Y ")));
		sizer->Add(m_pos_y = new wxTextCtrl(win, wxID_ANY, std::to_string(pos.y), wxDefaultPosition, INPUT_SIZE));

		Connect(m_pos_x->GetId(), wxEVT_COMMAND_TEXT_UPDATED, 
			wxCommandEventHandler(WxCompTransformPanel::UpdateTextValue));
		Connect(m_pos_y->GetId(), wxEVT_COMMAND_TEXT_UPDATED, 
			wxCommandEventHandler(WxCompTransformPanel::UpdateTextValue));

		pane_sizer->Add(sizer);
	}
	// angle
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Angle     ")));

		float angle = m_ctrans.GetTrans().GetAngle() * SM_RAD_TO_DEG;
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("X ")));
		sizer->Add(m_angle = new wxTextCtrl(win, wxID_ANY, std::to_string(angle), wxDefaultPosition, INPUT_SIZE));

		Connect(m_angle->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(WxCompTransformPanel::UpdateTextValue));

		pane_sizer->Add(sizer);
	}
	// scale
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Scale     ")));

		auto& scale = m_ctrans.GetTrans().GetScale();
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("X ")));
		sizer->Add(m_scale_x = new wxTextCtrl(win, wxID_ANY, std::to_string(scale.x), wxDefaultPosition, INPUT_SIZE));
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Y ")));
		sizer->Add(m_scale_y = new wxTextCtrl(win, wxID_ANY, std::to_string(scale.y), wxDefaultPosition, INPUT_SIZE));

		Connect(m_scale_x->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(WxCompTransformPanel::UpdateTextValue));
		Connect(m_scale_y->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(WxCompTransformPanel::UpdateTextValue));

		pane_sizer->Add(sizer);
	}
	// shear
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Shear     ")));

		auto& shear = m_ctrans.GetTrans().GetShear();
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("X ")));
		sizer->Add(m_shear_x = new wxTextCtrl(win, wxID_ANY, std::to_string(shear.x), wxDefaultPosition, INPUT_SIZE));
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Y ")));
		sizer->Add(m_shear_y = new wxTextCtrl(win, wxID_ANY, std::to_string(shear.y), wxDefaultPosition, INPUT_SIZE));

		Connect(m_shear_x->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(WxCompTransformPanel::UpdateTextValue));
		Connect(m_shear_y->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(WxCompTransformPanel::UpdateTextValue));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompTransformPanel::UpdateTextValue(wxCommandEvent& event)
{
	auto& trans = m_ctrans.GetTrans();

	// pos
	if (event.GetId() == m_pos_x->GetId()) 
	{
		double x;
		m_pos_x->GetValue().ToDouble(&x);
		auto& pos = trans.GetPosition();

//		m_ctrans.SetPosition(*m_nwp.node, sm::vec2(x, pos.y));

		if (m_nwp.root)
		{
			auto& cpatch = m_nwp.root->HasUniqueComp<n2::CompUniquePatch>() ?
				m_nwp.root->GetUniqueComp<n2::CompUniquePatch>() : 
				m_nwp.root->AddUniqueComp<n2::CompUniquePatch>();
			auto new_trans = m_ctrans.GetTrans();
			new_trans.SetPosition(sm::vec2(x, pos.y));
			std::unique_ptr<n2::EditOp> op = std::make_unique<n2::SetTransformOp>(new_trans.GetMatrix());
			cpatch.AddUniqueOp(m_nwp.node_id, op);
		}
	}
	else if (event.GetId() == m_pos_y->GetId()) 
	{
		double y;
		m_pos_y->GetValue().ToDouble(&y);
		auto& pos = trans.GetPosition();
		m_ctrans.SetPosition(*m_nwp.node, sm::vec2(pos.x, y));
	}
	// angle
	else if (event.GetId() == m_angle->GetId())
	{
		double ang;
		m_angle->GetValue().ToDouble(&ang);
		m_ctrans.SetAngle(*m_nwp.node, ang * SM_DEG_TO_RAD);
	}
	// scale
	else if (event.GetId() == m_scale_x->GetId())
	{
		double x;
		m_scale_x->GetValue().ToDouble(&x);
		auto& scale = trans.GetScale();
		m_ctrans.SetScale(*m_nwp.node, sm::vec2(x, scale.y));
	}
	else if (event.GetId() == m_scale_y->GetId())
	{
		double y;
		m_scale_y->GetValue().ToDouble(&y);
		auto& scale = trans.GetScale();
		m_ctrans.SetScale(*m_nwp.node, sm::vec2(scale.x, y));
	}
	// shear
	else if (event.GetId() == m_shear_x->GetId())
	{
		double x;
		m_shear_x->GetValue().ToDouble(&x);
		auto& shear = trans.GetShear();
		m_ctrans.SetShear(*m_nwp.node, sm::vec2(x, shear.y));
	}
	else if (event.GetId() == m_shear_y->GetId())
	{
		double y;
		m_shear_y->GetValue().ToDouble(&y);
		auto& shear = trans.GetShear();
		m_ctrans.SetShear(*m_nwp.node, sm::vec2(shear.x, y));
	}

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}