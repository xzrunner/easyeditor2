#include "ee2/WxCompMeshPanel.h"

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

WxCompMeshPanel::WxCompMeshPanel(wxWindow* parent, n2::CompMesh& cmask, 
	                             n0::SceneNode& obj)
	: ee0::WxCompPanel(parent, "Mesh")
	, m_cmesh(cmask)
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompMeshPanel::RefreshNodeComp()
{
	if (auto& mesh = m_cmesh.GetMesh()) {
		if (auto& base = mesh->GetBaseSymbol()) {
			auto& ceditor = base->GetUniqueComp<ee0::CompNodeEditor>();
			m_base_path->SetValue(ceditor.GetFilepath());
		}
	}
}

void WxCompMeshPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// base filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Base ")));

		std::string path;
		if (auto& mesh = m_cmesh.GetMesh()) {
			if (auto& base = mesh->GetBaseSymbol()) {
				auto& ceditor = base->GetUniqueComp<ee0::CompNodeEditor>();
				path = ceditor.GetFilepath();
			}
		}
		sizer->Add(m_base_path = new wxTextCtrl(win, wxID_ANY, path,
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(WxCompMeshPanel::OnSetBasePath));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompMeshPanel::OnSetBasePath(wxCommandEvent& event)
{
	auto obj = CreateNodeFromFile();
	if (!obj) {
		return;
	}

	auto mesh = std::make_unique<pt2::Mesh<n0::SceneNode>>();
//	mesh->SetMesh(std::make_unique<pm::TrianglesMesh>);
	m_cmesh.SetMesh(mesh);

	auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
	m_base_path->SetValue(ceditor.GetFilepath());
}

ee0::GameObj WxCompMeshPanel::CreateNodeFromFile()
{
	std::string filter = "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, filter);
	if (dlg.ShowModal() != wxID_OK) {
		return nullptr;
	}

	std::string filepath = dlg.GetPath().ToStdString();
	auto obj = ns::NodeFactory::Create(filepath);
	if (!obj) {
		return nullptr;
	}

	// editor
	auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetFilepath(filepath);

	return obj;
}

}