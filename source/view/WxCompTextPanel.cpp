#include "ee2/WxCompTextPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>
#include <ee0/WxColorGradientDlg.h>
#include <ee0/ConfigFile.h>

#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <sx/StringHelper.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>

namespace
{

const wxString HORI_ALIGN_LABELS[] = { 
	wxT("左"), wxT("右"), wxT("中"), wxT("自动") };
const wxString VERT_ALIGN_LABELS[] = { 
	wxT("上"), wxT("下"), wxT("中"), wxT("自动") };

const wxString OVER_LABEL_LABELS[] = {
	wxT("溢出"), wxT("截断"), wxT("压缩") };

}

namespace ee2
{

WxCompTextPanel::WxCompTextPanel(wxWindow* parent, n2::CompText& ctext,
	                             n0::SceneNode& obj, 
	                             const ee0::SubjectMgrPtr& sub_mgr)
	: ee0::WxCompPanel(parent, "Text")
	, m_ctext(ctext)
	, m_obj(obj)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();
	Expand();
}

void WxCompTextPanel::RefreshNodeComp()
{
	auto& text = m_ctext.GetText();
	auto& tb = text.tb;

	m_text->SetValue(text.text);

	m_width->SetValue(tb.width);
	m_height->SetValue(tb.height);

	m_font_type->SetSelection(tb.font_type);
	m_font_size->SetValue(tb.font_size);

	m_has_edge->SetValue(tb.has_edge);
	m_edge_size->SetValue(std::to_string(tb.edge_size));

	RefreshColorBtn(tb);

	m_align_h->SetSelection(tb.align_hori);
	m_align_v->SetSelection(tb.align_vert);

	m_space_h->SetValue(std::to_string(tb.space_hori));
	m_space_v->SetValue(std::to_string(tb.space_vert));

	m_over_label->SetSelection(tb.overlabel);
	m_richtext->SetValue(tb.richtext);
}

void WxCompTextPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	auto& text = m_ctext.GetText();
	auto& tb = text.tb;

	static const wxSize INPUT_SIZE(65, 19);

	// text
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "String");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(m_text = new wxTextCtrl(win, wxID_ANY, text.text, 
			wxDefaultPosition, wxSize(200, 60), wxTE_MULTILINE));
		Connect(m_text->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		pane_sizer->Add(sizer);
	}
	// size
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Label");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Width ")));
		sizer->Add(m_width = new wxSpinCtrl(win, wxID_ANY, std::to_string(tb.width),
			wxDefaultPosition, INPUT_SIZE, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 1, 1024, tb.width));
		Connect(m_width->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
			wxSpinEventHandler(WxCompTextPanel::SpinEventHandler));
		Connect(m_width->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Height ")));
		sizer->Add(m_height = new wxSpinCtrl(win, wxID_ANY, std::to_string(tb.height),
			wxDefaultPosition, INPUT_SIZE, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 1, 1024, tb.width));
		Connect(m_height->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
			wxSpinEventHandler(WxCompTextPanel::SpinEventHandler));
		Connect(m_height->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		pane_sizer->Add(sizer);
	}
	// font
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Font");
		wxSizer* vert_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			wxArrayString choices;
			auto& fonts = ee0::ConfigFile::Instance()->GetFonts();
			for (auto& font : fonts) {
				auto name = sx::StringHelper::UTF8ToGBK(font.first.c_str());
				choices.push_back(font.first);
			}
			sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Type ")));
			sizer->Add(m_font_type = new wxChoice(win, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices));
			Connect(m_font_type->GetId(), wxEVT_COMMAND_CHOICE_SELECTED,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));
			m_font_type->SetSelection(tb.font_type);

			sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Size ")));
			sizer->Add(m_font_size = new wxSpinCtrl(win, wxID_ANY, std::to_string(tb.font_size),
				wxDefaultPosition, INPUT_SIZE, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 1, 128, tb.font_size));
			Connect(m_font_size->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
				wxSpinEventHandler(WxCompTextPanel::SpinEventHandler));
			Connect(m_font_size->GetId(), wxEVT_COMMAND_TEXT_ENTER,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			vert_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Color ")));
			sizer->Add(m_font_color = new wxButton(win, wxID_ANY));
			Connect(m_font_color->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			vert_sizer->Add(sizer);
		}
		pane_sizer->Add(vert_sizer);
	}
	// edge
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Edge");
		wxSizer* vert_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(m_has_edge = new wxCheckBox(win, wxID_ANY, "Edge"));
			Connect(m_has_edge->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Size ")));
			sizer->Add(m_edge_size = new wxTextCtrl(win, wxID_ANY, std::to_string(tb.edge_size),
				wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
			Connect(m_edge_size->GetId(), wxEVT_COMMAND_TEXT_ENTER,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			vert_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Color ")));
			sizer->Add(m_edge_color = new wxButton(win, wxID_ANY));
			Connect(m_edge_color->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			vert_sizer->Add(sizer);
		}
		pane_sizer->Add(vert_sizer);
	}
	// align
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Align");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Hori ")));
		sizer->Add(m_align_h = new wxChoice(win, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, HORI_ALIGN_LABELS));
		Connect(m_align_h->GetId(), wxEVT_COMMAND_CHOICE_SELECTED,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));
		m_align_h->SetSelection(tb.align_hori);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Vert ")));
		sizer->Add(m_align_v = new wxChoice(win, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, VERT_ALIGN_LABELS));
		Connect(m_align_v->GetId(), wxEVT_COMMAND_CHOICE_SELECTED,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));
		m_align_v->SetSelection(tb.align_vert);

		pane_sizer->Add(sizer);
	}
	// space
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Space");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Hori ")));
		sizer->Add(m_space_h = new wxTextCtrl(win, wxID_ANY, std::to_string(tb.space_hori),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_space_h->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Vert ")));
		sizer->Add(m_space_v = new wxTextCtrl(win, wxID_ANY, std::to_string(tb.space_vert),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_space_v->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		pane_sizer->Add(sizer);
	}
	// other
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Others");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Over Label ")));
		sizer->Add(m_over_label = new wxChoice(win, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, OVER_LABEL_LABELS));
		Connect(m_over_label->GetId(), wxEVT_COMMAND_CHOICE_SELECTED,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));
		m_over_label->SetSelection(tb.overlabel);

		sizer->AddSpacer(10);

		sizer->Add(m_richtext = new wxCheckBox(win, wxID_ANY, "Richtext"));
		Connect(m_richtext->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		pane_sizer->Add(sizer);
	}
	RefreshColorBtn(tb);

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompTextPanel::CommandEventHandler(wxCommandEvent& event)
{
	pt2::Text& text = m_ctext.GetText();
	auto& tb = text.tb;

	int id = event.GetId();
	if (id == m_text->GetId()) {
		text.text = m_text->GetValue().ToStdString();
	} else if (id == m_width->GetId()) {
		tb.width = m_width->GetValue();
		UpdateBoundingBox(tb);
	} else if (id == m_height->GetId()) {
		tb.height = m_height->GetValue();
		UpdateBoundingBox(tb);
	} else if (id == m_font_type->GetId()) {
		tb.font_type = m_font_type->GetSelection();
	} else if (id == m_font_size->GetId()) {
		tb.font_size = m_font_size->GetValue();
	} else if (id == m_font_color->GetId()) {
		ee0::WxColorGradientDlg dlg(this, m_ctext.GetText().tb.font_color);
		if (dlg.ShowModal() == wxID_OK) 
		{
			tb.font_color = dlg.GetColor();
			RefreshColorBtn(tb);
		}
	} else if (id == m_has_edge->GetId()) {
		tb.has_edge = m_has_edge->GetValue();
	} else if (id == m_edge_size->GetId()) {
		tb.edge_size = std::stof(m_edge_size->GetValue().ToStdString());
	}  else if (id == m_edge_color->GetId()) {
		ee0::WxColorGradientDlg dlg(this, m_ctext.GetText().tb.edge_color);
		if (dlg.ShowModal() == wxID_OK) 
		{
			tb.edge_color = dlg.GetColor();
			RefreshColorBtn(tb);
		}
	} else if (id == m_align_h->GetId()) {
		tb.align_hori = static_cast<pt2::Textbox::HoriAlign>(m_align_h->GetSelection());
	} else if (id == m_align_v->GetId()) {
		tb.align_vert = static_cast<pt2::Textbox::VertAlign>(m_align_v->GetSelection());
	} else if (id == m_space_h->GetId()) {
		tb.space_hori = std::stof(m_space_h->GetValue().ToStdString());
	} else if (id == m_space_v->GetId()) {
		tb.space_vert = std::stof(m_space_v->GetValue().ToStdString());
	} else if (id == m_over_label->GetId()) {
		tb.overlabel = static_cast<pt2::Textbox::OverLabel>(m_over_label->GetSelection());
	} else if (id == m_richtext->GetId()) {
		tb.richtext = m_richtext->GetValue();
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxCompTextPanel::SpinEventHandler(wxSpinEvent& event)
{
	pt2::Text& text = m_ctext.GetText();
	auto& tb = text.tb;

	int id = event.GetId();
	if (id == m_width->GetId()) {
		tb.width = m_width->GetValue();
		UpdateBoundingBox(tb);
	} else if (id == m_height->GetId()) {
		tb.height = m_height->GetValue();
		UpdateBoundingBox(tb);
	} else if (id == m_font_size->GetId()) {
		tb.font_size = m_font_size->GetValue();
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxCompTextPanel::RefreshColorBtn(const pt2::Textbox& tb)
{
	if (tb.font_color.items.size() == 1) {
		m_font_color->SetLabelText("");
		m_font_color->SetBackgroundColour(ToWxColor(tb.font_color.items[0].col));
	} else {
		m_font_color->SetLabelText("gradient");
		m_font_color->SetBackgroundColour(*wxWHITE);
	}
	if (tb.edge_color.items.size() == 1) {
		m_edge_color->SetLabelText("");
		m_edge_color->SetBackgroundColour(ToWxColor(tb.edge_color.items[0].col));
	} else {
		m_edge_color->SetLabelText("gradient");
		m_edge_color->SetBackgroundColour(*wxWHITE);
	}
}

void WxCompTextPanel::UpdateBoundingBox(const pt2::Textbox& tb)
{
	auto& cbb = m_obj.GetUniqueComp<n2::CompBoundingBox>();
	cbb.SetSize(m_obj, sm::rect(tb.width, tb.height));
}

wxColour WxCompTextPanel::ToWxColor(const pt2::Color& col)
{
	return wxColour(col.r, col.g, col.b, col.a);
}

}