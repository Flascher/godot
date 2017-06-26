/*************************************************************************/
/*  curve_editor_plugin.h                                                */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2017 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef CURVE_EDITOR_PLUGIN_H
#define CURVE_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/resources/curve.h"

// Edits a y(x) curve
class CurveEditor : public Control {
	GDCLASS(CurveEditor, Control)
public:
	CurveEditor();

	Size2 get_minimum_size() const;

	void set_curve(Ref<Curve> curve);

	enum PresetID {
		PRESET_FLAT0 = 0,
		PRESET_FLAT1,
		PRESET_LINEAR,
		PRESET_EASE_IN,
		PRESET_EASE_OUT,
		PRESET_SMOOTHSTEP,
		PRESET_COUNT
	};

	enum ContextAction {
		CONTEXT_ADD_POINT = 0,
		CONTEXT_REMOVE_POINT
	};

	enum TangentIndex {
		TANGENT_NONE = -1,
		TANGENT_LEFT = 0,
		TANGENT_RIGHT = 1
	};

protected:
	void _notification(int p_what);

	static void _bind_methods();

private:
	void on_gui_input(const Ref<InputEvent> &p_event);
	void on_preset_item_selected(int preset_id);
	void _curve_changed();
	void on_context_menu_item_selected(int action_id);

	void open_context_menu(Vector2 pos);
	int get_point_at(Vector2 pos) const;
	int get_tangent_at(Vector2 pos) const;
	void add_point(Vector2 pos);
	void remove_point(int index);
	void set_selected_point(int index);
	void set_hover_point_index(int index);
	void push_undo(Array previous_curve_data);
	void update_view_transform();

	Vector2 get_tangent_view_pos(int i, TangentIndex tangent) const;
	Vector2 get_view_pos(Vector2 world_pos) const;
	Vector2 get_world_pos(Vector2 view_pos) const;

	void _draw();

	void stroke_rect(Rect2 rect, Color color);

private:
	Rect2 _world_rect;
	Transform2D _world_to_view;

	Ref<Curve> _curve_ref;
	PopupMenu *_context_menu;
	PopupMenu *_presets_menu;

	Array _undo_data;
	bool _has_undo_data;
	bool _undo_no_commit;

	Vector2 _context_click_pos;
	int _selected_point;
	int _hover_point;
	int _selected_tangent;
	bool _dragging;

	// Constant
	float _hover_radius;
	float _tangents_length;
};

class CurveEditorPlugin : public EditorPlugin {
	GDCLASS(CurveEditorPlugin, EditorPlugin)
public:
	CurveEditorPlugin(EditorNode *p_node);
	~CurveEditorPlugin();

	String get_name() const { return "Curve"; }
	bool has_main_screen() const { return false; }
	void edit(Object *p_object);
	bool handles(Object *p_object) const;
	void make_visible(bool p_visible);

private:
	static void _bind_methods();

	void _curve_texture_changed();

private:
	CurveEditor *_view;
	Ref<Resource> _current_ref;
	EditorNode *_editor_node;
	ToolButton *_toggle_button;
};

#endif // CURVE_EDITOR_PLUGIN_H