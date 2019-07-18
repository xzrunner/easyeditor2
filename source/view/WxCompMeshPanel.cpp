#include "ee2/WxCompMeshPanel.h"

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#include <node2/CompMesh.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <ns/NodeFactory.h>
#else
#include <ee0/CompEntityEditor.h>
#include <entity2/CompMesh.h>
#include <es/EntityFactory.h>
#include <entity0/World.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompMeshPanel::WxCompMeshPanel(wxWindow* parent, ECS_WORLD_PARAM const ee0::GameObj& obj)
	: ee0::WxCompPanel(parent, "Mesh")
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompMeshPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& cmesh = m_obj->GetSharedComp<n2::CompMesh>();
	if (auto& mesh = cmesh.GetMesh())
	{
		if (auto& base = mesh->GetBaseSymbol()) {
			auto& cid = base->GetUniqueComp<n0::CompIdentity>();
			m_base_path->SetValue(cid.GetFilepath());
		}
	}
#else
	auto& cmesh = m_world.GetComponent<e2::CompMesh>(m_obj);
	if (cmesh.mesh)
	{
		if (auto& base = cmesh.mesh->GetBaseSymbol()) {
			auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(*base);
			m_base_path->SetValue(*cid.filepath);
		}
	}
#endif // GAME_OBJ_ECS
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
#ifndef GAME_OBJ_ECS
		auto& cmesh = m_obj->GetSharedComp<n2::CompMesh>();
		if (auto& mesh = cmesh.GetMesh())
		{
			if (auto& base = mesh->GetBaseSymbol()) {
				auto& cid = base->GetUniqueComp<n0::CompIdentity>();
				path = cid.GetFilepath();
			}
		}
#else
		auto& cmesh = m_world.GetComponent<e2::CompMesh>(m_obj);
		if (cmesh.mesh)
		{
			if (auto& base = cmesh.mesh->GetBaseSymbol()) {
				auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(*base);
				path = *cid.filepath;
			}
		}
#endif // GAME_OBJ_ECS
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
	if (!GAME_OBJ_VALID(obj)) {
		return;
	}
#ifndef GAME_OBJ_ECS
	auto& cmesh = m_obj->GetSharedComp<n2::CompMesh>();
	auto mesh = std::make_unique<pt2::Mesh<n0::SceneNodePtr>>();
//	mesh->SetMesh(std::make_unique<pm2::TrianglesMesh>);
	cmesh.SetMesh(mesh);

	auto& cid = obj->GetUniqueComp<n0::CompIdentity>();
	m_base_path->SetValue(cid.GetFilepath());
#else
	auto& cmesh = m_world.GetComponent<e2::CompMesh>(m_obj);
	cmesh.mesh = std::make_unique<pt2::Mesh<e0::Entity>>();

	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	m_base_path->SetValue(*cid.filepath);
#endif // GAME_OBJ_ECS
}

ee0::GameObj WxCompMeshPanel::CreateNodeFromFile()
{
	std::string filter = "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, filter);
	if (dlg.ShowModal() != wxID_OK) {
		return GAME_OBJ_NULL;
	}

	std::string filepath = dlg.GetPath().ToStdString();
#ifndef GAME_OBJ_ECS
	auto obj = ns::NodeFactory::Create(filepath);
#else
	auto obj = es::EntityFactory::Create(m_world, filepath);
#endif // GAME_OBJ_ECS
	if (!GAME_OBJ_VALID(obj)) {
		return GAME_OBJ_NULL;
	}

	// editor
#ifndef GAME_OBJ_ECS
	auto& cid = obj->GetUniqueComp<n0::CompIdentity>();
	cid.SetFilepath(filepath);
	cid.SetID(0);
	cid.SetName("base");
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	cid.filepath = std::make_unique<std::string>(filepath);
#endif // GAME_OBJ_ECS

	return obj;
}

}