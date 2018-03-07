#include "ee2/WxCompMaskPanel.h"

#include <ee0/CompNodeEditor.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <ns/NodeFactory.h>

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompMaskPanel::WxCompMaskPanel(wxWindow* parent, n2::CompMask& cmask, 
	                             ee0::SubjectMgr& sub_mgr, n0::SceneNode& node)
	: ee0::WxCompPanel(parent, "Mask")
	, m_cmask(cmask)
	, m_sub_mgr(sub_mgr)
	, m_node(node)
{
	InitLayout();
	Expand();
}

void WxCompMaskPanel::RefreshNodeComp()
{
	auto& base = m_cmask.GetBaseNode();
	if (base)
	{
		auto& ceditor = base->GetUniqueComp<ee0::CompNodeEditor>();
		m_base_path->SetValue(ceditor.GetFilepath());
	}
	auto& mask = m_cmask.GetMaskNode();
	if (mask)
	{
		auto& ceditor = mask->GetUniqueComp<ee0::CompNodeEditor>();
		m_mask_path->SetValue(ceditor.GetFilepath());
	}
}

void WxCompMaskPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// base filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Base ")));

		std::string path;
		if (auto node = m_cmask.GetBaseNode()) {
			auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
			path = ceditor.GetFilepath();
		}
		sizer->Add(m_base_path = new wxTextCtrl(win, wxID_ANY, path,
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(WxCompMaskPanel::OnSetBasePath));

		pane_sizer->Add(sizer);
	}
	// mask filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Mask ")));

		std::string path;
		if (auto node = m_cmask.GetMaskNode()) {
			auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
			path = ceditor.GetFilepath();
		}
		sizer->Add(m_mask_path = new wxTextCtrl(win, wxID_ANY, path,
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(WxCompMaskPanel::OnSetMaskPath));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompMaskPanel::OnSetBasePath(wxCommandEvent& event)
{
	auto node = CreateNodeFromFile();
	if (!node) {
		return;
	}

	m_cmask.SetBaseNode(node);

	auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
	m_base_path->SetValue(ceditor.GetFilepath());
}

void WxCompMaskPanel::OnSetMaskPath(wxCommandEvent& event)
{
	auto node = CreateNodeFromFile();
	if (!node) {
		return;
	}

	m_cmask.SetMaskNode(node);

	auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
	m_mask_path->SetValue(ceditor.GetFilepath());

	auto& cbb = m_node.GetUniqueComp<n2::CompBoundingBox>();
	cbb.SetSize(m_node, node->GetUniqueComp<n2::CompBoundingBox>().GetSize());
}

n0::SceneNodePtr WxCompMaskPanel::CreateNodeFromFile()
{
	std::string filter = "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, filter);
	if (dlg.ShowModal() != wxID_OK) {
		return nullptr;
	}

	std::string filepath = dlg.GetPath().ToStdString();
	auto node = ns::NodeFactory::Create(filepath);
	if (!node) {
		return nullptr;
	}

	// editor
	auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetFilepath(filepath);

	return node;
}

}